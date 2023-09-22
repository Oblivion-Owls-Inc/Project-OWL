/// @file SceneSystem.cpp
/// @author Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief handles the process of resetting and changing scenes
/// @version 0.1
/// @date 2023-09-08
/// 
/// @copyright Copyright (c) 2023

#include "SceneSystem.h"

#include "EntitySystem.h"

#include "basics.h"
#include "Stream.h"

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
// public accessors
//-----------------------------------------------------------------------------

/// @brief gets the name of the current scene
/// @return the name of the current scene
std::string const& SceneSystem::getSceneName() const
{
    return currentSceneName;
}

//-----------------------------------------------------------------------------
// private constants
//-----------------------------------------------------------------------------

/// @brief The file extension for Scene files
std::string const SceneSystem::sceneFileExtension = ".scene.json";

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

/// @brief Loads the configData of the SceneSystem from JSON
/// @param configData the JSON config data to load
void SceneSystem::Load( rapidjson::Value const& configData )
{
    // TODO: JSON error handling
    baseScenePath = configData[ "baseScenePath" ].GetString();
    nextSceneName = configData[ "nextSceneName" ].GetString();
}

//-----------------------------------------------------------------------------
// private methods
//-----------------------------------------------------------------------------

/// @brief assembles the filepath of a scene with the given name
/// @param sceneName the name of the scene to assemble the filepath of
/// @return the filepath of the scene
std::string SceneSystem::ScenePath( std::string const& sceneName )
{
    return baseScenePath + sceneName + sceneFileExtension;
}

/// @brief Loads the next Scene
void SceneSystem::LoadScene()
{

    rapidjson::Document document = Stream::ReadFromJSON( ScenePath( currentSceneName ) );
    Stream root(document);

    // TODO: have some assetSystem or something like that handle loading assets
    assert( root.getObject().HasMember("entities") );
    EntitySystem::getInstance()->LoadEntities( Stream( root.getObject()[ "entities" ] ) );

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
// singleton stuff
//-----------------------------------------------------------------------------

/// @brief Constructs the SceneSystem
SceneSystem::SceneSystem() :
    nextSceneName( "" ),
    currentSceneName( "" ),
    baseScenePath( "Data/Scenes/" )
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
