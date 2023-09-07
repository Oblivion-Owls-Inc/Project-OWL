/**
 * @file DebugSystem.h
 * @author Jax Clayton (jax.clayton@digipen.edu)
 * @brief   Debug System
 * @version 0.1
 * @date 2023-09-05
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#pragma once
#include "basics.h"
#include "GUI.h"
#include "System.h"
#include <vector>


/**
 * @brief Debug System Class For Debugging
 * 
 */
class DebugSystem : public System
{
    public:
    DebugSystem(GLFWwindow* window = glfwGetCurrentContext());

    public:

        void SetWindowHandle(GLFWwindow* window) { _window = window; }
        void OnInit() override;
        void OnUpdate(float dt) override;
        void OnExit() override;
        static void ShowFPS();
        //unused
        void OnFixedUpdate() override;
        void OnSceneLoad() override;
        void OnSceneInit() override;
        void OnSceneExit() override;
    private:
        std::vector< GUI* > windows;
        GLFWwindow* _window;
        static bool FPS;
        ImGuiIO* io;
};

