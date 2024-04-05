/////////////////////////////////////////////////////////////////////////////////
/// @file       SceneChangeButton.cpp
/// @author     Jax Clayton (jax.clayton@digipen.edu)
/// @brief      Component that changes the scene when clicked
/// @version    0.1
/// @date       2/21/2024
/// 
/// @copyright  Copyright (c) 2024 Digipen Institute of Technology
/////////////////////////////////////////////////////////////////////////////////

#include "pch.h" // precompiled header has to be included first
#include "SceneChangeButton.h"
#include "InputSystem.h"
#include "SceneSystem.h"
#include "DebugSystem.h"

#include "ComponentReference.t.h"
#include "SceneTransition.h"

#include "Sprite.h"

//-----------------------------------------------------------------------------
// public: constructor / Destructor
//-----------------------------------------------------------------------------

 
    /// @brief  default constructor 
    SceneChangeButton::SceneChangeButton() :
        Component(typeid(SceneChangeButton))
    {}
  

//-----------------------------------------------------------------------------
// public: methods
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// public: accessors
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// public: virtual override methods
//-----------------------------------------------------------------------------


    /// @brief  called once when entering the scene
    void SceneChangeButton::OnInit()
    {
        /// Set the filter function for the listener
        m_Listener.SetFilterFunction([&](std::string const& EventName) -> bool
	    {
                return EventName == m_EventName;
	    });

        /// Set the Callback function for the listener
        m_Listener.SetResponseFunction([&](std::string const& EventName)
        {
                onButtonClick();
        });

        m_Listener.Init();

        m_SceneTransitionEntity.SetOwnerName( GetName() );
        m_SceneTransitionEntity.Init();
    }

    /// @brief  called once when exiting the scene
    void SceneChangeButton::OnExit()
    {
        m_Listener.Exit();

        m_SceneTransitionEntity.Exit();
    }


//-----------------------------------------------------------------------------
// private: methods
//-----------------------------------------------------------------------------

    /// @brief  called when the button is clicked to change the scene
    void SceneChangeButton::onButtonClick() const
    {
        if ( m_SceneTransition != nullptr )
        {
            m_SceneTransition->StartTransition( m_SceneName );
        }
    }

//-----------------------------------------------------------------------------
// public: inspection
//-----------------------------------------------------------------------------


    /// @brief  shows the inspector for SceneChangeButton
    void SceneChangeButton::Inspector()
    {
        ImGui::InputText( "Event Name" , &m_EventName );

        Scenes()->InspectorSelectScene( "scene name", &m_SceneName );

        m_SceneTransitionEntity.Inspect( "scene transition entity" );
    }


//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------

    /// @brief  reads the scene name from a JSON file
    void SceneChangeButton::readSceneName(nlohmann::ordered_json const& data)
    {
        Stream::Read( m_SceneName, data );
    }

    /// @brief  reads the event name from a JSON file
    void SceneChangeButton::readEventName(nlohmann::ordered_json const& data)
    {
        Stream::Read( m_EventName, data );
    }

    /// @brief  reads the Entity the SceneTransition Component is attached to
    /// @param  data    the JSON data to read from
    void SceneChangeButton::readSceneTransitionEntity( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_SceneTransitionEntity, data );
    }


//-----------------------------------------------------------------------------
// public: reading / writing
//-----------------------------------------------------------------------------


    /// @brief  gets the map of read methods for this SceneChangeButton
    /// @return the map of read methods for this SceneChangeButton
    ReadMethodMap< ISerializable > const& SceneChangeButton::GetReadMethods() const
    {
        static ReadMethodMap< SceneChangeButton > const readMethods = {
		    { "SceneName"            , &SceneChangeButton::readSceneName             },
            { "EventName"            , &SceneChangeButton::readEventName             },
            { "SceneTransitionEntity", &SceneChangeButton::readSceneTransitionEntity }
        };

        return (ReadMethodMap< ISerializable > const&)readMethods;
    }


    /// @brief  Writes all SceneChangeButton data to a JSON file.
    /// @return The JSON file containing the data.
    nlohmann::ordered_json SceneChangeButton::Write() const
    {
        nlohmann::ordered_json json;

        json[ "SceneName"             ] = Stream::Write( m_SceneName             );
        json[ "EventName"             ] = Stream::Write( m_EventName             );
        json[ "SceneTransitionEntity" ] = Stream::Write( m_SceneTransitionEntity );

        return json;
    }


//-----------------------------------------------------------------------------
// public: copying
//-----------------------------------------------------------------------------


    /// @brief  clones this SceneChangeButton
    /// @return the newly created clone of this SceneChangeButton
    SceneChangeButton* SceneChangeButton::Clone() const
    {
        return new SceneChangeButton( *this );
    }


//-----------------------------------------------------------------------------
// private: copying
//-----------------------------------------------------------------------------


    /// @brief  copy-constructor for the SceneChangeButton
    /// @param  other   the other SceneChangeButton to copy
    SceneChangeButton::SceneChangeButton(SceneChangeButton const& other) :
        Component( other ),
        m_SceneName            ( other.m_SceneName ),
        m_EventName            ( other.m_EventName ),
        m_SceneTransitionEntity( other.m_SceneTransitionEntity, { &m_SceneTransition } )
    {}


//-----------------------------------------------------------------------------
