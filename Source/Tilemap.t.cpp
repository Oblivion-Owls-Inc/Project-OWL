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

#ifndef BEHAVIORSYSTEM_H
#include "BehaviorSystem.h"
#endif

/// @brief    Default constructor
template < typename TileType >
Tilemap<TileType>::Tilemap() :
    TilemapBase( typeid( Tilemap ) )
{}


/// @return   A copy of this component
template < typename TileType >
Component * Tilemap<TileType>::Clone() const
{
    return new Tilemap<TileType>(*this); // TODO: copy ctor
}


/// @brief    Copy constructor: don't copy the callbacks
template < typename TileType >
Tilemap<TileType>::Tilemap(Tilemap const& other) :
    TilemapBase( typeid(Tilemap) ),
    m_Dimensions(other.m_Dimensions),
    m_InvMat(other.m_InvMat),
    m_Mat(other.m_Mat),
    m_TileScale(other.m_TileScale),
    m_Tilemap(other.m_Tilemap)
{}



//-----------------------------------------------------------------------------
//          Public methods
//-----------------------------------------------------------------------------

/// @brief          Sets the whole tilemap to a given array (vector)
/// @param  tiles   vector of tile IDs
template < typename TileType >
void Tilemap<TileType>::SetTilemap( std::vector<TileType> const& tiles ) 
{
    if ( tiles.size() )

    m_Tilemap = tiles;
    m_Dimensions.y = (int)tiles.size() / m_Dimensions.x;

    m_Modified = true;

} 


/// @brief          Sets the tile at given coordinate to given index.
/// @param x        column
/// @param y        row
/// @param tileID   index to change the tile to
template < typename TileType >
void Tilemap<TileType>::SetTile(glm::ivec2 coord, TileType const& tile)
{
    if (coord.x == -1)
        return;

    m_Tilemap[coord.y*m_Dimensions.x + coord.x] = tile;

    m_Modified = true;
}



/// @brief          Gets tilemap coordinate of the tile at given world position.
/// @param pos      Position in world space
/// @return         Tilemap coordinate of the tile. If the provided location
///                 is outside the tilemap, returns (-1,-1)
template < typename TileType >
glm::ivec2 Tilemap<TileType>::WorldPosToTileCoord(glm::vec2 pos)
{
    updateMat();

    // convert to tilemap coordinate
    glm::vec4 pos4(pos, 0, 1);
    glm::ivec2 coord = glm::vec2(m_InvMat * pos4);

    // return it if it's within the tilemap, or (-1,-1) otherwise
    int i = coord.y*m_Dimensions.x + coord.x;

    if (coord.x >= 0 && i >= 0 && coord.x < m_Dimensions.x && i < (int)m_Tilemap.size())
        return coord;
    else
        return {-1,-1};
}


/// @brief          Gets world position of the given tilemap coordinates.
/// @param coord    Tilemap coordinate
/// @return         World position of the tile at given tilemap coordinate
template < typename TileType >
glm::vec2 Tilemap<TileType>::TileCoordToWorldPos(glm::ivec2 coord)
{
    updateMat();
    glm::vec4 pos4(coord, 0, 1);
    
    return glm::vec2(m_Mat * pos4);
}


/// @brief          Adds a function to the list of callbacks. The given
///                 function will get called whenever the tilemap is updated.
/// @param function Callback function
template < typename TileType >
void Tilemap<TileType>::AddOnTilemapChangedCallback( int componentID, std::function<void()> function )
{
    m_Callbacks[componentID] = function;
}


/// @brief          Removes a function from the list of callbacks
/// @param objPtr   Pointer to object (this)
template < typename TileType >
void Tilemap<TileType>::RemoveOnTilemapChangedCallback(int componentID)
{
    if (m_Callbacks.find(componentID) != m_Callbacks.end())
        m_Callbacks.erase(componentID);
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
    if (!GetEntity())
        return;

    Behaviors< TilemapBase >()->AddBehavior(this);
    m_PTransform = GetEntity()->GetComponent<Transform>();

#ifndef NDEBUG
    if (!m_PTransform)
    {
        std::cout << "Tilemap: parent does not have transform" << std::endl;
        return;
    }
#endif
    
    updateMat();
}


/// @brief  called every frame
template < typename TileType >
void Tilemap<TileType>::OnUpdate(float dt)
{
    if (m_Modified)
        for (auto& func : m_Callbacks)
            func.second();

    m_Modified = false;
}


/// @brief  called when component is removed
template < typename TileType >
void Tilemap<TileType>::OnExit()
{
    Behaviors< TilemapBase >()->RemoveBehavior(this);
}

/// @brief  inspector for non-int tilemaps
/// @tparam TileType    the type of data in the Tilemap
template< typename TileType >
void Tilemap< TileType >::Inspector() {}


//-----------------------------------------------------------------------------
//              Helpers
//-----------------------------------------------------------------------------


    /// @brief  Updates inverse transform matrix, if parent's transform has changed.
    template < typename TileType >
    void Tilemap<TileType>::updateMat()
    {
        if (!m_PTransform)
            return;

        // combine tilemap scale with parent transform position  (will rotation ever be needed?)
        m_Mat = glm::scale(glm::mat4(1), {m_TileScale.x, -m_TileScale.y, 1});
        glm::vec4 pos = {m_PTransform->GetTranslation(),0,1};
        m_Mat[3] = pos;

        m_InvMat = glm::inverse(m_Mat);
    }



//-----------------------------------------------------------------------------
//              Reading
//-----------------------------------------------------------------------------


/// @brief          Read in the text this Tilemap displays
/// @param  stream  The json to read from.
template < typename TileType >
void Tilemap<TileType>::readTilemap( nlohmann::ordered_json const& data )
{
    if ( std::is_pointer_v< TileType > )
    { // don't read data if data is pointers
        return;
    }

    m_Tilemap.resize( data.size() );
    m_Dimensions.y = (int)data.size() / m_Dimensions.x;
    for ( int i = 0; i < data.size(); ++i )
    {
        Stream::Read( m_Tilemap[ i ], data[ i ] );
    }
}


/// @brief          Read in the row width of the tilemap
/// @param stream   The json to read from
template < typename TileType >
void Tilemap<TileType>::readDimensions( nlohmann::ordered_json const& data )
{
    m_Dimensions = Stream::Read< 2, int >( data );
    m_Tilemap.resize( m_Dimensions.x * m_Dimensions.y );
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




/// @brief Write all Tilemap component data to a JSON file.
/// @tparam TileType    the data type of each tile in the tilemap
/// @return The JSON file containing the Tilemap component data.
template < typename TileType >
nlohmann::ordered_json Tilemap<TileType>::Write() const
{
    nlohmann::ordered_json data;

    data[ "Dimensions" ] = Stream::Write( m_Dimensions );
    data[ "TileScale" ] = Stream::Write( m_TileScale );

    if ( std::is_pointer_v< TileType > )
    {
        return data;
    }

    nlohmann::ordered_json& tileData = data[ "TileData" ];
    for ( TileType const& tile : m_Tilemap )
    {
        tileData.push_back( Stream::Write( tile ) );
    }

    return data;
}
