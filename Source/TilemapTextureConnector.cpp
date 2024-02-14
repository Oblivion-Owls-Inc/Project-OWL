/// @file       TilemapTextureConnector.cpp
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      Component that uses a parent 'source' tilemap to update a child tilemap with connected textures
/// @version    0.1
/// @date       2023-10-20
/// 
/// @copyright  Copyright (c) 2023 Digipen Institute of Technology

#pragma once

#include "TilemapTextureConnector.h"

#include "Tilemap.h"

#include "Inspection.h"


//-----------------------------------------------------------------------------
// public: virtual override methods
//-----------------------------------------------------------------------------


    /// @brief  called once when entering the scene
    void TilemapTextureConnector::OnInit()
    {
        m_ParentTilemap.SetOnConnectCallback(
            [ this ]( Tilemap< int >* tilemap )
            {
                tilemap->AddOnTilemapChangedCallback(
                    GetId(),
                    std::bind(
                        &TilemapTextureConnector::onTilemapChangedCallback,
                        this,
                        std::placeholders::_1,
                        std::placeholders::_2,
                        std::placeholders::_3
                    )
                );
                if ( m_Tilemap != nullptr )
                {
                    updateWholeTilemap();
                }
            }
        );
        m_ParentTilemap.SetOnDisconnectCallback(
            [ this ]( Tilemap< int >* tilemap )
            {
                tilemap->RemoveOnTilemapChangedCallback( GetId() );
            }
        );

        m_Tilemap.SetOnConnectCallback(
            [ this ]( Tilemap< int >* tilemap )
            {
                if ( m_ParentTilemap != nullptr )
                {
                    updateWholeTilemap();
                }
            }
        );

        // TODO: figure out how to handle the edge case where this Entity's parent changes, which would invalidate m_ParentTilemap

        m_ParentTilemap.Init( GetEntity()->GetParent() );
        m_Tilemap.Init( GetEntity() );
    }

    /// @brief  called once when exiting the scene
    void TilemapTextureConnector::OnExit()
    {
        m_ParentTilemap.Exit( GetEntity()->GetParent() );
        m_Tilemap.Exit( GetEntity() );
    }


