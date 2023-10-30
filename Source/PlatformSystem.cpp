/// @file         PlatformSystem.cpp
/// @author       Eli Tsereteli (ilya.tsereteli@digipen.edu)
/// @brief        Patform System class: initializes window and graphics (GLFW and GLEW), 
///               shuts them down on exit.
///
/// @version      0.1
/// @copyright    Copyright (c) 2023
#include "PlatformSystem.h"
#include "DebugSystem.h"
#include "glew.h"       // initialize, error callback
#include "glfw3.h"      // initialize / shutdown
#include "glm/vec2.hpp" // for returning window dimensions
#include <iostream>     // cout
#include <cassert>



///-----------------------------------------------------------------------------
// Static variables
///-----------------------------------------------------------------------------

    /// @brief  Used to Toggle Fullscreen
    bool PlatformSystem::s_IsFullscreen = false;




/// @brief            (callback) Gets called when there's some OpenGL error. Prints error message
///                   to console, asserts for high severity errors.
/// @param message    Error message (text)
/// @param severity   Severity of the error.
/// @param others     They don't really matter. Can be used for more advanced debugging.
static void GLAPIENTRY ErrorHandler(GLenum source, GLenum m_Type, GLuint id, GLenum severity, 
                                GLsizei length, const GLchar* message, const void* userparam)
{

    bool x = false;
    
    #ifndef NDEBUG
    if (severity == GL_DEBUG_SEVERITY_HIGH)
    {
        Debug() << message << '\n';
        assert(x);  // Keep a breakpoint here: when it's triggered, change the x to true to skip assert,
        // and step outside this function to see where the error ocurred.
    }
    #endif // !NDEBUG
    // unused
    (void) source;
    (void) m_Type;
    (void) id;
    (void) length;
    (void) userparam;
}

/// @brief Initializes the PlatformSystem
void PlatformSystem::OnInit()
{

    // GLFW
    if ( !glfwInit() )
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);      // OpenGL 4.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);    // enable error callback
    glfwWindowHint(GLFW_RESIZABLE, false);              // fixed window size

    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); TODO: needed or nah?

    // Window
    m_Window = glfwCreateWindow( m_WindowSize.x, m_WindowSize.y, m_WindowName.c_str(), NULL, NULL);

    if (!m_Window)
    {
        #ifndef NDEBUG
                glfwTerminate();
                Debug() << "Failed to create GLFW window" << std::endl;
                assert(false);
        #else
                glfwTerminate();
		        Debug() << "Failed to create GLFW window" << std::endl;
		        exit(EXIT_FAILURE);  // Exit 
        #endif // !NDEBUG
    }
    glfwMakeContextCurrent(m_Window);
    glfwSwapInterval(1); // enable vsync

    // GLEW
    if (glewInit() != GLEW_OK)
    {
        #ifndef NDEBUG
                glfwTerminate();
                glfwDestroyWindow(m_Window);
                std::cerr << "Failed to initialize GLEW" << std::endl;
                assert(false);
        #else
                glfwTerminate();
                glfwDestroyWindow(m_Window);
                Debug() << "Failed to initialize GLEW" << std::endl;
                exit(EXIT_FAILURE);  // Exit 
        #endif // !NDEBUG
    }

    Debug() << "\nRunning OpenGL " << glGetString(GL_VERSION) << std::endl << std::endl; // display OpenGL version
    glDebugMessageCallback(ErrorHandler, NULL);                             // set error callback func
}

/// @brief    Shuts down the the platform.
void PlatformSystem::OnExit()
{
    glfwDestroyWindow( m_Window );
    glfwTerminate();
}

