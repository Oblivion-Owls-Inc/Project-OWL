/// @file       AudioSystem.cpp
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      System that implements FMOD and allows the loading and playing of audio
/// @version    0.1
/// @date       2023-09-12
/// 
/// @copyright  Copyright (c) 2023 Digipen Institute of Technology

#include "pch.h" // precompiled header has to be included first
#include "AudioSystem.h"

#include "AudioListener.h"

#include "basics.h"
#include "DebugSystem.h"

#include "Inspection.h"
    

//-----------------------------------------------------------------------------
// public: accessors
//-----------------------------------------------------------------------------


    /// @brief  gets the internal FMOD::System
    /// @return the FMOD::System
    FMOD::System* AudioSystem::GetFMOD()
    {
        return m_System;
    }


    /// @brief  gets the currently active AudioListener
    /// @return the currently active AudioListener
    AudioListener* AudioSystem::GetActiveListener() const
    {
        return m_ActiveListener;
    }


//-----------------------------------------------------------------------------
// public: methods
//-----------------------------------------------------------------------------


    /// @brief  gets the currently active AudioListener
    /// @param  listener    the AudioListener to make active
    void AudioSystem::SetActiveListener( AudioListener* listener )
    {
        if ( m_ActiveListener != nullptr )
        {
            m_ActiveListener->SetIsActive( false );
        }

        m_ActiveListener = listener;

        if ( listener != nullptr )
        {
            listener->SetIsActive( true );
        }
    }


    /// @brief  gets the channel group with the specified name
    /// @param  name    the name of the channel group to get
    /// @return the FMOD::ChannelGroup with the specified name
    FMOD::ChannelGroup* AudioSystem::GetChannelGroup( std::string const& name )
    {
        auto it = std::find_if(
            m_Groups.begin(), m_Groups.end(),
            [ name ]( AudioGroup const& group ) -> bool
            {
                return name == group.M_Name;
            }
        );
        if ( it == m_Groups.end() )
        {
            Debug() << "WARNING: unable to find AudioGroup with name \"" << name << "\"" << std::endl;
            return nullptr;
        }

        return it->M_Group;
    }


    /// @brief  gets the volume of the master channel
    /// @return the volume of the master channel
    float AudioSystem::GetVolume() const
    {
        return m_Volume;
    }

    /// @brief  sets the volume of the master channel
    /// @param  volume  the volume to set the master channel to
    void AudioSystem::SetVolume( float volume )
    {
        m_Volume = volume;

        FMOD::ChannelGroup* masterGroup;
        m_System->getMasterChannelGroup( &masterGroup );
        masterGroup->setVolume( m_Volume );
    }


    /// @brief  gets the volume of the specified channel
    /// @param  name    the name of the channel to get the volume of
    /// @return the volume of the channel
    float AudioSystem::GetVolume( std::string const& name ) const
    {
        auto it = std::find_if(
            m_Groups.begin(), m_Groups.end(),
            [ name ]( AudioGroup const& group ) -> bool
            {
                return name == group.M_Name;
            }
        );
        if ( it == m_Groups.end() )
        {
            Debug() << "WARNING: unable to find AudioGroup with name \"" << name << "\"" << std::endl;
            return 0.0f;
        }

        return it->M_Volume;
    }

    /// @brief  sets the volume of the specified channel
    /// @param  name    the name of the channel to set the volume of
    /// @param  volume  the volume to set the channel to
    void AudioSystem::SetVolume( std::string const& name, float volume )
    {
        auto it = std::find_if(
            m_Groups.begin(), m_Groups.end(),
            [ name ]( AudioGroup const& group ) -> bool
            {
                return name == group.M_Name;
            }
        );
        if ( it == m_Groups.end() )
        {
            Debug() << "WARNING: unable to find AudioGroup with name \"" << name << "\"" << std::endl;
            return;
        }

        it->M_Volume = volume;
        it->M_Group->setVolume( volume );
    }


