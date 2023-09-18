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
public: // methods

    /// @brief sets the next Scene to change to
    /// @param nextSceneName the name of the next scene
    void SetNextScene( std::string nextSceneName );

public: // inline accessors

    /// @brief gets the name of the current scene
    /// @return the name of the current scene
    __inline std::string const& getSceneName() const { return currentSceneName; }

private: // methods

    /// @brief assembles the filepath of a scene with the given name
    /// @param sceneName the name of the scene to assemble the filepath of
    /// @return the filepath of the scene
    std::string GetScenePath( std::string const& sceneName );

    /// @brief Loads the next Scene
    void LoadScene();

    /// @brief Initializes the current Scene
    void InitScene();

    /// @brief Exits the current Scene
    void ExitScene();

private: // virtual override methods

    /// @brief Gets called once every simulation frame. Use this function for anything that affects the simulation.    
    virtual void OnFixedUpdate() override;

    /// @brief Gets called once before the Engine closes
    virtual void OnExit() override;

private: // member variables

    /// @brief The name of the current Scene
    std::string currentSceneName;

    /// @brief The name of the next Scene
    std::string nextSceneName;

    /// @brief The base path of all Scene files
    std::string baseScenePath;

private: // constants

    /// @brief The file extension for Scene files
    static std::string const sceneFileExtension;

private: // class-specific Read Methods

    /// @brief reads a JSON value into baseScenePath
    /// @param jsonValue the JSON value storing the data to be put into baseScenePath
    void ReadBaseScenePath( rapidjson::Value const& jsonValue );

    /// @brief reads a JSON value into nextSceneName
    /// @param jsonValue the JSON value storing the data to be put into nextSceneName
    void ReadNextSceneName( rapidjson::Value const& jsonValue );


private: // unused virtual overrides

    /// @brief Gets called once when this System is added to the Engine
    virtual void OnInit() override {}

    /// @brief Gets called once every graphics frame. Do not use this function for anything that affects the simulation.
    /// @param dt the elapsed time in seconds since the previous frame
    virtual void OnUpdate( float dt ) override {}


    /// @brief Gets called whenever a new Scene is loaded
    virtual void OnSceneLoad() override {}

    /// @brief Gets called whenever a scene is initialized
    virtual void OnSceneInit() override {}

    /// @brief Gets called whenever a scene is exited
    virtual void OnSceneExit() override {}

private: // default Read method stuff

    /// @brief the Read Methods used in this System
    static std::map< std::string, ReadMethod< SceneSystem > > const ReadMethods;

    /// @brief Gets the read methods of this System
    /// @return the map of read methods of this System
    virtual std::map< std::string, ReadMethod< System > > const& GetReadMethods() override;

private: // singleton stuff

    /// @brief Constructs the SceneSystem
    SceneSystem();

    /// @brief The singleton instance of SceneSystem
    static SceneSystem * instance;

public:

    /// @brief gets the instance of SceneSystem
    /// @return the instance of the SceneSystem
    static SceneSystem * getInstance();

    // Prevent copying
    SceneSystem( SceneSystem& other ) = delete;
    void operator=( const SceneSystem& ) = delete;
};

