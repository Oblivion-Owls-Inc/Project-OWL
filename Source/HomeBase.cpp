///*****************************************************************/
/// @file	    HomeBase.cpp
/// @author     Tyler Birdsall (tyler.birdsall@digipen.edu)
/// @date	    10/27/2023
/// @brief      HomeBase class implimentation
/// @copyright  Digipen LLC (c) 2023
///*****************************************************************/


#include "pch.h" // precompiled header has to be included first
#include "HomeBase.h"

#include "ComponentReference.t.h"

#include "SceneTransition.h"
#include "ComponentSystem.h"
#include "Generator.h"

#include "EntitySystem.h"
#include "PlayerController.h"

#include "BehaviorSystem.h"


#include "SceneSystem.h"



//-----------------------------------------------------------------------------
// public: constructor
//-----------------------------------------------------------------------------


    /// @brief  constructor
    HomeBase::HomeBase() :
        Component( typeid( HomeBase ) )
    {}

//-----------------------------------------------------------------------------
// public: accessors
//-----------------------------------------------------------------------------

    /// @brief  gets the Health Component attached to this Entity
    /// @return the Health Component attached to this Entity
    Health* HomeBase::GetHealth() { return m_Health; }

    /// @brief  gets the current win state
    /// @return bool if we can win the game
    bool HomeBase::CanWin() { return m_CanWin; }

    /// @brief  plays the win sound
    void HomeBase::PlayWinSound() 
    { 
        m_AudioPlayer->SetSound(m_ActivateSound);
        m_AudioPlayer->Play();
    }


//-----------------------------------------------------------------------------
// public: methods
//-----------------------------------------------------------------------------


    /// @brief	destroy the base
    void HomeBase::Destroy()
    {
        if ( m_SceneTransition != nullptr )
        {
            m_SceneTransition->StartTransition( m_GameOverSceneName );
        }
    }

    
//-----------------------------------------------------------------------------
// public: virtual override methods
//-----------------------------------------------------------------------------

    
    /// @brief  called once when entering the scene
    void HomeBase::OnInit()
    {
        Components< HomeBase >()->AddComponent( this );

        m_Health.SetOnConnectCallback( [ this ]() {
            m_Health->AddOnHealthChangedCallback( GetId(), [ this ]() {
                if ( m_Health->GetHealth()->GetCurrent() <= 0 )
                {
                    Events()->BroadcastEvent< std::string >(m_EventNameCutsceneLose);
                    Debug() << "Event Emitted: " << m_EventNameCutsceneLose << std::endl;
                    m_AudioPlayer->SetSound(m_DeactivateSound);
                    m_AudioPlayer->Play();
                    //Destroy();
                }
            } );
        } );
        m_Health.SetOnDisconnectCallback( [ this ]() {
            m_Health->RemoveOnHealthChangedCallback( GetId() );
        } );

        m_Health.Init( GetEntity() );
        m_AudioPlayer.Init(GetEntity());

        m_SceneTransitionEntity.SetOwnerName( GetName() );
        m_SceneTransitionEntity.Init();

        /// Set the filter function for the listener
        m_ListenerBegin.SetFilterFunction([&](std::string const& EventNameBegin) -> bool
        {
            return EventNameBegin == m_EventNameBegin;
        });

        /// Set the Callback function for the listener
        m_ListenerBegin.SetResponseFunction([&](std::string const& EventNameBegin)
        {
            // do thing on start
            m_AudioPlayer->SetSound(m_DeactivateSound);
            m_AudioPlayer->Play();
            m_AudioPlayer->SetSound(m_DamageSound);
            GetEntity()->GetComponent<Generator>()->Activate();
        });

        /// Set the filter function for the listener
        m_ListenerEnd.SetFilterFunction([&](std::string const& EventNameEnd) -> bool
        {
            return EventNameEnd == m_EventNameEnd;
        });

        /// Set the Callback function for the listener
        m_ListenerEnd.SetResponseFunction([&](std::string const& EventNameEnd)
        {
            m_CanWin = true;
        });

        /// Set the filter function for the listener
        m_ListenerLose.SetFilterFunction([&](std::string const& EventNameEnd) -> bool
        {
              return EventNameEnd == m_EventNameLose;
        });

        /// Set the Callback function for the listener
        m_ListenerLose.SetResponseFunction([&](std::string const& EventNameEnd)
        {
            Destroy();
        });

        /// Set the filter function for the listener
        m_ListenerDoom.SetFilterFunction([&](std::string const& EventNameEnd) -> bool
        {
            return EventNameEnd == m_EventNameDoom;
        });

        /// Set the Callback function for the listener
        m_ListenerDoom.SetResponseFunction([&](std::string const& EventNameEnd)
        {
             for (PlayerController* player : Behaviors<PlayerController>()->GetComponents())
             {
                 Transform* transform = player->GetEntity()->GetComponent<Transform>();
                 Entity* clone = m_CameraPrefab->Clone();
                 clone->GetComponent<Transform>()->SetTranslation(transform->GetTranslation());
                 clone->AddToScene();
             }
        });

        m_ListenerBegin.Init();
        m_ListenerEnd.Init();
        m_ListenerLose.Init();
        m_ListenerDoom.Init();
        m_CameraPrefab.Init();

        m_ActivateSound.Init();
        m_DeactivateSound.Init();

    }

    /// @brief  called once when exiting the scene
    void HomeBase::OnExit()
    {
        Components< HomeBase >()->RemoveComponent( this );

        m_Health.Exit();
        m_AudioPlayer.Exit();

        m_SceneTransitionEntity.Exit();
        m_ListenerBegin.Exit();
        m_ListenerEnd.Exit();
        m_ListenerLose.Exit();
        m_ListenerDoom.Exit();
    }


