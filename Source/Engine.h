/// @file Engine.h
/// @author Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief Engine class
/// @version 0.1
/// @date 2023-09-05
/// 
/// @copyright Copyright (c) 2023

#pragma once

#include <vector>
#include <map>
#include <string>

#include "basics.h"

#include "System.h"

class Engine
{
    public:

        /// @brief Starts running the engine. Code will not advance past this point until the engine stops running.
        void Run();

        /// @brief Flags the engine to close once it finishes this loop
        void Close();

        /// @brief gets the duration of each fixed frame
        /// @return the amount of time in seconds that each fixed frame lasts
        float getFixedFrameDuration() const;

        /// @brief sets the duration of each fixed frame
        /// @param fixedFrameDuration the amount of time in seconds that each fixed frame lasts
        void setFixedFrameDuration( float fixedFrameDuration );

        /// @brief Gets the array of all Systems in the engine.
        /// @return the array of all Systems in the engine
        std::vector< System * > const& getSystems() const;

    private:

        /// @brief Container of all the Systems in the engine
        std::vector< System * > systems;

        /// @brief flag set to true when the engine needs to exit
        bool shouldExit;

        /// @brief The timestamp of the previous frame
        double previousTime;

        /// @brief The timestamp of the previous fixed frame
        double previousFixedTime;

        /// @brief The duration of each fixed frame
        float fixedFrameDuration;

        /// @brief Adds a System to the Engine.
        /// @tparam SystemType The type of system to add the Engine
        template < class SystemType >
        System* AddSystem();

        /// @brief contains the function for adding each System type to the Engine. Used for Loading systems from config.
        static std::map< std::string, System* (Engine::*)()> addSystemMethods;

        /// @brief Loads the engine config from "Data/EngineConfig.json"
        void Load();

        /// @brief Initializes the engine and all Systems in the Engine.
        void Init();

        /// @brief Updates the engine each frame
        void Update();

        /// @brief Calls all Systems in the Engine's Update function
        void UpdateSystems( float dt );

        /// @brief Calls all Systems in the Engine's FixedUpdate function
        void FixedUpdateSystems();

        /// @brief exits and closes the Engine
        void Exit();


        /// @brief The singleton instance of the Engine
        static Engine * instance;

        /// @brief Constructs a new Engine
        Engine();


    public:

        /// @brief gets the singleton instance of the Engine
        /// @return the singleton instance of the Engine
        static Engine * getInstance();

        // Prevent Engine from being copied
        Engine( Engine& other ) = delete;
        void operator=( const Engine& ) = delete;
};