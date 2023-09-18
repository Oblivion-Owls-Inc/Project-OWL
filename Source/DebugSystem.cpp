#include "DebugSystem.h"

#include "PlatformSystem.h"
#include "GUI.h"


//-----------------------------------------------------------------------------
// public methods
//-----------------------------------------------------------------------------

    /// @brief Show the Frames Per Second (FPS) display.
    void DebugSystem::ToggleFPS()
    {
        if (showFpsWindow)
        {
            showFpsWindow = false;
        }
        else
        {
            showFpsWindow = true;
        }
    }

    void DebugSystem::ToggleDev()
    {
        if (showDevWindow)
        {
            showDevWindow = false;
        }
        else
        {
            showDevWindow = true;
        }
    }

    /// @brief Print a formatted message to the screen.
    /// @param format The format string, similar to printf.
    void DebugSystem::ScreenPrint(const char* format, ...)
    {
        va_list args;
        va_start(args, format);
        logBuffer.appendfv(format, args);
        va_end(args);
    }

    /// @brief Show or create the DebugMenu GUI window.
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

//-----------------------------------------------------------------------------
// private virtual override methods
//-----------------------------------------------------------------------------

    /// @brief Perform initialization.
    void DebugSystem::OnInit()
    {
        _window = PlatformSystem::getInstance()->GetWindowHandle();
        // Setup ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        io = &ImGui::GetIO();
        ImGui_ImplGlfw_InitForOpenGL(_window, true);
        ImGui_ImplOpenGL3_Init("#version 430");
        ImGui::StyleColorsDark();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        // Stays at the Top
    }

    /// @brief PerDorm updates at a fixed time step.
    void DebugSystem::OnFixedUpdate()
    {
        if (glfwGetKey(_window, GLFW_KEY_F1) == GLFW_PRESS)
        {
            ShowDebugMenu();
        }
    }

    /// @brief Perform updates.
    /// @param dt The time elapsed since the last update.
    void DebugSystem::OnUpdate(float dt)
    {

        if (showFpsWindow)
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

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        // Stays at the Top
    }

    /// @brief Perform cleanup and shutdown.
    void DebugSystem::OnExit()
    {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }
    
//-----------------------------------------------------------------------------
// class-specific Read Methods
//-----------------------------------------------------------------------------

    void DebugSystem::ReadShowFPSWindow( rapidjson::Value const& jsonValue )
    {
        showFpsWindow = jsonValue.GetBool();
    }

    /// @brief the Read Methods used in this System
    std::map< std::string, ReadMethod< DebugSystem > > const DebugSystem::ReadMethods = {
        { "showFpsWindow", &ReadShowFPSWindow }
    };

//-----------------------------------------------------------------------------
// default Read method stuff
//-----------------------------------------------------------------------------

    /// @brief Gets the read methods of this System
    /// @return the map of read methods of this System
    std::map< std::string, ReadMethod< System > > const& DebugSystem::GetReadMethods()
    {
        return (std::map< std::string, ReadMethod< System > > const&)ReadMethods;
    }

//-----------------------------------------------------------------------------
// singleton stuff
//-----------------------------------------------------------------------------

    DebugSystem* DebugSystem::instance = nullptr;

    DebugSystem* DebugSystem::getInstance()
    {

        if (instance == nullptr) 
        {
            instance = new DebugSystem();
        }

        return instance;

    }

    /// @brief Initialize the DebugSystem.
    /// @param window The GLFW window handle (default is the current context).
    DebugSystem::DebugSystem() :
        _window(nullptr),
        io(nullptr),
        showFpsWindow(false),
        showDevWindow(false)
    {}

//-----------------------------------------------------------------------------
