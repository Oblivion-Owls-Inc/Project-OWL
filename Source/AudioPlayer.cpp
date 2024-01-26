/// @file       AudioPlayer.cpp
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      Component that plays audio
/// @version    0.1
/// @date       2023-09-13
/// 
/// @copyright  Copyright (c) 2023 Digipen Institute of Technology

#pragma once

#include "AudioPlayer.h"
#include "AssetLibrarySystem.h"

#include "Entity.h"
#include "Transform.h"
#include "RigidBody.h"

#include "Inspection.h"

//-----------------------------------------------------------------------------
// public: constructor / Destructor
//-----------------------------------------------------------------------------

    /// @brief  constructs a new AudioPlayer
    AudioPlayer::AudioPlayer() :
        Behavior( typeid( AudioPlayer ) )
    {}

//-----------------------------------------------------------------------------
// public: methods
//-----------------------------------------------------------------------------


    /// @brief  Starts playing this AudioPlayer's sound
    void AudioPlayer::Play()
    {
        if ( m_Sound == nullptr )
        {
            Debug() << "WARNING: AudioPlayer::m_Sound is NULL" << std::endl;
            return;
        }

        // if this Audio player is already playing a sound, unpuase it
        if ( GetIsPlaying() )
        {
            SetIsPaused( false );
            return;
        }

        // if not already playing a sound, start playing a new sound
        m_Channel = m_Sound->Play(
            m_ChannelGroup,
            random( m_Volume - m_Volume * m_VolumeVariance, m_Volume + m_Volume * m_VolumeVariance ),
            random( m_Pitch - m_Pitch * m_PitchVariance, m_Pitch + m_Pitch * m_PitchVariance ),
            m_DefaultLoopCount
        );

        // tell the channel which Audioplayer it belongs to
        m_Channel->setUserData( this );

        // have the channel call the callback
        m_Channel->setCallback( onFmodChannelCallback );

        if ( m_IsSpatial )
        {
            m_Channel->setMode( FMOD_3D );
            setSpatialAttributes();
        }
    }


    /// @brief  Stops the currently playing channel
    /// @note   FULLY STOPS the channel, doesn't just pause it
    void AudioPlayer::Stop()
    {
        if ( m_Channel == nullptr )
        {
            return;
        }
        m_Channel->stop();
        m_Channel = nullptr;
    }


    /// @brief  adds a callback function to be called when the sound completes
    /// @param  ownerId     the ID of the owner of the callback
    /// @param  callback    the function to be called when the sound completes
    /// @note   YOU MUST REMOVE THE CALLBACK USING THE CALLBACK HANDLE WHEN YOU ARE DONE WITH IT
    void AudioPlayer::AddOnSoundCompleteCallback( unsigned ownerId, std::function< void() > callback )
    {
        m_OnSoundCompleteCallbacks.emplace( ownerId, callback );
    }

    /// @brief  removes a callback function to be called when the sound completes
    /// @param  ownerId the ID of the owner of the callback to remove
    void AudioPlayer::RemoveOnSoundCompleteCallback( unsigned ownerId )
    {
        m_OnSoundCompleteCallbacks.erase( ownerId );
    }


