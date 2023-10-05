/// @file       AudioPlayer.h
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      Component that plays audio
/// @version    0.1
/// @date       2023-09-13
/// 
/// @copyright  Copyright (c) 2023 Digipen Institute of Technology

#pragma once

#include "Component.h"
#include "basics.h"

#include "Sound.h"

/// @brief  Component that can play audio
class AudioPlayer : public Component
{
//-----------------------------------------------------------------------------
public: // constructor / Destructor
//-----------------------------------------------------------------------------

    /// @brief  constructs a new AudioPlayer
    AudioPlayer();

    /// @brief  destroys this AudioPlayer
    ~AudioPlayer() = default;

//-----------------------------------------------------------------------------
public: // methods
//-----------------------------------------------------------------------------

    /// @brief  Starts playing this AudioPlayer's sound
    void Play();

//-----------------------------------------------------------------------------
public: // accessors
//-----------------------------------------------------------------------------

    /// @brief  gets the Sound that this AudioPlayer plays
    /// @return the Sound that this AudioPlayer plays
    Sound const* GetSound();
    /// @brief  sets the SOund that this AudioPlayer plays
    /// @param  sound   the sound that this AudioPlayer will play
    void SetSound( Sound const* sound );

    /// @brief  gets whether this AudioPlayer is currently playing anything
    /// @return whether this AudioPlayer is currently playing anything
    bool IsPlaying() const;

    /// @brief  gets whether this AudioPlayer is paused
    /// @return whether this AudioPlayer is paused
    bool IsPaused() const;
    /// @brief  Sets whether or not this AudioPlayer is paused
    /// @param  paused   whether to pause or unpause the AudioPlayer
    void SetPaused( bool paused );

    /// @brief  gets the volume of this AudioPlayer
    /// @return the volume of this AudioPlayer
    float GetVolume() const;
    /// @brief  sets the volume of this AudioPlayer
    /// @param  volume  the volume for this AudioPlayer
    void SetVolume( float volume );

    /// @brief  sets the pitch variance of this AudioPlayer
    /// @return the pitch variance of this AudioPlayer
    float GetPitchVariance() const;
    /// @brief  gets the pitch varaince of this AudioPlayer
    /// @param  pitchVariance   the pitch varaince for this AudioPlayer
    void SetPitchVariance( float pitchVariance );

    /// @brief  sets the volume variance of this AudioPlayer
    /// @return the volume variance of this AudioPlayer
    float GetVolumeVariance() const;
    /// @brief  gets the volume varaince of this AudioPlayer
    /// @param  volumeVariance  the volume varaince for this AudioPlayer
    void SetVolumeVariance( float volumeVariance );

//-----------------------------------------------------------------------------
private: // members
//-----------------------------------------------------------------------------

    /// @brief  the relative volume this AudioPlayer will play at
    float m_Volume;
    /// @brief  the pitch this AudioPlayer will play at
    float m_Pitch;
    /// @brief  the maximum variation of the volume
    float m_VolumeVariance;
    /// @brief  the maxumum variation of the pitch
    float m_PitchVariance;

    /// @brief  The sound that this AudioPlayer will play
    Sound const* m_Sound;

    /// @brief  The channel currently being used by this AudioPlayer
    FMOD::Channel* m_Channel;

    /// @brief  The channelGroup to play sounds in
    FMOD::ChannelGroup* m_ChannelGroup;

//-----------------------------------------------------------------------------
private: // reading
//-----------------------------------------------------------------------------

    /// @brief  read the sound of this component from json
    /// @param  data    the json data
    void readSound( Stream data );

    /// @brief  read the volume of this component from json
    /// @param  data    the json data
    void readVolume( Stream data );

    /// @brief  read the pitch of this component from json
    /// @param  data    the json data
    void readPitch( Stream data );

    /// @brief  read the volumeVariance of this component from json
    /// @param  data    the json data
    void readVolumeVariance( Stream data );

    /// @brief  read the pitchVariance of this component from json
    /// @param  data    the json data
    void readPitchVariance( Stream data );

    /// @brief  map of the read methods for this Component
    static ReadMethodMap< AudioPlayer > s_ReadMethods;

    /// @brief  gets the map of read methods for this Component
    /// @return the map of read methods for this Component
    virtual ReadMethodMap< Component > const& GetReadMethods() const override;

//-----------------------------------------------------------------------------
private: // copying
//-----------------------------------------------------------------------------

    /// @brief  copy-constructor for the AudioPlayer
    /// @param  other   the other AudioPlayer to copy
    AudioPlayer( const AudioPlayer& other );

    /// @brief  clones this AudioPlayer
    /// @return the newly created clone of this AudioPlayer
    virtual Component* Clone() const override;

//-----------------------------------------------------------------------------
public: // copying
//-----------------------------------------------------------------------------

    // diable = operator
    void operator=( const AudioPlayer& ) = delete;

//-----------------------------------------------------------------------------

};
