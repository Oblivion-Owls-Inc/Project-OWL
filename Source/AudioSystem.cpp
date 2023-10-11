/// @file       AudioSystem.cpp
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      System that implements FMOD and allows the loading and playing of audio
/// @version    0.1
/// @date       2023-09-12
/// 
/// @copyright  Copyright (c) 2023 Digipen Institute of Technology

#include "AudioSystem.h"
#include "basics.h"

//-----------------------------------------------------------------------------
// public: accessors
//-----------------------------------------------------------------------------

    /// @brief  gets the internal FMOD::System
    /// @return the FMOD::System
    FMOD::System* AudioSystem::GetFMOD()
    {
        return m_System;
    }

//-----------------------------------------------------------------------------
// private: virtual overrides
//-----------------------------------------------------------------------------

    /// @brief  Gets called once when this System is added to the Engine
    void AudioSystem::OnInit()
    {
        FMOD_RESULT result;

        result = FMOD::System_Create( &m_System );
        if ( result != FMOD_OK )
        {
            std::cerr << "Failed to initialize FMOD" << std::endl;
            return;
        }

        result = m_System->init( m_MaxChannels, FMOD_INIT_NORMAL, 0 );
        if ( result != FMOD_OK )
        {
            std::cerr << "Failed to initialize FMOD" << std::endl;
            return;
        }

        result = m_System->setCallback( fmodCallback, FMOD_SYSTEM_CALLBACK_ERROR );
        if ( result != FMOD_OK )
        {
            std::cerr << "Failed to set FMOD callback" << std::endl;
            return;
        }
    }

    /// @brief  Gets called once every graphics frame. Do not use this function for anything that affects the simulation.
    /// @param  dt  the elapsed time in seconds since the previous frame
    void AudioSystem::OnUpdate( float dt )
    {
        m_System->update();
    }

    /// @brief  Gets called once before the Engine closes
    void AudioSystem::OnExit()
    {
        m_System->release();
    }

//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------

    /// @brief  reads the max channels
    /// @param  stream  the data to read from
    void AudioSystem::readMaxChannels( Stream stream )
    {
        m_MaxChannels = stream.Read<int>();
    }

    /// @brief  map of the AudioSystem read methods
    ReadMethodMap< AudioSystem > const AudioSystem::s_ReadMethods = {
        { "MaxChannels", &readMaxChannels }
    };

//-----------------------------------------------------------------------------
// private: static methods
//-----------------------------------------------------------------------------

    /// @brief  FMOD callback function for error handling
    /// @param  system          handle to the FMOD system
    /// @param  type            the type of callback
    /// @param  commandData1    first callback parameter, dependent on callback type
    /// @param  commandData2    second callback parameter, dependent on callback type
    /// @param  userData        user data associated with the FMOD system
    /// @return FMOD_RESULT
    FMOD_RESULT AudioSystem::fmodCallback(
        FMOD_SYSTEM* system,
        FMOD_SYSTEM_CALLBACK_TYPE m_Type,
        void* commandData1,
        void* commandData2,
        void* userData
    )
    {    
        if ( m_Type == FMOD_SYSTEM_CALLBACK_ERROR )
        {
            FMOD_ERRORCALLBACK_INFO* info = (FMOD_ERRORCALLBACK_INFO*)commandData1;

            std::cerr << "There was an FMOD error:\n" <<
                "\tError Code:              " << info->result << '\n' <<
                "\tInstance Type:           " << info->instancetype << '\n' <<
                "\tFunction Name:           " << info->functionname << '\n' <<
                "\tFunction Parameters :    " << info->functionparams << '\n' << std::endl;

            return info->result;
        }

        return FMOD_OK;
    }

//-----------------------------------------------------------------------------
// singleton stuff
//-----------------------------------------------------------------------------

    /// @brief  Constructs the AudioSystem
    AudioSystem::AudioSystem() :
        System( "AudioSystem" ),
        m_System( nullptr ),
        m_MaxChannels( 1024 )
    {}

    /// @brief  The singleton instance of AudioSystem
    AudioSystem * AudioSystem::s_Instance = nullptr;

    /// @brief  gets the instance of AudioSystem
    /// @return the instance of the AudioSystem
    AudioSystem * AudioSystem::GetInstance()
    {
        if ( s_Instance == nullptr )
        {
            s_Instance = new AudioSystem();
        }
        return s_Instance;
    }

//-----------------------------------------------------------------------------
