/**
 * @file DebugSystem.h
 * @brief Debug System Class For Debugging
 */

#pragma once
#include "basics.h"
#include "GUI.h"
#include "System.h"
#include <vector>

 /**
  * @class DebugSystem
  * @brief Debug System Class For Debugging
  * @details This class provides functionality for debugging within your application.
  */
class DebugSystem : public System
{
    public:

        static DebugSystem* getInstance(const char* w_name, int w_width, int w_height);
        static DebugSystem* getInstance();

        /**
         * @brief Constructor for DebugSystem
         * @param window The GLFW window handle (default is the current context)
         */
        DebugSystem(GLFWwindow* window = glfwGetCurrentContext());

        /**
         * @brief Set the GLFW window handle
         * @param window The GLFW window handle to set
         */
        void SetWindowHandle(GLFWwindow* window) { _window = window; }

        /**
         * @brief Initialize the DebugSystem
         */
        void OnInit() override;

        /**
         * @brief Update the DebugSystem
         * @param dt The time elapsed since the last update
         */
        void OnUpdate(float dt) override;

        /**
         * @brief Exit and clean up the DebugSystem
         */
        void OnExit() override;

        /**
         * @brief Show the Frames Per Second (FPS) in a debug window
         * @details This function displays the Frames Per Second (FPS) in a debug window when called.
         */
        static void ShowFPS();

        /**
         * @brief Print a formatted message to the screen
         * @param format The format string, similar to printf
         * @details This function allows you to print a formatted message to the screen using ImGui.
         */
        static void ScreenPrint(const char* format, ...);

        static void ShowDebugMenu();


        // Unused functions (from the base class)
        void OnFixedUpdate() override;
        void OnSceneLoad() override;
        void OnSceneInit() override;
        void OnSceneExit() override;
        static ImGuiTextBuffer logBuffer;

    private:
        static std::vector<GUI*> windows; /**< A collection of GUI windows */
        GLFWwindow* _window; /**< The GLFW window handle */
        static bool FPS; /**< Flag to control FPS display */
        ImGuiIO* io; /**< Pointer to the ImGui Input/Output structure */
};