//-----------------------------------------------------------------------------
// public: accessors
//-----------------------------------------------------------------------------


    /// @brief  gets the Sound that this AudioPlayer plays
    /// @return the Sound that this AudioPlayer plays
    Sound const* AudioPlayer::GetSound()
    {
        return m_Sound;
    }

    /// @brief  sets the SOund that this AudioPlayer plays
    /// @param  sound   the sound that this AudioPlayer will play
    void AudioPlayer::SetSound( Sound const* sound )
    {
        m_Sound = sound;
        m_SoundName = AssetLibrary< Sound >()->GetAssetName( sound );
    }


    /// @brief  gets whether this AudioPlayer is currently playing anything
    /// @return whether this AudioPlayer is currently playing anything
    bool AudioPlayer::GetIsPlaying() const
    {
        return m_Channel != nullptr;
    }


    /// @brief  gets the current time of the currently playing sound
    /// @return the current time of the currently playing sound
    float AudioPlayer::GetTime() const
    {
        if ( m_Channel == nullptr )
        {
            return 0.0f;
        }

        unsigned int time;
        m_Channel->getPosition( &time, FMOD_TIMEUNIT_MS );
        return (float)time / 1000;
    }

    /// @brief  sets the current time of the currently playing sound
    /// @param  time    the current time of the currently playing sound
    void AudioPlayer::SetTime( float time )
    {
        if ( m_Channel == nullptr )
        {
            return;
        }

        m_Channel->setPosition( (unsigned int)(time * 1000), FMOD_TIMEUNIT_MS );
    }


    /// @brief  gets whether this AudioPlayer is paused
    /// @return whether this AudioPlayer is paused
    bool AudioPlayer::GetIsPaused() const
    {

        if ( m_Channel == nullptr )
        {
            return true;
        }

        bool paused;
        m_Channel->getPaused( &paused );
        return paused;
    }

    /// @brief  Sets whether or not this AudioPlayer is paused
    /// @param  paused   whether to pause or unpause the AudioPlayer
    void AudioPlayer::SetIsPaused( bool paused )
    {
        if ( m_Channel == nullptr )
        {
            return;
        }
        m_Channel->setPaused( paused );
    }


    /// @brief  gets the volume of this AudioPlayer
    /// @return the volume of this AudioPlayer
    float AudioPlayer::GetVolume() const
    {
        return m_Volume;
    }

    /// @brief  sets the volume of this AudioPlayer
    /// @param  volume  the volume for this AudioPlayer
    void AudioPlayer::SetVolume( float _volume )
    {
        m_Volume = _volume;

        if ( m_Channel != nullptr )
        {
            m_Channel->setVolume( m_Volume );
        }
    }


    /// @brief  gets the pitch of this AudioPlayer
    /// @return the pitch of this AudioPlayer
    float AudioPlayer::GetPitch() const
    {
        return m_Pitch;
    }

    /// @brief  sets the pitch of this AudioPlayer
    /// @param  pitch  the pitch for this AudioPlayer
    void AudioPlayer::SetPitch( float pitch )
    {
        m_Pitch = pitch;

        if ( m_Channel != nullptr )
        {
            m_Channel->setPitch( m_Pitch );
        }
    }


    /// @brief  sets the pitch variance of this AudioPlayer
    /// @return the pitch variance of this AudioPlayer
    float AudioPlayer::GetPitchVariance() const
    {
        return m_PitchVariance;
    }

    /// @brief  gets the pitch varaince of this AudioPlayer
    /// @param  pitchVariance   the pitch varaince for this AudioPlayer
    void AudioPlayer::SetPitchVariance( float _pitchVariance )
    {
        m_PitchVariance = _pitchVariance;
    }


    /// @brief  sets the volume variance of this AudioPlayer
    /// @return the volume variance of this AudioPlayer
    float AudioPlayer::GetVolumeVariance() const
    {
        return m_VolumeVariance;
    }

    /// @brief  gets the volume varaince of this AudioPlayer
    /// @param  volumeVariance  the volume varaince for this AudioPlayer
    void AudioPlayer::SetVolumeVariance( float _volumeVariance )
    {
        m_VolumeVariance = _volumeVariance;
    }


    /// @brief  gets the default loop count of this AudioPlayer
    /// @return the default loop count of this AudioPlayer
    int AudioPlayer::GetDefaultLoopCount() const
    {
        return m_DefaultLoopCount;
    }

    /// @brief  sets the default loop count of this AudioPlayer
    /// @param  defaultLoopCount    the default loop count of this AudioPlayer
    void AudioPlayer::SetDefaultLoopCount( int defaultLoopCount )
    {
        m_DefaultLoopCount = defaultLoopCount;
    }


    /// @brief  gets the current loop count
    /// @return the current loop count
    int AudioPlayer::GetLoopCount() const
    {
        if ( m_Channel == nullptr )
        {
            return 0;
        }
        int loopCount;
        m_Channel->getLoopCount( &loopCount );
        return loopCount;
    }

    /// @brief  sets the current loop count
    /// @param  loopCount   the current loop count
    void AudioPlayer::SetLoopCount( int loopCount )
    {
        if ( m_Channel == nullptr )
        {
            return;
        }
        m_Channel->setLoopCount( loopCount );
    }


    /// @brief  sets whether the AudioPlayer is spatial
    /// @param  isSpatial   whether the AudioPlayer should be spatial
    void AudioPlayer::SetIsSpatial( bool isSpatial )
    {
        m_IsSpatial = isSpatial;
        if ( m_Channel == nullptr )
        {
            return;
        }

        if ( isSpatial )
        {
            m_Channel->setMode( FMOD_3D );
            setSpatialAttributes();
        }
        else
        {
            m_Channel->setMode( FMOD_2D );
        }
    }

