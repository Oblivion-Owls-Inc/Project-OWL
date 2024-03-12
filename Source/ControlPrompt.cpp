/// @file       ControlPrompt.cpp
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      Displays a control prompt for an ActionReference
/// @version    0.1
/// @date       2024-03-11
/// 
/// @copyright  Copyright (c) 2024 Digipen Institute of Technology


#include "pch.h"

#include "ControlPrompt.h"

#include "ComponentReference.t.h"
#include "Sprite.h"


//-----------------------------------------------------------------------------
// public: constructor / Destructor
//-----------------------------------------------------------------------------


    /// @brief  default constructor
    ControlPrompt::ControlPrompt() :
        Component( typeid( ControlPrompt ) )
    {}


//-----------------------------------------------------------------------------
// public: methods
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// public: accessors
//-----------------------------------------------------------------------------


    /// @brief  gets the Sprite Component attached
    /// @return the Sprite Component attached
    Sprite* ControlPrompt::GetSprite()
    {
        return m_Sprite;
    }


//-----------------------------------------------------------------------------
// public: virtual override methods
//-----------------------------------------------------------------------------


    /// @brief  called once when entering the scene
    void ControlPrompt::OnInit()
    {
        m_Sprite.SetOnConnectCallback(
            [ this ]() {
                updateSprite();
            }
        );

        m_Action.SetOwnerName( GetName() );
        m_Action.Init();

        m_Sprite.Init( GetEntity() );
    }

    /// @brief  called once when exiting the scene
    void ControlPrompt::OnExit()
    {
        m_Sprite.Exit();
    }


//-----------------------------------------------------------------------------
// private: methods
//-----------------------------------------------------------------------------


    /// @brief  updates the attached Sprite to match the attached Action
    /// @brief  assumes m_Sprite is non-null, doesn't assume m_Action
    void ControlPrompt::updateSprite()
    {
        // TODO
    }


//-----------------------------------------------------------------------------
// public: inspection
//-----------------------------------------------------------------------------


    /// @brief  shows the inspector for ControlPrompt
    void ControlPrompt::Inspector()
    {
        if ( m_Action.Inspect( "Control Action" ) )
        {
            if ( m_Sprite != nullptr )
            {
                updateSprite();
            }
        }
    }


//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------


    /// @brief  read the Action to display a prompt for
    /// @param  data    the JSON data to read from
    void ControlPrompt::readAction( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_Action, data );
    }


//-----------------------------------------------------------------------------
// public: reading / writing
//-----------------------------------------------------------------------------


    /// @brief  gets the map of read methods for this ControlPrompt
    /// @return the map of read methods for this ControlPrompt
    ReadMethodMap< ISerializable > const& ControlPrompt::GetReadMethods() const
    {
        static ReadMethodMap< ControlPrompt > const readMethods = {
            { "Action", &ControlPrompt::readAction }
        };

        return (ReadMethodMap< ISerializable > const&)readMethods;
    }


    /// @brief  writes this ControlPrompt to JSON
    /// @return the JSON data of this ControlPrompt
    nlohmann::ordered_json ControlPrompt::Write() const
    {
        nlohmann::ordered_json json = nlohmann::ordered_json::object();

        json[ "Action" ] = Stream::Write( m_Action );

        return json;
    }

    
//-----------------------------------------------------------------------------
// public: copying
//-----------------------------------------------------------------------------


    /// @brief  clones this ControlPrompt
    /// @return the newly created clone of this ControlPrompt
    ControlPrompt* ControlPrompt::Clone() const
    {
        return new ControlPrompt( *this );
    }


//-----------------------------------------------------------------------------
// private: copying
//-----------------------------------------------------------------------------


    /// @brief  copy-constructor for the ControlPrompt
    /// @param  other   the other ControlPrompt to copy
    ControlPrompt::ControlPrompt( ControlPrompt const& other ) :
        Component( other ),
        m_Action( other.m_Action )
    {}


//-----------------------------------------------------------------------------
