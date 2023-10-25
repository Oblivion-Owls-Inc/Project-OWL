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

    ImGuiTextBuffer logBuffer;

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

    /// @brief Print a formatted message to the screen
    /// @param format The format string, similar to printf
    /// @details This function allows you to print a formatted message to the screen using ImGui.
    void ScreenPrint(const char* format, ...);

private:

    /// @brief The GLFW window handle
    GLFWwindow* _window;

    /// @brief Flag to control FPS display
    bool m_ShowFpsWindow;

    /// @brief Flag to control dev display
    bool m_ShowDebugWindow;

    /// @brief Pointer to the ImGui Input/Output structure
    ImGuiIO* io;

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

//-----------------------------------------------------------------------------
};

class DebugConsole
{
    public:
        DebugConsole(DebugSystem& system) : _system(system) {}

        template <typename T>
        DebugConsole& operator<<(const T& value)
        {
            _messageStream << value;
            return *this;
        }

        ~DebugConsole()
        {
            _system.ScreenPrint(_messageStream.str().c_str());
        }

    private:
        std::stringstream _messageStream;
        DebugSystem& _system;
};
