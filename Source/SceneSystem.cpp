/// @file SceneSystem.cpp
/// @author Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief handles the process of resetting and changing scenes
/// @version 0.1
/// @date 2023-09-08
/// 
/// @copyright Copyright (c) 2023

#include "SceneSystem.h"

#include "basics.h"

//-----------------------------------------------------------------------------
// public methods
//-----------------------------------------------------------------------------

    /// @brief sets the next Scene to change to
    /// @param nextSceneName the name of the next scene
    void SceneSystem::SetNextScene( std::string nextSceneName_ )
    {
        nextSceneName = nextSceneName_;
    }

//-----------------------------------------------------------------------------
// private methods
//-----------------------------------------------------------------------------

    /// @brief assembles the filepath of a scene with the given name
    /// @param sceneName the name of the scene to assemble the filepath of
    /// @return the filepath of the scene
    std::string SceneSystem::GetScenePath( std::string const& sceneName )
    {
        return baseScenePath + sceneName + sceneFileExtension;
    }


    /// @brief Loads the next Scene
    void SceneSystem::LoadScene()
    {

        // TODO: load from JSON

        for ( System* system : Engine::getInstance()->getSystems() )
        {
            system->OnSceneLoad();
        }
    }

    /// @brief Initializes the current Scene
    void SceneSystem::InitScene()
    {
        for ( System* system : Engine::getInstance()->getSystems() )
        {
            system->OnSceneInit();
        }
    }

    /// @brief Exits the current Scene
    void SceneSystem::ExitScene()
    {
        for ( System* system : Engine::getInstance()->getSystems() )
        {
            system->OnSceneExit();
        }
    }

//-----------------------------------------------------------------------------
// private static methods
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// virtual override methods
//-----------------------------------------------------------------------------

    /// @brief Gets called once every simulation frame. Use this function for anything that affects the simulation.    
    void SceneSystem::OnFixedUpdate()
    {
        if ( nextSceneName == "" ) {
            return;
        }

        ExitScene();

        currentSceneName = nextSceneName;
        nextSceneName = "";

        LoadScene();
        InitScene();
    }

    /// @brief Gets called once before the Engine closes
    void SceneSystem::OnExit()
    {
        // TODO: add the following line once JSON deserialization is implemented
        // assert ( currentSceneName != "" );

        ExitScene();

        currentSceneName = "";
    }

//-----------------------------------------------------------------------------
// private constants
//-----------------------------------------------------------------------------

    /// @brief The file extension for Scene files
    std::string const SceneSystem::sceneFileExtension = ".scene.json";

//-----------------------------------------------------------------------------
// class specific Read methods
//-----------------------------------------------------------------------------

    /// @brief reads a JSON value into baseScenePath
    /// @param jsonValue the JSON value storing the data to be put into baseScenePath
    void SceneSystem::ReadBaseScenePath( rapidjson::Value const& jsonValue )
    {
        baseScenePath = jsonValue.GetString();
    }

    /// @brief reads a JSON value into nextSceneName
    /// @param jsonValue the JSON value storing the data to be put into nextSceneName
    void SceneSystem::ReadNextSceneName( rapidjson::Value const& jsonValue )
    {
        nextSceneName = jsonValue.GetString();
    }

    /// @brief the Read Methods used in this System
    std::map< std::string, ReadMethod< SceneSystem > > const SceneSystem::ReadMethods = {
        { "baseScenePath", &ReadBaseScenePath },
        { "nextSceneName", &ReadNextSceneName }
    };

//-----------------------------------------------------------------------------
// default Read method stuff
//-----------------------------------------------------------------------------

    /// @brief Gets the read methods of this System
    /// @return the map of read methods of this System
    std::map< std::string, ReadMethod< System > > const& SceneSystem::GetReadMethods()
    {
        return (std::map< std::string, ReadMethod< System > > const&)ReadMethods;
    }

//-----------------------------------------------------------------------------
// singleton stuff
//-----------------------------------------------------------------------------

    /// @brief Constructs the SceneSystem
    SceneSystem::SceneSystem() :
        nextSceneName(""),
        currentSceneName(""),
        baseScenePath("Data/Scenes")
    {}

    /// @brief The singleton instance of SceneSystem
    SceneSystem* SceneSystem::instance = nullptr;

    /// @brief gets the instance of SceneSystem
    /// @return the instance of the SceneSystem
    SceneSystem* SceneSystem::getInstance()
    {
        if ( instance == nullptr )
        {
            instance = new SceneSystem();
        }
        return instance;
    }

//-----------------------------------------------------------------------------
