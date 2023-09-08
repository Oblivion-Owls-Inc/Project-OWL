#include "DebugSystem.h"
#include "GUI.h"


ImGuiTextBuffer DebugSystem::logBuffer;
std::vector<GUI*> DebugSystem::windows;
bool DebugSystem::FPS = false;


DebugSystem* DebugSystem::getInstance()
{

    static DebugSystem* instance = nullptr;
    if (instance == nullptr) {
        instance = new DebugSystem();
    }
    return instance;

}

DebugSystem* DebugSystem::getInstance(const char* w_name, int w_width, int w_height)
{
    
    static DebugSystem* instance = nullptr;
    if (instance == nullptr) {
        instance = new DebugSystem();
    }
    return instance;
    
}

/**
 * @brief Initialize the DebugSystem.
 * @param window The GLFW window handle (default is the current context).
 */
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

/**
 * @brief Perform initialization.
 */
void DebugSystem::OnInit()
{
}

/**
 * @brief Perform updates at a fixed time step.
 */
void DebugSystem::OnFixedUpdate()
{
    if (glfwGetKey(_window, GLFW_KEY_F1) == GLFW_PRESS)
    {
        ShowDebugMenu();
    }
}

/**
 * @brief Perform updates.
 * @param dt The time elapsed since the last update.
 */
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

/**
 * @brief Perform cleanup and shutdown.
 */
void DebugSystem::OnExit()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

/**
 * @brief Show the Frames Per Second (FPS) display.
 */
void DebugSystem::ShowFPS()
{
    FPS = true;
}

/**
 * @brief Print a formatted message to the screen.
 * @param format The format string, similar to printf.
 */
void DebugSystem::ScreenPrint(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    logBuffer.appendfv(format, args);
    va_end(args);
}

/**
 * @brief Show or create the DebugMenu GUI window.
 */
void DebugSystem::ShowDebugMenu()
{
    for (GUI* Menu : windows)
    {
        if (strcmp(Menu->GetWindowTitle(), "Debug Menu") == 0)
        {
            Menu->setActive();
            return;
        }
        else
        {
            DebugMenu* newWindow = new DebugMenu();
            windows.push_back(newWindow);
            return;
        }
    }

    DebugMenu* newWindow = new DebugMenu();
    windows.push_back(newWindow);
}

/**
 * @brief Called when a scene is loaded.
 */
void DebugSystem::OnSceneLoad()
{
    // Your scene loading code goes here.
}

/**
 * @brief Called when a scene is initialized.
 */
void DebugSystem::OnSceneInit()
{
    OnInit();
}

/**
 * @brief Called when a scene is exited.
 */
void DebugSystem::OnSceneExit()
{
    OnExit();
}
