/// @file     Tilemap.cpp
/// @author   Eli Tsereteli (ilya.tsereteli@digipen.edu)
/// 
/// @brief    Tilemap component - loads and manages a tilemap array.
#include "Tilemap.h"
#include "Entity.h"
#include "TilemapSprite.h"
#include "Transform.h"

/// @brief    Default constructor
Tilemap::Tilemap() :
    Component( typeid( Tilemap ) )
{}


/// @return   A copy of this component
Component * Tilemap::Clone() const
{
    return new Tilemap(*this);
}



//-----------------------------------------------------------------------------
//          Public methods
//-----------------------------------------------------------------------------

/// @brief          Sets the whole tilemap to a given array (vector)
/// @param  tiles   vector of tile IDs
void Tilemap::SetTilemap( std::vector<int> const& tiles ) 
{
    m_Tilemap = tiles;
    loadTilemapIntoSprite();
} 


/// @brief          Sets the tile at given coordinate to given index.
/// @param x        column
/// @param y        row
/// @param tileID   index to change the tile to
void Tilemap::SetTile(glm::ivec2 coord, int tileID)
{
    m_Tilemap[coord.y*m_RowWidth + coord.x] = tileID;
    loadTilemapIntoSprite();
}



/// @brief          Gets tilemap coordinate of the tile at given world position.
/// @param pos      Position in world space
/// @return         Tilemap coordinate of the tile. If the provided location
///                 is outside the tilemap, returns (-1,-1)
glm::ivec2 Tilemap::WorldPosToTileCoord(glm::vec2 pos)
{
    updateMat();

    // convert to tilemap coordinate
    glm::vec4 pos4(pos, 0, 1);
    glm::ivec2 coord = glm::round( glm::vec2(m_InvMat * pos4) );
    coord.y = -coord.y;

    // return it if it's within the tilemap, or (-1,-1) otherwise
    int i = coord.y*m_RowWidth + coord.x;
    if (coord.x >= 0 && i >= 0 && coord.x < m_RowWidth && i < (int)m_Tilemap.size())
        return coord;
    else
        return {-1,-1};
}


/// @brief          Gets world position of the given tilemap coordinates.
/// @param coord    Tilemap coordinate
/// @return         World position of the tile at given tilemap coordinate
glm::vec2 Tilemap::TileCoordToWorldPos(glm::ivec2 coord)
{
    updateMat();

    glm::vec4 pos4(coord, 0, 1);
    
    return glm::vec2(m_Mat * pos4);
}


//-----------------------------------------------------------------------------
//          Virtual overrides
//-----------------------------------------------------------------------------

/// @brief  called when entering a scene
void Tilemap::OnInit()
{
    loadTilemapIntoSprite();

    updateMat();
}



//-----------------------------------------------------------------------------
//              Helpers
//-----------------------------------------------------------------------------

/// @brief      Loads text data into tilemap sprite.
void Tilemap::loadTilemapIntoSprite()
{
    // Make sure this Tilemap has parent, and parent has TilemapSprite
    Entity* parent = GetParent();
    if (!parent)
        return;

    TilemapSprite* ts =  parent->GetComponent<TilemapSprite>();
    if (!ts)
        return;

    ts->LoadTileArray(m_Tilemap);
    m_RowWidth = ts->GetRowWidth();
}



/// @brief  Updates inverse transform matrix, if parent's transform has changed.
void Tilemap::updateMat()
{
    Transform* tr = GetParent()->GetComponent<Transform>();
    if (!tr || tr->GetMatrix() == m_Mat)
        return;

    m_Mat = tr->GetMatrix();
    
    // account for stride multiplier
    TilemapSprite* ts = GetParent()->GetComponent<TilemapSprite>();
    if (ts)
    {
        glm::vec2 sm = ts->GetStrideMultiplier();
        m_Mat[0][0] *= sm.x;
        m_Mat[1][1] *= sm.y;
    }

    m_InvMat = glm::inverse(m_Mat);
}


//-----------------------------------------------------------------------------
//              Reading
//-----------------------------------------------------------------------------

/// @brief          Read in the text this Tilemap displays
/// @param  stream  The json to read from.
void Tilemap::readTilemap( nlohmann::ordered_json const& data )
{
    for (int i = 0; i < data.size(); ++i)
    {
        int ID = Stream::Read<int>(data[i]); // for debugging
        m_Tilemap.push_back(ID);
    }
}


/// @brief      The map of read methods for this Component
ReadMethodMap< Tilemap > const Tilemap::s_ReadMethods = {
    { "TileData", &readTilemap  }
};
