#include "DebugSystem.h"

#include "PlatformSystem.h"
#include "implot.h"
#include "GUI.h"
//Chrono
#include <chrono>

template<typename T, size_t size>
struct ScrollingBuffer
{
    ScrollingBuffer()
    {

    }

    T Values[size] = {};

    void push(T element) {
        //for (int i = size - 2; i >= 0; i--)
        //	Values[i] = Values[i + 1];
        for (int i = 0; i < size; i++)
            Values[i] = Values[i + 1];

        Values[size - 1] = element;
    }
    T* Data() { return &Values[0]; }
};

DebugSystem* DebugSystem::instance = nullptr;

DebugSystem* DebugSystem::GetInstance()
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

/// @brief Perform initialization.
void DebugSystem::OnInit()
{
    _window = PlatformSystem::GetInstance()->GetWindowHandle();
    // Setup ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    io = &ImGui::GetIO();
    io->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    ImPlot::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(_window, true);
    ImGui_ImplOpenGL3_Init("#version 430");
    ImGui::StyleColorsClassic();
    io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;
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
    static const int count = 128;
    static float fpses[count] = {};
    static float elapsed = 0.0f;
    static float min = 0, max = 0;
    static float fps = 0.0f;
    elapsed += dt;
    if (elapsed > 0.05f)
    {
        min = 1000.0f; max = 0.0f;
        elapsed -= 0.05f;
        for (int i = count - 1; i > 0; i--)
        {
            fpses[i] = fpses[i - 1];
            if (fpses[i] < min)     min = fpses[i];
            if (fpses[i] > max)     max = fpses[i];
        }
        fps = 1.0f / dt;
        fpses[0] = fps;
    }
    if (showFpsWindow)
    {
        static auto endTime = std::chrono::system_clock::now();

        static int CurrentSample = 0;
        static const int SampleSize = 3;
        static double Samples[SampleSize] = { 0.0f }; //Value accumulation for sample calculation
        static const int ScrollingBufferSize = 100; // total number of samples

        static ScrollingBuffer<double, ScrollingBufferSize> FPS_Values; //scrolling buffer
        auto startTime = std::chrono::system_clock::now();

        std::chrono::duration<double> timeElapsed = startTime - endTime;

        double fps = 1.0 / timeElapsed.count();
        Samples[CurrentSample] = fps;
        CurrentSample++;

        if (CurrentSample == SampleSize)
        {
            double Adder = 0;
            for (int i = 0; i < SampleSize; i++)
            {
                Adder += Samples[i];
            }
            auto newSample = Adder / SampleSize;
            //std::cerr << newSample << std::endl;
            FPS_Values.push(newSample);
            CurrentSample = 0;
        }
        endTime = startTime;


        static ImPlotAxisFlags axis_flags = ImPlotAxisFlags_NoDecorations | ImPlotAxisFlags_Lock;

        static ImPlotFlags PlotFlags = ImPlotFlags_NoLegend;
        if (ImPlot::BeginPlot("FPS", ImVec2(-1, 150), PlotFlags)) {
            ImPlot::SetupAxes(nullptr, nullptr, axis_flags);
            ImPlot::SetupAxisLimits(ImAxis_X1, 0, 100, ImGuiCond_None);
            ImPlot::SetupAxisLimits(ImAxis_Y1, 0, 200.0f, ImGuiCond_None);
            ImPlot::SetNextFillStyle(IMPLOT_AUTO_COL, 0.5f);

            ImPlot::PlotLine("FPS", FPS_Values.Data(), ScrollingBufferSize, 1);
            //ImPlot::PlotShaded("FPS", FPS_Values.Data(), nullptr, ScrollingBufferSize, 0, 0, sizeof(float));
            //ImPlot::PlotLine("Mouse Y", &sdata2.Data[0].x, &sdata2.Data[0].y, sdata2.Data.size(), 0, sdata2.Offset, 2 * sizeof(float));
            ImPlot::EndPlot();
        }
    }

    for (GUI* Menu : windows)
    {
        Menu->Render();
    }

    // Stays at the Bottom
    // Render ImGui
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    auto io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    // Stays at the Top
}

/// @brief Perform cleanup and shutdown.
void DebugSystem::OnExit()
{
    ImGui::Render();
    //ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    //ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImPlot::DestroyContext();
    ImGui::DestroyContext();
}

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
            Menu->SetActive();
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
// private: reading
//-----------------------------------------------------------------------------

    /// @brief          reads whether to show the FPS window
    /// @param stream   the data to read from
    void DebugSystem::readShowFpsWindow( Stream stream )
    {
        showFpsWindow = stream.Read<bool>();
    }

    /// @brief map containing read methods
    ReadMethodMap< DebugSystem > const DebugSystem::s_ReadMethods = {
        { "ShowFpsWindow", &readShowFpsWindow }
    };

    /// @brief  gets the map of read methods
    /// @return the map of read methods
    ReadMethodMap< System > const& DebugSystem::GetReadMethods() const
    {
        return (ReadMethodMap< System > const&)s_ReadMethods;
    }
