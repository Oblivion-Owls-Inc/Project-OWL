/// @file       SceneSystem.cpp
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      handles the process of resetting and changing scenes
/// @version    0.1
/// @date       2023-09-08
/// 
/// @copyright  Copyright (c) 2023 Digipen Institute of Technology

#include "SceneSystem.h"

#include "EntitySystem.h"
#include "DebugSystem.h"

#include "AssetLibrarySystem.h"
#include "Sound.h"
#include "Texture.h"
#include "TransformAnimation.h"
#include "AnimationAsset.h"

#include "basics.h"
#include "Stream.h"

#include <filesystem>

//-----------------------------------------------------------------------------
// public methods
//-----------------------------------------------------------------------------

    /// @brief  sets the next Scene to change to
    /// @param  nextSceneName   the name of the next scene
    void SceneSystem::SetNextScene( std::string const& nextSceneName_ )
    {
        m_NextSceneName = nextSceneName_;
    }

    /// @brief  saves the current scene to a file
    /// @param  sceneName   the file to save the scene to
    void SceneSystem::SaveScene( std::string const& sceneName ) const
    {
        // if file not specified, override current scene file
        std::string const* name = &sceneName;
        if ( name->empty() )
        {
            name = &m_CurrentSceneName;
        }

        Stream::WriteToFile( scenePath( *name ), Scene().Write() );
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

    /// @brief  Gets called once when the engine starts
    void SceneSystem::OnInit()
    {
        getSceneNames();
    }

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

    /// @brief  Displays the DebugWindow GUI for this System
    void SceneSystem::DebugWindow()
    {

        unsigned selectedScene = listScenes();


        // Input text for typing scene name
        static char buffer[128] = ""; // Buffer to hold the input, you can save this
        ImGui::InputText("Type Scene Name", buffer, IM_ARRAYSIZE(buffer));

        // Possibly a button to load the scene, either from the dropdown or the input text
        if (ImGui::Button("Load Scene"))
        {
            if (strlen(buffer) > 0)
            {
                std::string sceneName(buffer);
                SetNextScene(sceneName); // Load scene using name typed into buffer
                buffer[0] = '\0'; // Clear the buffer
            }
            else
            {
                SetNextScene(m_SceneNames[selectedScene]); // Load scene using selected scene
            }
        }

        if ( ImGui::Button( "Save Scene" ) )
        {
            SaveScene( buffer );
        }

        if (ImGui::Button("Restart Scene"))
        {
            SetNextScene(m_CurrentSceneName);
        }
    }

//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------

    /// @brief  reads the base scene path
    /// @param  data    the data to read from
    void SceneSystem::readBaseScenePath( nlohmann::ordered_json const& data )
    {
        m_BaseScenePath = Stream::Read<std::string>( data );
    }

    /// @brief  reads the next scene name
    /// @param  data    the data to read from
    void SceneSystem::readNextSceneName( nlohmann::ordered_json const& data )
    {
        m_NextSceneName = Stream::Read<std::string>( data );
    }

    /// @brief  map of the SceneSystem read methods
    ReadMethodMap< SceneSystem > const SceneSystem::s_ReadMethods = {
        { "BaseScenePath", &readBaseScenePath },
        { "NextSceneName", &readNextSceneName }
    };

    /// @brief  writes this System to json
    nlohmann::ordered_json SceneSystem::Write() const
    {
        nlohmann::ordered_json json;
        json.object();
        json[ "BaseScenePath" ] = m_BaseScenePath;
        json[ "NextSceneName" ] = m_CurrentSceneName;
        return json;
    }

//-----------------------------------------------------------------------------
// private: scene loading
//-----------------------------------------------------------------------------

    /// @brief  reads the assets in a Scene
    /// @param  data    the data to read from
    void SceneSystem::Scene::readAssets( nlohmann::ordered_json const& data )
    {
        for ( auto& [ key, value ] : data.items() )
        {
            
            // basically just std::find but different because we're not actually using a std::map
            auto it = s_AssetLibraries.begin();
            for ( ; it != s_AssetLibraries.end(); ++it )
            {
                if ( it->first == key )
                {
                    break;
                }
            }

            if ( it == s_AssetLibraries.end() )
            {
                std::stringstream errorMessage;
                errorMessage <<
                    "JSON error: unexpected token \"" << key <<
                    "\" encountered while reading Scene asset types";
                throw std::runtime_error( errorMessage.str() );
            }
            it->second()->LoadAssets( value );
        }
    }

    /// @brief  reads the entities in a Scene
    /// @param  data    the data to read from
    void SceneSystem::Scene::readEntities( nlohmann::ordered_json const& data )
    {
        EntitySystem::GetInstance()->LoadEntities( data );
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
    std::vector< std::pair< std::string, BaseAssetLibrarySystem* (*)() > > const SceneSystem::Scene::s_AssetLibraries = {
        { "Sounds"             , &getAssetLibrary< Sound >              },
        { "Textures"           , &getAssetLibrary< Texture >            },
        { "TransformAnimations", &getAssetLibrary< TransformAnimation > },
        { "Animations"         , &getAssetLibrary< AnimationAsset >     },
        { "Archetypes"         , &getAssetLibrary< Entity >             }
    };

    /// @brief  writes this Scene to json
    /// @return the written json data
    nlohmann::ordered_json SceneSystem::Scene::Write() const
    {
        nlohmann::ordered_json json;

        nlohmann::ordered_json& assets = json[ "Assets" ];
        for ( auto& [ key, value ] : s_AssetLibraries )
        {
            assets[ key ] = value()->SaveAssets();
        }

        json[ "Entities" ] = Entities()->SaveEntities();

        return json;
    }

//-----------------------------------------------------------------------------
// private methods
//-----------------------------------------------------------------------------

    /// @brief  assembles the filepath of a scene with the given name
    /// @param  sceneName   the name of the scene to assemble the filepath of
    /// @return the filepath of the scene
    std::string SceneSystem::scenePath( std::string const& sceneName ) const
    {
        return m_BaseScenePath + sceneName + s_SceneFileExtension;
    }

    /// @brief  Loads the next Scene
    void SceneSystem::loadScene()
    {

        nlohmann::ordered_json json = Stream::ReadFromFile( scenePath( m_CurrentSceneName ) );

        Scene scene = Scene();
        try
        {
            Stream::Read< ISerializable >( &scene, json );
        }
        catch ( std::runtime_error error )
        {
            std::cerr << error.what();
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

    unsigned SceneSystem::listScenes()
    {
        static int selectedScene = 0; // Default index for dropdown, you can save this
        if ( m_SceneNames.size() > 0 )
        {
            if (ImGui::BeginCombo("Scenes", m_SceneNames[selectedScene].c_str())) // Display the selected scene name
            {
                getSceneNames();
                for (int i = 0; i < m_SceneNames.size(); i++)
                {
                    if (m_SceneNames[i] == m_CurrentSceneName)
                    {
						continue;
					}

                    bool isSelected = (selectedScene == i);
                    if (ImGui::Selectable(m_SceneNames[i].c_str(), isSelected))
                    {
                        selectedScene = i; // Change the selected scene
                    }

                    if (isSelected)
                    {
                        ImGui::SetItemDefaultFocus(); // Automatically focus on the selected item
                    }
                }
                ImGui::EndCombo();
            }
        }

        return selectedScene;
    }

    /// @brief  Gets all of the Scenes in the scenes directory
    void SceneSystem::getSceneNames()
    {
        m_SceneNames.clear();
        for ( auto const& file : std::filesystem::directory_iterator( m_BaseScenePath ) )
        {
            std::string filename = file.path().filename().string();
            size_t nameLength = filename.size() - s_SceneFileExtension.size();
            if ( filename.substr( nameLength ) != s_SceneFileExtension )
            {
                continue;
            }
            m_SceneNames.push_back( filename.substr( 0, nameLength ) );
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
        m_BaseScenePath( "Data/Scenes/" ),
        m_SceneNames()
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