/// @brief Debug window for PlatformSystem
void PlatformSystem::DebugWindow()
{
	ImGui::Text( "Window Size: %d x %d", m_WindowSize.x, m_WindowSize.y );
	ImGui::Text( "Window Name: %s", m_WindowName.c_str() );
    
    char buttonLabel[128];
    snprintf(buttonLabel, sizeof(buttonLabel), s_IsFullscreen ?
        "Fullscreen Mode" : "Windowed Mode");
    if (ImGui::Button(buttonLabel))
    {
		ToggleFullscreen();
    }

}

void PlatformSystem::ToggleFullscreen()
{
    static int savedWidth = m_WindowSize.x;
    static int savedHeight = m_WindowSize.y;
    static int savedPosX, savedPosY;

    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);


    if (!s_IsFullscreen)
    {
        // Save the current window size and position before going fullscreen
        glfwGetWindowPos(m_Window, &savedPosX, &savedPosY);
        savedWidth = m_WindowSize.x;
        savedHeight = m_WindowSize.y;

        // Set the window to fullscreen
        glfwSetWindowMonitor(m_Window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);

        // Update m_WindowSize to the new dimensions
        m_WindowSize = glm::ivec2(mode->width, mode->height);

        glfwSwapInterval(1); // enable vsync

    }
    else
    {
         // Update m_WindowSize to the new dimensions
         m_WindowSize = glm::ivec2(savedWidth, savedHeight);

        // Restore the window's previous size and position
        glfwSetWindowMonitor(m_Window, nullptr, savedPosX, savedPosY, savedWidth, savedHeight, 0);

        glfwSwapInterval(1); // enable vsync
    }

    s_IsFullscreen = !s_IsFullscreen;  // Toggle the fullscreen flag
}

/// @brief    Returns the window handle.
/// @return   GLFWwindow pointer: Current window handle.
GLFWwindow* PlatformSystem::GetWindowHandle() const
{
    return m_Window;
}

/// @brief    Returns window dimensions as a vec2.
/// @return   glm ivec2: x = width, y = height.
glm::ivec2 PlatformSystem::GetWindowDimensions() const
{
    return m_WindowSize;
}

/// @brief    Checks if the window is closing.
/// @return   bool: true if the window is closing.
bool PlatformSystem::WindowClosing() const
{
    return glfwWindowShouldClose(m_Window);
}

//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------

    /// @brief reads the window size
    /// @param stream the data to read from
    void PlatformSystem::readWindowSize( nlohmann::ordered_json const& data )
    {
        m_WindowSize = Stream::Read< 2, int >( data );
    }

    /// @brief reads the window name
    /// @param stream the data to read from
    void PlatformSystem::readWindowName( nlohmann::ordered_json const& data )
    {
        m_WindowName = Stream::Read< std::string >( data );
    }

    /// @brief map of the PlatformSystem read methods
    ReadMethodMap< PlatformSystem > const PlatformSystem::s_ReadMethods = {
        { "WindowSize", &readWindowSize },
        { "WindowName", &readWindowName }
    };

//-----------------------------------------------------------------------------
// public: writing
//-----------------------------------------------------------------------------

    /// @brief  writes this System config
    /// @return the writting System config
    nlohmann::ordered_json PlatformSystem::Write() const
    {
        nlohmann::ordered_json json;

        json[ "WindowSize" ] = Stream::Write< 2, int >( m_WindowSize );
        json[ "WindowName" ] = m_WindowName;

        return json;
    }

//-----------------------------------------------------------------------------
// singleton stuff
//-----------------------------------------------------------------------------

    /// @brief Constructor
    PlatformSystem::PlatformSystem() :
        System( "PlatformSystem" )
    {}

    /// @brief The singleton instance of ExampleSystem
    PlatformSystem * PlatformSystem::instance = nullptr;

    /// @brief    (Singleton) Gets the instance of this system.
    /// @return   PlatformSystem pointer: new or existing instance of this system.
    PlatformSystem * PlatformSystem::GetInstance()
    {
        if ( instance == nullptr )
        {
            instance = new PlatformSystem();
        }
        return instance;
    }

//-----------------------------------------------------------------------------
