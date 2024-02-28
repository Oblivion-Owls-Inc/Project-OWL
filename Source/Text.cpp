/// @file       Text.cpp
/// @author     Eli Tsereteli (ilya.tsereteli@digipen.edu)
/// @brief      Component that makes use of Tilemap to draw text.
/// @version    0.1
/// @date       2023
/// 
/// @copyright  Copyright (c) 2024 Digipen Institute of Technology


#include "Text.h"

#include "ComponentReference.t.h"
#include "Tilemap.h"

#include "imgui_stdlib.h"


//-----------------------------------------------------------------------------
// public: constructor
//-----------------------------------------------------------------------------


    /// @brief   default constructor
    Text::Text() :
        Component( typeid( Text ) )
    {}

    
//-----------------------------------------------------------------------------
// public: accessors
//-----------------------------------------------------------------------------


    /// @brief  gets the text of this Text
    /// @return the text of this Text
    std::string const& Text::GetText() const
    {
        return m_Text;
    }


    /// @brief          sets the text of this Text
    /// @param  text    the text of this Text
    void Text::SetText( std::string const& text )
    {
        m_Text = text;
        loadTextIntoTilemap();
    }

    
//-----------------------------------------------------------------------------
// public: virutal override methods
//-----------------------------------------------------------------------------


    /// @brief  called when entering a scene
    void Text::OnInit()
    {
        m_Tilemap.Init( GetEntity() );

        loadTextIntoTilemap();
    }

    /// @brief  called when exiting a scene
    void Text::OnExit()
    {
        m_Tilemap.Exit();
    }


//-----------------------------------------------------------------------------
// private: methods
//-----------------------------------------------------------------------------


    /// @brief  Loads text data into tilemap sprite.
    void Text::loadTextIntoTilemap()
    {
        if ( m_Tilemap == nullptr )
        {
            return;
        }

        // Convert to zero-index spritesheet indices, then load in
        int size = (int)m_Text.size();
        std::vector< int > tiles( size );
        for ( int i = 0; i < size; i++ )
        {
            tiles[ i ] = (int)m_Text[ i ] - 32;
        }

        m_Tilemap->SetTilemap( tiles );
    }


//-----------------------------------------------------------------------------
// public: inspection
//-----------------------------------------------------------------------------


    /// @brief  inspects this Text Component
    void Text::Inspector()
    {
        if ( ImGui::InputText( "text", &m_Text ) )
        {
            loadTextIntoTilemap();
        }
    }


//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------


    /// @brief  reads in the text this Text displays
    /// @param  data    the json to read from
    void Text::readText( nlohmann::ordered_json const& data )
    {
         Stream::Read( m_Text, data );       
    }

    
//-----------------------------------------------------------------------------
// public: reading / writing
//-----------------------------------------------------------------------------


    /// @brief gets the map of read methods for this Component
    /// @return the map of read methods for this Component
    ReadMethodMap< ISerializable > const& Text::GetReadMethods() const
    {
        static ReadMethodMap< Text > const readMethods = {
            { "Text", &Text::readText }
        };

        return (ReadMethodMap< ISerializable > const&)readMethods;
    }


    /// @brief Write all Text component data to a JSON file.
    /// @return The JSON file containing the Text component data.
    nlohmann::ordered_json Text::Write() const
    {
        nlohmann::ordered_json json;

        json[ "Text" ] = Stream::Write( m_Text );

        return json;
    }

    
//-----------------------------------------------------------------------------
// public: copying
//-----------------------------------------------------------------------------


    /// @return  copy of this component
    Text* Text::Clone() const
    {
        return new Text(*this);
    }

    
//-----------------------------------------------------------------------------
// private: copying
//-----------------------------------------------------------------------------


    /// @brief  copy constructor
    /// @param  other   the Text component to copy
    Text::Text( Text const& other ) :
        Component( other )
    {}


//-----------------------------------------------------------------------------
