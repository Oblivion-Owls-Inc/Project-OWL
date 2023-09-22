/// @file AudioSystem.cpp
/// @author Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief System that implements FMOD and allows the loading and playing of audio
/// @version 0.1
/// @date 2023-09-12
/// 
/// @copyright Copyright (c) 2023

#include "AudioSystem.h"
#include "basics.h"

//-----------------------------------------------------------------------------
// pubilc methods
//-----------------------------------------------------------------------------

    /// @brief gets the internal FMOD::System
    /// @return the FMOD::System
    FMOD::System* AudioSystem::GetFMOD()
    {
        return system;
    }

//-----------------------------------------------------------------------------
// private static methods
//-----------------------------------------------------------------------------

    /// @brief FMOD callback function for error handling
    /// @param system handle to the FMOD system
    /// @param type the type of callback
    /// @param commandData1 first callback parameter, dependent on callback type
    /// @param commandData2 second callback parameter, dependent on callback type
    /// @param userData user data associated with the FMOD system
    /// @return FMOD_RESULT
    FMOD_RESULT AudioSystem::FMODCallback(
        FMOD_SYSTEM* system,
        FMOD_SYSTEM_CALLBACK_TYPE type,
        void* commandData1,
        void* commandData2,
        void* userData
    ) {
        
        if ( type == FMOD_SYSTEM_CALLBACK_ERROR )
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
// private virtual methods
//-----------------------------------------------------------------------------

    /// @brief Gets called once when this System is added to the Engine
    void AudioSystem::OnInit()
    {
        FMOD_RESULT result;

        result = FMOD::System_Create( &system );
        if ( result != FMOD_OK )
        {
            std::cerr << "Failed to initialize FMOD" << std::endl;
            return;
        }

        result = system->init( maxChannels, FMOD_INIT_NORMAL, 0 );
        if ( result != FMOD_OK )
        {
            std::cerr << "Failed to initialize FMOD" << std::endl;
            return;
        }

        result = system->setCallback( FMODCallback, FMOD_SYSTEM_CALLBACK_ERROR );
        if ( result != FMOD_OK )
        {
            std::cerr << "Failed to set FMOD callback" << std::endl;
            return;
        }
    }

    /// @brief Gets called once every graphics frame. Do not use this function for anything that affects the simulation.
    /// @param dt the elapsed time in seconds since the previous frame
    void AudioSystem::OnUpdate( float dt )
    {
        system->update();
    }

    /// @brief Gets called once before the Engine closes
    void AudioSystem::OnExit()
    {
        system->release();
    }

//-----------------------------------------------------------------------------
// singleton stuff
//-----------------------------------------------------------------------------

    /// @brief Constructs the AudioSystem
    AudioSystem::AudioSystem() :
        system( nullptr ),
        maxChannels( 1024 )
    {}

    /// @brief The singleton instance of AudioSystem
    AudioSystem * AudioSystem::instance = nullptr;

    /// @brief gets the instance of AudioSystem
    /// @return the instance of the AudioSystem
    AudioSystem * AudioSystem::GetInstance()
    {
        if ( instance == nullptr )
        {
            instance = new AudioSystem();
        }
        return instance;
    }

//-----------------------------------------------------------------------------
