/// @file SceneSystem.cpp
/// @author Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief handles the process of resetting and changing scenes
/// @version 0.1
/// @date 2023-09-08
/// 
/// @copyright Copyright (c) 2023

#include "SceneSystem.h"

#include "basics.h"


/// @brief sets the next Scene to change to
/// @param nextSceneName the name of the next scene
void SceneSystem::SetNextScene( std::string nextSceneName_ )
{
    nextSceneName = nextSceneName_;
}

/// @brief gets the name of the current scene
/// @return the name of the current scene
std::string const& SceneSystem::getSceneName() const
{
    return currentSceneName;
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


/// @brief Gets called once when this System is added to the Engine
void SceneSystem::OnInit()
{

    // TODO: add the following line once JSON deserialization is implemented
    // assert( nextSceneName != "" );
    
    currentSceneName = nextSceneName;
    nextSceneName = "";

    LoadScene();
    InitScene();
}

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


/// @brief Constructs the SceneSystem
SceneSystem::SceneSystem( std::string const& initialSceneName ) :
    nextSceneName(initialSceneName),
    currentSceneName("")
{}

/// @brief The singleton instance of SceneSystem
SceneSystem* SceneSystem::instance = nullptr;

/// @brief gets the instance of SceneSystem
/// @return the instance of the SceneSystem
SceneSystem* SceneSystem::getInstance( std::string const& initialSceneName )
{
    if ( instance == nullptr )
    {
        instance = new SceneSystem( initialSceneName );
    }
    return instance;
}
