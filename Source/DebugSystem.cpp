///*****************************************************************/
/// @file DebugSystem.cpp
/// @author Jax Clayton (jax.clayton@digipen.edu)
/// 
/// @brief Debug System Class For Debugging
/// @brief The Debug Console for verbose debugging
/// 
/// @details This class provides functionality for debugging 
///          and using ImGui with other Systems.
///*****************************************************************/

///*****************************************************************/
/// Includes
///*****************************************************************/
#include "DebugSystem.h"
#include "PlatformSystem.h"
#include "InputSystem.h"
#include "FileExplorer.h"
#include "CheatSystem.h"
#include "AssetLibrarySystem.h"
#include "SceneSystem.h"
#include "BehaviorSystem.h"
#include "Sound.h"
#include "Texture.h"
#include "Animation.h"
#include "TransformAnimation.h"
#include "BulletBehavior.h"
#include "implot.h"
#include "Transform.h"
#include "Engine.h"
#include "EntitySystem.h"
#include <chrono>
#include <string>

///*****************************************************************/
/// @struct ScrollingBuffer
/// @brief A struct for storing a scrolling buffer of values. Used for FPS
///*****************************************************************/
template<typename T, size_t size>
struct ScrollingBuffer
{
    ScrollingBuffer()
    {
    }

    T Values[size + 1] = {};

    void push(T element) 
    {
        for (int i = 0; i < size; i++)
            Values[i] = Values[i + 1];

        Values[size - 1] = element;
    }
    T* Data() { return &Values[0]; }
};

void DebugSystem::SetupImGuiConfigPath()
{
    // Retrieve the APPDATA environment variable using _dupenv_s
    char* appDataPath = nullptr;
    size_t size;
    errno_t err = _dupenv_s(&appDataPath, &size, "APPDATA");

    if (err || appDataPath == nullptr)
    {
        std::cerr << "Error: Unable to retrieve APPDATA environment variable." << std::endl;
        io->IniFilename = nullptr;  
    }
    else
    {
        // Construct the full path to the imgui.ini file
        std::string iniDirectory = std::string(appDataPath) + "\\Dig_Deeper";
        std::string iniFilePath = iniDirectory + "\\imgui.ini";

        io->IniFilename = _strdup(iniFilePath.c_str());  // Allocate a copy of the string for ImGui

        // Clean up
        free(appDataPath); // Use free() to deallocate memory allocated by _dupenv_s
    }
}

/// @brief Perform initialization.
void DebugSystem::OnInit()
{
    _window = PlatformSystem::GetInstance()->GetWindowHandle();
    // Setup ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    io = &ImGui::GetIO();

#ifdef NDEBUG //if in release mode
    SetupImGuiConfigPath(); //set up the imgui config path to the appdata folder
#endif // NDEBUG

    ImFont* font = io->Fonts->AddFontDefault();
    if (font) {
        font->Scale = 1.3f;  // Increase the scale to make the font larger
    }
    io->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    ImPlot::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(_window, true);
    ImGui_ImplOpenGL3_Init("#version 430");
    //ImGui::StyleColorsClassic();
    io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    // Stays at the Top

    SetDebugEnable( true );
}

/// @brief Perform updates.
/// @param dt The time elapsed since the last update.
void DebugSystem::OnUpdate(float dt)
{
#ifndef NDEBUG

    for ( System* system : Engine::GetInstance()->GetSystems() )
    {
        if ( system->GetDebugEnabled() )
        {
            system->DebugWindow();
        }
    }

    if (m_ShowFpsWindow)
    {
        ShowFPSWindow();
    }

#endif // !NDEBUG

    if (Input()->GetKeyTriggered(GLFW_KEY_RIGHT_ALT), Input()->GetKeyTriggered(GLFW_KEY_ENTER))
    {
		m_Fullscreen = !m_Fullscreen;
		PlatformSystem::GetInstance()->setFullscreen(m_Fullscreen);
	}

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    ImguiStartFrame();
}

