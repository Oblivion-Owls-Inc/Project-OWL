/// @file AssetLibrarySystem.h
/// @author Tyler Birdsall (tyler.birdsall@digipen.edu)
/// @brief AssetLibrary System 
/// @version 0.1
/// @date 2023-09-26
/// 
/// @copyright Copyright (c) 2023

#pragma once

#include "System.h"

/// @brief AssetLibrary System meant to be copy-pasted when creating new Systems
class AssetLibrarySystem : public System
{
private: // virtual override methods

    

private: // unused virtual overrides

    /// @brief Gets called once when this System is added to the Engine
    virtual void OnInit() override {}


    /// @brief Gets called once every simulation frame. Use this function for anything that affects the simulation.
    virtual void OnFixedUpdate() override {}


    /// @brief Gets called once every graphics frame. Do not use this function for anything that affects the simulation.
    /// @param dt the elapsed time in seconds since the previous frame
    virtual void OnUpdate( float dt ) override {}


    /// @brief Gets called once before the Engine closes
    virtual void OnExit() override {}


    /// @brief Gets called whenever a new Scene is loaded
    virtual void OnSceneLoad() override {}


    /// @brief Gets called whenever a scene is initialized
    virtual void OnSceneInit() override {}


    /// @brief Gets called whenever a scene is exited
    virtual void OnSceneExit() override {}


    /// @brief Loads the config data of this System
    /// @param configData the JSON object with all of the configData for this System
    virtual void Load( rapidjson::Value const& configData ) override {}

private: // singleton stuff

    /// @brief Constructs the AssetLibrarySystem
    AssetLibrarySystem();


    /// @brief The singleton instance of AssetLibrarySystem
    static AssetLibrarySystem * instance;

public: // singleton stuff

    /// @brief gets the instance of AssetLibrarySystem
    /// @return the instance of the AssetLibrarySystem
    static AssetLibrarySystem * getInstance();

    // Prevent copying
    AssetLibrarySystem(AssetLibrarySystem& other) = delete;
    void operator=(const AssetLibrarySystem&) = delete;
};

