/// @file       AudioSystem.cpp
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      System that implements FMOD and allows the loading and playing of audio
/// @version    0.1
/// @date       2023-09-12
/// 
/// @copyright  Copyright (c) 2023 Digipen Institute of Technology

#include "AudioSystem.h"
#include "basics.h"
#include "DebugSystem.h"
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
            Debug() << "Failed to initialize FMOD" << std::endl << std::endl;
            return;
        }

        result = m_System->init( m_MaxChannels, FMOD_INIT_NORMAL, 0 );
        if ( result != FMOD_OK )
        {
            Debug() << "Failed to initialize FMOD" << std::endl << std::endl;
            return;
        }

        result = m_System->setCallback( fmodCallback, FMOD_SYSTEM_CALLBACK_ERROR );
        if ( result != FMOD_OK )
        {
            Debug() << "Failed to set FMOD callback" << std::endl << std::endl;
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
    void AudioSystem::readMaxChannels( nlohmann::ordered_json const& data )
    {
        m_MaxChannels = Stream::Read<int>( data );
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

            static char const* errorCodeNames[] = {
                "FMOD_OK",
                "FMOD_ERR_BADCOMMAND",
                "FMOD_ERR_CHANNEL_ALLOC",
                "FMOD_ERR_CHANNEL_STOLEN",
                "FMOD_ERR_DMA",
                "FMOD_ERR_DSP_CONNECTION",
                "FMOD_ERR_DSP_DONTPROCESS",
                "FMOD_ERR_DSP_FORMAT",
                "FMOD_ERR_DSP_INUSE",
                "FMOD_ERR_DSP_NOTFOUND",
                "FMOD_ERR_DSP_RESERVED",
                "FMOD_ERR_DSP_SILENCE",
                "FMOD_ERR_DSP_TYPE",
                "FMOD_ERR_FILE_BAD",
                "FMOD_ERR_FILE_COULDNOTSEEK",
                "FMOD_ERR_FILE_DISKEJECTED",
                "FMOD_ERR_FILE_EOF",
                "FMOD_ERR_FILE_ENDOFDATA",
                "FMOD_ERR_FILE_NOTFOUND",
                "FMOD_ERR_FORMAT",
                "FMOD_ERR_HEADER_MISMATCH",
                "FMOD_ERR_HTTP",
                "FMOD_ERR_HTTP_ACCESS",
                "FMOD_ERR_HTTP_PROXY_AUTH",
                "FMOD_ERR_HTTP_SERVER_ERROR",
                "FMOD_ERR_HTTP_TIMEOUT",
                "FMOD_ERR_INITIALIZATION",
                "FMOD_ERR_INITIALIZED",
                "FMOD_ERR_INTERNAL",
                "FMOD_ERR_INVALID_FLOAT",
                "FMOD_ERR_INVALID_HANDLE",
                "FMOD_ERR_INVALID_PARAM",
                "FMOD_ERR_INVALID_POSITION",
                "FMOD_ERR_INVALID_SPEAKER",
                "FMOD_ERR_INVALID_SYNCPOINT",
                "FMOD_ERR_INVALID_THREAD",
                "FMOD_ERR_INVALID_VECTOR",
                "FMOD_ERR_MAXAUDIBLE",
                "FMOD_ERR_MEMORY",
                "FMOD_ERR_MEMORY_CANTPOINT",
                "FMOD_ERR_NEEDS3D",
                "FMOD_ERR_NEEDSHARDWARE",
                "FMOD_ERR_NET_CONNECT",
                "FMOD_ERR_NET_SOCKET_ERROR",
                "FMOD_ERR_NET_URL",
                "FMOD_ERR_NET_WOULD_BLOCK",
                "FMOD_ERR_NOTREADY",
                "FMOD_ERR_OUTPUT_ALLOCATED",
                "FMOD_ERR_OUTPUT_CREATEBUFFER",
                "FMOD_ERR_OUTPUT_DRIVERCALL",
                "FMOD_ERR_OUTPUT_FORMAT",
                "FMOD_ERR_OUTPUT_INIT",
                "FMOD_ERR_OUTPUT_NODRIVERS",
                "FMOD_ERR_PLUGIN",
                "FMOD_ERR_PLUGIN_MISSING",
                "FMOD_ERR_PLUGIN_RESOURCE",
                "FMOD_ERR_PLUGIN_VERSION",
                "FMOD_ERR_RECORD",
                "FMOD_ERR_REVERB_CHANNELGROUP",
                "FMOD_ERR_REVERB_INSTANCE",
                "FMOD_ERR_SUBSOUNDS",
                "FMOD_ERR_SUBSOUND_ALLOCATED",
                "FMOD_ERR_SUBSOUND_CANTMOVE",
                "FMOD_ERR_TAGNOTFOUND",
                "FMOD_ERR_TOOMANYCHANNELS",
                "FMOD_ERR_TRUNCATED",
                "FMOD_ERR_UNIMPLEMENTED",
                "FMOD_ERR_UNINITIALIZED",
                "FMOD_ERR_UNSUPPORTED",
                "FMOD_ERR_VERSION",
                "FMOD_ERR_EVENT_ALREADY_LOADED",
                "FMOD_ERR_EVENT_LIVEUPDATE_BUSY",
                "FMOD_ERR_EVENT_LIVEUPDATE_MISMATCH",
                "FMOD_ERR_EVENT_LIVEUPDATE_TIMEOUT",
                "FMOD_ERR_EVENT_NOTFOUND",
                "FMOD_ERR_STUDIO_UNINITIALIZED",
                "FMOD_ERR_STUDIO_NOT_LOADED",
                "FMOD_ERR_INVALID_STRING",
                "FMOD_ERR_ALREADY_LOCKED",
                "FMOD_ERR_NOT_LOCKED",
                "FMOD_ERR_RECORD_DISCONNECTED",
                "FMOD_ERR_TOOMANYSAMPLES"
            };

            static char const* instanceTypeNames[] = {
                "FMOD_ERRORCALLBACK_INSTANCETYPE_NONE",
                "FMOD_ERRORCALLBACK_INSTANCETYPE_SYSTEM",
                "FMOD_ERRORCALLBACK_INSTANCETYPE_CHANNEL",
                "FMOD_ERRORCALLBACK_INSTANCETYPE_CHANNELGROUP",
                "FMOD_ERRORCALLBACK_INSTANCETYPE_CHANNELCONTROL",
                "FMOD_ERRORCALLBACK_INSTANCETYPE_SOUND",
                "FMOD_ERRORCALLBACK_INSTANCETYPE_SOUNDGROUP",
                "FMOD_ERRORCALLBACK_INSTANCETYPE_DSP",
                "FMOD_ERRORCALLBACK_INSTANCETYPE_DSPCONNECTION",
                "FMOD_ERRORCALLBACK_INSTANCETYPE_GEOMETRY",
                "FMOD_ERRORCALLBACK_INSTANCETYPE_REVERB3D",
                "FMOD_ERRORCALLBACK_INSTANCETYPE_STUDIO_SYSTEM",
                "FMOD_ERRORCALLBACK_INSTANCETYPE_STUDIO_EVENTDESCRIPTION",
                "FMOD_ERRORCALLBACK_INSTANCETYPE_STUDIO_EVENTINSTANCE",
                "FMOD_ERRORCALLBACK_INSTANCETYPE_STUDIO_PARAMETERINSTANCE",
                "FMOD_ERRORCALLBACK_INSTANCETYPE_STUDIO_BUS",
                "FMOD_ERRORCALLBACK_INSTANCETYPE_STUDIO_VCA",
                "FMOD_ERRORCALLBACK_INSTANCETYPE_STUDIO_BANK",
                "FMOD_ERRORCALLBACK_INSTANCETYPE_STUDIO_COMMANDREPLAY"
            };

            Debug() << "There was an FMOD error:\n" <<
                "\tError Code:          " << errorCodeNames[ info->result ] << '\n' <<
                "\tInstance Type:       " << instanceTypeNames[ info->instancetype ] << '\n' <<
                "\tFunction Name:       " << info->functionname << '\n' <<
                "\tFunction Parameters: " << info->functionparams << '\n' << std::endl;

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
    {
    }

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
