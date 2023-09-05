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

#include "Engine.h"


/**
* @brief Constructs a new Engine
*/
Engine::Engine()
  : shouldExit( false ),
    fixedFrameDuration(
        std::chrono::duration_cast< std::chrono::high_resolution_clock::duration >(
            std::chrono::duration< float, std::chrono::seconds >( 1.0f / 20.0f )
        )
    )
{}

/**
* @brief Adds a System to the Engine, at the back of the systems array
* @param system The system to add the Engine
*/
void Engine::AddSystem( System * system )
{
    systems.push_back( system );
    system->setIndex( systems.size() - 1 );
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
    Init();

    while (shouldExit == false) {
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
* @brief Initializes the engine before running it
*/
void Engine::Init()
{
    // TODO: initialize the engine
    previousTime = std::chrono::high_resolution_clock::now();
    previousFixedTime = previousTime;
}

/**
* @brief Updates the engine each frame
*/
void Engine::Update()
{
    std::chrono::high_resolution_clock::time_point currentTime = std::chrono::high_resolution_clock::now();

    if (currentTime - previousFixedTime > fixedFrameDuration) {
        FixedUpdateSystems();
        previousFixedTime += fixedFrameDuration;
    }

    UpdateSystems( std::chrono::duration<float>( currentTime - previousTime ).count() );
    previousTime = currentTime;
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

