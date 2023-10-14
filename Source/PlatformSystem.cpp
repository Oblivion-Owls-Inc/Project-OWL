/// @file         PlatformSystem.cpp
/// @author       Eli Tsereteli (ilya.tsereteli@digipen.edu)
/// @brief        Patform System class: initializes window and graphics (GLFW and GLEW), 
///               shuts them down on exit.
///
/// @version      0.1
/// @copyright    Copyright (c) 2023
#include "PlatformSystem.h"
#include "glew.h"       // initialize, error callback
#include "glfw3.h"      // initialize / shutdown
#include "glm/vec2.hpp" // for returning window dimensions
#include <iostream>     // cout
#include <cassert>

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
        std::cout << message << '\n';
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
    window = glfwCreateWindow( windowWidth, windowHeight, windowName.c_str(), NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        std::cerr << "Failed to create GLFW window" << std::endl;
        assert(false);
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // enable vsync

    // GLEW
    if (glewInit() != GLEW_OK)
    {
        glfwTerminate();
        glfwDestroyWindow(window);
        std::cerr << "Failed to initialize GLEW" << std::endl;
        assert(false);
    }

    std::cout << "\nRunning OpenGL " << glGetString(GL_VERSION) << std::endl; // display OpenGL version
    glDebugMessageCallback(ErrorHandler, NULL);                             // set error callback func
}

/// @brief    Shuts down the the platform.
void PlatformSystem::OnExit()
{
    glfwDestroyWindow( window );
    glfwTerminate();
    std::cout << "\nShutdown complete." << std::endl;
}


//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------

    /// @brief reads the window width
    /// @param stream the data to read from
    void PlatformSystem::readWindowWidth( nlohmann::json const& data )
    {
        windowWidth = stream.Read<int>();
    }

    /// @brief reads the window width
    /// @param stream the data to read from
    void PlatformSystem::readWindowHeight( nlohmann::json const& data )
    {
        windowHeight = stream.Read<int>();
    }

    /// @brief map of the PlatformSystem read methods
    ReadMethodMap< PlatformSystem > const PlatformSystem::s_ReadMethods = {
        { "WindowWidth",  &readWindowWidth  },
        { "WindowHeight", &readWindowHeight }
    };

/// @brief    Returns the window handle.
/// @return   GLFWwindow pointer: Current window handle.
GLFWwindow* PlatformSystem::GetWindowHandle() const
{
    return window;
}

/// @brief    Returns window dimensions as a vec2.
/// @return   glm vec2: x = width, y = height.
glm::vec2 PlatformSystem::GetWindowDimensions() const
{
    return { windowWidth, windowHeight};
}

void PlatformSystem::DebugWindow()
{

}

/// @brief    Checks if the window is closing.
/// @return   bool: true if the window is closing.
bool PlatformSystem::WindowClosing() const
{
    return glfwWindowShouldClose(window);
}

//-----------------------------------------------------------------------------
// singleton stuff
//-----------------------------------------------------------------------------

    /// @brief Constructor
    PlatformSystem::PlatformSystem() :
        System( "PlatformSystem" ),
        window( nullptr ),
        windowWidth( 800 ),
        windowHeight( 600 ),
        windowName( "engineTest" )
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
