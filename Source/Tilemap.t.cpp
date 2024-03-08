/// @file     Tilemap.t.cpp
/// @author   Eli Tsereteli (ilya.tsereteli@digipen.edu)
/// 
/// @brief    Tilemap component - loads and manages a tilemap array.

#define TILEMAP_C

// TODO: callbacks should also get called whenever updating stride mult and row width.
//       (those setters are currently inlines in .h)

#ifndef TILEMAP_H
#include "Tilemap.h"
#endif

#include "Entity.h"  // parent
#include "Transform.h"

#include <imgui.h>


//-----------------------------------------------------------------------------
// constructors
//-----------------------------------------------------------------------------

    /// @brief    Default constructor
    template < typename TileType >
    Tilemap<TileType>::Tilemap() :
        TilemapBase( typeid( Tilemap ) )
    {}


    /// @return   A copy of this component
    template < typename TileType >
    Tilemap< TileType >* Tilemap< TileType >::Clone() const
    {
        return new Tilemap< TileType >( *this );
    }


    /// @brief    Copy constructor: don't copy the callbacks
    template < typename TileType >
    Tilemap<TileType>::Tilemap(Tilemap const& other) :
        TilemapBase( typeid(Tilemap) ),
        m_Dimensions(other.m_Dimensions),
        m_InvMat(other.m_InvMat),
        m_Mat(other.m_Mat),
        m_TileScale(other.m_TileScale),
        m_Tiles(other.m_Tiles)
    {}



//-----------------------------------------------------------------------------
//          Public methods
//-----------------------------------------------------------------------------

    /// @brief          Sets the whole tilemap to a given array (vector)
    /// @param  tiles   vector of tile IDs
    template < typename TileType >
    void Tilemap<TileType>::SetTilemap( std::vector<TileType> const& tiles ) 
    {
        m_Tiles = tiles;
        m_Dimensions.y = (int)tiles.size() / m_Dimensions.x;

        callOnTilemapChangedCallbacks();

    } 


    /// @brief          Sets the tile at given coordinate to given index.
    /// @param x        column
    /// @param y        row
    /// @param tileID   index to change the tile to
    template < typename TileType >
    void Tilemap<TileType>::SetTile(glm::ivec2 coord, TileType const& tile)
    {
        int index = coord.y * m_Dimensions.x + coord.x;

        TileType previousValue = m_Tiles[ index ];
        m_Tiles[ index ] = tile;

        if ( previousValue != tile )
        {
            callOnTilemapChangedCallbacks( coord, previousValue );
        }
    }



    /// @brief          Gets tilemap coordinate of the tile at given world position.
    /// @param pos      Position in world space
    /// @return         Tilemap coordinate of the tile. If the provided location
    ///                 is outside the tilemap, returns (-1,-1)
    template < typename TileType >
    glm::ivec2 Tilemap<TileType>::WorldPosToTileCoord( glm::vec2 pos ) const
    {
        const_cast< Tilemap< TileType >* >( this )->updateMat();

        // convert to tilemap coordinate
        glm::vec4 pos4(pos, 0, 1);
        glm::ivec2 coord = glm::ivec2( m_InvMat * pos4 );

        if (
            coord.x < 0 || coord.y < 0 ||
            coord.x >= m_Dimensions.x || coord.y >= m_Dimensions.y
        )
        {
            return { -1, -1 };
        }

        return coord;
    }


    /// @brief          Gets world position of the given tilemap coordinates.
    /// @param coord    Tilemap coordinate
    /// @return         World position of the tile at given tilemap coordinate
    template < typename TileType >
    glm::vec2 Tilemap<TileType>::TileCoordToWorldPos( glm::ivec2 coord ) const
    {
        const_cast< Tilemap< TileType >* >( this )->updateMat();
        glm::vec4 pos4( (glm::vec2)coord + glm::vec2( 0.5f ), 0, 1 );
    
        return glm::vec2( m_Mat * pos4 );
    }


    /// @brief  Adds a function to the list of callbacks. The given function will get called whenever the tilemap is updated.
    /// @param  ownerId     the ID of the owner of the callback
    /// @param  callback    the callback function
    /// @tparam TileType    the data type of each tile
    template < typename TileType >
    void Tilemap< TileType >::AddOnTilemapChangedCallback( unsigned ownerId, OnTilemapChangedCallback callback )
    {
        m_OnTilemapChangedCallbacks[ ownerId ] = callback;
    }


    /// @brief  Removes a function from the list of callbacks
    /// @param  ownerId     the ID of the owner of the callback
    /// @tparam TileType    the data type of each tile
    template < typename TileType >
    void Tilemap<TileType>::RemoveOnTilemapChangedCallback( unsigned ownerId )
    {
        auto callbackIterator = m_OnTilemapChangedCallbacks.find( ownerId );
        if ( callbackIterator != m_OnTilemapChangedCallbacks.end() )
        {
            m_OnTilemapChangedCallbacks.erase( callbackIterator );
        }
    }


    /// @brief  gets whether a tile is within this Tilemap
    /// @param  tilePos     the tile position to check
    /// @tparam TileType    the data type of each tile
    /// @return whether the tile position is within this Tilemap
    template < typename TileType >
    bool Tilemap<TileType>::IsPositionWithinBounds( glm::ivec2 const& tilePos ) const
    {
        return (
            tilePos.x >= 0 && tilePos.x < m_Dimensions.x &&
            tilePos.y >= 0 && tilePos.t < m_Dimensions.y
        );
    }

