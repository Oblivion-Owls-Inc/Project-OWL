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
class ExampleSystem : public System
{
public: // methods

public: // inline accessors

private: // methods

private: // static methods

private: // virtual override methods


private: // member variables

    /// @brief an example member of this System
    int exampleMember;

private: // class-specific Read Methods

    /// @brief method that read a JSON value into exampleMember
    /// @param readValue the JSON value storing the data to be put into exampleMember
    void ReadExampleMember( rapidjson::Value const& readValue );

private: // unused virtual overrides

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

private: // default Read method stuff

    /// @brief the Read Methods used in this System
    static std::map< std::string, ReadMethod< ExampleSystem > > const ReadMethods;

    /// @brief Gets the read methods of this System
    /// @return the map of read methods of this System
    virtual std::map< std::string, ReadMethod< System > > const& GetReadMethods() override;

private: // singleton stuff

    /// @brief Constructs the ExampleSystem
    ExampleSystem();

    /// @brief The singleton instance of ExampleSystem
    static ExampleSystem * instance;

public: // singleton stuff

    /// @brief gets the instance of ExampleSystem
    /// @return the instance of the ExampleSystem
    static ExampleSystem * getInstance();

    // Prevent copying
    ExampleSystem(ExampleSystem& other) = delete;
    void operator=(const ExampleSystem&) = delete;
};

