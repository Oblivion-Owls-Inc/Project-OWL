/// @file       SceneTransition.cpp
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      fades a texture out and in when transitioning between scenes
/// @version    0.1
/// @date       DATE HERE
/// 
/// @copyright  Copyright (c) 2024 Digipen Institute of Technology

#include "pch.h"

#include "SceneTransition.h"

#include "LightingSystem.h"
#include "BehaviorSystem.h"

#include "ComponentReference.t.h"
#include "Sprite.h"

#include "SceneSystem.h"
#include "Engine.h"


//-----------------------------------------------------------------------------
// public: constructor / Destructor
//-----------------------------------------------------------------------------


    /// @brief  default constructor
    SceneTransition::SceneTransition() :
        Behavior( typeid( SceneTransition ) )
    {}


//-----------------------------------------------------------------------------
// public: methods
//-----------------------------------------------------------------------------

    
    /// @brief  starts transitioning to the specified scene
    /// @param  nextSceneName   the name of the scene to transition to
    void SceneTransition::StartTransition( std::string const& nextSceneName )
    {
        SetNextSceneName( nextSceneName );
        StartTransition();
    }

    /// @brief  starts transitioning to the next scene
    void SceneTransition::StartTransition()
    {
        if ( m_NextSceneName == "" )
        {
            Debug() << "WARNING: cannot transition to a scene without a name (" << GetName() << ")" << std::endl;
            return;
        }

        m_State = State::Out;
    }


//-----------------------------------------------------------------------------
// public: accessors
//-----------------------------------------------------------------------------


    /// @brief  gets the name of the scene this SceneTransition will transition to
    /// @return the name of the scene this SceneTransition will transition to
    std::string const& SceneTransition::GetNextSceneName() const
    {
        return m_NextSceneName;
    }

    /// @brief  sets the name of the scene this SceneTransition will transition to
    /// @param  nextSceneName   the name of the scene this SceneTransition will transition to
    void SceneTransition::SetNextSceneName( std::string const& nextSceneName )
    {
        m_NextSceneName = nextSceneName;
    }


//-----------------------------------------------------------------------------
// public: virtual override methods
//-----------------------------------------------------------------------------


    /// @brief  called once when entering the scene
    void SceneTransition::OnInit()
    {
        Behaviors< SceneTransition >()->AddComponent( this );

        m_Sprite.Init( GetEntity() );

        m_State = State::In;
        m_TransitionTimer = m_TransitionDuration;
        if ( m_Sprite != nullptr )
        {
            #ifdef DEBUG
                m_Sprite->SetOpacity( 0.0f );
            #else
                m_Sprite->SetOpacity( 1.0f );
            #endif
        }

        Lights()->SetLightingEnabled( m_HasLighting );
        Lights()->SetShadowLayer( m_LightingLayer );
    }

    /// @brief  called once when exiting the scene
    void SceneTransition::OnExit()
    {
        Behaviors< SceneTransition >()->RemoveComponent( this );

        m_Sprite.Exit();

        Lights()->SetLightingEnabled(false);
    }


    /// @brief  called once every graphics frame
    void SceneTransition::OnFixedUpdate()
    {
        switch ( m_State )
        {
        case State::In:
            m_TransitionTimer -= GameEngine()->GetFixedFrameDuration();
            if ( m_TransitionTimer <= 0.0f )
            {
                m_TransitionTimer = 0.0f;
                m_State = State::Idle;
            }
            break;
        case State::Out:
            m_TransitionTimer += GameEngine()->GetFixedFrameDuration();
            if ( m_TransitionTimer >= m_TransitionDuration )
            {
                m_TransitionTimer = m_TransitionDuration;
                Scenes()->SetNextScene( m_NextSceneName );
            }
            break;
        default:
            return;
        }

        if ( m_Sprite != nullptr )
        {
            m_Sprite->SetOpacity( m_TransitionTimer / m_TransitionDuration );
        }
    }


//-----------------------------------------------------------------------------
// private: methods
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// public: inspection
//-----------------------------------------------------------------------------


    /// @brief  shows the inspector for SceneTransition
    void SceneTransition::Inspector()
    {
        ImGui::DragFloat( "transition duration", &m_TransitionDuration, 0.05f, 0.0f, INFINITY );
        ImGui::DragInt( "lighting layer", &m_LightingLayer, 1, 0, 100 );
        ImGui::Checkbox("has lighting", &m_HasLighting);

        Scenes()->InspectorSelectScene( "transition to scene", &m_NextSceneName );
    }


//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------


    /// @brief  reads how long the transition out of/into this frame lasts
    /// @param  data    the JSON data to read from
    void SceneTransition::readTransitionDuration( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_TransitionDuration, data );
    }

    /// @brief  reads the name of the scene this SceneTransition will transition to
    /// @param  data    the JSON data to read from
    void SceneTransition::readNextSceneName( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_NextSceneName, data );
    }

    /// @brief  reads whether the scene has lighting
    /// @param  data    the JSON data to read from
    void SceneTransition::readHasLighting(nlohmann::ordered_json const& data)
    {
		Stream::Read(m_HasLighting, data);
	}

    /// @brief  reads the lighting layer of the next scene
    /// @param  data    the JSON data to read from
    void SceneTransition::readLightingLayer(nlohmann::ordered_json const& data)
    {
        Stream::Read(m_LightingLayer, data);
    }


//-----------------------------------------------------------------------------
// public: reading / writing
//-----------------------------------------------------------------------------


    /// @brief  gets the map of read methods for this SceneTransition
    /// @return the map of read methods for this SceneTransition
    ReadMethodMap< ISerializable > const& SceneTransition::GetReadMethods() const
    {
        static ReadMethodMap< SceneTransition > const readMethods = {
            { "TransitionDuration", &SceneTransition::readTransitionDuration },
            { "NextSceneName"     , &SceneTransition::readNextSceneName      },
			{ "HasLighting"       , &SceneTransition::readHasLighting        },
			{ "LightingLayer"     , &SceneTransition::readLightingLayer      }
        };

        return (ReadMethodMap< ISerializable > const&)readMethods;
    }


    /// @brief  writes this SceneTransition to JSON
    /// @return the JSON data of this SceneTransition
    nlohmann::ordered_json SceneTransition::Write() const
    {
        nlohmann::ordered_json json = nlohmann::ordered_json::object();

        json[ "TransitionDuration" ] = Stream::Write( m_TransitionDuration );
        json[ "NextSceneName"      ] = Stream::Write( m_NextSceneName      );
        json[ "HasLighting"        ] = Stream::Write( m_HasLighting        );
        json[ "LightingLayer"      ] = Stream::Write( m_LightingLayer      );

        return json;
    }

    
//-----------------------------------------------------------------------------
// public: copying
//-----------------------------------------------------------------------------


    /// @brief  clones this SceneTransition
    /// @return the newly created clone of this SceneTransition
    SceneTransition* SceneTransition::Clone() const
    {
        return new SceneTransition( *this );
    }


//-----------------------------------------------------------------------------
// private: copying
//-----------------------------------------------------------------------------


    /// @brief  copy-constructor for the SceneTransition
    /// @param  other   the other SceneTransition to copy
    SceneTransition::SceneTransition( SceneTransition const& other ) :
        Behavior( other ),
        m_TransitionDuration( other.m_TransitionDuration ),
        m_NextSceneName     ( other.m_NextSceneName      ),
        m_HasLighting	    ( other.m_HasLighting        ),
        m_LightingLayer     ( other.m_LightingLayer      )
    {}


//-----------------------------------------------------------------------------
