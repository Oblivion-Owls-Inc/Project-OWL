/// @file       SceneSystem.h
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      handles the process of resetting and changing scenes
/// @version    0.1
/// @date       2023-09-08
/// 
/// @copyright  Copyright (c) 2023 Digipen Institute of Technology

#pragma once

#include "System.h"

#include "AssetLibrarySystem.h"

#include "Engine.h"
#include <string>

/// @brief  Example System meant to be copy-pasted when creating new Systems
class SceneSystem : public System
{

//-----------------------------------------------------------------------------
public: // methods
//-----------------------------------------------------------------------------

    /// @brief  sets the next Scene to change to
    /// @param  nextSceneName   the name of the next scene
    void SetNextScene( std::string nextSceneName );

//-----------------------------------------------------------------------------
public: // accessors
//-----------------------------------------------------------------------------

    /// @brief  gets the name of the current scene
    /// @return the name of the current scene
    std::string const& GetSceneName() const;

//-----------------------------------------------------------------------------
private: // member variables
//-----------------------------------------------------------------------------

    /// @brief  The name of the current Scene
    std::string m_CurrentSceneName;

    /// @brief  The name of the next Scene
    std::string m_NextSceneName;

    /// @brief  The base path of all Scene files
    std::string m_BaseScenePath;

//-----------------------------------------------------------------------------
private: // constants
//-----------------------------------------------------------------------------

    /// @brief  The file extension for Scene files
    static std::string const s_SceneFileExtension;

//-----------------------------------------------------------------------------
private: // virtual override methods
//-----------------------------------------------------------------------------

    /// @brief  Gets called once every simulation frame. Use this function for anything that affects the simulation.    
    virtual void OnFixedUpdate() override;

    /// @brief  Gets called once before the Engine closes
    virtual void OnExit() override;

//-----------------------------------------------------------------------------
private: // reading
//-----------------------------------------------------------------------------

    /// @brief  reads the base scene path
    /// @param  stream  the data to read from
    void readBaseScenePath( Stream stream );

    /// @brief  reads the next scene name
    /// @param  stream  the data to read from
    void readNextSceneName( Stream stream );

    /// @brief  map of the SceneSystem read methods
    static ReadMethodMap< SceneSystem > const s_ReadMethods;

    /// @brief  gets this System's read methods
    /// @return this System's read methods
    virtual ReadMethodMap< System > const& GetReadMethods() const override;

//-----------------------------------------------------------------------------
private: // methods
//-----------------------------------------------------------------------------

    /// @brief  assembles the filepath of a scene with the given name
    /// @param  sceneName   the name of the scene to assemble the filepath of
    /// @return the filepath of the scene
    std::string scenePath( std::string const& sceneName );

    /// @brief  Loads the next Scene
    void loadScene();

    /// @brief  Initializes the current Scene
    void initScene();

    /// @brief  Exits the current Scene
    void exitScene();

//-----------------------------------------------------------------------------
private: // scene loading
//-----------------------------------------------------------------------------

    class Scene
    {
    public:

        /// @brief  gets the Scene read methods
        /// @return a map of the Scene read methods
        ReadMethodMap< Scene > const& GetReadMethods() const;

    private:
        
        /// @brief  reads the assets in a Scene
        /// @param  stream  the data to read from
        void readAssets( Stream stream );

        /// @brief  reads the entities in a Scene
        /// @param  stream  the data to read from
        void readEntities( Stream stream );
        
        /// @brief  the read methods for a Scene
        static ReadMethodMap< Scene > const s_ReadMethods;

        /// @brief  gets the asset library of the specified type
        /// @tparam AssetType   the asset type to get the library of
        /// @return the AssetLibrarySystem instance of the specified asset type
        template< class AssetType >
        static BaseAssetLibrarySystem* getAssetLibrary();

        /// @brief map of asset libraries used to read assets
        static std::map< std::string, BaseAssetLibrarySystem* (*)() > const s_AssetLibraries;

    };

//-----------------------------------------------------------------------------
private: // singleton stuff
//-----------------------------------------------------------------------------

    /// @brief  Constructs the SceneSystem
    SceneSystem();

    /// @brief  The singleton instance of SceneSystem
    static SceneSystem * s_Instance;

//-----------------------------------------------------------------------------
public: // singleton stuff
//-----------------------------------------------------------------------------

    /// @brief  gets the instance of SceneSystem
    /// @return the instance of the SceneSystem
    static SceneSystem * GetInstance();

    // Prevent copying
    SceneSystem( SceneSystem& other ) = delete;
    void operator=( const SceneSystem& ) = delete;

//-----------------------------------------------------------------------------
};