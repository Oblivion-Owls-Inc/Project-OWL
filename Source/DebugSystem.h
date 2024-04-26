///*****************************************************************/
/// @file DebugSystem.h
/// @author Jax Clayton (jax.clayton@digipen.edu)
/// 
/// @brief Debug System Class For Debugging
/// @brief The Debug Console for verbose debugging
/// 
/// @details This class provides functionality for debugging 
///          and using ImGui with other Systems.
/// @copyright  Copyright (c) 2024 Digipen Institute of Technology
///*****************************************************************/
#pragma once


///*****************************************************************/
/// Includes:
///*****************************************************************/

#include "pch.h" 

#include "System.h"
#include "Console.h"
#include "PlayBar.h"


/// @brief Forward declaration of the Entity class
class Entity;

/// @brief Creates a Enum for the MenuItemType that 
/// 	  is used to differentiate between the different Asset types to Create
enum class MenuItemType {
    None = -1,
    NewEntity,
    NewArchetype,
    NewSound,
    NewTexture,
    NewTransformAnimation,
    NewSpriteAnimation,
    _Count
};

/// @class DebugSystem
/// @brief Debug System Class For Debugging
/// @details This class provides functionality for debugging within your application.
class DebugSystem : public System
{
public:

    /// @brief  gets the instance of the DebugSystem
    static DebugSystem* GetInstance();

    /// @brief Constructor for DebugSystem
    /// @param window The GLFW window handle (default is the current context)
    DebugSystem();

    /// @brief Set the GLFW window handle
    /// @param window The GLFW window handle to set
    void SetWindowHandle(GLFWwindow* window) { _window = window; }


    /// @brief Initialize the DebugSystem
    void OnInit() override;

    /// @brief Update the DebugSystem
    /// @param dt The time elapsed since the last update
    void OnUpdate(float dt) override;

    /// @brief Exit and clean up the DebugSystem
    void OnExit() override;

    /// @brief Gets Called by the Debug system to display debug information
    virtual void DebugWindow() override;

    /// @brief  Resets the Viewport when debug is closed
    void ResetViewport();

    /// @brief  sets whether the non-editor systems are enabled
    /// @param  enabled wether to enable 
    bool SetNonEditorSystemsEnabled(bool enabled);

    /// @brief  sets whether the editor systems are enabled
    bool IsEditorRunning() const { return m_EditorRunning; }


    /// @brief  gets the DebugSystem PlayBar
    /// @return the DebugSystem PlayBar
    PlayBar& GetPlayBar();


///----------------------------------------------------------------------------
public: // DebugStream
///----------------------------------------------------------------------------
   
    /// @brief Wrapper to write to the DebugStream
    /// @param message - the message to write to the DebugStream
    template <typename Type>
    void Write(const Type& value)
    {
		std::ostringstream oss;
		oss << value;

        #ifndef NDEBUG  // only write to the Console in Debug mode
        std::cout << oss.str();
        #endif

		Stream::WriteToTraceLog(oss.str());
        WritetoConsole(oss.str());
	}

//-----------------------------------------------------------------------------
private: // Members
//-----------------------------------------------------------------------------

    /// @brief The GLFW window handle
    GLFWwindow* _window;

    /// @brief Flag to control FPS display
    bool m_ShowFpsWindow;

    /// @brief Flag to control dev display
    bool m_ShowEntityCreateWindow = false;

    /// @brief Flag to control display of the ImGui demo window
    bool m_ShowDemoWindow = false;
    
    /// @brief flag to control if the window is fullscreen
    bool m_Fullscreen = false;

    /// @brief Flag to show the Asset System List
    bool m_ShowAssetSystemList = false;

    /// @brief Flag to show the Behavior System List
    bool m_ShowBehaviorSystemList = false;

    /// @brief Flag to show the Save Scene Window
    bool m_ShowSceneSaveWindow = false;

    /// @brief If the Editor is Running
    bool m_EditorRunning =
    #ifdef NDEBUG
        true;
    #else
        false;
    #endif

    /// @brief Flag to show the Save Engine Window
    bool m_ShowEngineSaveWindow = false;

    /// @brief Flag to show the Load Scene Window
    bool m_ShowSceneLoadWindow = false;

    /// @brief Flag to show if the Load File Window is open
    bool m_LoadDataFile = false;
    
    /// @brief Flag to Show the various Asset Prefab Windows
    bool m_CreationWindows[ (int)MenuItemType::_Count ] = { 0 };

    /// @brief Map of all the systems and if they are currently open.
    std::map< std::string, bool> m_SystemDebugWindows;

    /// @brief  the Play Bar
    PlayBar m_PlayBar;

    /// @brief Pointer to the ImGui Input/Output structure
    ImGuiIO* io;

    /// @brief the buffer for the DebugStream
    std::stringstream _buffer;

    /// @brief Whether to show the Debug windows for All the Systems
    bool m_ShowDebugWindows = true;

