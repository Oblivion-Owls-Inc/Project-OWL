/// @file SandboxSystem.h
/// @author Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief Sandbox system meant to mimic CS-230 scenes
/// @version 0.1
/// @date 2023-09-05
/// 
/// @copyright Copyright (c) 2023

#pragma once

#include "System.h"

/// @brief Sandbox system meant to mimic CS-230 scenes
class SandboxSystem : public System
{
private: // virtual override methods

    /// @brief Gets called whenever a new Scene is loaded
    virtual void OnSceneLoad() override;


    /// @brief Gets called whenever a scene is initialized
    virtual void OnSceneInit() override;

    /// @brief Gets called once every simulation frame. Use this function for anything that affects the simulation.
    virtual void OnFixedUpdate() override;

    /// @brief Gets called once every graphics frame. Do not use this function for anything that affects the simulation.
    /// @param dt the elapsed time in seconds since the previous frame
    virtual void OnUpdate( float dt ) override;

    /// @brief Gets called whenever a scene is exited
    virtual void OnSceneExit() override;

private: // unused virtual overrides

    /// @brief Gets called once when this System is added to the Engine
    virtual void OnInit() override {}


    /// @brief Gets called once before the Engine closes
    virtual void OnExit() override {}


//-----------------------------------------------------------------------------
private: // reading
//-----------------------------------------------------------------------------

    /// @brief map of the SandboxSystem read methods
    static ReadMethodMap< SandboxSystem > const s_ReadMethods;

    /// @brief  gets this System's read methods
    /// @return this System's read methods
    virtual ReadMethodMap< System > const& GetReadMethods() const override;

private: // singleton stuff

    /// @brief Constructs the SandboxSystem
    SandboxSystem();


    /// @brief The singleton instance of SandboxSystem
    static SandboxSystem * instance;

public: // singleton stuff

    /// @brief gets the instance of SandboxSystem
    /// @return the instance of the SandboxSystem
    static SandboxSystem * GetInstance();

    // Prevent copying
    SandboxSystem(SandboxSystem& other) = delete;
    void operator=(const SandboxSystem&) = delete;
};

