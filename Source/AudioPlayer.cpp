/// @file       AudioPlayer.cpp
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      Component that plays audio
/// @version    0.1
/// @date       2023-09-13
/// 
/// @copyright  Copyright (c) 2023 Digipen Institute of Technology

#pragma once

#include "AudioPlayer.h"

//-----------------------------------------------------------------------------
// public: constructor / Destructor
//-----------------------------------------------------------------------------

    /// @brief  constructs a new AudioPlayer
    AudioPlayer::AudioPlayer() :
        Component( typeid( AudioPlayer ) ),
        m_Sound( nullptr ),
        m_Channel( nullptr ),
        m_ChannelGroup( nullptr ),
        m_Volume( 1.0f ),
        m_Pitch( 1.0f ),
        m_VolumeVariance( 0.0f ),
        m_PitchVariance( 0.0f )
    {}

//-----------------------------------------------------------------------------
// public: methods
//-----------------------------------------------------------------------------

    /// @brief  Starts playing this AudioPlayer's sound
    void AudioPlayer::Play()
    {
        m_Channel = m_Sound->Play(
            m_ChannelGroup,
            random( m_Volume - m_VolumeVariance, m_Volume + m_VolumeVariance ),
            random( m_Pitch - m_PitchVariance, m_Pitch + m_PitchVariance )
        );
    }

//-----------------------------------------------------------------------------
// public: accessors
//-----------------------------------------------------------------------------

    /// @brief  gets the Sound that this AudioPlayer plays
    /// @return the Sound that this AudioPlayer plays
    Sound* AudioPlayer::GetSound()
    {
        return m_Sound;
    }
    /// @brief  sets the SOund that this AudioPlayer plays
    /// @param  sound   the sound that this AudioPlayer will play
    void AudioPlayer::SetSound( Sound* _sound )
    {
        m_Sound = _sound;
    }

    /// @brief  gets whether this AudioPlayer is currently playing anything
    /// @return whether this AudioPlayer is currently playing anything
    bool AudioPlayer::IsPlaying() const
    {
        bool isPlaying;
        m_Channel->isPlaying( &isPlaying );
        return isPlaying;
    }

    /// @brief  gets whether this AudioPlayer is paused
    /// @return whether this AudioPlayer is paused
    bool AudioPlayer::IsPaused() const
    {
        bool paused;
        m_Channel->getPaused( &paused );
        return paused;
    }
    /// @brief  Sets whether or not this AudioPlayer is paused
    /// @param  paused   whether to pause or unpause the AudioPlayer
    void AudioPlayer::SetPaused( bool paused )
    {
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

//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------

    /// @brief  read the volume of this component from json
    /// @param  data    the json data
    void AudioPlayer::readVolume( Stream data )
    {
        m_Volume = data.Read<float>();
    }

    /// @brief  read the pitch of this component from json
    /// @param  data    the json data
    void AudioPlayer::readPitch( Stream data )
    {
        m_Pitch = data.Read<float>();
    }

    /// @brief  read the volumeVariance of this component from json
    /// @param  data    the json data
    void AudioPlayer::readVolumeVariance( Stream data )
    {
        m_VolumeVariance = data.Read<float>();
    }

    /// @brief  read the pitchVariance of this component from json
    /// @param  data    the json data
    void AudioPlayer::readPitchVariance( Stream data )
    {
        m_PitchVariance = data.Read<float>();
    }

    /// @brief  map of the read methods for this Component
    ReadMethodMap< AudioPlayer > AudioPlayer::s_ReadMethods = {
        { "volume",         &readVolume         },
        { "pitch",          &readPitch          },
        { "volumeVariance", &readVolumeVariance },
        { "pitchVariance",  &readPitchVariance  }
    };

    /// @brief  gets the map of read methods for this Component
    /// @return the map of read methods for this Component
    ReadMethodMap< Component > const& AudioPlayer::GetReadMethods() const
    {
        return (ReadMethodMap< Component > const&)s_ReadMethods;
    }

//-----------------------------------------------------------------------------
// private: cloning
//-----------------------------------------------------------------------------

    /// @brief  copy-constructor for the AudioPlayer
    /// @param  other   the other AudioPlayer to copy
    AudioPlayer::AudioPlayer( const AudioPlayer& other ) :
        Component( other ),
        m_Sound( other.m_Sound ),
        m_ChannelGroup( other.m_ChannelGroup ),
        m_Volume( other.m_Volume ),
        m_Pitch( other.m_Pitch ),
        m_VolumeVariance( other.m_VolumeVariance ),
        m_PitchVariance( other.m_PitchVariance )
    {}

    /// @brief  clones this AudioPlayer
    /// @return the newly created clone of this AudioPlayer
    Component* AudioPlayer::Clone() const
    {
        return new AudioPlayer( *this );
    }

//-----------------------------------------------------------------------------
