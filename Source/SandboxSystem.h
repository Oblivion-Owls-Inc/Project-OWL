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

private: // default Read method stuff

    /// @brief the Read Methods used in this System
    static std::map< std::string, ReadMethod< SandboxSystem > > const ReadMethods;

    /// @brief Gets the read methods of this System
    /// @return the map of read methods of this System
    virtual std::map< std::string, ReadMethod< System > > const& GetReadMethods() override;

private: // singleton stuff

    /// @brief Constructs the SandboxSystem
    SandboxSystem();


    /// @brief The singleton instance of SandboxSystem
    static SandboxSystem * instance;

public: // singleton stuff

    /// @brief gets the instance of SandboxSystem
    /// @return the instance of the SandboxSystem
    static SandboxSystem * getInstance();

    // Prevent copying
    SandboxSystem(SandboxSystem& other) = delete;
    void operator=(const SandboxSystem&) = delete;
};

