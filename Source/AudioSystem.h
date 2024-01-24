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


class AudioListener;


/// @brief  Example System meant to be copy-pasted when creating new Systems
class AudioSystem : public System
{
//-----------------------------------------------------------------------------
public: // accessors
//-----------------------------------------------------------------------------


    /// @brief  gets the internal FMOD::System
    /// @return the FMOD::System
    FMOD::System* GetFMOD();


    /// @brief  gets the currently active AudioListener
    /// @return the currently active AudioListener
    AudioListener* GetActiveListener() const;


//-----------------------------------------------------------------------------
public: // methods
//-----------------------------------------------------------------------------


    /// @brief  gets the currently active AudioListener
    /// @param  listener    the AudioListener to make active
    void SetActiveListener( AudioListener* listener );


//-----------------------------------------------------------------------------
public: // virtual override methods
//-----------------------------------------------------------------------------


    /// @brief  Gets called once when this System is added to the Engine
    virtual void OnInit() override;

    /// @brief  Gets called once before the Engine closes
    virtual void OnExit() override;


    /// @brief  Gets called once every graphics frame. Do not use this function for anything that affects the simulation.
    /// @param  dt  the elapsed time in seconds since the previous frame
    virtual void OnUpdate( float dt ) override;

    
//-----------------------------------------------------------------------------
private: // member variables
//-----------------------------------------------------------------------------


    /// @brief  The FMOD system
    FMOD::System* m_System = nullptr;

    /// @brief  The maximum number of FMOD audio channels
    int m_MaxChannels = 256;


    /// @brief  the currently active AudioListener
    AudioListener* m_ActiveListener = nullptr;


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
private: // reading
//-----------------------------------------------------------------------------


    /// @brief  reads the max channels
    /// @param  data  the data to read from
    void readMaxChannels( nlohmann::ordered_json const& data );

    
//-----------------------------------------------------------------------------
public: // reading / writing
//-----------------------------------------------------------------------------


    /// @brief  gets this System's read methods
    /// @return this System's read methods
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override;


    /// @brief  writes this AudioSystem to json
    /// @return the json data of the AudioSystem
    virtual nlohmann::ordered_json Write() const override;

    
//-----------------------------------------------------------------------------
public: // singleton stuff
//-----------------------------------------------------------------------------


    /// @brief  gets the instance of AudioSystem
    /// @return the instance of the AudioSystem
    static AudioSystem* GetInstance();


//-----------------------------------------------------------------------------
private: // singleton stuff
//-----------------------------------------------------------------------------


    /// @brief  Constructs the AudioSystem
    AudioSystem();


    // Prevent copying
    AudioSystem( AudioSystem const& ) = delete;
    void operator =( AudioSystem const& ) = delete;


//-----------------------------------------------------------------------------
};


/// @brief  shorthand method for AudioSystem::GetInstance()
/// @return the AudioSystem instance
__inline AudioSystem* Audio()
{
    return AudioSystem::GetInstance();
}