//-----------------------------------------------------------------------------
// public: inspection
//-----------------------------------------------------------------------------


    /// @brief  inspector for this HomeBase
    void HomeBase::Inspector()
    {
        if ( m_Health == nullptr )
        {
            ImGui::Text( "WARNING: no Health Component attached" );
        }

        Scenes()->InspectorSelectScene( "game over scene", &m_GameOverSceneName );

        m_SceneTransitionEntity.Inspect( "scene transition entity" );

        ImGui::InputText("Event Name Begin", &m_EventNameBegin);
        ImGui::InputText("Event Name End", &m_EventNameEnd);
        ImGui::InputText("Event Name Lose", &m_EventNameLose);
        ImGui::InputText("Event Name Doom", &m_EventNameDoom);
        ImGui::InputText("Event Name Cutscene Lose", &m_EventNameCutsceneLose);
        ImGui::Separator();
        m_ActivateSound.Inspect("Drive Sound");
        m_DeactivateSound.Inspect("Breakdown Sound");
        m_DamageSound.Inspect("Damage Sound");
        m_CameraPrefab.Inspect("Camera Prefab");
    }


//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------

    
    /// @brief  reads the name of the scene to transition to when the base dies
    /// @param  data    the JSON data to read from
    void HomeBase::readGameOverSceneName( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_GameOverSceneName, data );
    }

    /// @brief  reads the Entity the SceneTransition Component is attached to
    /// @param  data    the JSON data to read from
    void HomeBase::readSceneTransitionEntity( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_SceneTransitionEntity, data );
    }

    /// @brief  reads the EventNameBegin from a JSON file
    /// @param data    the JSON file to read from
    void HomeBase::readEventNameBegin(nlohmann::ordered_json const& data)
    {
        Stream::Read(m_EventNameBegin, data);
    }

    /// @brief  reads the EventNameEnd from a JSON file
    /// @param data    the JSON file to read from
    void HomeBase::readEventNameEnd(nlohmann::ordered_json const& data)
    {
        Stream::Read(m_EventNameEnd, data);
    }

    /// @brief  reads the EventNameEnd from a JSON file
    /// @param data    the JSON file to read from
    void HomeBase::readEventNameLose(nlohmann::ordered_json const& data)
    {
        Stream::Read(m_EventNameLose, data);
    }

    /// @brief  reads the EventNameDoom from a JSON file
    /// @param data    the JSON file to read from
    void HomeBase::readEventNameDoom(nlohmann::ordered_json const& data)
    {
        Stream::Read(m_EventNameDoom, data);
    }

    /// @brief  reads the EventNameCutsceneLose from a JSON file
    /// @param data    the JSON file to read from
    void HomeBase::readEventNameCutsceneLose(nlohmann::ordered_json const& data)
    {
        Stream::Read(m_EventNameCutsceneLose, data);
    }

    /// @brief  reads the CameraPrefab from a JSON file
    /// @param data    the JSON file to read from
    void HomeBase::readCameraPrefab(nlohmann::ordered_json const& data)
    {
        Stream::Read(m_CameraPrefab, data);
    }

    /// @brief reads the DriveSound from the JSON data
    /// @param data - the JSON data to read from
    void HomeBase::readDriveSound(nlohmann::ordered_json const& data)
    {
        Stream::Read(m_ActivateSound, data);
    }

    /// @brief reads the break down sound from the JSON data
    /// @param data - the JSON data to read from
    void HomeBase::readDeactivateSound(nlohmann::ordered_json const& data)
    {
       Stream::Read(m_DeactivateSound, data);
    }

    /// @brief reads the damage sound from the JSON data
    /// @param data - the JSON data to read from
    void HomeBase::readDamageSound(nlohmann::ordered_json const& data)
    {
        Stream::Read(m_DamageSound, data);
    }

    
