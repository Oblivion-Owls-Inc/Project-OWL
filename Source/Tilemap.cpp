/// @file       Tilemap.cpp
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      Tilemap component
/// @version    0.1
/// @date       2023-10-06
/// 
/// @copyright  Copyright (c) 2023 Digipen Institute of Technology

#include "pch.h" // precompiled header has to be included first
#include "Tilemap.h"

// @brief  inspector for int tilemaps
template<>
void Tilemap<int>::Inspector()
{
    int width = GetDimensions().x;
    int totalTiles = GetDimensions().x * GetDimensions().y;

    glm::ivec2 dimensions = GetDimensions();
    if ( ImGui::DragInt2( "Tilemap Size", &dimensions[0], 0.05f, 1, INT_MAX ) )
    {
        SetDimensions( dimensions );
        callOnTilemapChangedCallbacks();
    }

    glm::vec2 tileSize = GetTileScale();
    if ( ImGui::DragFloat2( "Tile Scale", &tileSize[ 0 ], 0.05f ) )
    {
        SetTileScale( tileSize );
    }

    static std::string buffer;
    ImGui::InputTextMultiline( "Tilemap Data CSV", &buffer );

    if ( ImGui::Button( "Tilemap to CSV" ) )
    {
        buffer.clear();
        for ( int i = 0; i < m_Tiles.size(); ++i )
        {
            buffer += std::to_string( m_Tiles[ i ] );
            buffer += ( i % m_Dimensions.x == m_Dimensions.x - 1 ) ? '\n' : ',';
        }

        buffer.resize( buffer.size() * 2 );
    }

    if ( ImGui::Button( "CSV to Tilemap" ) )
    {
        m_Dimensions = glm::ivec2( 0 );
        m_Tiles.clear();

        std::istringstream csvData( buffer );
        csvData >> std::noskipws;

        while ( true )
        {
            int tile;
            csvData >> tile;
            m_Tiles.push_back( tile );

            char c;
            if ( !csvData.get( c ) )
            {
                break;
            }

            if ( c == '\n' && m_Dimensions.x == 0 )
            {
                m_Dimensions.x = (int)m_Tiles.size();
            }
        }

        if ( m_Dimensions.x != 0 )
        {
            m_Dimensions.y = (int)m_Tiles.size() / m_Dimensions.x;

            int extraTiles = m_Tiles.size() % m_Dimensions.x;
            m_Tiles.resize( m_Tiles.size() + ( extraTiles == 0 ? 0 : m_Dimensions.x - extraTiles ) );
        }

        callOnTilemapChangedCallbacks();
    }
}