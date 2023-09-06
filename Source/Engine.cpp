/**
* @file Engine.cpp
* @author Steve Bukowinski (steve.bukowinski@digipen.edu)
* @brief Engine class
* @version 0.1
* @date 2023-09-05
* 
* @copyright Copyright (c) 2023
* 
*/

#pragma once

#include "basics.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "Engine.h"

// TODO: move this out of the engine into its own System
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}


/**
* @brief Constructs a new Engine
*/
Engine::Engine()
  : shouldExit( false ),
    fixedFrameDuration( 1.0f / 20.0f ),
    previousFixedTime(0.0),
    previousTime(0.0),
    window(nullptr)
{}

/**
* @brief Adds a System to the Engine, at the back of the systems array
* @param system The system to add the Engine
*/
void Engine::AddSystem( System * system )
{
    systems.push_back( system );
    system->setIndex( static_cast<int>(systems.size() - 1) );
    system->OnInit(); // TODO: maybe move this so that all systems are added, then all systems initialize?
}

/**
* @brief Adds a System to the Engine at the specified location in the systems array.
* @param system The system to add the Engine
* @param index The index in the Engine to insert the System into.
*/
void Engine::AddSystem( System * system, unsigned index )
{
    systems.insert( systems.begin() + index, system );
    // keep all indices valid.
    for ( ; index < systems.size(); ++index ) {
        systems[index]->setIndex(index);
    }
    system->OnInit();
}

/**
* @brief Starts running the engine. Code will not advance past this point until the engine stops running.
*/
void Engine::Run()
{

    // TODO: move glfwWindowShouldClose out of the Engine and into its own System?
    while ( shouldExit == false && !glfwWindowShouldClose(window) ) {
        Update();
    }

    Exit();
}

/**
* @brief Flags the engine to close once it finishes this loop
*/
void Engine::Close()
{
    shouldExit = true;
}

/**
* @brief gets the duration of each fixed frame
* @return the amount of time in seconds that each fixed frame lasts
*/
float Engine::getFixedFrameDuration() const
{
    return fixedFrameDuration;
}


/**
* @brief gets the GLFWwindow of the Engine
* @return the GLFWwindow in use by the engine
*/
GLFWwindow* Engine::getWindow() const
{
    return window;
}

/**
* @brief Initializes the engine before running it
*/
void Engine::Init()
{
    // TODO: initialize the engine
    previousTime = glfwGetTime();
    previousFixedTime = previousTime;

    // TODO: move the below code out of the engine and into its own systems
        // Initialize GLFW
        if (!glfwInit()) 
        {
            std::cerr << "Failed to initialize GLFW" << std::endl;
            return;
        }

        // Request an OpenGL 4.3 core profile context
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        // Create a GLFW window
        window = glfwCreateWindow(800, 600, "OpenGL Example", NULL, NULL);
        if (!window) 
        {
            glfwTerminate();
            std::cerr << "Failed to create GLFW window" << std::endl;
            return;
        }
        // Make the window's context current
        glfwMakeContextCurrent(window);
    #if 1
        // Initialize GLEW
        if (glewInit() != GLEW_OK) 
        {
            std::cerr << "Failed to initialize GLEW" << std::endl;
            return;
        }
    #endif
        // Setup ImGui context
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        (void)io;
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 430");

        // Set the clear color (background color)
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

        // Set up a callback for the Escape key
        glfwSetKeyCallback(window, keyCallback);

    // TODO: move the above code out of the engine and into its own systems
}

/**
* @brief Updates the engine each frame
*/
void Engine::Update()
{
    double currentTime = glfwGetTime();

    if (currentTime - previousFixedTime > fixedFrameDuration) {
        FixedUpdateSystems();
        previousFixedTime += fixedFrameDuration;
    }

    UpdateSystems( static_cast<float>(currentTime - previousTime) );
    previousTime = currentTime;

    // TODO: move the below code out of Engine and into its own Systems
        // Poll for and process events
        glfwPollEvents();

        // Start the ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        
        // Rendering code goes here
        ImGui::Begin("Mouse Position");

        // Get the mouse cursor position
        double mouseX, mouseY;
        glfwGetCursorPos(window, &mouseX, &mouseY);

        // Display mouse position in ImGui window
        ImGui::Text("Mouse X: %.2f", mouseX);
        ImGui::Text("Mouse Y: %.2f", mouseY);
        
        ImGui::End();
        
        // ImGui rendering
        ImGui::Render();

        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Swap front and back buffers
        glfwSwapBuffers(window);
    // TODO: move the above code out of Engine and into its own System
}

/**
* @brief Calls all Systems' OnUpdate function
*/
void Engine::UpdateSystems( float dt )
{
    for ( System * system : systems )
    {
        system->OnUpdate(dt);
    }
}

/**
* @brief Calls all Systems' OnFixedUpdate function
*/
void Engine::FixedUpdateSystems()
{
    for ( System * system : systems )
    {
        system->OnFixedUpdate();
    }
}

/**
* @brief Calls all Systems' OnExit function
*/
void Engine::Exit()
{
    for ( System * system : systems )
    {
        system->OnExit();
    }

    // TODO: remove the following from Engine and put it into its own System
    // Terminate GLFW
    glfwTerminate();
}

