/// @file System.h
/// @author Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief Base class for all Systems
/// @version 0.1
/// @date 2023-09-05
/// 
/// @copyright Copyright (c) 2023

#pragma once

#include "../rapidjson/document.h"

/// @brief Base class for all Systems
class System
{
    public:
        /// @brief Gets called once when this System is added to the Engine
        virtual void OnInit() = 0;

        /// @brief Gets called once every simulation frame. Use this function for anything that affects the simulation.
        virtual void OnFixedUpdate() = 0;

        /// @brief Gets called once every graphics frame. Do not use this function for anything that affects the simulation.
        /// @param dt the elapsed time in seconds since the previous frame
        virtual void OnUpdate(float dt) = 0;

        /// @brief Gets called once before the Engine closes
        virtual void OnExit() = 0;

        /// @brief Gets called whenever a new Scene is loaded
        virtual void OnSceneLoad() = 0;

        /// @brief Gets called whenever a scene is initialized
        virtual void OnSceneInit() = 0;

        /// @brief Gets called whenever a scene is exited
        virtual void OnSceneExit() = 0;

        /// @brief Loads the config data of this System
        /// @param configData the JSON object with all of the configData for this System
        virtual void Load( rapidjson::Value const& configData ) = 0;


    protected:
    
        /// @brief Constructs a System
        System() = default;

    public:
    
        // Prevent Systems from being copied
        System( System& other ) = delete;
        void operator=( const System& ) = delete;

};