//-----------------------------------------------------------------------------
//          Virtual overrides
//-----------------------------------------------------------------------------

    /// @brief  called when entering a scene
    template < typename TileType >
    void Tilemap<TileType>::OnInit()
    {
        m_Transform.SetOnConnectCallback(
            [ this ]()
            {
                m_Transform->AddOnTransformChangedCallback( GetId(), std::bind( &Tilemap::updateMat, this ) );
                updateMat();
            }
        );
        m_Transform.SetOnDisconnectCallback(
            [ this ]()
            {
                m_Transform->RemoveOnTransformChangedCallback( GetId() );
            }
        );

        m_Transform.Init( GetEntity() );
    }

    /// @brief  called when exiting a scene
    template < typename TileType >
    void Tilemap<TileType>::OnExit()
    {
        m_Transform.Exit();
    }


    /// @brief  inspector for non-int tilemaps
    /// @tparam TileType    the type of data in the Tilemap
    template< typename TileType >
    void Tilemap< TileType >::Inspector()
    {
        glm::ivec2 dimensions = GetDimensions();
        if ( ImGui::DragInt2( "Tilemap Size", &dimensions[0], 0.05f, 1, INT_MAX ) )
        {
            SetDimensions( dimensions );
        }
    }


//-----------------------------------------------------------------------------
// private: methods
//-----------------------------------------------------------------------------


    /// @brief  calls all the OnTilemapChanged callbacks attached to this Tilemap
    /// @param  tilepos         the position of the tile that was changed - will be (-1, -1) if whole tilemap changed
    /// @param  previousValue   the value of the tile before it was changed - garbage if whole tilemap changed
    /// @tparam TileType        the data type of each tile
    template < typename TileType >
    void Tilemap< TileType >::callOnTilemapChangedCallbacks( glm::ivec2 const& tilePos, TileType const& previousValue )
    {
        for ( auto const& [ ownerId, callback ] : m_OnTilemapChangedCallbacks )
        {
            callback( this, tilePos, previousValue );
        }
    }


    /// @brief  Updates inverse transform matrix, if parent's transform has changed.
    template < typename TileType >
    void Tilemap<TileType>::updateMat()
    {
        if ( m_Transform == nullptr )
        {
            return;
        }

        // combine tilemap scale with parent transform position  (will rotation ever be needed?)
        m_Mat = glm::scale( glm::mat4( 1 ), { m_TileScale.x, -m_TileScale.y, 1 } );
        glm::vec4 pos = { m_Transform->GetTranslation(), 0, 1 };
        m_Mat[ 3 ] = pos;

        m_InvMat = glm::inverse( m_Mat );
    }


//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------


    /// @brief          Read in the text this Tilemap displays
    /// @param  stream  The json to read from.
    template < typename TileType >
    void Tilemap<TileType>::readTilemap( nlohmann::ordered_json const& data )
    {
        if ( std::is_same_v< TileType , int > == false )
        { // only serialize int tilemaps
            return;
        }

        m_Dimensions.y = (int)data.size() / m_Dimensions.x;
        int extraTiles = data.size() % m_Dimensions.x;
        m_Tiles.resize( data.size() + ( extraTiles == 0 ? 0 : m_Dimensions.x - extraTiles ) );

        m_Dimensions.y = (int)data.size() / m_Dimensions.x;
        for ( int i = 0; i < data.size(); ++i )
        {
            Stream::Read( m_Tiles[ i ], data[ i ] );
        }

        callOnTilemapChangedCallbacks();
    }


    /// @brief          Read in the row width of the tilemap
    /// @param stream   The json to read from
    template < typename TileType >
    void Tilemap<TileType>::readDimensions( nlohmann::ordered_json const& data )
    {
        m_Dimensions = Stream::Read< 2, int >( data );
        m_Tiles.resize( m_Dimensions.x * m_Dimensions.y );
    }


    /// @brief            Read in the stride multiplier
    /// @param  stream    The json to read from.
    template < typename TileType >
    void Tilemap<TileType>::readTileScale( nlohmann::ordered_json const& data )
    {
        m_TileScale = Stream::Read< 2, float >( data );
    }


    /// @brief      The map of read methods for this Component
    template < typename TileType >
    ReadMethodMap< Tilemap<TileType> > const Tilemap<TileType>::s_ReadMethods = {
        { "TileData", &readTilemap  },
        { "Dimensions", &readDimensions },
        { "TileScale", &readTileScale }
    };


//-----------------------------------------------------------------------------
// public: writing
//-----------------------------------------------------------------------------


    /// @brief Write all Tilemap component data to a JSON file.
    /// @tparam TileType    the data type of each tile in the tilemap
    /// @return The JSON file containing the Tilemap component data.
    template < typename TileType >
    nlohmann::ordered_json Tilemap<TileType>::Write() const
    {
        nlohmann::ordered_json data;

        data[ "Dimensions" ] = Stream::Write( m_Dimensions );
        data[ "TileScale" ] = Stream::Write( m_TileScale );

        if ( std::is_same_v< TileType, int > == false )
        {
            return data;
        }

        nlohmann::ordered_json& tileData = data[ "TileData" ];
        for ( TileType const& tile : m_Tiles )
        {
            tileData.push_back( Stream::Write( tile ) );
        }

        return data;
    }


//-----------------------------------------------------------------------------