//-----------------------------------------------------------------------------
// private: helper methods
//-----------------------------------------------------------------------------


    /// @brief  called whenever the source tilemap is modified
    /// @param  tilemap         the tilemap this callback was called from
    /// @param  tilepos         the position of the tile that was changed - will be (-1, -1) if whole tilemap changed
    /// @param  previousValue   the value of the tile before it was changed - garbage if whole tilemap changed
    void TilemapTextureConnector::onTilemapChangedCallback(
        Tilemap< int >* tilemap,
        glm::ivec2 const& tilePos,
        int const& previousValue
    )
    {
        if ( tilePos == glm::ivec2( -1, -1 ) )
        {
            updateWholeTilemap();
            return;
        }

        updateNeighborhood( tilePos );
    }


    /// @brief  updates the textures of the whole tilemap
    void TilemapTextureConnector::updateWholeTilemap()
    {
        glm::ivec2 tilePos;
        glm::ivec2 dimensions = m_ParentTilemap->GetDimensions();

        m_Tilemap->SetDimensions( dimensions * 2 );

        for ( tilePos.x = 0; tilePos.x < dimensions.x; ++tilePos.x )
        {
            for ( tilePos.y = 0; tilePos.y < dimensions.y; ++tilePos.y )
            {
                updateTile( tilePos );
            }
        }
    }


    /// @brief  updates the textures of all tiles around the specified tile
    /// @param  centerTile  the center tile of the neighborhood to update
    void TilemapTextureConnector::updateNeighborhood( glm::ivec2 const& centerTile )
    {
        glm::ivec2 min = glm::ivec2( std::max( centerTile.x - 1, 0 ), std::max( centerTile.y -1, 0 ) );

        glm::ivec2 dimensions = m_ParentTilemap->GetDimensions();
        glm::ivec2 max = glm::ivec2(
            std::min( centerTile.x + 1, dimensions.x - 1 ),
            std::min( centerTile.y + 1, dimensions.y - 1 )
        );

        glm::ivec2 tilePos;
        for ( tilePos.x = min.x; tilePos.x <= max.x; ++tilePos.x )
        {
            for ( tilePos.y = min.y; tilePos.y <= max.y; ++tilePos.y )
            {
                updateTile( tilePos );
            }
        }
    }

    /// @brief  updates the textures of the specified tile
    /// @param  tilePos the position of the (source) tile to update
    void TilemapTextureConnector::updateTile( glm::ivec2 const& tilePos )
    {
        int tileId = m_ParentTilemap->GetTile( tilePos );

        if ( tileId == -1 )
        {
            m_Tilemap->SetTile( tilePos * 2 + glm::ivec2( 0, 0 ), -1 );
            m_Tilemap->SetTile( tilePos * 2 + glm::ivec2( 1, 0 ), -1 );
            m_Tilemap->SetTile( tilePos * 2 + glm::ivec2( 0, 1 ), -1 );
            m_Tilemap->SetTile( tilePos * 2 + glm::ivec2( 1, 1 ), -1 );
            return;
        }

        updateTopLeftCorner    ( tilePos, tileId );
        updateTopRightCorner   ( tilePos, tileId );
        updateBottomLeftCorner ( tilePos, tileId );
        updateBottomRightCorner( tilePos, tileId );
    }


    /// @brief  updates the top left corner of the specified tile
    /// @param  tilePos the position of the (source) tile to update
    /// @param  tileId  the ID of the type of tile being updated
    void TilemapTextureConnector::updateTopLeftCorner( glm::ivec2 const& tilePos, int tileId )
    {
        constexpr glm::ivec2 offsets[ 3 ] = {
            glm::ivec2(  0, -1 ),
            glm::ivec2( -1, -1 ),
            glm::ivec2( -1,  0 )
        };

        int textureIndex = getCornerTexture( tilePos, tileId, offsets, m_TopLeftTextures );
        m_Tilemap->SetTile( tilePos * 2 + glm::ivec2( 0, 0 ), textureIndex );

    }

    /// @brief  updates the top right corner of the specified tile
    /// @param  tilePos the position of the (source) tile to update
    /// @param  tileId  the ID of the type of tile being updated
    void TilemapTextureConnector::updateTopRightCorner( glm::ivec2 const& tilePos, int tileId )
    {
        constexpr glm::ivec2 offsets[ 3 ] = {
            glm::ivec2(  1,  0 ),
            glm::ivec2(  1, -1 ),
            glm::ivec2(  0, -1 )
        };

        int textureIndex = getCornerTexture( tilePos, tileId, offsets, m_TopRightTextures );
        m_Tilemap->SetTile( tilePos * 2 + glm::ivec2( 1, 0 ), textureIndex );
    }

    /// @brief  updates the bottom left corner of the specified tile
    /// @param  tilePos the position of the (source) tile to update
    /// @param  tileId  the ID of the type of tile being updated
    void TilemapTextureConnector::updateBottomLeftCorner( glm::ivec2 const& tilePos, int tileId )
    {
        constexpr glm::ivec2 offsets[ 3 ] = {
            glm::ivec2( -1,  0 ),
            glm::ivec2( -1,  1 ),
            glm::ivec2(  0,  1 )
        };

        int textureIndex = getCornerTexture( tilePos, tileId, offsets, m_BottomLeftTextures );
        m_Tilemap->SetTile( tilePos * 2 + glm::ivec2( 0, 1 ), textureIndex );
    }

    /// @brief  updates the bottom right corner of the specified tile
    /// @param  tilePos the position of the (source) tile to update
    /// @param  tileId  the ID of the type of tile being updated
    void TilemapTextureConnector::updateBottomRightCorner( glm::ivec2 const& tilePos, int tileId )
    {
        constexpr glm::ivec2 offsets[ 3 ] = {
            glm::ivec2(  0,  1 ),
            glm::ivec2(  1,  1 ),
            glm::ivec2(  1,  0 )
        };

        int textureIndex = getCornerTexture( tilePos, tileId, offsets, m_BottomRightTextures );
        m_Tilemap->SetTile( tilePos * 2 + glm::ivec2( 1, 1 ), textureIndex );
    }


    /// @brief  gets the texture index of a corner
    /// @param  tilePos     the position of the (source) tile to update
    /// @param  tileId      the ID of the type of tile being updated
    /// @param  offsets     the tile position offsets to build the connection flags with
    /// @param  textures    the texture index offsets to display
    /// @return the texture index for this corner
    int TilemapTextureConnector::getCornerTexture( glm::ivec2 const& tilePos, int tileId, glm::ivec2 const offsets[], int const textures[] )
    {
        unsigned flags = 0;
        glm::ivec2 dimensions = m_ParentTilemap->GetDimensions();
        for ( int i = 0; i < 3; ++i )
        {
            glm::ivec2 offsetPos = tilePos + offsets[ i ];
            if (
                offsetPos.x < 0 || offsetPos.x >= dimensions.x ||
                offsetPos.y < 0 || offsetPos.y >= dimensions.y
            )
            {
                continue;
            }

            if ( m_ParentTilemap->GetTile( offsetPos ) == tileId )
            {
                flags |= 1 << i;
            }
        }

        return m_FirstTileOffset + tileId * m_TexturesPerTile + textures[ flags ];
    }


