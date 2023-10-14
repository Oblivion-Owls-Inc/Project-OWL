/// @file       AudioSystem.h
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      System that implements FMOD and allows the loading and playing of audio
/// @version    0.1
/// @date       2023-09-12
/// 
/// @copyright  Copyright (c) 2023

#pragma once

#include "System.h"
#include <fmod.hpp>

/// @brief  Example System meant to be copy-pasted when creating new Systems
class AudioSystem : public System
{

//-----------------------------------------------------------------------------
public: // accessors
//-----------------------------------------------------------------------------

    /// @brief  gets the internal FMOD::System
    /// @return the FMOD::System
    FMOD::System* GetFMOD();

//-----------------------------------------------------------------------------
private: // virtual override methods
//-----------------------------------------------------------------------------

    /// @brief  Gets called once when this System is added to the Engine
    virtual void OnInit() override;

    /// @brief  Gets called once every graphics frame. Do not use this function for anything that affects the simulation.
    /// @param  dt  the elapsed time in seconds since the previous frame
    virtual void OnUpdate( float dt ) override;

    /// @brief  Gets called once before the Engine closes
    virtual void OnExit() override;

//-----------------------------------------------------------------------------
private: // reading
//-----------------------------------------------------------------------------

    /// @brief  reads the max channels
    /// @param  stream  the data to read from
    void readMaxChannels( nlohmann::json const& data );

    /// @brief  map of the AudioSystem read methods
    static ReadMethodMap< AudioSystem > const s_ReadMethods;

    /// @brief  gets this System's read methods
    /// @return this System's read methods
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override
    {
        return (ReadMethodMap< ISerializable > const&)s_ReadMethods;
    }
    
//-----------------------------------------------------------------------------
private: // member variables
//-----------------------------------------------------------------------------

    /// @brief  The FMOD system
    FMOD::System* m_System;

    /// @brief  The maximum number of FMOD audio channels
    int m_MaxChannels;

//-----------------------------------------------------------------------------
private: // static methods
//-----------------------------------------------------------------------------

    /// @brief  FMOD callback function for error handling
    /// @param  system          handle to the FMOD system
    /// @param  type            the type of callback
    /// @param  commandData1    first callback parameter, dependent on callback type
    /// @param  commandData2    second callback parameter, dependent on callback type
    /// @param  userData        user data associated with the FMOD system
    /// @return FMOD_RESULT
    static FMOD_RESULT fmodCallback(
        FMOD_SYSTEM* system,
        FMOD_SYSTEM_CALLBACK_TYPE m_Type,
        void* commandData1,
        void* commandData2,
        void* userData
    );

//-----------------------------------------------------------------------------
private: // singleton stuff
//-----------------------------------------------------------------------------

    /// @brief  Constructs the AudioSystem
    AudioSystem();

    /// @brief  The singleton instance of AudioSystem
    static AudioSystem * s_Instance;

//-----------------------------------------------------------------------------
public: // singleton stuff
//-----------------------------------------------------------------------------

    /// @brief  gets the instance of AudioSystem
    /// @return the instance of the AudioSystem
    static AudioSystem * GetInstance();

    // Prevent copying
    AudioSystem( AudioSystem& other ) = delete;
    void operator=( const AudioSystem& ) = delete;
};