//-----------------------------------------------------------------------------
// private: virtual override methods
//-----------------------------------------------------------------------------


    /// @brief  called once when entering the scene
    void AudioPlayer::OnInit()
    {
        if ( m_SoundName.empty() == false )
        {
            m_Sound = AssetLibrary< Sound >()->GetAsset( m_SoundName );
        }

        m_Transform = GetEntity()->GetComponent< Transform >();
        m_RigidBody = GetEntity()->GetComponent< RigidBody >();

        if ( m_PlayOnInit )
        {
            Play();
        }
    }

    /// @brief  called once when exiting the scene
    void AudioPlayer::OnExit()
    {
        Stop();
    }


    /// @brief  called once every graphics frame
    /// @param  dt  the duration of the frame in seconds
    void AudioPlayer::OnUpdate( float dt )
    {
        if ( m_IsSpatial && m_Channel != nullptr )
        {
            setSpatialAttributes();
        }
    }


//-----------------------------------------------------------------------------
// private: methods
//-----------------------------------------------------------------------------


    /// @brief  callback called when an fmod channel finishes playing
    /// @param  channelControl  the channel the callback is from
    /// @param  controlType     identifier to distinguish between channel and channelgroup
    /// @param  callbackType    the type of callback
    /// @param  commandData1    first callback parameter
    /// @param  commandData2    second callback parameter
    FMOD_RESULT F_CALLBACK AudioPlayer::onFmodChannelCallback(
        FMOD_CHANNELCONTROL* channelControl,
        FMOD_CHANNELCONTROL_TYPE controlType,
        FMOD_CHANNELCONTROL_CALLBACK_TYPE callbackType,
        void* commandData1,
        void* commandData2
    )
    {
        if (
            controlType != FMOD_CHANNELCONTROL_CHANNEL ||
            callbackType != FMOD_CHANNELCONTROL_CALLBACK_END
        ) // only handle callbacks for channels ending
        {
            return FMOD_OK;
        }

        // get the AudioPlayer that this channel started
        AudioPlayer* self;
        ((FMOD::Channel*)channelControl)->getUserData( (void**)&self );

        for ( auto& [ key, callback ] : self->m_OnSoundCompleteCallbacks )
        {
            callback();
        }

        self->m_Channel = nullptr;

        return FMOD_OK;
    }


    /// @brief  sets the spatial attributes of the current channel
    void AudioPlayer::setSpatialAttributes()
    {
        glm::vec2 pos2 = m_Transform != nullptr ? m_Transform->GetTranslation() : glm::vec2( 0.0f, 0.0f );
        glm::vec2 vel2 = m_RigidBody != nullptr ? m_RigidBody->GetVelocity() : glm::vec2( 0.0f, 0.0f );

        FMOD_VECTOR pos3 = { pos2.x, pos2.y, 0.0f };
        FMOD_VECTOR vel3 = { vel2.x, vel2.y, 0.0f };

        m_Channel->set3DAttributes( &pos3, &vel3 );
    }


//-----------------------------------------------------------------------------
// public: inspection
//-----------------------------------------------------------------------------


    /// @brief  shows the inspector for AudioPlayer
    void AudioPlayer::Inspector()
    {

        Inspection::SelectAssetFromLibrary< Sound >( "sound", &m_Sound, &m_SoundName );

        bool paused = GetIsPaused();
        if ( ImGui::Checkbox( "Paused", &paused ) )
        {
            SetIsPaused( paused );
        }

        if ( ImGui::Checkbox( "is spatial", &m_IsSpatial ) )
        {
            SetIsSpatial( m_IsSpatial );
        }

        ImGui::InputInt( "Default Loop Count", &m_DefaultLoopCount, 1, 5 );

        int currentLoopCount = GetLoopCount();
        if ( ImGui::InputInt( "Current Loop Count", &currentLoopCount, 1, 5 ) )
        {
            SetLoopCount( currentLoopCount );
        }

        if ( ImGui::DragFloat( "Volume", &m_Volume, 0.1f, 0.0f, 100.0f, "%.3f", ImGuiSliderFlags_Logarithmic ) )
        {
            SetVolume( m_Volume );
        }

        ImGui::DragFloat( "Volume Variance", &m_VolumeVariance, 0.01f, 0.0f, 1.0f );

        if ( ImGui::DragFloat( "Pitch", &m_Pitch, 0.1f, 0.0f, 100.0f, "%.3f", ImGuiSliderFlags_Logarithmic ) )
        {
            SetPitch( m_Pitch );
        }

        ImGui::DragFloat( "Pitch Variance", &m_PitchVariance, 0.01f, 0.0f, 1.0f );

        if ( ImGui::Button( "Play" ) )
        {
            Play();
        }

        if ( ImGui::Button( "Stop" ) )
        {
            Stop();
        }

    }


