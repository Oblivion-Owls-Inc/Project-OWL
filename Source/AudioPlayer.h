/// @file AudioPlayer.h
/// @author Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief Component that plays audio
/// @version 0.1
/// @date 2023-09-13
/// 
/// @copyright Copyright (c) 2023

#pragma once

#include "Component.h"
#include "basics.h"

#include "Sound.h"

/// @brief Component that can play audio
class AudioPlayer : public Component
{
public: // constructor / Destructor

    /// @brief constructs a new AudioPlayer
    AudioPlayer();

    /// @brief destroys this AudioPlayer
    ~AudioPlayer();

public: // methods

    /// @brief gets the Sound that this AudioPlayer plays
    /// @return the Sound that this AudioPlayer plays
    Sound* getSound();
    /// @brief sets the SOund that this AudioPlayer plays
    /// @param sound the sound that this AudioPlayer will play
    void setSound( Sound* sound );

    /// @brief gets whether this AudioPlayer is currently playing anything
    /// @return whether this AudioPlayer is currently playing anything
    bool isPlaying() const;

    /// @brief Starts playing this AudioPlayer's sound
    void Play();

    /// @brief gets whether this AudioPlayer is paused
    /// @return whether this AudioPlayer is paused
    bool isPaused() const;
    /// @brief Sets whether or not this AudioPlayer is paused
    /// @param paused 
    void setPaused( bool paused );

    /// @brief gets the volume of this AudioPlayer
    /// @return the volume of this AudioPlayer
    float getVolume() const;
    /// @brief sets the volume of this AudioPlayer
    /// @param volume the volume for this AudioPlayer
    void setVolume( float volume );

    /// @brief sets the pitch variance of this AudioPlayer
    /// @return the pitch variance of this AudioPlayer
    float getPitchVariance() const;
    /// @brief gets the pitch varaince of this AudioPlayer
    /// @param pitchVariance the pitch varaince for this AudioPlayer
    void setPitchVariance( float pitchVariance );

    /// @brief sets the volume variance of this AudioPlayer
    /// @return the volume variance of this AudioPlayer
    float getVolumeVariance() const;
    /// @brief gets the volume varaince of this AudioPlayer
    /// @param volumeVariance the volume varaince for this AudioPlayer
    void setVolumeVariance( float volumeVariance );

private: // members

    /// @brief the relative volume this AudioPlayer will play at
    float volume;
    /// @brief the pitch this AudioPlayer will play at
    float pitch;
    /// @brief the maximum variation of the volume
    float volumeVariance;
    /// @brief the maxumum variation of the pitch
    float pitchVariance;

    /// @brief The sound that this AudioPlayer will play
    Sound* sound;

    /// @brief The channel currently being used by this AudioPlayer
    FMOD::Channel* channel;

    /// @brief The channelGroup to play sounds in
    FMOD::ChannelGroup* channelGroup;

private: // copying

    /// @brief copy-constructor for the AudioPlayer
    /// @param other the other AudioPlayer to copy
    AudioPlayer( const AudioPlayer& other );

    /// @brief clones this AudioPlayer
    /// @return the newly created clone of this AudioPlayer
    virtual Component* Clone() const override;

public:

    // diable = operator
    void operator=( const AudioPlayer& ) = delete;

};