    /// @brief  the names of all Systems that are enabled in the Editor
    std::set< std::string > const m_EditorSystemNames = {
        "PlatformSystem",
        "InputSystem",
        "SceneSystem",
        "EntitySystem",
        "CameraSystem",
        "RenderSystem",
        "AudioSystem",
        "DebugSystem",
        "ParticleSystem",
        "BehaviorSystem<EditorCameraController>",
        "PathfindSystem",
        "BehaviorSystem<Generator>",
        "ControlPromptSystem",
        "LightingSystem"
    };

//-----------------------------------------------------------------------------
private: // methods
//-----------------------------------------------------------------------------

    /// @brief Shows the FPS window
    void ShowFPSWindow();
    
    /// @brief Shows the Debug window
    void ImguiStartFrame();

    /// @brief  Shows at a fixed framerate
    void OnFixedUpdate() override;

    /// @brief Shows the Debug window
    void MenuWindows();

    /// @brief Shows the List of Systems that begin with the given prefix
    /// @param prefix - the prefix to filter the list of Systems by
    void ShowSystemList(const std::string& prefix);

    /// @brief Sets up the ImGui configuration path
    void SetupImGuiConfigPath();

    /// @brief Writes to the Console
    void WritetoConsole(const std::string& message);
//-----------------------------------------------------------------------------
private: // reading
//-----------------------------------------------------------------------------

    /// @brief          reads whether to show the FPS window
    /// @param stream   the data to read from
    void readShowFpsWindow( nlohmann::ordered_json const& data );

    /// @brief Reads whether to show the debug window
    /// @param stream  the data to read from
    void readShowDebugWindow( nlohmann::ordered_json const& data );

    /// @brief Reads which systems the debug windows were open for
    /// @param data The json data to read from
    void readOpenSystemWindows(nlohmann::ordered_json const& data);
    
        
    void LoadDebugWindowStates();

    nlohmann::ordered_json WriteSystemDebugWindowStates();

    /// @brief map containing read methods
    static ReadMethodMap< DebugSystem > const s_ReadMethods;

    /// @brief  gets the map of read methods
    /// @return the map of read methods
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override
    {
        return (ReadMethodMap< ISerializable > const&)s_ReadMethods;
    }

//-----------------------------------------------------------------------------
public: // writing
//-----------------------------------------------------------------------------

    /// @brief  writes this System config
    /// @return the writting System config
    virtual nlohmann::ordered_json Write() const override;
};


///-----------------------------------------------------------------------------
/// DebugStream
///-----------------------------------------------------------------------------

/// @brief Templated function to write to the DebugStream
/// @tparam T - the type of data to write to the DebugStream
/// @param debugSystem - the DebugSystem to write to
/// @param value - the value to write to the DebugStream
/// @return 
template<typename T>
__inline DebugSystem& operator<<(DebugSystem* debugSystem, const T& value)
{
    debugSystem->Write(value);
    return *debugSystem;
}

/// @brief Overloaded function to write to the DebugStream
/// @param debugSystem - the DebugSystem to write to
/// @param value - the value to write to the DebugStream
/// @return - the DebugSystem
template<typename T>
__inline DebugSystem& operator<<(DebugSystem& debugSystem, T value)
{
    debugSystem.Write(value);
    return debugSystem;
}

/// @brief Overloaded function to write to the DebugStream
/// @param debugSystem - the DebugSystem to write to
/// @param value - the value to write to the DebugStream
/// @return - the DebugSystem
__inline DebugSystem& operator<<(DebugSystem& debugSystem, const char* value)
{
    debugSystem.Write(value);
    return debugSystem;
}


/// @brief Overloaded function to write GLubyte* to the DebugStream
/// @param debugSystem - the DebugSystem to write to
/// @param value -  the value to write to the DebugStream
/// @return - the DebugSystem
__inline DebugSystem& operator<<(DebugSystem& debugSystem, const GLubyte* value)
{
    debugSystem.Write(reinterpret_cast<const char*>(value));
    return debugSystem;
}


/// @brief 0verloaded function to write to the DebugStream
/// @param debugSystem - the DebugSystem to write to
/// @param os - the ostream to write to the DebugStream
/// @return - the DebugSystem
inline DebugSystem& operator<<(DebugSystem& debugSystem, std::ostream& (*os)(std::ostream&))
{
    if (os == static_cast<std::ostream & (*)(std::ostream&)>(std::endl))
    {
        // Handle std::endl if necessary
        debugSystem.Write("\n");
    }
    return debugSystem;
}

inline DebugSystem& operator<<(DebugSystem& debugSystem, const std::string& value)
{
    debugSystem.Write(value);
    return debugSystem;
}

/// @brief shorthand for getting the DebugSystem
/// @return instance of the DebugSyste
__inline DebugSystem& Debug() { return *DebugSystem::GetInstance(); }