//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------


    /// @brief  read the sound of this component from json
    /// @param  data    the json data
    void AudioPlayer::readSound( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_SoundName, data );
    }

    /// @brief  read the volume of this component from json
    /// @param  data    the json data
    void AudioPlayer::readVolume( nlohmann::ordered_json const& data )
    {
        m_Volume = Stream::Read<float>(data);
    }

    /// @brief  read the pitch of this component from json
    /// @param  data    the json data
    void AudioPlayer::readPitch( nlohmann::ordered_json const& data )
    {
        m_Pitch = Stream::Read<float>(data);
    }

    /// @brief  read the volumeVariance of this component from json
    /// @param  data    the json data
    void AudioPlayer::readVolumeVariance( nlohmann::ordered_json const& data )
    {
        m_VolumeVariance = Stream::Read<float>(data);
    }

    /// @brief  read the pitchVariance of this component from json
    /// @param  data    the json data
    void AudioPlayer::readPitchVariance( nlohmann::ordered_json const& data )
    {
        m_PitchVariance = Stream::Read<float>(data);
    }

    /// @brief  read DefaultLoopCount of this component from json
    /// @param  data    the json data
    void AudioPlayer::readDefaultLoopCount( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_DefaultLoopCount, data );
    }

    /// @brief  read PlayOnInit of this component from json
    /// @param  data    the json data
    void AudioPlayer::readPlayOnInit( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_PlayOnInit, data );
    }


    /// @brief  read IsSpatial of this component from json
    /// @param  data    the json data
    void AudioPlayer::readIsSpatial( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_IsSpatial, data );
    }



//-----------------------------------------------------------------------------
// public: reading / writing
//-----------------------------------------------------------------------------

    
    /// @brief  gets the map of read methods for this AudioPlayer
    /// @return the map of read methods for this AudioPlayer
    ReadMethodMap< ISerializable > const& AudioPlayer::GetReadMethods() const
    {
        static ReadMethodMap< AudioPlayer > const readMethods = {
            { "Sound"           , &AudioPlayer::readSound            },
            { "Volume"          , &AudioPlayer::readVolume           },
            { "Pitch"           , &AudioPlayer::readPitch            },
            { "VolumeVariance"  , &AudioPlayer::readVolumeVariance   },
            { "PitchVariance"   , &AudioPlayer::readPitchVariance    },
            { "DefaultLoopCount", &AudioPlayer::readDefaultLoopCount },
            { "PlayOnInit"      , &AudioPlayer::readPlayOnInit       },
            { "IsSpatial"       , &AudioPlayer::readIsSpatial        }
        };

        return (ReadMethodMap< ISerializable > const&)readMethods;
    }


    /// @brief  Writes all AudioPlayr data to a JSON file.
    /// @return The JSON file containing the data.
    nlohmann::ordered_json AudioPlayer::Write() const
    {
        nlohmann::ordered_json data;

        data[ "Sound"            ] = Stream::Write( m_SoundName        );
        data[ "Volume"           ] = Stream::Write( m_Volume           );
        data[ "Pitch"            ] = Stream::Write( m_Pitch            );
        data[ "VolumeVariance"   ] = Stream::Write( m_VolumeVariance   );
        data[ "PitchVariance"    ] = Stream::Write( m_PitchVariance    );
        data[ "DefaultLoopCount" ] = Stream::Write( m_DefaultLoopCount );
        data[ "PlayOnInit"       ] = Stream::Write( m_PlayOnInit       );
        data[ "IsSpatial"        ] = Stream::Write( m_IsSpatial        );

        return data;
    }


//-----------------------------------------------------------------------------
// private: cloning
//-----------------------------------------------------------------------------

    
    /// @brief  clones this AudioPlayer
    /// @return the newly created clone of this AudioPlayer
    AudioPlayer* AudioPlayer::Clone() const
    {
        return new AudioPlayer( *this );
    }


//-----------------------------------------------------------------------------
// private: cloning
//-----------------------------------------------------------------------------


    /// @brief  copy-constructor for the AudioPlayer
    /// @param  other   the other AudioPlayer to copy
    AudioPlayer::AudioPlayer( AudioPlayer const& other ) :
        Behavior( other ),
        m_Sound         ( other.m_Sound          ),
        m_SoundName     ( other.m_SoundName      ),
        m_ChannelGroup  ( other.m_ChannelGroup   ),
        m_Volume        ( other.m_Volume         ),
        m_Pitch         ( other.m_Pitch          ),
        m_VolumeVariance( other.m_VolumeVariance ),
        m_PitchVariance ( other.m_PitchVariance  ),
        m_IsSpatial     ( other.m_IsSpatial      )
    {}


//-----------------------------------------------------------------------------
