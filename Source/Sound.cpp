/// @file Sound.cpp
/// @author Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief Wrapper class for FMOD sounds
/// @version 0.1
/// @date 2023-09-13
/// 
/// @copyright Copyright (c) 2023

#include "Sound.h"
#include "AudioSystem.h"

//-----------------------------------------------------------------------------
// constructor / destructor
//-----------------------------------------------------------------------------

    /// @brief Constructs a new Sound
    /// @param filepath the filepath of the sound to load
    /// @param looping whether or not the sound should loop
    Sound::Sound( char const* filepath, bool looping )
    {
        AudioSystem::getInstance()->getFMOD()->createSound(
            filepath,
            looping ? FMOD_LOOP_NORMAL : FMOD_DEFAULT,
            nullptr,
            &sound
        );
    }

    /// @brief Destroys this Sound
    Sound::~Sound()
    {
        sound->release();
    }

//-----------------------------------------------------------------------------
// public methods
//-----------------------------------------------------------------------------

    /// @brief Plays this sound
    /// @param volume the relative volume to play this sound at
    /// @param pitch the relative pitch to play this sound at
    /// @return The channel that this sound is being played on
    FMOD::Channel* Sound::Play(
        FMOD::ChannelGroup* group,
        float volume,
        float pitch
    ) const
    {
        FMOD::Channel* channel;
        AudioSystem::getInstance()->getFMOD()->playSound(
            sound,
            group,
            true,
            &channel
        );

        channel->setVolume( volume );
        channel->setPitch( pitch );

        channel->setPaused( false );

        return channel;
    }


    /// @brief gets the length of this sound
    /// @return the length of this sound in seconds
    float Sound::getLength() const
    {
        unsigned int length;
        sound->getLength( &length, FMOD_TIMEUNIT_MS );
        return length / 1000.0f;
    }

//-----------------------------------------------------------------------------
