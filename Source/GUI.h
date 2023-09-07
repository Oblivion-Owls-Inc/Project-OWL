#pragma once
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "glfw/glfw3.h"

class GUI 
{
    public:
        GUI();
        ~GUI();
        void Render();

    private:
        float color[5];
        bool active;
        ImGuiIO io;
};

class DebugMenu : public GUI
{

};