/// @file     Tilemap.cpp
/// @author   Eli Tsereteli (ilya.tsereteli@digipen.edu)
/// 
/// @brief    A version of Sprite specifically for rendering text. Uses instancing to draw multiple 
///           letters simultaneously.
#include "Tilemap.h"
#include "Entity.h"
#include "TilemapSprite.h"

/// @brief   default constructor
Tilemap::Tilemap() :
    Component( typeid( Tilemap ) )
{}

/// @return  copy of this component
Component * Tilemap::Clone() const
{
    return new Tilemap(*this);
}


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


/// @brief  called when entering a scene
void Tilemap::OnInit()
{
    loadTilemapIntoSprite();
}


/// @brief          Sets the tile at given coordinate to given index.
/// @param x        column
/// @param y        row
/// @param tileID   index to change the tile to
void Tilemap::SetTile(int x, int y, int tileID)
{
    m_Tilemap[y*m_RowWidth + x] = tileID;
    loadTilemapIntoSprite();
}


//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------

/// @brief          Read in the text this Tilemap displays
/// @param  stream  The json to read from.
void Tilemap::readTilemap(Stream stream)
{
    for (auto& tileData : stream.GetArray())
    {
        int ID = Stream(tileData).Read<int>(); // for debugging
        m_Tilemap.push_back(ID);
    }
}


/// @brief the map of read methods for this Component
ReadMethodMap< Tilemap > const Tilemap::s_ReadMethods = {
    { "TileData", &readTilemap  }
};

/// @brief gets the map of read methods for this Component
/// @return the map of read methods for this Component
ReadMethodMap< Component > const& Tilemap::GetReadMethods() const
{
    return (ReadMethodMap< Component> const&)s_ReadMethods;
}

//-----------------------------------------------------------------------------
