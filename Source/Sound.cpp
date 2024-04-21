/// @file       Sound.cpp
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      Wrapper class for FMOD sounds
/// @version    0.1
/// @date       2023-09-13
/// 
/// @copyright  Copyright (c) 2024 Digipen Institute of Technology



#include "pch.h" // precompiled header has to be included first
#include "Sound.h"
#include "AudioSystem.h"

#include "Inspection.h"

//-----------------------------------------------------------------------------
// constructor / destructor
//-----------------------------------------------------------------------------

    /// @brief  constructs a new Sound
    /// @param  filepath    the filepath of the sound to load
    /// @param  isLoopable  whether or not the sound should loop
    Sound::Sound( char const* filepath, bool isLoopable ) :
        m_Filepath  ( filepath   ),
        m_IsLoopable( isLoopable )
    {
        Reload();
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
        float pitch,
        int loopCount
    ) const
    {
        if ( m_Sound == nullptr )
        {
            Debug() << "Warning: Sound had no audio data attached, could not be played" << std::endl;
            return nullptr;
        }

        FMOD::Channel* channel;
        AudioSystem::GetInstance()->GetFMOD()->playSound(
            m_Sound,
            group,
            true,
            &channel
        );

        channel->setVolume( volume * m_Volume );
        channel->setPitch( pitch );
        channel->setLoopCount( loopCount );

        channel->setPaused( false );

        return channel;
    }


    /// @brief  reloads this Sound using current settings
    void Sound::Reload()
    {
        if ( m_Sound != nullptr )
        {
            m_Sound->release();
            m_Sound = nullptr;
        }

        AudioSystem::GetInstance()->GetFMOD()->createSound(
            m_Filepath.c_str(),
            m_IsLoopable ? FMOD_LOOP_NORMAL : FMOD_DEFAULT,
            nullptr,
            &m_Sound
        );
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

    /// @brief  gets the volume of this sound
    /// @return the volume of this sound
    float Sound::GetVolume() const
    {
        return m_Volume;
    }

//-----------------------------------------------------------------------------
// inspector
//-----------------------------------------------------------------------------


    /// @brief Used by the Debug System to display information about this Sound
    void Sound::Inspect()
    {
        std::string const soundDirectory = "Data/Sounds";


        Inspection::SelectFileFromDirectory( "Filepath", &m_Filepath, soundDirectory );

        ImGui::Checkbox( "Loopable", &m_IsLoopable );

        ImGui::DragFloat( "volume", &m_Volume, 0.05f, 0.0f, INFINITY );

        if ( ImGui::Button( "Reload Sound" ) )
        {
            Reload();
        }

    }


//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------


    /// @brief  reads filepath
    /// @param  data    the JSON data to read from
    void Sound::readFilepath( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_Filepath, data );
    }

    /// @brief  reads IsLoopabe
    /// @param  data    the JSON data to read from
    void Sound::readIsLoopable( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_IsLoopable, data );
    }

    /// @brief  reads the default volume of this sound
    /// @param  data    the JSON data to read from
    void Sound::readVolume( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_Volume, data );
    }

    /// @brief  runs after Sound has been loaded 
    void Sound::AfterLoad()
    {
        Reload();
    }

    /// @brief Write all Sound data to a JSON file.
    /// @return The JSON data containing the Sound data.
    nlohmann::ordered_json Sound::Write() const
    {
        nlohmann::ordered_json data;

        data[ "IsLoopable" ] = Stream::Write( m_IsLoopable );
        data[ "Filepath"   ] = Stream::Write( m_Filepath   );
        data[ "Volume"     ] = Stream::Write( m_Volume     );

        return data;
    }

    /// @brief  map of the SceneSystem read methods
    ReadMethodMap< Sound > const Sound::s_ReadMethods = {
        { "IsLoopable", &readIsLoopable },
        { "Filepath"  , &readFilepath   },
        { "Volume"    , &readVolume     }
    };


//-----------------------------------------------------------------------------