//-----------------------------------------------------------------------------
// public: virtual overrides
//-----------------------------------------------------------------------------


    /// @brief  Gets called once when this System is added to the Engine
    void AudioSystem::OnInit()
    {
        FMOD_RESULT result;

        result = FMOD::System_Create( &m_System );
        if ( result != FMOD_OK )
        {
            Debug() << "ERROR: Failed to create FMOD system" << std::endl << std::endl;
            return;
        }

        result = m_System->init( m_MaxChannels, FMOD_INIT_3D_RIGHTHANDED, 0 );
        if ( result != FMOD_OK )
        {
            Debug() << "ERROR: Failed to initialize FMOD system" << std::endl << std::endl;
            return;
        }

        result = m_System->setCallback( fmodCallback, FMOD_SYSTEM_CALLBACK_ERROR );
        if ( result != FMOD_OK )
        {
            Debug() << "ERROR: Failed to set FMOD callback" << std::endl << std::endl;
        }


        FMOD::ChannelGroup* masterGroup;
        m_System->getMasterChannelGroup( &masterGroup );
        masterGroup->setVolume( m_Volume );


        for ( AudioGroup& group : m_Groups )
        {
            group.Init( m_System );
        }
    }

    /// @brief  Gets called once before the Engine closes
    void AudioSystem::OnExit()
    {
        for ( AudioGroup& group : m_Groups )
        {
            group.Exit();
        }

        m_System->release();
    }


    /// @brief  Gets called once every graphics frame. Do not use this function for anything that affects the simulation.
    /// @param  dt  the elapsed time in seconds since the previous frame
    void AudioSystem::OnUpdate( float dt )
    {
        if ( m_ActiveListener != nullptr )
        {
            m_ActiveListener->OnUpdate( dt );
        }

        m_System->update();
    }

    
//-----------------------------------------------------------------------------
// private: struct AudioGroup
//-----------------------------------------------------------------------------
        
        
    //-----------------------------------------------------------------------------
    // public: methods
    //-----------------------------------------------------------------------------
    
        
        /// @brief  initializes this AudioGroup
        /// @param  system  the FMOD system to initialize the AudioGroup in
        void AudioSystem::AudioGroup::Init( FMOD::System* system )
        {
            system->createChannelGroup( M_Name.c_str(), &M_Group );

            if ( M_Group == nullptr )
            {
                Debug() << "Failed to create FMOD ChannelGroup \"" << M_Name << "\"" << std::endl;
                return;
            }

            M_Group->setVolume( M_Volume );
            M_Group->setMute( M_Mute );
        }


        /// @brief  exits this AudioGroup;
        void AudioSystem::AudioGroup::Exit()
        {
            if ( M_Group != nullptr )
            {
                M_Group->release();
            }
        }


    //-----------------------------------------------------------------------------
    // public: inspection
    //-----------------------------------------------------------------------------
    

        /// @brief  inspects this ChannelGroup
        /// @return whether this ChannelGroup was changed
        bool AudioSystem::AudioGroup::Inspect()
        {
            bool changed = false;

            changed |= ImGui::InputText( "name", &M_Name );
            ImGui::SetItemTooltip( "underlying channel name will not update until the engine restarts" );

            if ( ImGui::DragFloat( "volume", &M_Volume, 0.05f, 0.0f, INFINITY ) )
            {
                M_Group->setVolume( M_Volume );
                changed = true;
            }

            if ( ImGui::Checkbox( "mute", &M_Mute ) )
            {
                M_Group->setMute( M_Mute );
                changed = true;
            }

            return changed;
        }

        
    //-----------------------------------------------------------------------------
    // public: reading / writing
    //-----------------------------------------------------------------------------
    

        /// @brief  gets this System's read methods
        /// @return this System's read methods
        ReadMethodMap< ISerializable > const& AudioSystem::AudioGroup::GetReadMethods() const
        {
            static ReadMethodMap< AudioGroup > const readMethods = {
                { "Name"  , &AudioGroup::readName   },
                { "Volume", &AudioGroup::readVolume },
                { "Mute"  , &AudioGroup::readMute   }
            };

            return (ReadMethodMap< ISerializable > const&)readMethods;
        }


        /// @brief  writes this AudioSystem to json
        /// @return the json data of the AudioSystem
        nlohmann::ordered_json AudioSystem::AudioGroup::Write() const
        {
            nlohmann::ordered_json json = nlohmann::ordered_json::object();

            json[ "Name"   ] = Stream::Write( M_Name   );
            json[ "Volume" ] = Stream::Write( M_Volume );
            json[ "Mute"   ] = Stream::Write( M_Mute   );

            return json;
        }

        
    //-----------------------------------------------------------------------------
    // private: reading
    //-----------------------------------------------------------------------------
    

        /// @brief  reads the name of this AudioGroup
        /// @param  data    the JSON data to read from
        void AudioSystem::AudioGroup::readName( nlohmann::ordered_json const& data )
        {
            Stream::Read( M_Name, data );
        }


        /// @brief  reads the volume of this AudioGroup
        /// @param  data    the JSON data to read from
        void AudioSystem::AudioGroup::readVolume( nlohmann::ordered_json const& data )
        {
            Stream::Read( M_Volume, data );
        }

        /// @brief  reads whether this AudioGroup is muted
        /// @param  data    the JSON data to read from
        void AudioSystem::AudioGroup::readMute( nlohmann::ordered_json const& data )
        {
            Stream::Read( M_Mute, data );
        }


    //-----------------------------------------------------------------------------


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

            Debug() << "ERROR: " << "There was an FMOD error:\n" <<
                "\tError Code:          " << errorCodeNames[ info->result ] << '\n' <<
                "\tInstance Type:       " << instanceTypeNames[ info->instancetype ] << '\n' <<
                "\tFunction Name:       " << info->functionname << '\n' <<
                "\tFunction Parameters: " << info->functionparams << '\n' << std::endl;

            return info->result;
        }

        return FMOD_OK;
    }

    