/// @brief Displays and Creates the Editor Window
void DebugSystem::DebugWindow()
{
    static bool gameplayRunning = true;

    bool debugWindowShown = GetDebugEnabled();
    ImGui::Begin("Editor Window", &debugWindowShown, ImGuiWindowFlags_MenuBar);
    SetDebugEnable( debugWindowShown );

    ImGui::SetWindowSize(ImVec2(700, 700), ImGuiCond_FirstUseEver);
    ImGui::SetWindowPos(ImVec2(0, 0), ImGuiCond_FirstUseEver);


    /// This Creates the Menu Bar to hold the Menu Items [File, Settings, Insert, Systems]
    if (ImGui::BeginMenuBar())
    {
        /// File menu which holds [Load Scene, Save Scene, Save Engine Config, Close]
        if (ImGui::BeginMenu("File"))
        {
            ///Loads the Scene
            if (ImGui::MenuItem("Load Scene", "Ctrl+N"))
            {
                m_ShowSceneLoadWindow = true;
            }

            if (ImGui::MenuItem("Load File"))
            {
                m_LoadDataFile = true;
			}

            /// Reloads the Scene
            if (ImGui::MenuItem("Reload Scene"))
            {
                SceneSystem::GetInstance()->ResetScene();
			}

            /// Opens the Save Menu which holds [Save All, Save Scene, Save Engine Config]
            if (ImGui::BeginMenu("Save.."))
            {
                /// Saves the Scene and Engine Config
                if (ImGui::MenuItem("Save All"))
                {
					m_ShowSceneSaveWindow = true;
					m_ShowEngineSaveWindow = true;
				}

                /// Saves the Scene
                if (ImGui::MenuItem("Save Scene"))
                {
                    m_ShowSceneSaveWindow = true;
				}

                /// Saves the Engine Config
                if (ImGui::MenuItem("Save Engine Config"))
                {
                    m_ShowEngineSaveWindow = true;
                }

                ImGui::EndMenu();
            }

            /// Closes the Debug Window
            if (ImGui::MenuItem("Close"))
            {
                SetDebugEnable( false ); /// Closes the Debug Window for now
            }
            ImGui::EndMenu();
        }

        /// Settings menu which holds [Show FPS, Fullscreen, Show Demo]
        if (ImGui::BeginMenu("Settings"))
        {
            /// Shows the FPS Window
            if (ImGui::MenuItem(m_ShowFpsWindow ? "Hide FPS" : "Show FPS"))
            {
               m_ShowFpsWindow = !m_ShowFpsWindow;
            }

            /// Toggles Fullscreen
            if (ImGui::MenuItem(m_Fullscreen ? "Windowed" : "Fullscreen"))
            {
				m_Fullscreen = !m_Fullscreen;
				PlatformSystem::GetInstance()->setFullscreen(m_Fullscreen);
			}

            /// Shows the ImGui Demo Window
            if (ImGui::MenuItem(m_ShowDemoWindow ? "Hide DEMO" : "Show DEMO"))
            {
				m_ShowDemoWindow = !m_ShowDemoWindow;
            }

            /// Pauses the Gameplay
            if (ImGui::MenuItem(gameplayRunning ? "Pause Gameplay" : "Resume Gameplay"))
            {
				gameplayRunning = !gameplayRunning;
				SetNonEditorSystemsEnabled(gameplayRunning);
			}

            ImGui::EndMenu();
        }

        /// Insert menu which holds [New Entity, New Asset Prefab]
        if (ImGui::BeginMenu("Insert"))
        {
            /// Creates a new Entity
            if (ImGui::MenuItem("New Entity"))
            {
                m_ShowEntityCreateWindow = true;
            }

            /// Starts the Asset Prefab Menu which holds 
            /// [New Sound, New Texture, New Transform Animation, New Sprite Animation]
            if (ImGui::BeginMenu("New Asset Prefab"))
            {
                /// Opens the New Sound Window
                if (ImGui::MenuItem("New Sound")) 
                {
                    m_CreationWindows[(int)MenuItemType::NewSound] = true;
                }

                /// Opens the New Texture Window
                if (ImGui::MenuItem("New Texture")) 
                {
                    m_CreationWindows[(int)MenuItemType::NewTexture] = true;
                }

                /// Opens the New Transform Animation Window
                if (ImGui::MenuItem("New Transform Animation")) 
                {
                    m_CreationWindows[(int)MenuItemType::NewTransformAnimation] = true;
                }

                /// Opens the New Sprite Animation Window
                if (ImGui::MenuItem("New Sprite Animation")) 
                {
                    m_CreationWindows[(int)MenuItemType::NewSpriteAnimation] = true;
                }

                /// Close the Menu for the Asset Prefabs
                ImGui::EndMenu();
            }

            /// Close the Menu for the Insert Menu
            ImGui::EndMenu();
        }

        /// Creates the Systems Menu which holds [Asset Prefabs, Behavior Systems]
        if (ImGui::BeginMenu("Systems"))
        {
            /// Creates the Asset Prefabs Menu which holds [AssetLibrary]
            if (ImGui::BeginMenu("Asset Prefabs"))
            {
                /// Shows the AssetLibrary System List
                ShowSystemList("AssetLibrary");
                ImGui::EndMenu();
            }

            /// Creates the Behavior Systems Menu which holds [BehaviorSystem]
            if (ImGui::BeginMenu("Behavior Systems"))
            {
                /// Shows the Behavior System List
                ShowSystemList("BehaviorSystem");
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Other Systems"))
            {
                ShowSystemList("Other");
                ImGui::EndMenu();
            }

            /// Close the Menu for the Systems Menu
            ImGui::EndMenu();
        }

        /// Close the Menu Bar
        ImGui::EndMenuBar();
    }

    /// Opens the Different Windows for the Editor Window
    MenuWindows();

    /// Creates a Spit Window for the EntityList and their components in the main
    /// editor window
    Entities()->DebugWindow();

    /// Ends the Editor Window
    ImGui::End();
}

void DebugSystem::ResetViewport()
{
    static bool dockspaceOpen = true;
    if (dockspaceOpen)
    {
        io->ConfigFlags &= ~ImGuiConfigFlags_ViewportsEnable;
        dockspaceOpen = false;
    }
    else
    {
        io->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
        dockspaceOpen = true;
    }
}

/// @brief Opens the Different Windows for the Editor Window
void DebugSystem::MenuWindows()
{
    ///----------------------------------------------///
    /// Open Entity | Debug | Asset System Windows  ///
    ///--------------------------------------------///

    /// Opens the Entity Create Window
    if (m_ShowEntityCreateWindow)
    {
       /// if the Entity Create Window is closed, then close the window
        m_ShowEntityCreateWindow = Entities()->EntityCreateWindow();
    }

    /// Opens the Debug Window for me to Compare 
    if (m_ShowDemoWindow)
    {
        /// if the Demo Window is closed, then close the window
		ImGui::ShowDemoWindow(&m_ShowDemoWindow);
	}
    
    /// Opens the Asset System List
    if (m_ShowAssetSystemList)
    {
        /// Shows the AssetLibrary System List
        ShowSystemList("AssetLibrary");
    }

    /// Opens the Behavior System List
    if (m_ShowBehaviorSystemList)
    {
        /// Shows the Behavior System List
		ShowSystemList("BehaviorSystem");
	}

    if (m_LoadDataFile)
    {
        FileExplorer::Explore(&m_LoadDataFile);
	}

    ///-------------------------------------------///
    /// Save Scene and Engine Config Windows      ///
    ///-------------------------------------------///

    /// Opens the Save Scene Window
    if (m_ShowSceneLoadWindow)
    {
        /// if the Save Scene Window is closed, then close the window
        m_ShowSceneLoadWindow = SceneSystem::GetInstance()->LoadScene();
    }

    /// Opens the Save Scene Window
    if (m_ShowSceneSaveWindow)
    {
        /// if the Save Scene Window is closed, then close the window
		m_ShowSceneSaveWindow = SceneSystem::GetInstance()->SaveScene();
    }

    /// Opens the Save Engine Window
    if (m_ShowEngineSaveWindow)
    {
        /// if the Save Engine Window is closed, then close the window
        m_ShowEngineSaveWindow = GameEngine()->SaveEngineConfig();
	}

    ///-------------------------------------------///
    /// Asset Prefab Creation Windows             ///
    ///-------------------------------------------///

    /// Opens the New Asset Prefab Windows for Sound
    if (m_CreationWindows[(int)MenuItemType::NewSound])
    {
        /// if the New Sound Window is closed, then close the window
        if (!AssetLibrary<Sound>()->DebugCreateAssetWindow())
        {
            /// Closes the New Sound Window
            m_CreationWindows[(int)MenuItemType::NewSound] = false;
        }
    }

    /// Opens the New Asset Prefab Windows for Texture
    if (m_CreationWindows[(int)MenuItemType::NewTexture])
    {
        /// if the New Texture Window is closed, then close the window
        if (!AssetLibrary<Texture>()->DebugCreateAssetWindow())
        {
            /// Closes the New Texture Window
			m_CreationWindows[(int)MenuItemType::NewTexture] = false;
		}
	}

    /// Opens the New Asset Prefab Windows for Transform Animation
    if (m_CreationWindows[(int)MenuItemType::NewTransformAnimation])
    {
        /// if the New Transform Animation Window is closed, then close the window
        if (!AssetLibrary<TransformAnimation>()->DebugCreateAssetWindow())
        {
            /// Closes the New Transform Animation Window
            m_CreationWindows[(int)MenuItemType::NewTransformAnimation] = false;
        }
    }

    /// Opens the New Asset Prefab Windows for Sprite Animation
    if (m_CreationWindows[(int)MenuItemType::NewSpriteAnimation])
    {
        /// if the New Sprite Animation Window is closed, then close the window
        if (!AssetLibrary<AnimationAsset>()->DebugCreateAssetWindow())
        {
			m_CreationWindows[(int)MenuItemType::NewSpriteAnimation] = false;
		}
	}
}

/// @brief Shows the System List for the Editor Window
/// @param prefix - the prefix for the System List to find the correct Systems
void DebugSystem::ShowSystemList(const std::string& prefix)
{
    std::vector<System*> filteredSystems; /// Vector of Systems to be filtered

    if (prefix == "Other")
    {
        for (auto& system : Engine::GetInstance()->GetSystems())
        {
            if (system->GetName().compare(0, std::string("AssetLibrary").length(), std::string("AssetLibrary")) != 0 &&
                system->GetName().compare(0, std::string("BehaviorSystem").length(), std::string("BehaviorSystem")) != 0)
            {
                // Skip the debug system
                if (system == GetInstance())
                    continue;
            
				filteredSystems.push_back(system);
			}
		}
	}
    else
    {

        /// Loops through all the Systems in the Engine
        for (auto& system : Engine::GetInstance()->GetSystems())
        {
            // Skip the debug system
            if (system == GetInstance())
                continue;

            // Check if the system's name begins with the specified prefix
            if (system->GetName().compare(0, prefix.length(), prefix) == 0)
            {
                /// Adds the System to the filteredSystems vector
                filteredSystems.push_back(system);
            }
        }
    }

    /// Loops through all the Systems in the filteredSystems vector
    for (auto& system : filteredSystems)
    {
        // Create a menu item for each system
        if (ImGui::MenuItem(system->GetName().c_str()))
        {
            /// Toggles the System on or off
            system->SetDebugEnable(!system->GetDebugEnabled());
        }
    }
}

/// @brief PerDorm updates at a fixed time step.
void DebugSystem::OnFixedUpdate()
{
    #ifndef NDEBUG  
    if ( InputSystem::GetInstance()->GetKeyTriggered( GLFW_KEY_GRAVE_ACCENT ) )
    {
        SetDebugEnable( !GetDebugEnabled() );
    }
    #endif
}


/// @brief Perform cleanup and shutdown.
void DebugSystem::OnExit()
{
    ImGui::Render();
    ImGui_ImplGlfw_Shutdown();
    ImPlot::DestroyContext();
    ImGui::DestroyContext();
}


/// @brief  sets whether the non-editor systems are enabled
/// @param  enabled wether to enable or disable the systems
void DebugSystem::SetNonEditorSystemsEnabled( bool enabled )
{
    for ( System* system : Engine::GetInstance()->GetSystems() )
    {
        if (system == this || Cheats())
        {
			continue;
		}

        if ( m_EditorSystemNames.contains( system->GetName() ) == false )
        {
            system->SetEnabled( enabled );
        }
    }
}


void DebugSystem::ShowFPSWindow()
{
    static auto endTime = std::chrono::system_clock::now();

    static int CurrentSample = 0;
    static const int SampleSize = 3;
    static double Samples[SampleSize] = { 0.0f }; //Value accumulation for sample calculation
    static const int ScrollingBufferSize = 100; // total number of samples

    static ScrollingBuffer<double, ScrollingBufferSize> FPS_Values; //scrolling buffer
    auto startTime = std::chrono::system_clock::now();

    std::chrono::duration<double> timeElapsed = startTime - endTime;

    double fps = 1.0 / timeElapsed.count();
    Samples[CurrentSample] = fps;
    CurrentSample++;

    if (CurrentSample == SampleSize)
    {
        double Adder = 0;
        for (int i = 0; i < SampleSize; i++)
        {
            Adder += Samples[i];
        }
        auto newSample = Adder / SampleSize;
        FPS_Values.push(newSample);
        CurrentSample = 0;
    }
    endTime = startTime;


    static ImPlotAxisFlags axis_flags = ImPlotAxisFlags_NoDecorations
        | ImPlotAxisFlags_Lock;

    static ImPlotFlags PlotFlags = ImPlotFlags_NoLegend;
    if (ImPlot::BeginPlot("FPS", ImVec2(-1, 150), PlotFlags)) 
    {
        ImPlot::SetupAxes(nullptr, nullptr, axis_flags);
        ImPlot::SetupAxisLimits(ImAxis_X1, 0, 100, ImGuiCond_None);
        ImPlot::SetupAxisLimits(ImAxis_Y1, 0, 200.0f, ImGuiCond_None);
        ImPlot::SetNextFillStyle(IMPLOT_AUTO_COL, 0.5f);

        ImPlot::PlotLine("FPS", FPS_Values.Data(), ScrollingBufferSize, 1);
        ImPlot::EndPlot();
    }
}

void DebugSystem::ImguiStartFrame()
{
    auto& io = ImGui::GetIO();

    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------

    /// @brief  reads whether to show the FPS window
    /// @param  data    the data to read from
    void DebugSystem::readShowFpsWindow( nlohmann::ordered_json const& data )
    {
        m_ShowFpsWindow = Stream::Read<bool>( data );
    }

    /// @brief  reads whether to show the debug window
    /// @param  data    the data to read from
    void DebugSystem::readShowDebugWindow( nlohmann::ordered_json const& data )
    {
		SetDebugEnable( Stream::Read<bool>( data ) );
    }

    /// @brief map containing read methods
    ReadMethodMap< DebugSystem > const DebugSystem::s_ReadMethods = {
        { "ShowFpsWindow", &readShowFpsWindow },
		{ "ShowDebugWindow", &readShowDebugWindow }
    };

//-----------------------------------------------------------------------------
// public: writing
//-----------------------------------------------------------------------------

    /// @brief  writes this System config
    /// @return the writting System config
    nlohmann::ordered_json DebugSystem::Write() const
    {
        nlohmann::ordered_json json;
        
        json[ "ShowFpsWindow" ] = m_ShowFpsWindow;
        json[ "ShowDebugWindow" ] = GetDebugEnabled();

        return json;
    }

//-----------------------------------------------------------------------------
// singleton stuff
//-----------------------------------------------------------------------------

    /// @brief Initialize the DebugSystem.
    /// @param window The GLFW window handle (default is the current context).
    DebugSystem::DebugSystem() :
        System( "DebugSystem" ),
        _window(nullptr),
        io(nullptr),
        m_ShowFpsWindow(false)
    {}

    DebugSystem* DebugSystem::instance = nullptr;

    DebugSystem* DebugSystem::GetInstance()
    {

        if (instance == nullptr) 
        {
            instance = new DebugSystem();
        }

        return instance;

    }

//-----------------------------------------------------------------------------
