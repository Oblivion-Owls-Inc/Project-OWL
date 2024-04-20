///*****************************************************************/
/// @file	    HomeBase.h
/// @author     Tyler Birdsall (tyler.birdsall@digipen.edu)
/// @date	    10/27/2023
/// @brief      HomeBase class header
/// @copyright  Digipen LLC (c) 2023
///*****************************************************************/

#pragma once
#include "Component.h"

#include "AudioPlayer.h"
#include "EntityReference.h"
#include "ComponentReference.h"
#include "AssetReference.h"
#include "EventListener.h"
#include "Sound.h"

class Health;
class SceneTransition;

class HomeBase : public Component
{
//-----------------------------------------------------------------------------
public: // constructor / destructors
//-----------------------------------------------------------------------------


    /// @brief  constructor
    HomeBase();

 //-----------------------------------------------------------------------------
public: // accessors
 //-----------------------------------------------------------------------------

    /// @brief  gets the Health Component attached to this Entity
    /// @return the Health Component attached to this Entity
    Health* GetHealth();

    /// @brief  gets the current win state
    /// @return bool if we can win the game
    bool CanWin();

    /// @brief  plays the win sound
    void PlayWinSound();

//-----------------------------------------------------------------------------
public: // public method
//-----------------------------------------------------------------------------


    /// @brief destroy the base
    void Destroy();
    
    
//-----------------------------------------------------------------------------
public: // virtual override methods
//-----------------------------------------------------------------------------

    
    /// @brief  called once when entering the scene
    virtual void OnInit() override;

    /// @brief  called once when exiting the scene
    virtual void OnExit() override;


//-----------------------------------------------------------------------------
private: // members
//-----------------------------------------------------------------------------

    
    /// @brief  the name of the scene to transition to when the base dies
    std::string m_GameOverSceneName = "Gameover";

    /// @brief  camera for doomsday
    AssetReference< Entity > m_CameraPrefab;

    /// @brief  the SceneTransition Component responsible for changing scenes
    ComponentReference< SceneTransition > m_SceneTransition;

    /// @brief  the Entity the SceneTransition Component is attached to
    EntityReference m_SceneTransitionEntity = EntityReference( { &m_SceneTransition } );

    /// @brief  the Health Component attached to this Entity
    ComponentReference< Health > m_Health;

    /// @brief Listener for the start event
    EventListener<std::string> m_ListenerBegin;

    /// @brief  the name of the event to listen for
    std::string m_EventNameBegin;

    /// @brief Listener for the end event
    EventListener<std::string> m_ListenerEnd;

    /// @brief  the name of the event to listen for
    std::string m_EventNameEnd;

    /// @brief Listener for the standard lose event
    EventListener<std::string> m_ListenerLose;
    
    /// @brief  the name of the event to listen for
    std::string m_EventNameLose;

    /// @brief Listener for the doomsday death event
    EventListener<std::string> m_ListenerDoom;

    /// @brief  the name of the event to listen for
    std::string m_EventNameDoom;

    /// @brief  the name of the event to listen for
    std::string m_EventNameCutsceneLose;

    /// @brief  the sound to play when driving / in and out 
    AssetReference< Sound > m_ActivateSound;

    /// @brief  the sound to play when the base breaks down 
    AssetReference< Sound > m_DeactivateSound;

    /// @brief  the sound to play when the base breaks down 
    AssetReference< Sound > m_DamageSound;

    /// @brief  the audio player for this entity
    ComponentReference< AudioPlayer > m_AudioPlayer;

    bool m_CanWin = false;


//-----------------------------------------------------------------------------
public: // inspection
//-----------------------------------------------------------------------------

    /// @brief  inspector for this HomeBase
    virtual void Inspector() override;

    
//-----------------------------------------------------------------------------
private: // reading
//-----------------------------------------------------------------------------

    
    /// @brief  reads the name of the scene to transition to when the base dies
    /// @param  data    the JSON data to read from
    void readGameOverSceneName( nlohmann::ordered_json const& data );

    /// @brief  reads the Entity the SceneTransition Component is attached to
    /// @param  data    the JSON data to read from
    void readSceneTransitionEntity( nlohmann::ordered_json const& data );

    /// @brief  reads the EventNameBegin from a JSON file
    /// @param data    the JSON file to read from
    void readEventNameBegin(nlohmann::ordered_json const& data);

    /// @brief  reads the EventNameEnd from a JSON file
    /// @param data    the JSON file to read from
    void readEventNameEnd(nlohmann::ordered_json const& data);

    /// @brief  reads the EventNameLose from a JSON file
    /// @param data    the JSON file to read from
    void readEventNameLose(nlohmann::ordered_json const& data);

    /// @brief  reads the EventNameDoom from a JSON file
    /// @param data    the JSON file to read from
    void readEventNameDoom(nlohmann::ordered_json const& data);

    /// @brief  reads the EventNameCutsceneLose from a JSON file
    /// @param data    the JSON file to read from
    void readEventNameCutsceneLose(nlohmann::ordered_json const& data);

    /// @brief  reads the CameraPrefab from a JSON file
    /// @param data    the JSON file to read from
    void readCameraPrefab(nlohmann::ordered_json const& data);

    /// @brief reads the DriveSound from the JSON data
    /// @param data - the JSON data to read from
    void readDriveSound(nlohmann::ordered_json const& data);

    /// @brief reads the break down sound from the JSON data
    /// @param data - the JSON data to read from
    void readDeactivateSound(nlohmann::ordered_json const& data);

    /// @brief reads the damage sound from the JSON data
    /// @param data - the JSON data to read from
    void readDamageSound(nlohmann::ordered_json const& data);

//-----------------------------------------------------------------------------
public: // reading / writing
//-----------------------------------------------------------------------------


    /// @brief  gets the map of read methods for this HomeBase
    /// @return the map of read methods for this HomeBase
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override;


    /// @brief  Writes all HomeBase data to a JSON file.
    /// @return The JSON file containing the data.
    virtual nlohmann::ordered_json Write() const override;


//-----------------------------------------------------------------------------
public: // copying
//-----------------------------------------------------------------------------


    /// @brief Base clone
    virtual HomeBase* Clone() const override;


//-----------------------------------------------------------------------------
private: // copying
//-----------------------------------------------------------------------------
    

    /// @brief copy ctor
    HomeBase( HomeBase const& other);


    void operator =( HomeBase const& ) = delete;


//-----------------------------------------------------------------------------
};


