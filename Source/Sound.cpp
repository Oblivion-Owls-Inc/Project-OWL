/// @file       Sound.cpp
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      Wrapper class for FMOD sounds
/// @version    0.1
/// @date       2023-09-13
/// 
/// @copyright  Copyright (c) 2023 Digipen Institute of Technology

#include "Sound.h"
#include "AudioSystem.h"

//-----------------------------------------------------------------------------
// constructor / destructor
//-----------------------------------------------------------------------------

    /// @brief default constructor
    Sound::Sound() :
        m_Filepath(),
        m_IsLooping( false ),
        m_Sound( nullptr )
    {}

    /// @brief  constructs a new Sound
    /// @param  filepath    the filepath of the sound to load
    /// @param  looping     whether or not the sound should loop
    Sound::Sound( char const* filepath, bool looping )
    {
        AudioSystem::GetInstance()->GetFMOD()->createSound(
            filepath,
            looping ? FMOD_LOOP_NORMAL : FMOD_DEFAULT,
            nullptr,
            &m_Sound
        );
    }

    /// @brief  Destroys this Sound
    Sound::~Sound()
    {
        m_Sound->release();
    }

//-----------------------------------------------------------------------------
// public: methods
//-----------------------------------------------------------------------------

    /// @brief  Plays this sound
    /// @param  volume  the relative volume to play this sound at
    /// @param  pitch   the relative pitch to play this sound at
    /// @return The channel that this sound is being played on
    FMOD::Channel* Sound::Play(
        FMOD::ChannelGroup* group,
        float volume,
        float pitch
    ) const
    {
        FMOD::Channel* channel;
        AudioSystem::GetInstance()->GetFMOD()->playSound(
            m_Sound,
            group,
            true,
            &channel
        );

        channel->setVolume( volume );
        channel->setPitch( pitch );

        channel->setPaused( false );

        return channel;
    }

//-----------------------------------------------------------------------------
// public: accessors
//-----------------------------------------------------------------------------

    /// @brief  gets the length of this sound
    /// @return the length of this sound in seconds
    float Sound::GetLength() const
    {
        unsigned int length;
        m_Sound->getLength( &length, FMOD_TIMEUNIT_MS );
        return length / 1000.0f;
    }

    void Sound::Inspect()
    {
        ///todo: add inspector
    }

//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------

    /// @brief  reads filepath
    /// @param  data    the JSON data to read from
    void Sound::readFilepath( nlohmann::json const& data )
    {
        m_Filepath = Stream::Read<std::string>( data );
    }

    /// @brief  reads isLooping
    /// @param  data    the JSON data to read from
    void Sound::readIsLooping( nlohmann::json const& data )
    {
        m_IsLooping = Stream::Read<bool>( data );
    }

    /// @brief  runs after Sound has been loaded 
    void Sound::AfterLoad()
    {
        if ( m_Sound != nullptr )
        {
            m_Sound->release();
            m_Sound = nullptr;
        }

        AudioSystem::GetInstance()->GetFMOD()->createSound(
            m_Filepath.c_str(),
            m_IsLooping ? FMOD_LOOP_NORMAL : FMOD_DEFAULT,
            nullptr,
            &m_Sound
        );
    }

    /// @brief  map of the SceneSystem read methods
    ReadMethodMap< Sound > const Sound::s_ReadMethods = {
        { "IsLooping", &readIsLooping },
        { "Filepath" , &readFilepath  }
    };


//-----------------------------------------------------------------------------
