

#include "DebugSystem.h"
#include "GUI.h"

DebugSystem::DebugSystem(GLFWwindow* window) : window(window), io(ImGui::GetIO())
{
}

void DebugSystem::OnInit()
{
     // Setup ImGui context
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 430");

}

void DebugSystem::OnFixedUpdate()
{
}

void DebugSystem::OnUpdate(float dt)
{
}

void DebugSystem::OnExit()
{
    // Terminate ImGui
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void DebugSystem::OnSceneLoad()
{
}

void DebugSystem::OnSceneInit()
{
}


void DebugSystem::OnSceneExit()
{
}