//-----------------------------------------------------------------------------
// public: reading / writing
//-----------------------------------------------------------------------------


    /// @brief  gets the map of read methods for this HomeBase
    /// @return the map of read methods for this HomeBase
    ReadMethodMap< ISerializable > const& HomeBase::GetReadMethods() const
    {
        static ReadMethodMap< HomeBase > const readMethods = {
            { "GameOverScene"        , &HomeBase::readGameOverSceneName     },
            { "SceneTransitionEntity", &HomeBase::readSceneTransitionEntity },
            { "EventNameBegin"       , &HomeBase::readEventNameBegin        },
            { "EventNameEnd"         , &HomeBase::readEventNameEnd          },
            { "EventNameLose"        , &HomeBase::readEventNameLose         },
            { "EventNameDoom"        , &HomeBase::readEventNameDoom         },
            { "EventNameCutsceneLose", &HomeBase::readEventNameCutsceneLose },
            { "CameraPrefab"         , &HomeBase::readCameraPrefab          },
            { "DriveSound"           , &HomeBase::readDriveSound            },
            { "DeactivateSound"      , &HomeBase::readDeactivateSound       },
            { "DamageSound"          , &HomeBase::readDamageSound           }
        };

        return (ReadMethodMap< ISerializable > const&)readMethods;

    }


    /// @brief  Writes all HomeBase data to a JSON file.
    /// @return The JSON file containing the data.
    nlohmann::ordered_json HomeBase::Write() const
    {
        nlohmann::ordered_json json;

        json[ "GameOverScene"         ] = Stream::Write( m_GameOverSceneName     );
        json[ "SceneTransitionEntity" ] = Stream::Write( m_SceneTransitionEntity );
        json[ "EventNameBegin"        ] = m_EventNameBegin;
        json[ "EventNameEnd"          ] = m_EventNameEnd;
        json[ "EventNameLose"         ] = m_EventNameLose;
        json[ "EventNameDoom"         ] = m_EventNameDoom;
        json[ "EventNameCutsceneLose" ] = m_EventNameCutsceneLose;
        json[ "CameraPrefab"          ] = Stream::Write(m_CameraPrefab);
        json[ "DriveSound"            ] = Stream::Write(m_ActivateSound);
        json[ "DeactivateSound"       ] = Stream::Write(m_DeactivateSound);
        json[ "DamageSound"           ] = Stream::Write(m_DamageSound);


        return json;
    }


//-----------------------------------------------------------------------------
// public: copying
//-----------------------------------------------------------------------------


    /// @brief  clone
    HomeBase* HomeBase::Clone() const
    {
        return new HomeBase( *this );
    }

    
//-----------------------------------------------------------------------------
// private: copying
//-----------------------------------------------------------------------------
    

    /// @brief  copy constructor
    HomeBase::HomeBase( HomeBase const& other ) :
        Component( other ),
        m_GameOverSceneName    ( other.m_GameOverSceneName ),
        m_SceneTransitionEntity( other.m_SceneTransitionEntity, { &m_SceneTransition } ),
        m_EventNameBegin(other.m_EventNameBegin),
        m_EventNameEnd(other.m_EventNameEnd),
        m_EventNameLose(other.m_EventNameLose),
        m_EventNameDoom(other.m_EventNameDoom),
        m_EventNameCutsceneLose(other.m_EventNameCutsceneLose),
        m_CameraPrefab(other.m_CameraPrefab),
        m_ActivateSound(other.m_ActivateSound),
        m_DeactivateSound(other.m_DeactivateSound),
        m_DamageSound(other.m_DamageSound)
    {
        m_CameraPrefab = other.m_CameraPrefab;
    }


//-----------------------------------------------------------------------------
