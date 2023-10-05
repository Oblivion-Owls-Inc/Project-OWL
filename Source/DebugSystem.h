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

        /// @brief Show the Frames Per Second (FPS) in a debug window
        /// @details This function displays the Frames Per Second (FPS) in a debug window when called.
        void ToggleFPS();

        /// @brief Print a formatted message to the screen
        /// @param format The format string, similar to printf
        /// @details This function allows you to print a formatted message to the screen using ImGui.
        void ScreenPrint(const char* format, ...);

        /// @brief Gets Called by the Debug system to display debug information
        virtual void DebugWindow() override;

private:

        void ShowDebugWindow();

        void ShowFPSWindow();
    
        void ImguiStartFrame();

        void OnFixedUpdate() override;
    
//-----------------------------------------------------------------------------
private: // reading
//-----------------------------------------------------------------------------

    /// @brief          reads whether to show the FPS window
    /// @param stream   the data to read from
    void readShowFpsWindow( Stream stream );

    /// @brief Reads whether to show the debug window
    /// @param stream  the data to read from
    void readShowDebugWindow( Stream stream );

    /// @brief map containing read methods
    static ReadMethodMap< DebugSystem > const s_ReadMethods;

    /// @brief  gets the map of read methods
    /// @return the map of read methods
    virtual ReadMethodMap< System > const& GetReadMethods() const override;

    private:
        GLFWwindow* _window; /// @brief The GLFW window handle
        bool m_ShowFpsWindow; /// @brief Flag to control FPS display
        bool m_ShowDebugWindow; /// @brief Flag to control dev display
        ImGuiIO* io; /// @brief Pointer to the ImGui Input/Output structure
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
