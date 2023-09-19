/// @file SceneSystem.h
/// @author Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief handles the process of resetting and changing scenes
/// @version 0.1
/// @date 2023-09-08
/// 
/// @copyright Copyright (c) 2023

#pragma once

#include "System.h"

#include "Engine.h"
#include <string>

/// @brief Example System meant to be copy-pasted when creating new Systems
class SceneSystem : public System
{

//-----------------------------------------------------------------------------
public: // methods
//-----------------------------------------------------------------------------

    /// @brief sets the next Scene to change to
    /// @param nextSceneName the name of the next scene
    void SetNextScene( std::string nextSceneName );

//-----------------------------------------------------------------------------
public: // accessors
//-----------------------------------------------------------------------------

    /// @brief gets the name of the current scene
    /// @return the name of the current scene
    std::string const& getSceneName() const;

//-----------------------------------------------------------------------------
private: // member variables
//-----------------------------------------------------------------------------

    /// @brief The name of the current Scene
    std::string currentSceneName;

    /// @brief The name of the next Scene
    std::string nextSceneName;

    /// @brief The base path of all Scene files
    std::string baseScenePath;

//-----------------------------------------------------------------------------
private: // constants
//-----------------------------------------------------------------------------

    /// @brief The file extension for Scene files
    static std::string const sceneFileExtension;

//-----------------------------------------------------------------------------
private: // virtual override methods
//-----------------------------------------------------------------------------

    /// @brief Gets called once every simulation frame. Use this function for anything that affects the simulation.    
    virtual void OnFixedUpdate() override;

    /// @brief Gets called once before the Engine closes
    virtual void OnExit() override;

    /// @brief Loads the configData of the SceneSystem from JSON
    /// @param configData the JSON config data to load
    virtual void Load( rapidjson::Value const& configData ) override;

//-----------------------------------------------------------------------------
private: // methods
//-----------------------------------------------------------------------------

    /// @brief assembles the filepath of a scene with the given name
    /// @param sceneName the name of the scene to assemble the filepath of
    /// @return the filepath of the scene
    std::string ScenePath( std::string const& sceneName );

    /// @brief Loads the next Scene
    void LoadScene();

    /// @brief Initializes the current Scene
    void InitScene();

    /// @brief Exits the current Scene
    void ExitScene();

//-----------------------------------------------------------------------------
private: // unused virtual override methods
//-----------------------------------------------------------------------------

    virtual void OnInit() override {}
    virtual void OnUpdate( float dt ) override {}
    virtual void OnSceneLoad() override {}
    virtual void OnSceneInit() override {}
    virtual void OnSceneExit() override {}

//-----------------------------------------------------------------------------
private: // singleton stuff
//-----------------------------------------------------------------------------

    /// @brief Constructs the SceneSystem
    SceneSystem();

    /// @brief The singleton instance of SceneSystem
    static SceneSystem * instance;

//-----------------------------------------------------------------------------
public: // singleton stuff
//-----------------------------------------------------------------------------

    /// @brief gets the instance of SceneSystem
    /// @return the instance of the SceneSystem
    static SceneSystem * getInstance();

    // Prevent copying
    SceneSystem( SceneSystem& other ) = delete;
    void operator=( const SceneSystem& ) = delete;
};

//-----------------------------------------------------------------------------
