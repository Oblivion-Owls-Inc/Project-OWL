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

#include "PlatformSystem.h"
#include "DebugSystem.h"

PlatformSystem *platform;
DebugSystem* Debug;

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
    previousTime(0.0)
{}

/**
* @brief Adds a System to the Engine, at the back of the systems array
* @param system The system to add the Engine
*/
void Engine::AddSystem( System * system )
{
    systems.push_back( system );
    system->setIndex( static_cast<int>(systems.size() - 1) );
    
    // ELI: removed systems init
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

    // ELI: removed systems init
}

/**
* @brief Starts running the engine. Code will not advance past this point until the engine stops running.
*/
void Engine::Run()
{

    // TODO: move glfwWindowShouldClose out of the Engine and into its own System? 
    // ELI: yea
    while (shouldExit == false && !platform->WindowClosing()) {
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
* @brief Initializes the engine before running it
*/
void Engine::Init()
{
    // TODO: initialize the engine
    previousTime = glfwGetTime();
    previousFixedTime = previousTime;

    // ELI: moved systems init here
    for (auto system : systems)
        system->OnInit();

    platform = (PlatformSystem*)GetPlatform();
    Debug = (DebugSystem*)GetDebug();

    GLFWwindow* window = platform->GetWindowHandle();

    Debug->SetWindowHandle(window);
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
    GLFWwindow* window = platform->GetWindowHandle();

    double currentTime = glfwGetTime();

    if (currentTime - previousFixedTime > fixedFrameDuration) {
        FixedUpdateSystems();
        previousFixedTime += fixedFrameDuration;
    }

    UpdateSystems( static_cast<float>(currentTime - previousTime) );

    
    previousTime = currentTime;

    DebugSystem::ShowFPS();
    //DebugSystem::ShowDebugMenu();


    // TODO: move the below code out of Engine and into its own Systems
        // Poll for and process events
        glfwPollEvents();


        int display_w, display_h;

        glfwGetFramebufferSize(window, &display_w, &display_h);

        glViewport(0, 0, display_w, display_h);

        // Swap front and back buffers
        glfwSwapBuffers(window);
        glClear(GL_COLOR_BUFFER_BIT);
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
}

