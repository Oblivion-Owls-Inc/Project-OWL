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
std::string const& SceneSystem::GetSceneName() const
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

//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------

    /// @brief reads the base scene path
    /// @param stream the data to read from
    void SceneSystem::readBaseScenePath( Stream stream )
    {
        baseScenePath = stream.Read<std::string>();
    }

    /// @brief reads the next scene name
    /// @param stream the data to read from
    void SceneSystem::readNextSceneName( Stream stream )
    {
        nextSceneName = stream.Read<std::string>();
    }

    /// @brief map of the SceneSystem read methods
    ReadMethodMap< SceneSystem > const SceneSystem::s_ReadMethods = {
        { "BaseScenePath", &readBaseScenePath },
        { "NextSceneName", &readNextSceneName }
    };

    /// @brief  gets this System's read methods
    /// @return this System's read methods
    ReadMethodMap< System > const& SceneSystem::GetReadMethods() const
    {
        return (ReadMethodMap< System > const&)s_ReadMethods;
    }

//-----------------------------------------------------------------------------
// private: scene loading
//-----------------------------------------------------------------------------

    /// @brief  gets the Scene read methods
    /// @return a map of the Scene read methods
    ReadMethodMap< SceneSystem::Scene > const& SceneSystem::Scene::GetReadMethods() const
    {
        return s_ReadMethods;
    }

    /// @brief          reads the assets in a Scene
    /// @param stream   the data to read from
    void SceneSystem::Scene::readAssets( Stream stream )
    {
        // TODO: implement asset reading
    }

    /// @brief          reads the entities in a Scene
    /// @param stream   the data to read from
    void SceneSystem::Scene::readEntities( Stream stream )
    {
        EntitySystem::GetInstance()->LoadEntities( stream );
    }

    /// @brief the read methods for a Scene
    ReadMethodMap< SceneSystem::Scene > const SceneSystem::Scene::s_ReadMethods = {
        { "Assets",   &readAssets   },
        { "Entities", &readEntities }
    };

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

        Scene scene = Scene();
        // TODO: have some asSetSystem or something like that handle loading assets
        try
        {
            Stream( document ).Read( &scene );
        }
        catch ( std::runtime_error error )
        {
            std::cerr << error.what();
        }

        for ( System* system : Engine::GetInstance()->GetSystems() )
        {
            system->OnSceneLoad();
        }
    }

    /// @brief Initializes the current Scene
    void SceneSystem::InitScene()
    {
        for ( System* system : Engine::GetInstance()->GetSystems() )
        {
            system->OnSceneInit();
        }
    }

    /// @brief Exits the current Scene
    void SceneSystem::ExitScene()
    {
        for ( System* system : Engine::GetInstance()->GetSystems() )
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
    SceneSystem* SceneSystem::GetInstance()
    {
        if ( instance == nullptr )
        {
            instance = new SceneSystem();
        }
        return instance;
    }

//-----------------------------------------------------------------------------
