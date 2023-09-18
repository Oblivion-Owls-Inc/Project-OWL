/// @file Engine.cpp
/// @author Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief Engine class
/// @version 0.1
/// @date 2023-09-05
///
/// @copyright Copyright (c) 2023

#pragma once

#include "Engine.h"

#include "basics.h"
#include "../rapidjson/document.h"
#include <fstream>
// #include "imgui.h"
// #include "imgui_impl_glfw.h"
// #include "imgui_impl_opengl3.h"

#include "PlatformSystem.h"
#include "SceneSystem.h"
#include "DebugSystem.h"
#include "RenderSystem.h"
#include "AudioSystem.h"
#include "SandboxSystem.h"
#include "InputSystem.h"
#include "EntitySystem.h"

// TODO: move this out of the engine into its own System
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

/// @brief Starts running the engine. Code will not advance past this point until the engine stops running.
void Engine::Run()
{

    Load();

    Init();

    while (
        shouldExit == false &&
        PlatformSystem::getInstance()->WindowClosing() == false
    ) {
        Update();
    }

    Exit();
}

/// @brief Flags the engine to close once it finishes this loop
void Engine::Close()
{
    shouldExit = true;
}

/// @brief gets the duration of each fixed frame
/// @return the amount of time in seconds that each fixed frame lasts
float Engine::getFixedFrameDuration() const
{
    return fixedFrameDuration;
}

/// @brief sets the duration of each fixed frame
/// @param _fixedFrameDuration the amount of time in seconds that each fixed frame lasts
void Engine::setFixedFrameDuration( float _fixedFrameDuration )
{
    fixedFrameDuration = _fixedFrameDuration;
}

/// @brief Gets the array of all Systems in the engine.
/// @return the array of all Systems in the engine
std::vector< System* > const& Engine::getSystems() const
{
    return systems;
}


/// @brief Adds a System to the Engine.
/// @tparam SystemType The type of system to add the Engine
template < class SystemType >
System* Engine::AddSystem()
{
    System* system = SystemType::getInstance();
    systems.push_back( system );
    return system;
}

/// @brief contains the function for adding each System type to the Engine. Used for Loading systems from config.
std::map< std::string, System* (Engine::*)()> Engine::addSystemMethods = {
    { "PlatformSystem", &AddSystem< PlatformSystem >    },
    { "InputSystem",    &AddSystem< InputSystem >       },
    { "SceneSystem",    &AddSystem< SceneSystem >       },
    { "RenderSystem",   &AddSystem< RenderSystem >      },
    { "DebugSystem",    &AddSystem< DebugSystem >       },
    { "AudioSystem",    &AddSystem< AudioSystem >       },
    { "EntitySystem",   &AddSystem< EntitySystem >      },
    { "SandboxSystem",  &AddSystem< SandboxSystem >     }
};

/// @brief Loads the engine config from "Data/EngineConfig.json"
void Engine::Load()
{
    // v v v TODO: put this in a helper function in its own class v v v
    std::ifstream configFile( "Data/EngineConfig.json" );

    std::stringstream configStream;
    configStream << configFile.rdbuf();

    configFile.close();

    rapidjson::Document configData;
    configData.Parse( configStream.str().c_str() );
    // ^ ^ ^ TODO: put this in a helper function in its own class ^ ^ ^

    // v v v TODO: JSON error handing (maybe w/ helper functions in their own class) v v v
    fixedFrameDuration = configData[ "fixedFrameDuration" ].GetFloat();

    for ( auto& systemConfig : configData[ "systems" ].GetObject() )
    {
        auto addSystemMethod = addSystemMethods.at( systemConfig.name.GetString() ); // get the appropriate System creation method
        System* system = (this->*addSystemMethod)(); // create and add the System to the Engine
        ReadSystem( system, systemConfig.value ); // Load the System's config
    }
    // ^ ^ ^ TODO: JSON error handling (maybe w/ helper functions in their own class) ^ ^ ^

}


/// @brief Reads and loads the specified System config from JSON
/// @param system the system to load
/// @param systemConfig the JSON data of the System's configuration
void Engine::ReadSystem( System* system, rapidjson::Value const& systemConfig )
{
    std::map< std::string, ReadMethod< System > > readMethods = system->GetReadMethods();

    // v v v TODO: JSON error handling v v v
    for ( auto& setting : systemConfig.GetObject() )
    {
        ReadMethod< System > readMethod = readMethods.at( setting.name.GetString() ); // get the appropriate System read method
        (system->*readMethod)( setting.value ); // call the system read method to have the system read the value
    }
    // ^ ^ ^ TODO: JSON error handling ^ ^ ^
}

/// @brief Initializes the engine and all Systems in the Engine
void Engine::Init()
{
    // initialize the time values
    previousTime = glfwGetTime();
    previousFixedTime = previousTime;

    // initialize all the systems
    for (System * system : systems)
    {
        system->OnInit();
    }

    // TODO: move the below code out of the engine into its own systems

    // this will go in GraphicsSystem
        // Set the clear color (background color)
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    // this will stay here until we figure out a better way of replacing it
        // Set up a callback for the Escape key
        GLFWwindow* window = PlatformSystem::getInstance()->GetWindowHandle();
        glfwSetKeyCallback( window, keyCallback );
    // TODO: move the above code out of the engine and into its own systems
}

/// @brief Updates the engine each frame
void Engine::Update()
{

    double currentTime = glfwGetTime();

    UpdateSystems( static_cast<float>(currentTime - previousTime) );

    if (currentTime - previousFixedTime > fixedFrameDuration)
    {
        FixedUpdateSystems();
        previousFixedTime += fixedFrameDuration;
    }
    
    previousTime = currentTime;


// TODO: move the below code out of Engine and into its own Systems
        
// this goes to GraphicsSystem
            
    GLFWwindow* window = PlatformSystem::getInstance()->GetWindowHandle();

    // ensure viewport size matches window size
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);

    // Swap front and back buffers
    glfwSwapBuffers(window);
    glClear(GL_COLOR_BUFFER_BIT);

// this goes to InputSystem
            
    // Poll for and process events
    glfwPollEvents();

// TODO: move the above code out of Engine and into its own System
}

/// @brief Calls all Systems' OnUpdate function
void Engine::UpdateSystems( float dt )
{
    for ( System * system : systems )
    {
        system->OnUpdate(dt);
    }
}

/// @brief Calls all Systems' OnFixedUpdate function
void Engine::FixedUpdateSystems()
{
    for ( System * system : systems )
    {
        system->OnFixedUpdate();
    }
}

/// @brief Calls all Systems' OnExit function
void Engine::Exit()
{
    for ( System * system : systems )
    {
        system->OnExit();
    }

    // TODO: properly delete systems
}



/// @brief The singleton instance of the Engine
Engine * Engine::instance;

/// @brief Constructs a new Engine
Engine::Engine() :
    shouldExit( false ),
    fixedFrameDuration( 1.0f / 20.0f ),
    previousFixedTime( 0.0 ),
    previousTime( 0.0 )
{}

/// @brief gets the singleton instance of the Engine
/// @return the singleton instance of the Engine
Engine* Engine::getInstance()
{
    if ( instance == nullptr )
    {
        instance = new Engine();
    }
    return instance;
}
