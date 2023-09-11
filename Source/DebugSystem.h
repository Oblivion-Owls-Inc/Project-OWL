/// @file DebugSystem.h
/// @brief Debug System Class For Debugging

#pragma once
#include "basics.h"
#include "GUI.h"
#include "System.h"
#include <vector>
#include <iostream>
#include <sstream>

/// @class DebugSystem
/// @brief Debug System Class For Debugging
/// @details This class provides functionality for debugging within your application.
class DebugSystem : public System
{
    public:
        static DebugSystem* instance;
        static DebugSystem* getInstance();

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

        void ToggleDev();

        /// @brief Print a formatted message to the screen
        /// @param format The format string, similar to printf
        /// @details This function allows you to print a formatted message to the screen using ImGui.
        void ScreenPrint(const char* format, ...);

        void ShowDebugMenu();

        void OnFixedUpdate() override;


        // Unused functions (from the base class)
        virtual void OnSceneLoad() override {}
        virtual void OnSceneInit() override {}
        virtual void OnSceneExit() override {}
        virtual void Load( rapidjson::Value const& ) override {}

    private:
        std::vector<GUI*> windows; /// @brief A collection of GUI windows
        GLFWwindow* _window; /// @brief The GLFW window handle
        bool showFpsWindow; /// @brief Flag to control FPS display
        bool showDevWindow; /// @brief Flag to control dev display
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
