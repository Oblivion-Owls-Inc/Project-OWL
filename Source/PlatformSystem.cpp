/**
* @file         PlatformSystem.cpp
* @author       Eli Tsereteli (ilya.tsereteli@digipen.edu)
* @brief        Patform System class: initializes window and graphics (GLFW and GLEW), 
                shuts them down on exit.
*
* @version      0.1
* @copyright    Copyright (c) 2023
*/
#include "PlatformSystem.h"
#include "glew.h"       // initialize, error callback
#include "glfw3.h"      // initialize / shutdown
#include "glm/vec2.hpp" // for returning window dimensions
#include <iostream>     // cout
#include <cassert>

/**
* @brief            (callback) Gets called when there's some OpenGL error. Prints error message
*                   to console, asserts for high severity errors.
* @param message    Error message (text)
* @param severity   Severity of the error.
* @param others     They don't really matter. Can be used for more advanced debugging.
*/
static void GLAPIENTRY ErrorHandler(GLenum source, GLenum type, GLuint id, GLenum severity, 
                                GLsizei length, const GLchar* message, const void* userparam)
{
    std::cout << message << '\n';

    bool x = false;
    if (severity == GL_DEBUG_SEVERITY_HIGH)
        assert(x);  // Keep a breakpoint here: when it's triggered, change the x to true to skip assert,
                    // and step outside this function to see where the error ocurred.

    // unused
    (void) source;
    (void) type;
    (void) id;
    (void) length;
    (void) userparam;
}

/**
* @brief            Constructor: initializes GLFW window and GLEW, enables error callback.
* @param w_name     Window name.
* @param w_width    Window width.
* @param w_height   Window height.
*/
PlatformSystem::PlatformSystem(const char* w_name, int w_width, int w_height) : 
	_window(nullptr),
	_width(w_width), _height(w_height) 
{
    // GLFW
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);      // OpenGL 4.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);    // enable error callback
    glfwWindowHint(GLFW_RESIZABLE, false);              // fixed window size
    glfwSwapInterval(1); // enable vsync

    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); TODO: needed or nah?

    // Window
    _window = glfwCreateWindow(w_width, w_height, w_name, NULL, NULL);
    if (!_window)
    {
        glfwTerminate();
        std::cerr << "Failed to create GLFW window" << std::endl;
        assert(false);
    }
    glfwMakeContextCurrent(_window);

    // GLEW
    if (glewInit() != GLEW_OK)
    {
        glfwTerminate();
        glfwDestroyWindow(_window);
        std::cerr << "Failed to initialize GLEW" << std::endl;
        assert(false);
    }

    std::cout << "Running OpenGL " << glGetString(GL_VERSION) << std::endl; // display OpenGL version
    glDebugMessageCallback(ErrorHandler, NULL);                             // set error callback func
}

/**
* @brief    Shuts down the the platform.
*/
void PlatformSystem::OnExit()
{
    glfwDestroyWindow( _window );
    glfwTerminate();
    std::cout << "\nShutdown complete." << std::endl;
}

/**
* @brief    Returns the window handle.
* @return   GLFWwindow pointer: Current window handle.
*/
GLFWwindow* PlatformSystem::GetWindowHandle() const
{
    return _window;
}

/**
* @brief    Returns window dimensions as a vec2.
* @return   glm vec2: x = width, y = height.
*/
glm::vec2 PlatformSystem::GetWindowDimensions() const
{
    return { _width, _height };
}

/**
* @brief    Checks if the window is closing.
* @return   bool: true if the window is closing.
*/
bool PlatformSystem::WindowClosing() const
{
    return glfwWindowShouldClose(_window);
}



/// @brief The singleton instance of ExampleSystem
PlatformSystem * PlatformSystem::instance = nullptr;

/**
* @brief    (Singleton) Gets the instance of this system.
* @return   PlatformSystem pointer: new or existing instance of this system.
*/
PlatformSystem * PlatformSystem::getInstance()
{
    if ( instance == nullptr )
    {
        instance = new PlatformSystem();
    }
    return instance;
}

/**
* @brief            (Singleton) Constructor getInstance method, should only be called in main.cpp
* @param w_name     Window name.
* @param w_width    Window width.
* @param w_height   Window height.
* @return           PlatformSystem pointer: new or existing instance of this system.
*/
PlatformSystem * PlatformSystem::getInstance( const char* w_name = "Prototype", int w_width = 800, int w_height = 600 )
{
    if (instance == nullptr)
    {
        instance = new PlatformSystem( w_name, w_width, w_height );
    }
    return instance;
}

