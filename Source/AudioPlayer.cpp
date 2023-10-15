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
    Sound const* AudioPlayer::GetSound()
    {
        return m_Sound;
    }
    /// @brief  sets the SOund that this AudioPlayer plays
    /// @param  sound   the sound that this AudioPlayer will play
    void AudioPlayer::SetSound( Sound const* _sound )
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

    /// @brief  read the sound of this component from json
    /// @param  data    the json data
    void AudioPlayer::readSound( nlohmann::ordered_json const& data )
    {
        m_Sound = AssetLibrary<Sound>()->GetAsset( Stream::Read<std::string>(data) );
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

    /// @brief  Writes all AudioPlayr data to a JSON file.
    /// @return The JSON file containing the data.
    nlohmann::ordered_json AudioPlayer::Write() const
    {
        nlohmann::ordered_json data;
    
        data["Volume"] = m_Volume;
        data["Pitch"] = m_Pitch;
        data["VolumeVariance"] = m_VolumeVariance;
        data["PitchVariance"] = m_PitchVariance;

        std::string const& name = AssetLibrary<Sound>()->GetAssetName( m_Sound );
        if (!name.empty())
        {
            data["Sound"] = name;
        }

        return data;
    }

    /// @brief  map of the read methods for this Component
    ReadMethodMap< AudioPlayer > AudioPlayer::s_ReadMethods = {
        { "Sound",          &readSound          },
        { "Volume",         &readVolume         },
        { "Pitch",          &readPitch          },
        { "VolumeVariance", &readVolumeVariance },
        { "PitchVariance",  &readPitchVariance  }
    };

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