//-----------------------------------------------------------------------------
// public: inspection
//-----------------------------------------------------------------------------

    
    /// @brief  displays this TilemapTextureConnector in the Inspector
    void TilemapTextureConnector::Inspector()
    {
        bool changed = false;

        changed |= ImGui::DragInt( "first texture offset", &m_FirstTileOffset, 0.05f, 0, INT_MAX );
        ImGui::SetItemTooltip( "the index of the first connected texture in the spritesheet" );

        changed |= ImGui::DragInt( "textures per tile", &m_TexturesPerTile, 0.05f, 1, INT_MAX );
        ImGui::SetItemTooltip( "the number of textures that each tile has" );


        // lambda function used to inspect each textureIndex in the array
        int texturesPerTile = m_TexturesPerTile;
        auto elementInspector = [ texturesPerTile ]( int* textureIndex ) -> bool
        {
            return ImGui::DragInt( "", textureIndex, 0.05f, 0, texturesPerTile );
        };

        changed |= Inspection::InspectStaticArray< int, 8 >(
            "top left textures",
            m_TopLeftTextures,
            elementInspector
        );

        changed |= Inspection::InspectStaticArray< int, 8 >(
            "top right textures",
            m_TopRightTextures,
            elementInspector
         );
         
        changed |= Inspection::InspectStaticArray< int, 8 >(
            "bottom left textures",
            m_BottomLeftTextures,
            elementInspector
        );

        changed |= Inspection::InspectStaticArray< int, 8 >(
            "bottom right textures",
            m_BottomRightTextures,
            elementInspector
        );

        if ( changed )
        {
            updateWholeTilemap();
        }
    }


//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------


    /// @brief  the offset from the start of the tilesheet of the first usable texture
    /// @param  data    the json data to read from
    void TilemapTextureConnector::readFirstTileOffset( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_FirstTileOffset, data );
    }

    /// @brief  how many textures each tile type has
    /// @param  data    the json data to read from
    void TilemapTextureConnector::readTexturesPerTile( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_TexturesPerTile, data );
    }


    /// @brief  the texture offsets to use for the top left tile
    /// @param  data    the json data to read from
    void TilemapTextureConnector::readTopLeftTextures( nlohmann::ordered_json const& data )
    {
        for ( int i = 0; i < data.size(); ++i )
        {
            Stream::Read( m_TopLeftTextures[ i ], data[ i ] );
        }
    }

    /// @brief  the texture offsets to use for the top right tile
    /// @param  data    the json data to read from
    void TilemapTextureConnector::readTopRightTextures( nlohmann::ordered_json const& data )
    {
        for ( int i = 0; i < data.size(); ++i )
        {
            Stream::Read( m_TopRightTextures[ i ], data[ i ] );
        }
    }

    /// @brief  the texture offsets to use for the bottom left tile
    /// @param  data    the json data to read from
    void TilemapTextureConnector::readBottomLeftTextures( nlohmann::ordered_json const& data )
    {
        for ( int i = 0; i < data.size(); ++i )
        {
            Stream::Read( m_BottomLeftTextures[ i ], data[ i ] );
        }
    }

    /// @brief  the texture offsets to use for the bottom right tile
    /// @param  data    the json data to read from
    void TilemapTextureConnector::readBottomRightTextures( nlohmann::ordered_json const& data )
    {
        for ( int i = 0; i < data.size(); ++i )
        {
            Stream::Read( m_BottomRightTextures[ i ], data[ i ] );
        }
    }
    


