/// @file       SceneSystem.cpp
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      handles the process of resetting and changing scenes
/// @version    0.1
/// @date       2023-09-08
/// 
/// @copyright  Copyright (c) 2023 Digipen Institute of Technology

#include "SceneSystem.h"

#include "EntitySystem.h"

#include "AssetLibrarySystem.h"
#include "Sound.h"
#include "Texture.h"

#include "basics.h"
#include "Stream.h"

//-----------------------------------------------------------------------------
// public methods
//-----------------------------------------------------------------------------

    /// @brief  sets the next Scene to change to
    /// @param  nextSceneName   the name of the next scene
    void SceneSystem::SetNextScene( std::string nextSceneName_ )
    {
        m_NextSceneName = nextSceneName_;
    }

//-----------------------------------------------------------------------------
// public accessors
//-----------------------------------------------------------------------------

    /// @brief  gets the name of the current scene
    /// @return the name of the current scene
    std::string const& SceneSystem::GetSceneName() const
    {
        return m_CurrentSceneName;
    }

//-----------------------------------------------------------------------------
// private constants
//-----------------------------------------------------------------------------

    /// @brief  The file extension for Scene files
    std::string const SceneSystem::s_SceneFileExtension = ".scene.json";

//-----------------------------------------------------------------------------
// virtual override methods
//-----------------------------------------------------------------------------

    /// @brief  Gets called once every simulation frame. Use this function for anything that affects the simulation.    
    void SceneSystem::OnFixedUpdate()
    {
        if ( m_NextSceneName == "" ) {
            return;
        }

        exitScene();

        m_CurrentSceneName = m_NextSceneName;
        m_NextSceneName = "";

        loadScene();
        initScene();
    }

    /// @brief  Gets called once before the Engine closes
    void SceneSystem::OnExit()
    {
        assert ( m_CurrentSceneName != "" );

        exitScene();

        m_CurrentSceneName = "";
    }

//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------

    /// @brief  reads the base scene path
    /// @param  stream  the data to read from
    void SceneSystem::readBaseScenePath( Stream stream )
    {
        m_BaseScenePath = stream.Read<std::string>();
    }

    /// @brief  reads the next scene name
    /// @param  stream  the data to read from
    void SceneSystem::readNextSceneName( Stream stream )
    {
        m_NextSceneName = stream.Read<std::string>();
    }

    /// @brief  map of the SceneSystem read methods
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

    /// @brief  reads the assets in a Scene
    /// @param  stream  the data to read from
    void SceneSystem::Scene::readAssets( Stream stream )
    {
        for ( auto& assetTypeData : stream.GetObject() )
        {
            auto it = s_AssetLibraries.find( assetTypeData.name.GetString() );
            if ( it == s_AssetLibraries.end() )
            {
                std::stringstream errorMessage;
                errorMessage <<
                    "JSON error: unexpected token \"" << assetTypeData.name.GetString() <<
                    "\" encountered while reading Scene asset types";
                throw std::runtime_error( errorMessage.str() );
            }
            (*it).second()->LoadAssets( Stream( assetTypeData.value ) );
        }
    }

    /// @brief  reads the entities in a Scene
    /// @param  stream  the data to read from
    void SceneSystem::Scene::readEntities( Stream stream )
    {
        EntitySystem::GetInstance()->LoadEntities( stream );
    }

    /// @brief  the read methods for a Scene
    ReadMethodMap< SceneSystem::Scene > const SceneSystem::Scene::s_ReadMethods = {
        { "Assets",   &readAssets   },
        { "Entities", &readEntities }
    };

    /// @brief  gets the asset library of the specified type
    /// @tparam AssetType   the asset type to get the library of
    /// @return the AssetLibrarySystem instance of the specified asset type
    template< class AssetType >
    static BaseAssetLibrarySystem* SceneSystem::Scene::getAssetLibrary()
    {
        return AssetLibrarySystem< AssetType >::GetInstance();
    }

    /// @brief map of asset libraries used to read assets
    std::map< std::string, BaseAssetLibrarySystem* (*)() > const SceneSystem::Scene::s_AssetLibraries = {
        { "Archetypes"  , &getAssetLibrary< Entity >    },
        { "Sounds"      , &getAssetLibrary< Sound >     },
        { "Textures"    , &getAssetLibrary< Texture >   }
    };

//-----------------------------------------------------------------------------
// private methods
//-----------------------------------------------------------------------------

    /// @brief  assembles the filepath of a scene with the given name
    /// @param  sceneName   the name of the scene to assemble the filepath of
    /// @return the filepath of the scene
    std::string SceneSystem::scenePath( std::string const& sceneName )
    {
        return m_BaseScenePath + sceneName + s_SceneFileExtension;
    }

    /// @brief  Loads the next Scene
    void SceneSystem::loadScene()
    {

        rapidjson::Document document = Stream::ReadFromJSON( scenePath( m_CurrentSceneName ) );

        Scene scene = Scene();
        // TODO: have some AssetSystem or something like that handle loading assets
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

    /// @brief  Initializes the current Scene
    void SceneSystem::initScene()
    {
        for ( System* system : Engine::GetInstance()->GetSystems() )
        {
            system->OnSceneInit();
        }
    }

    /// @brief  Exits the current Scene
    void SceneSystem::exitScene()
    {
        for ( System* system : Engine::GetInstance()->GetSystems() )
        {
            system->OnSceneExit();
        }
    }

//-----------------------------------------------------------------------------
// singleton stuff
//-----------------------------------------------------------------------------

    /// @brief  Constructs the SceneSystem
    SceneSystem::SceneSystem() :
        System( "SceneSystem" ),
        m_NextSceneName( "" ),
        m_CurrentSceneName( "" ),
        m_BaseScenePath( "Data/Scenes/" )
    {}

    /// @brief  The singleton instance of SceneSystem
    SceneSystem* SceneSystem::s_Instance = nullptr;

    /// @brief  gets the instance of SceneSystem
    /// @return the instance of the SceneSystem
    SceneSystem* SceneSystem::GetInstance()
    {
        if ( s_Instance == nullptr )
        {
            s_Instance = new SceneSystem();
        }
        return s_Instance;
    }

//-----------------------------------------------------------------------------
