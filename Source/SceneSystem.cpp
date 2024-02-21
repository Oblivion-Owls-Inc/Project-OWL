/// @file       SceneSystem.cpp
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      handles the process of resetting and changing scenes
/// @version    0.1
/// @date       2023-09-08
/// 
/// @copyright  Copyright (c) 2023 Digipen Institute of Technology

#include "SceneSystem.h"

#include "EntitySystem.h"
#include "Entity.h"

#include "DebugSystem.h"

#include "AssetLibrarySystem.h"
#include "Sound.h"
#include "Texture.h"
#include "TransformAnimation.h"
#include "AnimationAsset.h"

#include "basics.h"
#include "Stream.h"

#include <filesystem>
#include <algorithm>

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

        Debug() << "Saving scene \"" << *name << "\"..." << std::endl;

        Stream::WriteToFile( scenePath( *name ), Scene().Write() );
    }

    /// @brief Used To create the Save Scene Window
    /// @return - true if the window is open, false if the window is closed
    bool SceneSystem::SaveScene()
    {
        /// Used to keep the window open
        bool _show = true;
        
        //// Creates the window and Auto resizes it
        if (ImGui::Begin("Save Scene", &_show, ImGuiWindowFlags_AlwaysAutoResize))
        {
            /// if at any point the window is closed, return false
            if (!_show)
            {
				ImGui::End();
				return false; // close window
			}

            // Input text for typing scene name
            static char buffer[128] = ""; // Buffer to hold the input, you can save this
            ImGui::InputText("Type Scene Name", buffer, IM_ARRAYSIZE(buffer));

            /// A button to save the scene
            if (ImGui::Button("Save Scene"))
            {
                /// Save the scene using the name typed into the buffer
                SaveScene(buffer);

                ImGui::End();
                return false; // close window
            }
            ImGui::SameLine();
            /// Creates a button to cancel the save
            if (ImGui::Button("Cancel"))
            {
                /// Close the window
				ImGui::End();
				return false; // close window
			}
        }

        ImGui::End();
        return true;
    }

    /// @brief  Used to reset the current scene
    void SceneSystem::ResetScene()
    {
		SetNextScene( m_CurrentSceneName );
    }

    /// @brief Creates the Load Scene Window
    /// @return - true if the window is open, false if the window is closed
    bool SceneSystem::LoadScene()
    {
        /// Used to keep the window open
        bool _show = true;
        
        /// Creates the window and Auto resizes it
        if (ImGui::Begin("Load Scene", &_show, ImGuiWindowFlags_AlwaysAutoResize))
        {
            /// if at any point the window is closed, return false
            if (!_show)
            {
                ImGui::End();
                return false; // close window
            }

            /// Creates a dropdown menu of all the scenes in the scenes directory
            unsigned selectedScene = listScenes();
            
            /// Input text for typing scene name
            static char buffer[128] = ""; // Buffer to hold the input, you can save this
            ImGui::InputText("Type Scene Name", buffer, IM_ARRAYSIZE(buffer));
            
            if (ImGui::GetIO().KeysDown[GLFW_KEY_ENTER] || (ImGui::Button("Load Scene")))
            {
                if (strlen(buffer) > 0)
                {
                    std::string sceneName(buffer);
                    SetNextScene(sceneName); // Load scene using name typed into buffer
                    buffer[0] = '\0'; // Clear the buffer

                    /// Close the window
                    ImGui::End();
                    return false; // close window
                }
                else
                {
                    SetNextScene(m_SceneNames[selectedScene]); // Load scene using selected scene
                    ImGui::End();
                    return false; // close window
                }
            }


        }

		ImGui::End();
        return true; // keep window open
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
            auto it = std::(find_if(
                s_AssetLibraries.begin(), s_AssetLibraries.end(),
                [key](std::pair< std::string, BaseAssetLibrarySystem* (*)() > const& it) -> bool
                {
                    return it.first == key;
                }
            );
            if (it == s_AssetLibraries.end())
            {
                Debug() << "WARNING: (JSON):  unrecognized token " << key << " at " << Stream::GetDebugLocation() << std::endl;
                break;
            }

            Stream::PushDebugLocation( key + "." );

            it->second()->LoadAssets( value );

            Stream::PopDebugLocation();
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
        Debug() << "Loading Scene \"" << m_CurrentSceneName << "\"..." << std::endl;

        Scene scene = Scene();
        Stream::ReadFromFile( &scene, scenePath( m_CurrentSceneName ) );
    }

    /// @brief  Initializes the current Scene
    void SceneSystem::initScene()
    {
        Debug() << "Initializing Scene \"" << m_CurrentSceneName << "\"..." << std::endl;

        for ( System* system : Engine::GetInstance()->GetSystems() )
        {
            system->OnSceneInit();
        }
    }

    /// @brief  Exits the current Scene
    void SceneSystem::exitScene()
    {
        Debug() << "Exiting Scene \"" << m_CurrentSceneName << "\"..." << std::endl;

        for ( System* system : Engine::GetInstance()->GetSystems() )
        {
            system->OnSceneExit();
        }
    }

    /// @brief Creates the Load Scene Window and returns the selected scene index
    /// @return - the selected scene index
    unsigned SceneSystem::listScenes()
    {
        static int selectedScene = -1; // Default index for dropdown, you can save this
        if ( selectedScene < 0 )
        {
            selectedScene = (int)(std::find( m_SceneNames.begin(), m_SceneNames.end(), m_CurrentSceneName ) - m_SceneNames.begin());
            if ( selectedScene == m_SceneNames.size() )
            {
                selectedScene = 0;
            }
        }

        if ( m_SceneNames.size() > 0 )
        {
            if (ImGui::BeginCombo( "Scenes", m_SceneNames[ selectedScene ].c_str()) ) // Display the selected scene name
            {
                getSceneNames();
                for (int i = 0; i < m_SceneNames.size(); i++)
                {
                    if ( ImGui::Selectable( m_SceneNames[i].c_str(), selectedScene == i ) )
                    {
                        selectedScene = i; // Change the selected scene
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
