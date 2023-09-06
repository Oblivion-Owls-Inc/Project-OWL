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
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "System.h"

/**
 * @brief Debug System Class For Debugging
 * 
 */
class DebugSystem : public System
{
    DebugSystem(GLFWwindow* window);

    public:
        void OnInit() override;
        void OnFixedUpdate() override;
        void OnUpdate(float dt) override;
        void OnExit() override;
        void OnSceneLoad() override;
        void OnSceneInit() override;
        void OnSceneExit() override;
    private:
        GLFWwindow* window;
        ImGuiIO& io;
};

