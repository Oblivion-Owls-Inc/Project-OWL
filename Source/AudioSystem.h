/// @file AudioSystem.h
/// @author Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief System that implements FMOD and allows the loading and playing of audio
/// @version 0.1
/// @date 2023-09-12
/// 
/// @copyright Copyright (c) 2023

#pragma once

#include "System.h"
#include <fmod.hpp>

/// @brief Example System meant to be copy-pasted when creating new Systems
class AudioSystem : public System
{
public: // methods

    /// @brief gets the internal FMOD::System
    /// @return the FMOD::System
    FMOD::System* getFMOD();

private: // member variables

    /// @brief The FMOD system
    FMOD::System* system;

    /// @brief The maximum number of FMOD audio channels
    int maxChannels;

private: // static methods

    /// @brief FMOD callback function for error handling
    /// @param system handle to the FMOD system
    /// @param type the type of callback
    /// @param commandData1 first callback parameter, dependent on callback type
    /// @param commandData2 second callback parameter, dependent on callback type
    /// @param userData user data associated with the FMOD system
    static FMOD_RESULT FMODCallback(
        FMOD_SYSTEM* system,
        FMOD_SYSTEM_CALLBACK_TYPE type,
        void* commandData1,
        void* commandData2,
        void* userData
    );

private: // virtual methods

    /// @brief Gets called once when this System is added to the Engine
    virtual void OnInit() override;

    /// @brief Gets called once every graphics frame. Do not use this function for anything that affects the simulation.
    /// @param dt the elapsed time in seconds since the previous frame
    virtual void OnUpdate( float dt ) override;

    /// @brief Gets called once before the Engine closes
    virtual void OnExit() override;

private: // unused virtual methods

    /// @brief Gets called once every simulation frame. Use this function for anything that affects the simulation.
    virtual void OnFixedUpdate() override {}

    /// @brief Gets called whenever a new Scene is loaded
    virtual void OnSceneLoad() override {}


    /// @brief Gets called whenever a scene is initialized
    virtual void OnSceneInit() override {}


    /// @brief Gets called whenever a scene is exited
    virtual void OnSceneExit() override {}


    /// @brief Loads the config data of this System
    /// @param configData the JSON object with all of the configData for this System
    virtual void Load( rapidjson::Value const& configData ) override {}

private: // constructor/singleton stuff

    /// @brief Constructs the AudioSystem
    AudioSystem();

    /// @brief The singleton instance of AudioSystem
    static AudioSystem * instance;

public: // singleton stuff

    /// @brief gets the instance of AudioSystem
    /// @return the instance of the AudioSystem
    static AudioSystem * getInstance();

    // Prevent copying
    AudioSystem(AudioSystem& other) = delete;
    void operator=(const AudioSystem&) = delete;
};

