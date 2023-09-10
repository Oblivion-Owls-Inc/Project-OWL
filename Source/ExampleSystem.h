/// @file ExampleSystem.h
/// @author Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief Example System meant to be copy-pasted when creating new Systems
/// @version 0.1
/// @date 2023-09-05
/// 
/// @copyright Copyright (c) 2023

#pragma once

#include "System.h"

/// @brief Example System meant to be copy-pasted when creating new Systems
class ExampleSystem : System
{
private:

    /// @brief Gets called once when this System is added to the Engine
    virtual void OnInit() override {}


    /// @brief Gets called once every simulation frame. Use this function for anything that affects the simulation.
    virtual void OnFixedUpdate() override {}


    /// @brief Gets called once every graphics frame. Do not use this function for anything that affects the simulation.
    /// @param dt the elapsed time in seconds since the previous frame
    virtual void OnUpdate( float dt ) override {}


    /// @brief Gets called once before the Engine closes
    virtual void OnExit() override {}


    /// @brief Gets called whenever a new Scene is loaded
    virtual void OnSceneLoad() override {}


    /// @brief Gets called whenever a scene is initialized
    virtual void OnSceneInit() override {}


    /// @brief Gets called whenever a scene is exited
    virtual void OnSceneExit() override {}


    /// @brief Loads the config data of this System
    /// @param configData the JSON object with all of the configData for this System
    virtual void Load( rapidjson::Value const& configData ) override {}


     /// @brief Constructs the ExampleSystem
    ExampleSystem();


     /// @brief The singleton instance of ExampleSystem
    static ExampleSystem * instance;

public:

     /// @brief gets the instance of ExampleSystem
     /// @return the instance of the ExampleSystem
    static ExampleSystem * getInstance();

    // MAY INCLUDE A DUPLICATE OF THE ABOVE FUNCTION WITH CONSTRUCTOR ARGUMENTS HERE


    // Prevent copying
    ExampleSystem(ExampleSystem& other) = delete;
    void operator=(const ExampleSystem&) = delete;
};

