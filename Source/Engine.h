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

#include "basics.h"

#include "System.h"

class Engine
{

    public:

        /**
        * @brief Constructs a new Engine
        */
        Engine();


        /**
        * @brief Initializes the engine. Should be called beofr
        */
        void Init();

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

        /**
         * @brief gets the duration of each fixed frame
         * @return the amount of time in seconds that each fixed frame lasts
        */
        float getFixedFrameDuration() const;

        /**
         * @brief gets the GLFWwindow of the Engine
         * @return the GLFWwindow in use by the engine
        */
        GLFWwindow * getWindow() const;

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
        double previousTime;

        /**
         * @brief The timestamp of the previous fixed frame
        */
        double previousFixedTime;

        /**
        * @brief The duration of each fixed frame
        */
        float fixedFrameDuration;

        // TODO: maybe this out of Engine and into its own System?
        GLFWwindow * window;

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



    public:

        // Prevent Engine from being copied
        Engine(Engine& other) = delete;
        void operator=(const Engine&) = delete;
};