//-----------------------------------------------------------------------------
// public: reading / writing
//-----------------------------------------------------------------------------


    /// @brief  gets the map of read methods for this TilemapTextureConnector
    /// @return the map of read methods for this TilemapTextureConnector
    ReadMethodMap< ISerializable > const& TilemapTextureConnector::GetReadMethods() const
    {
        static ReadMethodMap< TilemapTextureConnector > const readMethods = {
            { "FirstTileOffset"    , &TilemapTextureConnector::readFirstTileOffset     },
            { "TexturesPerTile"    , &TilemapTextureConnector::readTexturesPerTile     },
            { "TopLeftTextures"    , &TilemapTextureConnector::readTopLeftTextures     },
            { "TopRightTextures"   , &TilemapTextureConnector::readTopRightTextures    },
            { "BottomLeftTextures" , &TilemapTextureConnector::readBottomLeftTextures  },
            { "BottomRightTextures", &TilemapTextureConnector::readBottomRightTextures }
        };

        return (ReadMethodMap< ISerializable > const&)readMethods;
    }


    /// @brief  Write all TilemapTextureConnector data to a JSON file.
    /// @return The JSON file containing the TilemapTextureConnector data.
    nlohmann::ordered_json TilemapTextureConnector::Write() const
    {
        nlohmann::ordered_json json;

        json[ "FirstTileOffset" ] = Stream::Write( m_FirstTileOffset );
        json[ "TexturesPerTile" ] = Stream::Write( m_TexturesPerTile );

        nlohmann::ordered_json& topLeft = json[ "TopLeftTextures" ];
        for ( int const& element : m_TopLeftTextures )
        {
            topLeft.push_back( Stream::Write( element ) );
        }

        nlohmann::ordered_json& topRight = json[ "TopRightTextures" ];
        for ( int const& element : m_TopRightTextures )
        {
            topRight.push_back( Stream::Write( element ) );
        }

        nlohmann::ordered_json& bottomLeft = json[ "BottomLeftTextures" ];
        for ( int const& element : m_BottomLeftTextures )
        {
            bottomLeft.push_back( Stream::Write( element ) );
        }

        nlohmann::ordered_json& bottomRight = json[ "BottomRightTextures" ];
        for ( int const& element : m_BottomRightTextures )
        {
            bottomRight.push_back( Stream::Write( element ) );
        }

        return json;
    }



//-----------------------------------------------------------------------------
// public: constructor / Destructor
//-----------------------------------------------------------------------------


    /// @brief  constructor
    TilemapTextureConnector::TilemapTextureConnector() :
        Component( typeid( TilemapTextureConnector ) )
    {}


//-----------------------------------------------------------------------------
// public: copying
//-----------------------------------------------------------------------------


    /// @brief  clones this TilemapTextureConnector
    /// @return the newly created clone of this TilemapTextureConnector
    TilemapTextureConnector* TilemapTextureConnector::Clone() const
    {
        return new TilemapTextureConnector( *this );
    }


//-----------------------------------------------------------------------------
// private: copying
//-----------------------------------------------------------------------------


    /// @brief  copy-constructor for the TilemapTextureConnector
    /// @param  other   the other TilemapTextureConnector to copy
    TilemapTextureConnector::TilemapTextureConnector( TilemapTextureConnector const& other ) :
        Component( other ),
        m_FirstTileOffset( other.m_FirstTileOffset ),
        m_TexturesPerTile( other.m_TexturesPerTile )
    {
        for ( int i = 0; i < 8; ++i )
        {
            m_TopLeftTextures    [ i ] = other.m_TopLeftTextures    [ i ];
            m_TopRightTextures   [ i ] = other.m_TopRightTextures   [ i ];
            m_BottomLeftTextures [ i ] = other.m_BottomLeftTextures [ i ];
            m_BottomRightTextures[ i ] = other.m_BottomRightTextures[ i ];
        }
    }


//-----------------------------------------------------------------------------
