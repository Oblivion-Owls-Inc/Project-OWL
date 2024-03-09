/// @file       AudioListener.cpp
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      Component that picks up spatial audio
/// @version    0.1
/// @date       2023-10-20
/// 
/// @copyright  Copyright (c) 2023 Digipen Institute of Technology



#include "pch.h" // precompiled header has to be included first

#include "AudioListener.h"

#include "AudioSystem.h"

#include "ComponentReference.t.h"

#include "EntitySystem.h"



//-----------------------------------------------------------------------------
// public: types
//-----------------------------------------------------------------------------

    
//-----------------------------------------------------------------------------
// private: types
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// public: accessors
//-----------------------------------------------------------------------------


    /// @brief  sets whether this AudioListener is the active listener in the scene
    /// @param  isActive whether this AudioListener should be the active listener in the scene
    void AudioListener::SetIsActive( bool isActive )
    {
        m_IsActive = isActive;
    }


//-----------------------------------------------------------------------------
// public: methods
//-----------------------------------------------------------------------------


    /// @brief  makes this the active AudioListener in the scene
    void AudioListener::MakeActive()
    {
        Audio()->SetActiveListener( this );
    }


//-----------------------------------------------------------------------------
// public: virtual override methods
//-----------------------------------------------------------------------------


    /// @brief  called once when entering the scene
    void AudioListener::OnInit()
    {
        m_Transform.Init( GetEntity() );
        m_RigidBody.Init( GetEntity() );

        if ( m_IsActive )
        {
            MakeActive();
        }
    }

    /// @brief  called once when exiting the scene
    void AudioListener::OnExit()
    {
        if ( m_IsActive )
        {
            Audio()->SetActiveListener( nullptr );
        }

        m_Transform.Exit();
        m_RigidBody.Exit();
    }


    /// @brief  gets called every graphics frame
    /// @param  dt  the duration of the frame in seconds
    void AudioListener::OnUpdate( float dt )
    {
        if ( m_IsActive == false )
        {
            return;
        }

        glm::vec2 pos2 = m_Transform != nullptr ? m_Transform->GetTranslation() : glm::vec2( 0.0f, 0.0f );
        glm::vec2 vel2 = m_RigidBody != nullptr ? m_RigidBody->GetVelocity() : glm::vec2( 0.0f, 0.0f );

        FMOD_VECTOR pos3 = { pos2.x, pos2.y, m_ZOffset };
        FMOD_VECTOR vel3 = { vel2.x, vel2.y, 0.0f };
        FMOD_VECTOR fwd  = { 0.0f, 0.0f, -1.0f };
        FMOD_VECTOR up   = { 0.0f, 1.0f, 0.0f };

        Audio()->GetFMOD()->set3DListenerAttributes( 0, &pos3, &vel3, &fwd, &up );
    }


//-----------------------------------------------------------------------------
// private: helper methods
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// private: inspection
//-----------------------------------------------------------------------------

    
    /// @brief  displays this AudioListener in the Inspector
    void AudioListener::Inspector()
    {
        if ( m_Transform == nullptr )
        {
            ImGui::Text( "No Transform attached to this AudioListener, using pos = (0, 0)" );
        }
        if ( m_RigidBody == nullptr )
        {
            ImGui::Text( "No RigidBody attached to this AudioListener, using vel = (0, 0)" );
        }

        ImGui::DragFloat( "z offset", &m_ZOffset, 0.05f );

        if ( ImGui::Checkbox( "is active", &m_IsActive ) )
        {
            if ( Entities()->HasEntity( GetEntity() ) )
            {
                if ( m_IsActive )
                {
                    MakeActive();
                }
                else
                {
                    Audio()->SetActiveListener( nullptr );
                }
            }
        }
    }


//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------


    /// @brief  reads the z-axis offset out of the screen to calculate spatial audio with
    /// @param  data    the json data to read from
    void AudioListener::readZOffset( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_ZOffset, data );
    }

    /// @brief  reads whether this AudioListener is the active listener in the scene
    /// @param  data    the json data to read from
    void AudioListener::readIsActive( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_IsActive, data );
    }


//-----------------------------------------------------------------------------
// public: reading / writing
//-----------------------------------------------------------------------------


    /// @brief  gets the map of read methods for this Component
    /// @return the map of read methods for this Component
    ReadMethodMap< ISerializable > const& AudioListener::GetReadMethods() const
    {
        static ReadMethodMap< AudioListener > const readMethods = {
            { "ZOffset" , &AudioListener::readZOffset  },
            { "IsActive", &AudioListener::readIsActive }
        };

        return (ReadMethodMap< ISerializable > const&)readMethods;
    }


    /// @brief  Write all AudioListener data to a JSON file.
    /// @return The JSON file containing the AudioListener data.
    nlohmann::ordered_json AudioListener::Write() const
    {
        nlohmann::ordered_json json;

        json[ "ZOffset"  ] = Stream::Write( m_ZOffset  );
        json[ "IsActive" ] = Stream::Write( m_IsActive );

        return json;
    }


//-----------------------------------------------------------------------------
// public: constructor / Destructor
//-----------------------------------------------------------------------------


    /// @brief  constructor
    AudioListener::AudioListener() :
        Behavior( typeid( AudioListener ) )
    {}


//-----------------------------------------------------------------------------
// public: copying
//-----------------------------------------------------------------------------


    /// @brief  clones this AudioListener
    /// @return the newly created clone of this AudioListener
    AudioListener* AudioListener::Clone() const
    {
        return new AudioListener( *this );
    }


//-----------------------------------------------------------------------------
// private: copying
//-----------------------------------------------------------------------------


    /// @brief  copy-constructor for the AudioListener
    /// @param  other   the other AudioListener to copy
    AudioListener::AudioListener( AudioListener const& other ) :
        Behavior( other ),
        m_IsActive( other.m_IsActive ),
        m_ZOffset ( other.m_ZOffset  )
    {}


//-----------------------------------------------------------------------------
