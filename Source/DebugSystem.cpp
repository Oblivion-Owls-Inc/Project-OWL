

#include "DebugSystem.h"
#include "GUI.h"

bool DebugSystem::FPS = false;

DebugSystem::DebugSystem(GLFWwindow* window) : _window(window), io(nullptr)
{
    // Setup ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    io = &ImGui::GetIO();
    ImGui_ImplGlfw_InitForOpenGL(_window, true);
    ImGui_ImplOpenGL3_Init("#version 430");
    ImGui::StyleColorsDark();
}

void DebugSystem::OnInit()
{

}

void DebugSystem::OnFixedUpdate()
{
}

void DebugSystem::OnUpdate(float dt)
{

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    // Stays at the Top

    if (FPS)
    {
        ImGui::Begin("FPS");
        ImGui::Text("FPS: %d", static_cast<int>(1.0f / dt));
        ImGui::End();
    }


    for (GUI* Menu : windows)
    {
        Menu->Render();
    }





    // Stays at the Bottom
    // Render ImGui
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

}

void DebugSystem::OnExit()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void DebugSystem::ShowFPS()
{
    FPS = true;
}

void DebugSystem::OnSceneLoad()
{
}

void DebugSystem::OnSceneInit()
{
    OnInit();
}


void DebugSystem::OnSceneExit()
{
    OnExit();
}

