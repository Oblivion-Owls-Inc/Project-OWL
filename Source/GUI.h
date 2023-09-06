#pragma once
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "glfw/glfw3.h"

class GUI 
{
    public:
        GUI(GLFWwindow* window);
        ~GUI();
        void Render();
        // Add other GUI-related functions here

    private:
        GLFWwindow* window;
        bool active;
        float *color;
        ImGuiIO io;
};

