///*****************************************************************/
/// @file DebugSystem.cpp
/// @author Jax Clayton (jax.clayton@digipen.edu)
/// 
/// @brief Debug System Class For Debugging
/// @brief The Debug Console for verbose debugging
/// 
/// @details This class provides functionality for debugging 
///          and using ImGui with other Systems.
///*****************************************************************/

///*****************************************************************/
/// Includes
///*****************************************************************/
#include "DebugSystem.h"
#include "PlatformSystem.h"
#include "InputSystem.h"
#include "implot.h"
#include "Transform.h"
#include "Engine.h"
#include "EntitySystem.h"
#include <chrono>



///*****************************************************************/
/// @struct ScrollingBuffer
/// @brief A struct for storing a scrolling buffer of values. Used for FPS
///*****************************************************************/
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


/// @brief Perform initialization.
void DebugSystem::OnInit()
{
    _window = PlatformSystem::GetInstance()->GetWindowHandle();
    // Setup ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    io = &ImGui::GetIO();
    ImFont* font = io->Fonts->AddFontDefault();
    if (font) {
        font->Scale = 1.3f;  // Increase the scale to make the font larger
    }
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

    if (m_ShowFpsWindow)
        ShowFPSWindow();
    
    if (m_ShowDebugWindow)
        DebugWindow();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    ImguiStartFrame();
}

void DebugSystem::DebugWindow()
{
    //ImGui::ShowDemoWindow();

    if ( !ImGui::Begin( "Debug Window" ) )
    {
        return;
    }

    static bool gameplayRunning = true;
    if ( ImGui::Button( gameplayRunning ? "Disable gameplay (editor only)" : "Enable gameplay" ) )
    {
        gameplayRunning = !gameplayRunning;
        SetNonEditorSystemsEnabled( gameplayRunning );
    }
    
    for ( auto& system : Engine::GetInstance()->GetSystems() )
    {
        // Skip the debug system and the platform system
        if (system == GetInstance())
	 	    continue;
    
       if (ImGui::TreeNodeEx(system->GetName().c_str()))
	   {
	 	    system->DebugWindow();
            ImGui::TreePop();
	   }
    }
    
    if ( ImGui::Button( m_ShowFpsWindow ? "Show FPS" : "Hide FPS" ) )
    {
        m_ShowFpsWindow = !m_ShowFpsWindow;
    }
    
    // engine config saving
    static char buffer[ 128 ] = "Data/EngineConfig.json"; // Buffer to hold the input, you can save this
    ImGui::InputText( "Engine config filepath", buffer, IM_ARRAYSIZE( buffer ) );
    
    if ( ImGui::Button( "Save Engine Config" ) )
    {
        Stream::WriteToFile( buffer, Engine::GetInstance()->Write() );
    }
    
    
    ImGui::End();
}


/// @brief PerDorm updates at a fixed time step.
void DebugSystem::OnFixedUpdate()
{

    if (InputSystem::GetInstance()->GetKeyTriggered(GLFW_KEY_F1))
        m_ShowDebugWindow = !m_ShowDebugWindow;

}

/// @brief Perform cleanup and shutdown.
void DebugSystem::OnExit()
{
    ImGui::Render();
    ImGui_ImplGlfw_Shutdown();
    ImPlot::DestroyContext();
    ImGui::DestroyContext();
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


/// @brief  sets whether the non-editor systems are enabled
/// @param  enabled wether to enable or disable the systems
void DebugSystem::SetNonEditorSystemsEnabled( bool enabled )
{
    for ( System* system : Engine::GetInstance()->GetSystems() )
    {
        if ( m_EditorSystemNames.contains( system->GetName() ) == false )
        {
            system->SetEnabled( enabled );
        }
    }
}


void DebugSystem::ShowFPSWindow()
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


    static ImPlotAxisFlags axis_flags = ImPlotAxisFlags_NoDecorations
        | ImPlotAxisFlags_Lock;

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

void DebugSystem::ImguiStartFrame()
{
    auto& io = ImGui::GetIO();

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
}

//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------

    /// @brief  reads whether to show the FPS window
    /// @param  data    the data to read from
    void DebugSystem::readShowFpsWindow( nlohmann::ordered_json const& data )
    {
        m_ShowFpsWindow = Stream::Read<bool>( data );
    }

    /// @brief  reads whether to show the debug window
    /// @param  data    the data to read from
    void DebugSystem::readShowDebugWindow( nlohmann::ordered_json const& data )
    {
		m_ShowDebugWindow = Stream::Read<bool>( data );
    }

    /// @brief map containing read methods
    ReadMethodMap< DebugSystem > const DebugSystem::s_ReadMethods = {
        { "ShowFpsWindow", &readShowFpsWindow },
		{ "ShowDebugWindow", &readShowDebugWindow }
    };

//-----------------------------------------------------------------------------
// public: writing
//-----------------------------------------------------------------------------

    /// @brief  writes this System config
    /// @return the writting System config
    nlohmann::ordered_json DebugSystem::Write() const
    {
        nlohmann::ordered_json json;
        
        json[ "ShowFpsWindow" ] = m_ShowFpsWindow;
        json[ "ShowDebugWindow" ] = m_ShowDebugWindow;

        return json;
    }

//-----------------------------------------------------------------------------
// singleton stuff
//-----------------------------------------------------------------------------

    /// @brief Initialize the DebugSystem.
    /// @param window The GLFW window handle (default is the current context).
    DebugSystem::DebugSystem() :
        System( "DebugSystem" ),
        _window(nullptr),
        io(nullptr),
        m_ShowFpsWindow(false),
        m_ShowDebugWindow(false)
    {}

    DebugSystem* DebugSystem::instance = nullptr;

    DebugSystem* DebugSystem::GetInstance()
    {

        if (instance == nullptr) 
        {
            instance = new DebugSystem();
        }

        return instance;

    }

//-----------------------------------------------------------------------------
