/// @file AudioPlayer.cpp
/// @author Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief Component that plays audio
/// @version 0.1
/// @date 2023-09-13
/// 
/// @copyright Copyright (c) 2023

#pragma once

#include "AudioPlayer.h"

#include <cstdlib>

//-----------------------------------------------------------------------------
// public: constructor / Destructor
//-----------------------------------------------------------------------------

    /// @brief constructs a new AudioPlayer
    AudioPlayer::AudioPlayer() :
        Component( typeid( AudioPlayer ) ),
        sound( nullptr ),
        channel( nullptr ),
        channelGroup( nullptr ),
        volume( 1.0f ),
        pitch( 1.0f ),
        volumeVariance( 0.0f ),
        pitchVariance( 0.0f )
    {}

//-----------------------------------------------------------------------------
// public: methods
//-----------------------------------------------------------------------------

    /// @brief gets the Sound that this AudioPlayer plays
    /// @return the Sound that this AudioPlayer plays
    Sound* AudioPlayer::getSound()
    {
        return sound;
    }
    /// @brief sets the SOund that this AudioPlayer plays
    /// @param sound the sound that this AudioPlayer will play
    void AudioPlayer::setSound( Sound* _sound )
    {
        sound = _sound;
    }

    /// @brief gets whether this AudioPlayer is currently playing anything
    /// @return whether this AudioPlayer is currently playing anything
    bool AudioPlayer::isPlaying() const
    {
        bool isPlaying;
        channel->isPlaying( &isPlaying );
        return isPlaying;
    }

    /// @brief Starts playing this AudioPlayer's sound
    void AudioPlayer::Play()
    {
        channel = sound->Play(
            channelGroup,
            random( volume - volumeVariance, volume + volumeVariance ),
            random( pitch - pitchVariance, pitch + pitchVariance )
        );
    }

    /// @brief gets whether this AudioPlayer is paused
    /// @return whether this AudioPlayer is paused
    bool AudioPlayer::isPaused() const
    {
        bool paused;
        channel->getPaused( &paused );
        return paused;
    }
    /// @brief Sets whether or not this AudioPlayer is paused
    /// @param paused 
    void AudioPlayer::setPaused( bool paused )
    {
        channel->setPaused( paused );
    }

    /// @brief gets the volume of this AudioPlayer
    /// @return the volume of this AudioPlayer
    float AudioPlayer::getVolume() const
    {
        return volume;
    }
    /// @brief sets the volume of this AudioPlayer
    /// @param volume the volume for this AudioPlayer
    void AudioPlayer::setVolume( float _volume )
    {
        volume = _volume;
    }

    /// @brief sets the pitch variance of this AudioPlayer
    /// @return the pitch variance of this AudioPlayer
    float AudioPlayer::getPitchVariance() const
    {
        return pitchVariance;
    }
    /// @brief gets the pitch varaince of this AudioPlayer
    /// @param pitchVariance the pitch varaince for this AudioPlayer
    void AudioPlayer::setPitchVariance( float _pitchVariance )
    {
        pitchVariance = _pitchVariance;
    }

    /// @brief sets the volume variance of this AudioPlayer
    /// @return the volume variance of this AudioPlayer
    float AudioPlayer::getVolumeVariance() const
    {
        return volumeVariance;
    }
    /// @brief gets the volume varaince of this AudioPlayer
    /// @param volumeVariance the volume varaince for this AudioPlayer
    void AudioPlayer::setVolumeVariance( float _volumeVariance )
    {
        volumeVariance = _volumeVariance;
    }

//-----------------------------------------------------------------------------
// private: cloning
//-----------------------------------------------------------------------------

    /// @brief copy-constructor for the AudioPlayer
    /// @param other the other AudioPlayer to copy
    AudioPlayer::AudioPlayer( const AudioPlayer& other ) :
        Component( other ),
        sound( other.sound ),
        channelGroup( other.channelGroup ),
        volume( other.volume ),
        pitch( other.pitch ),
        volumeVariance( other.volumeVariance ),
        pitchVariance( other.pitchVariance )
    {}

    /// @brief clones this AudioPlayer
    /// @return the newly created clone of this AudioPlayer
    Component* AudioPlayer::Clone() const
    {
        return new AudioPlayer( *this );
    }

//-----------------------------------------------------------------------------
