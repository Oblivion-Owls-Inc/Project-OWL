///*****************************************************************/
/// @file PauseSystem.h
/// @author Jax Clayton (jax.clayton@digipen.edu)
/// @brief A system that pauses the game when the pause button is pressed.
/// 
/// @copyright  Copyright (c) 2024 Digipen Institute of Technology
///*****************************************************************/

#pragma once

#include "pch.h" // precompiled header has to be included first
#include "System.h"


class PauseSystem : public System
{

//-----------------------------------------------------------------------------
public: // methods
//-----------------------------------------------------------------------------

    /// @brief  Sets the game to running or not
    /// @param  running If the game is running
    void SetRunning(bool running);

    /// @brief  Gets if the game is running
    bool GetRunning() const { return m_Running; }

//-----------------------------------------------------------------------------
private: // virtual override methods
//-----------------------------------------------------------------------------

    /// @brief  Gets called whenever a scene is exited
    virtual void OnSceneExit() override;

    /// @brief Gets Called by the Debug system to display debug information
    virtual void DebugWindow() override;


///-----------------------------------------------------------------------------
private: // Members
///-----------------------------------------------------------------------------


    /// @brief  If the the game is running
    bool m_Running = true; 


    /// @brief  The names of the systems that DONT get paused
    std::set< std::string > const m_EditorSystemNames =
    {
        "PlatformSystem",
        "InputSystem",
        "CheatSystem",
        "BehaviorSystem<UiButton>",
        "BehaviorSystem<UiSlider>",
        "BehaviorSystem<Popup>",
        "BehaviorSystem<PauseComponent>",
        "BehaviorSystem<SceneTransition>",
        "SceneSystem",
        "EntitySystem",
        "CameraSystem",
        "RenderSystem",
        "AudioSystem",
        "DebugSystem",
        "ParticleSystem",
        "PauseSystem",
        "EventSystem"
    };


///-----------------------------------------------------------------------------
private: // Methods
///-----------------------------------------------------------------------------

    /// @brief  Pause the systems in the game
    void togglePause();


//-----------------------------------------------------------------------------
private: // reading
//-----------------------------------------------------------------------------

    /// @brief map of the PauseSystem read methods
    static ReadMethodMap< PauseSystem > const s_ReadMethods;

    /// @brief  gets this System's read methods
    /// @return this System's read methods
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override
    {
        return (ReadMethodMap< ISerializable > const&)s_ReadMethods;
    }

    /// @brief  writes the PauseSystem config to json
    /// @return the written json data
    virtual nlohmann::ordered_json Write() const override;


//-----------------------------------------------------------------------------
public: // singleton stuff
//-----------------------------------------------------------------------------
    /// @brief  gets the instance of PauseSystem
    /// @return the instance of the PauseSystem
    static PauseSystem* GetInstance();

//-----------------------------------------------------------------------------
private: // singleton stuff
//-----------------------------------------------------------------------------

    /// @brief  Constructs the PauseSystem
    PauseSystem();


    // Prevent copying
    PauseSystem( PauseSystem const& ) = delete;
    void operator =( PauseSystem const& ) = delete;


//-----------------------------------------------------------------------------
};

/// @brief A macro to access the PauseSystem
/// @return A PauseSystem pointer
__inline PauseSystem* Pause() { return PauseSystem::GetInstance(); }
