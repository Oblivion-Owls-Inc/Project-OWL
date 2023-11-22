/// @file       Tilemap.cpp
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      Tilemap component
/// @version    0.1
/// @date       2023-10-06
/// 
/// @copyright  Copyright (c) 2023 Digipen Institute of Technology

#include "Tilemap.h"

// @brief  inspector for int tilemaps
template<>
void Tilemap<int>::Inspector()
{
    int width = GetDimensions().x;
    int totalTiles = GetDimensions().x * GetDimensions().y;

    static std::string buffer;
    ImGui::InputText( "Tilemap Data CSV", &buffer );

    if ( ImGui::Button( "Tilemap to CSV" ) )
    {
        buffer.clear();
        for ( int i = 0; i < m_Tilemap.size(); ++i )
        {
            buffer += std::to_string( m_Tilemap[ i ] );
            buffer += ( i % m_Dimensions.x != m_Dimensions.x - 1 ) ? ',' : '\n';
        }

        buffer.resize( buffer.size() * 2 );
    }

    if ( ImGui::Button( "CSV to Tilemap" ) )
    {
        m_Dimensions = glm::ivec2( 0 );
        m_Tilemap.clear();

        std::istringstream csvData( buffer );
        csvData >> std::noskipws;

        int i = 1;
        while ( true )
        {
            int tile;
            csvData >> tile;
            if ( tile == -1 )
            {
                tile = 0;
            }
            m_Tilemap.push_back( tile );

            char c;
            if ( !csvData.get( c ) )
            {
                break;
            }

            if ( c == '\n' && m_Dimensions.x == 0 )
            {
                m_Dimensions.x = i;
            }

            ++i;
        }

        if ( m_Dimensions.x != 0 )
        {
            m_Dimensions.y = (int)m_Tilemap.size() / m_Dimensions.x;
        }

        m_Modified = true;
    }
}