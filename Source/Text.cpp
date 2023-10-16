/// @file     Text.cpp
/// @author   Eli Tsereteli (ilya.tsereteli@digipen.edu)
/// 
/// @brief    Component that makes use of Tilemap to draw text.
#include "Text.h"
#include "Entity.h"
#include "Tilemap.h"

// TODO: update when text (Tilemap) changes

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
void Text::loadTextIntoTilemap()
{
    // Make sure this Text has parent, and parent has Tilemap
    Entity* parent = GetParent();
    if (!parent)
        return;

    Tilemap<int>* tm =  parent->GetComponent<Tilemap<int>>();
    if (!tm)
        return;

    // Convert to zero-index spritesheet indices, then load in
    int size = (int)m_Text.size();
    std::vector<int> tiles(size);
    for (int i=0; i<size; i++)
        tiles[i] = (int)m_Text[i] - 32;

    tm->SetTilemap(tiles);
}


/// @brief  called when entering a scene
void Text::OnInit()
{
    loadTextIntoTilemap();
}


//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------

/// @brief          Read in the text this Text displays
/// @param  data  The json to read from.
void Text::readText( nlohmann::ordered_json const& data )
{
    m_Text = Stream::Read<std::string>(data);       
}

/// @brief Write all Text component data to a JSON file.
/// @return The JSON file containing the Text component data.
nlohmann::ordered_json Text::Write() const
{
    nlohmann::ordered_json data;

    data["String"] = m_Text;

    return data;
}


/// @brief the map of read methods for this Component
ReadMethodMap< Text > const Text::s_ReadMethods = {
    { "String"            , &readText  }
};

//-----------------------------------------------------------------------------
