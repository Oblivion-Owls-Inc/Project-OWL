/// @file     Text.cpp
/// @author   Eli Tsereteli (ilya.tsereteli@digipen.edu)
/// 
/// @brief    A version of Sprite specifically for rendering text. Uses instancing to draw multiple 
///           letters simultaneously.
#include "Text.h"
#include "Entity.h"
#include "TilemapSprite.h"

/// @brief   default constructor
Text::Text() :
    Component( typeid( Text ) )
{}

/// @return  copy of this component
Component * Text::Clone() const
{
    return new Text(*this);
}


/// @brief      Loads text data into tilemap sprite.
void Text::loadTextIntoSprite()
{
    // Make sure this Text has parent, and parent has TilemapSprite
    Entity* parent = GetParent();
    if (!parent)
        return;

    TilemapSprite* ts =  parent->GetComponent<TilemapSprite>();
    if (!ts)
        return;

    // Convert to zero-index spritesheet indices, then load in
    std::string tiles = m_Text;
    int size = (int)tiles.size();
    for (int i=0; i<size; i++)
        tiles[i] -= 32;

    ts->LoadTileArray(tiles.c_str(), size);
}


/// @brief  called when entering a scene
void Text::OnInit()
{
    loadTextIntoSprite();
}


//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------

/// @brief          Read in the text this Text displays
/// @param  data  The json to read from.
void Text::readText( nlohmann::json const& data )
{
    m_Text = Stream::Read<std::string>(data);       
}


/// @brief the map of read methods for this Component
ReadMethodMap< Text > const Text::s_ReadMethods = {
    { "String"            , &readText  }
};

//-----------------------------------------------------------------------------
