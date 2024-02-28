/// @file       Text.h
/// @author     Eli Tsereteli (ilya.tsereteli@digipen.edu)
/// @brief      Component that makes use of Tilemap to draw text.
/// @version    0.1
/// @date       2023
/// 
/// @copyright  Copyright (c) 2024 Digipen Institute of Technology


#pragma once

#include "Component.h"

#include "ComponentReference.h"
#include "Tilemap.h"


/// @brief  Component that makes use of Tilemap to draw text.
class Text : public Component
{
//-----------------------------------------------------------------------------
public: // constructor
//-----------------------------------------------------------------------------


    /// @brief   Default constructor
    Text();


//-----------------------------------------------------------------------------
public: // accessors
//-----------------------------------------------------------------------------


    /// @brief  gets the text of this Text
    /// @return the text of this Text
    std::string const& GetText() const;


    /// @brief          sets the text of this Text
    /// @param  text    the text of this Text
    void SetText( std::string const& text );


//-----------------------------------------------------------------------------
public: // virutal override methods
//-----------------------------------------------------------------------------


    /// @brief  called when entering a scene
    virtual void OnInit() override;

    /// @brief  called when exiting a scene
    virtual void OnExit() override;


//-----------------------------------------------------------------------------
private: // members
//-----------------------------------------------------------------------------


    /// @brief  text displayed by the textSprite
    std::string m_Text;

    
    /// @brief  the Tilemap Component attached to this Entity
    ComponentReference< Tilemap< int > > m_Tilemap;


//-----------------------------------------------------------------------------
private: // methods
//-----------------------------------------------------------------------------


    /// @brief  Loads text array into TilemapSprite of the parent for rendering.
    void loadTextIntoTilemap();


//-----------------------------------------------------------------------------
public: // inspection
//-----------------------------------------------------------------------------


    /// @brief  inspects this Text Component
    virtual void Inspector() override;


//-----------------------------------------------------------------------------
private: // Reading
//-----------------------------------------------------------------------------


    /// @brief  Read in the text this Text displays
    /// @param  stream  The json to read from.
    void readText( nlohmann::ordered_json const& data );


//-----------------------------------------------------------------------------
public: // reading / writing
//-----------------------------------------------------------------------------


    /// @brief gets the map of read methods for this Component
    /// @return the map of read methods for this Component
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override;


    /// @brief Write all Text component data to a JSON file.
    /// @return The JSON file containing the Text component data.
    virtual nlohmann::ordered_json Write() const override;

    
//-----------------------------------------------------------------------------
public: // copying
//-----------------------------------------------------------------------------


    /// @brief  makes a copy of this Text Component
    /// @return a copy of this Text Component
    virtual Text* Clone() const override;

    
//-----------------------------------------------------------------------------
private: // copying
//-----------------------------------------------------------------------------


    /// @brief  copy constructor
    /// @param  other   the Text component to copy
    Text( Text const& other );


    // disable assignment
    void operator =( Text const& ) = delete;


//-----------------------------------------------------------------------------
};
