/**
* @file Engine.h
* @author Steve Bukowinski (steve.bukowinski@digipen.edu)
* @brief Engine class
* @version 0.1
* @date 2023-09-05
* 
* @copyright Copyright (c) 2023
* 
*/

#pragma once

#include <vector>
#include <chrono>

#include "System.h"

class Engine
{

    public:

        /**
        * @brief Constructs a new Engine
        */
        Engine();

        /**
        * @brief Adds a System to the Engine.
        * @param system The system to add the Engine
        */
        void AddSystem( System * system );

        /**
         * @brief Adds a System to the Engine.
         * @param system The system to add the Engine
         * @param index The index in the Engine to insert the System into.
        */
        void AddSystem( System * system, unsigned index );

        /**
         * @brief Starts running the engine. Code will not advance past this point until the engine stops running.
        */
        void Run();

        /**
         * @brief Flags the engine to close once it finishes this loop
        */
        void Close();

    private:

        /**
         * @brief Container of all the Systems in the engine
        */
        std::vector< System * > systems;

        /**
         * @brief flag set to true when the engine needs to exit
        */
        bool shouldExit;

        /**
         * @brief The timestamp of the previous frame
        */
        std::chrono::high_resolution_clock::time_point previousTime;

        /**
         * @brief The timestamp of the previous fixed frame
        */
        std::chrono::high_resolution_clock::time_point previousFixedTime;

        /**
        * @brief The duration of each fixed frame
        */
        std::chrono::high_resolution_clock::duration fixedFrameDuration;


        /**
         * @brief Initializes the engine before running it
        */
        void Init();

        /**
         * @brief Updates the engine each frame
        */
        void Update();

        /**
         * @brief Calls all Systems in the Engine's Update function
        */
        void UpdateSystems(float dt);

        /**
         * @brief Calls all Systems in the Engine's FixedUpdate function
        */
        void FixedUpdateSystems();

        /**
         * @brief exits and closes the Engine
        */
        void Exit();

        /**
         * @brief Helper function for AddSystem() to insert systems into the middle of the array.
         * @param system 
         * @param index 
        */
        void InsertSystem( System * system, int index );

    public:

        // Prevent Engine from being copied
        Engine(Engine& other) = delete;
        void operator=(const Engine&) = delete;
};