//-----------------------------------------------------------------------------
// public: inspection
//-----------------------------------------------------------------------------
    
    
    /// @brief  inspects a reference to a channel group
    /// @param  label           the label of the channel group
    /// @param  groupName       the name of the channel group reference being inspected
    /// @param  channelGroup    the channel group reference being inspeced
    /// @return whether the reference to the channel group was changed
    bool AudioSystem::InspectChannelGroup( char const* label, std::string* groupName, FMOD::ChannelGroup** channelGroup )
    {
        if ( ImGui::BeginCombo( label, groupName->c_str() ) == false )
        {
            return false;
        }

        for ( AudioGroup& group : m_Groups )
        {
            if ( ImGui::Selectable( group.M_Name.c_str(), channelGroup != nullptr && group.M_Group == *channelGroup ) )
            {
                *groupName = group.M_Name;
                if (channelGroup != nullptr)
                {
                    *channelGroup = group.M_Group;
                }
                ImGui::EndCombo();
                return true;
            }
        }

        ImGui::EndCombo();
        return false;
    }


    /// @brief  shows this System's DebugWindow
    void AudioSystem::DebugWindow()
    {
        ImGui::DragInt( "max channels", &m_MaxChannels, 0.05f, 1, INT_MAX );
        ImGui::SetItemTooltip( "max channels will not update until the Engine restarts" );

        if ( ImGui::DragFloat( "Master Volume", &m_Volume, 0.05f, 0.0f, INFINITY ) )
        {
            if ( m_System != nullptr )
            {
                FMOD::ChannelGroup* masterChannelGroup;
                m_System->getMasterChannelGroup( &masterChannelGroup );
                masterChannelGroup->setVolume( m_Volume );
            }
        }

        Inspection::InspectArray< AudioGroup >(
            "audio groups", &m_Groups,
            []( AudioGroup* group ) -> bool
            {
                return group->Inspect();
            },
            [ this ]( AudioGroup* group )
            {
                group->Init( m_System );
            },
            []( AudioGroup* group )
            {
                group->Exit();
            }
        );
    }


//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------


    /// @brief  reads the max channels
    /// @param  stream  the data to read from
    void AudioSystem::readMaxChannels( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_MaxChannels, data );
    }

    /// @brief  reads the master volume of the AudioSystem
    /// @param  data    the JSON data to read from
    void AudioSystem::readVolume( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_Volume, data );
    }

    /// @brief  reads the groups in the AudioSystem
    /// @param  data    the JSON data to read from
    void AudioSystem::readGroups( nlohmann::ordered_json const& data )
    {
        Stream::Read< AudioGroup >( &m_Groups, data );
    }


//-----------------------------------------------------------------------------
// public: reading / writing
//-----------------------------------------------------------------------------


    /// @brief  gets this System's read methods
    /// @return this System's read methods
    ReadMethodMap< ISerializable > const& AudioSystem::GetReadMethods() const
    {
        static ReadMethodMap< AudioSystem > const readMethods = {
            { "MaxChannels", &AudioSystem::readMaxChannels },
            { "Volume"     , &AudioSystem::readVolume      },
            { "Groups"     , &AudioSystem::readGroups      }
        };

        return (ReadMethodMap< ISerializable > const&)readMethods;
    }


    /// @brief  writes this AudioSystem to json
    /// @return the json data of the AudioSystem
    nlohmann::ordered_json AudioSystem::Write() const
    {
        nlohmann::ordered_json json;

        json[ "MaxChannels" ] = Stream::Write( m_MaxChannels );
        json[ "Volume"      ] = Stream::Write( m_Volume      );
        json[ "Groups"      ] = Stream::WriteArray( m_Groups );

        return json;
    }


//-----------------------------------------------------------------------------
// public: singleton stuff
//-----------------------------------------------------------------------------


    /// @brief  gets the instance of AudioSystem
    /// @return the instance of the AudioSystem
    AudioSystem* AudioSystem::GetInstance()
    {
        static std::unique_ptr< AudioSystem > s_Instance = nullptr;
        if (s_Instance == nullptr )
        {
            s_Instance.reset( new AudioSystem() );
        }
        return s_Instance.get();
    }


//-----------------------------------------------------------------------------
// private: singleton stuff
//-----------------------------------------------------------------------------


    /// @brief  Constructs the AudioSystem
    AudioSystem::AudioSystem() :
        System( "AudioSystem" )
    {}


//-----------------------------------------------------------------------------
