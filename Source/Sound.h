/// @file Sound.h
/// @author Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief Wrapper class for FMOD sounds
/// @version 0.1
/// @date 2023-09-13
/// 
/// @copyright Copyright (c) 2023

#pragma once

#include <fmod.hpp>

/// @brief Example System meant to be copy-pasted when creating new Systems
class Sound
{
private: // member variables

    /// @brief The actual FMOD::Sound
    FMOD::Sound* sound;

public: // constructor / destructor

    /// @brief Constructs a new Sound
    /// @param filepath the filepath of the sound to load
    /// @param looping whether or not the sound should loop
    Sound( char const* filepath, bool looping );

    /// @brief Destroys this Sound
    ~Sound();

public: // methods

    /// @brief Plays this sound
    /// @param volume the relative volume to play this sound at
    /// @param pitch the relative pitch to play this sound at
    /// @return The channel that this sound is being played on
    FMOD::Channel* Play(
        FMOD::ChannelGroup* group = nullptr,
        float volume = 1.0f,
        float pitch = 1.0f
    ) const;

    /// @brief gets the length of this sound
    /// @return the length of this sound in seconds
    float GetLength() const;

};
