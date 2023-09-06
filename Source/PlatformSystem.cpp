/**
* @file PlatformSystem.cpp
* @author Eli Tsereteli (ilya.tsereteli@digipen.edu)
* @brief Patform System class: initializes window and graphics (GLFW and GLEW) when created, 
*                              asserts during init if any of those failed. Shuts them down on exit.
* @version 0.1
*
* @copyright Copyright (c) 2023
*
*/
#include "PlatformSystem.h"
#include "glew.h"
#include "glfw3.h"
#include "glm/vec2.hpp" // return window dimensions
#include <iostream>     // cout
#include <assert.h>

// This function will get called when there's some OpenGL error.
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

PlatformSystem::PlatformSystem(const char* w_name, int w_width, int w_height) : 
	_window(nullptr), _initialized(false),
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

    //    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  needed or nah?

    // Window
    _window = glfwCreateWindow(w_width, w_height, w_name, NULL, NULL);
    if (!_window)
    {
        glfwTerminate();
        std::cerr << "Failed to create GLFW window" << std::endl;
        return;
    }
    glfwMakeContextCurrent(_window);

    // GLEW
    if (glewInit() != GLEW_OK)
    {
        glfwTerminate();
        glfwDestroyWindow(_window);
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return;
    }

    std::cout << "Running OpenGL " << glGetString(GL_VERSION) << std::endl; // display OpenGL version
    glDebugMessageCallback(ErrorHandler, NULL);                             // set error callback func

    _initialized = true;
}


void PlatformSystem::OnInit()
{
    assert(_initialized); // asserts if prior initialization failed
}

void PlatformSystem::OnUpdate(float dt)
{

}

void PlatformSystem::OnExit()
{
    if (_initialized)
    {
        glfwDestroyWindow(_window);
        glfwTerminate();
        std::cout << "\nShutdown complete." << std::endl;
    }
}


GLFWwindow* PlatformSystem::GetWindowHandle() const { return _window; }

glm::vec2 PlatformSystem::GetWindowDimensions() const { return { _width, _height }; }

bool PlatformSystem::WindowClosing() const { return glfwWindowShouldClose(_window); }

