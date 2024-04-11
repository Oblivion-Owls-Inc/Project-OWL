/// @file       SceneSystem.h
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      handles the process of resetting and changing scenes
/// @version    0.1
/// @date       2023-09-08
/// 
/// @copyright  Copyright (c) 2023 Digipen Institute of Technology

#pragma once

#include "pch.h" 
#include "System.h"

#include "AssetLibrarySystem.h"

#include "Engine.h"
/// @brief  Example System meant to be copy-pasted when creating new Systems
class SceneSystem : public System
{

//-----------------------------------------------------------------------------
public: // methods
//-----------------------------------------------------------------------------

    /// @brief  sets the next Scene to change to
    /// @param  nextSceneName   the name of the next scene
    void SetNextScene( std::string const& nextSceneName );

    /// @brief  saves the current scene to a file
    /// @param  sceneName   the file to save the scene to
    void SaveScene( std::string const& sceneName = "" ) const;

    bool InspectorSaveScene();

    /// @brief Used to reset the current scene
    void ResetScene();

    /// @brief Used by the DebugSystem to load a scene
    bool InspectorLoadScene();


    /// @brief  saves the current scene to an Autosave file
    void Autosave();

    /// @brief  sets the next scene to the autosaved scene
    void LoadAutosave();


    /// @brief  selects a scene name in an inspector
    /// @param  label       the label of the inspector
    /// @param  sceneName   pointer to where to store the selected scene
    /// @return whether a scene was selected
    bool InspectorSelectScene( char const* label, std::string* sceneName );


//-----------------------------------------------------------------------------
public: // accessors
//-----------------------------------------------------------------------------

    /// @brief  gets the name of the current scene
    /// @return the name of the current scene
    std::string const& GetSceneName() const;

    /// @brief Saves the previous scene
    void SavePreviousScene() const;

//-----------------------------------------------------------------------------
private: // member variables
//-----------------------------------------------------------------------------

    /// @brief  The name of the current Scene
    std::string m_CurrentSceneName = "";

    /// @brief Open this scene on start
    std::string m_StartingSceneName = "";

    /// @brief  The name of the next Scene
    std::string m_NextSceneName = "";

    /// @brief  The base path of all Scene files
    /// @brief  NOTE: this is also accessed from a separate thread. be careful with it
    std::string m_BaseScenePath = "Data/Scenes/";


    /// @brief  the name of the autosave scene
    std::string m_AutosaveName = "Autosaved_Scene";

    /// @brief  whether the next scene loaded must be copied into the autosave
    bool m_MustCopyAutosave = true;


    /// @brief  scene JSON files parsed in advance
    /// @brief  NOTE: this is also accessed from a separate thread. be careful with it
    std::map< std::string, nlohmann::ordered_json > m_PreparsedScenes = {};

    /// @brief  thread used to parse scene files in the background
    std::thread m_PreparseThread;


    /// @brief  array of all Scene names in the Scenes directory
    std::vector< std::string > m_SceneNames = {};


//-----------------------------------------------------------------------------
private: // constants
//-----------------------------------------------------------------------------


    /// @brief  The file extension for Scene files
    static std::string const s_SceneFileExtension;


//-----------------------------------------------------------------------------
private: // virtual override methods
//-----------------------------------------------------------------------------

    /// @brief  Gets called once when the engine starts
    virtual void OnInit() override;

    /// @brief  Gets called once every simulation frame. Use this function for anything that affects the simulation.    
    virtual void OnFixedUpdate() override;

    /// @brief  Gets called once before the Engine closes
    virtual void OnExit() override;

    /// @brief  Displays the DebugWindow GUI for this System
    virtual void DebugWindow() override;


//-----------------------------------------------------------------------------
private: // reading
//-----------------------------------------------------------------------------

    /// @brief  reads the base scene path
    /// @param  stream  the data to read from
    void readBaseScenePath( nlohmann::ordered_json const& data );

    /// @brief  reads the next scene name
    /// @param  stream  the data to read from
    void readStartingSceneName( nlohmann::ordered_json const& data );

    /// @brief  reads the name of the autosave scene
    /// @param  stream  the data to read from
    void readAutosaveName( nlohmann::ordered_json const& data );

    /// @brief  map of the SceneSystem read methods
    static ReadMethodMap< SceneSystem > const s_ReadMethods;

    /// @brief  gets this System's read methods
    /// @return this System's read methods
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override
    {
        return (ReadMethodMap< ISerializable > const&)s_ReadMethods;
    }

//-----------------------------------------------------------------------------
public: // writing
//-----------------------------------------------------------------------------

    /// @brief  writes this System to json
    virtual nlohmann::ordered_json Write() const override;

//-----------------------------------------------------------------------------
private: // methods
//-----------------------------------------------------------------------------

    /// @brief  assembles the filepath of a scene with the given name
    /// @param  sceneName   the name of the scene to assemble the filepath of
    /// @return the filepath of the scene
    std::string scenePath( std::string const& sceneName ) const;

    /// @brief  Loads the next Scene
    void loadScene();

    /// @brief  Initializes the current Scene
    void initScene();

    /// @brief  Exits the current Scene
    void exitScene();

    /// @brief  Lists all Scenes in a DebugWindow dropdown
    /// @return the selected scene index
    unsigned inspectorListScenes();

    /// @brief  Gets all of the Scenes in the scenes directory
    void getSceneNames();


    /// @brief  preparses the JSON files of the scenes in the PreparsedScenes map
    /// @brief  intended to be run on a separate thread
    void preparseScenes();


//-----------------------------------------------------------------------------
private: // scene loading
//-----------------------------------------------------------------------------

    class Scene : public ISerializable
    {
    private:
        
        /// @brief  reads the assets in a Scene
        /// @param  stream  the data to read from
        void readAssets( nlohmann::ordered_json const& data );

        /// @brief  reads the entities in a Scene
        /// @param  stream  the data to read from
        void readEntities( nlohmann::ordered_json const& data );

        /// @brief  reads the names of scenes to pre-parse from this scene
        /// @param  data    the JSON data to read from
        void readPreparsedScenes( nlohmann::ordered_json const& data );

        
        /// @brief  the read methods for a Scene
        static ReadMethodMap< Scene > const s_ReadMethods;


        /// @brief  gets the asset library of the specified type
        /// @tparam AssetType   the asset type to get the library of
        /// @return the AssetLibrarySystem instance of the specified asset type
        template< class AssetType >
        static BaseAssetLibrarySystem* getAssetLibrary();

        /// @brief "map" of asset libraries used to read assets
        static std::vector< std::pair< std::string, BaseAssetLibrarySystem* (*)() > > const s_AssetLibraries;

    public:

        /// @brief  gets the Scene read methods
        /// @return a map of the Scene read methods
        virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override
        {
            return (ReadMethodMap< ISerializable > const&)s_ReadMethods;
        }

        /// @brief  writes this Scene to json
        /// @return the written json data
        virtual nlohmann::ordered_json Write() const override;

    };

//-----------------------------------------------------------------------------
private: // singleton stuff
//-----------------------------------------------------------------------------

    /// @brief  Constructs the SceneSystem
    SceneSystem();

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

/// @brief  shorthand method to get the SceneSystem instance
/// @return the SceneSystem instance
__inline SceneSystem* Scenes()
{
    return SceneSystem::GetInstance();
}

//-----------------------------------------------------------------------------
