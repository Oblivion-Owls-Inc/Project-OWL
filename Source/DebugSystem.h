///*****************************************************************/
/// @file DebugSystem.h
/// @author Jax Clayton (jax.clayton@digipen.edu)
/// 
/// @brief Debug System Class For Debugging
/// @brief The Debug Console for verbose debugging
/// 
/// @details This class provides functionality for debugging 
///          and using ImGui with other Systems.
///*****************************************************************/
#pragma once


///*****************************************************************/
/// Includes:
///*****************************************************************/
#include "basics.h"
#include "System.h"
#include <vector>
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"
#include <iostream>
#include <sstream>
#include <set>



/// @brief Forward declaration of the Entity class
class Entity;

/// @class DebugSystem
/// @brief Debug System Class For Debugging
/// @details This class provides functionality for debugging within your application.
class DebugSystem : public System
{
public:
    static DebugSystem* instance;

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
        std::cout << oss.str();
		Stream::WriteToTraceLog(oss.str());
	}

//-----------------------------------------------------------------------------
private: // Members
//-----------------------------------------------------------------------------

    /// @brief The GLFW window handle
    GLFWwindow* _window;

    /// @brief Flag to control FPS display
    bool m_ShowFpsWindow;

    /// @brief Flag to control dev display
    bool m_ShowDebugWindow;

    /// @brief Pointer to the ImGui Input/Output structure
    ImGuiIO* io;

    /// @brief the buffer for the DebugStream
    std::stringstream _buffer;

    /// @brief  the names of all Systems that are enabled in the Editor
    std::set< std::string > const m_EditorSystemNames = {
        "PlatformSystem",
        "InputSystem",
        "SceneSystem",
        "EntitySystem",
        "CameraSystem",
        "RenderSystem",
        "AudioSystem",
        "DebugSystem"
    };

//-----------------------------------------------------------------------------
private: // methods
//-----------------------------------------------------------------------------

    /// @brief  sets whether the non-editor systems are enabled
    /// @param  enabled wether to enable 
    void SetNonEditorSystemsEnabled( bool enabled );

    void ShowFPSWindow();
    
    void ImguiStartFrame();

    void OnFixedUpdate() override;
    
//-----------------------------------------------------------------------------
private: // reading
//-----------------------------------------------------------------------------

    /// @brief          reads whether to show the FPS window
    /// @param stream   the data to read from
    void readShowFpsWindow( nlohmann::ordered_json const& data );

    /// @brief Reads whether to show the debug window
    /// @param stream  the data to read from
    void readShowDebugWindow( nlohmann::ordered_json const& data );

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
