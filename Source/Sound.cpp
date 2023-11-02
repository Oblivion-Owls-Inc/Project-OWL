/// @file       Sound.cpp
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      Wrapper class for FMOD sounds
/// @version    0.1
/// @date       2023-09-13
/// 
/// @copyright  Copyright (c) 2023 Digipen Institute of Technology

#include "Sound.h"
#include "AudioSystem.h"

#include "Inspection.h"

#include <imgui.h>

//-----------------------------------------------------------------------------
// constructor / destructor
//-----------------------------------------------------------------------------

    /// @brief  constructs a new Sound
    /// @param  filepath    the filepath of the sound to load
    /// @param  isLoopable  whether or not the sound should loop
    Sound::Sound( char const* filepath, bool isLoopable ) :
        m_Filepath( filepath ),
        m_IsLoopabe( isLoopable )
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
        FMOD::Channel* channel;
        AudioSystem::GetInstance()->GetFMOD()->playSound(
            m_Sound,
            group,
            true,
            &channel
        );

        channel->setVolume( volume );
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
            m_IsLoopabe ? FMOD_LOOP_NORMAL : FMOD_DEFAULT,
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

//-----------------------------------------------------------------------------
// inspector
//-----------------------------------------------------------------------------


    /// @brief Used by the Debug System to display information about this Sound
    void Sound::Inspect()
    {
        std::string const soundDirectory = "Data/Sounds";


        Inspection::SelectFileFromDirectory( "Filepath", &m_Filepath, soundDirectory );

        ImGui::Checkbox( "Loopable", &m_IsLoopabe );

        if ( ImGui::Button( "Reload Sound" ) )
        {
            Reload();
        }

    }


    // /// @brief  selects a filepath from a directory
    // void Sound::inspectorSelectFilepathFromDirectory( char const* directoryPath )
    // {
    //     for ( auto const& file : std::filesystem::directory_iterator( directoryPath ) )
    //     {
    //         std::string filepath = file.path().filename().string();
    //         if ( file.is_directory() )
    //         {
    //             if ( ImGui::TreeNode( filepath.c_str() ) )
    //             {
    //                 inspectorSelectFilepathFromDirectory( (directoryPath + ("/" + filepath)).c_str() );
    //                 ImGui::TreePop();
    //             }
    //         }
    //         else
    //         {
    //             if ( ImGui::Selectable( filepath.c_str(), filepath == m_Filepath ) )
    //             {
    //                 m_Filepath = directoryPath + ("/" + filepath);
    //             }
    //         }
    //     }
    // }


//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------

    /// @brief  reads filepath
    /// @param  data    the JSON data to read from
    void Sound::readFilepath( nlohmann::ordered_json const& data )
    {
        m_Filepath = Stream::Read<std::string>( data );
    }

    /// @brief  reads IsLoopabe
    /// @param  data    the JSON data to read from
    void Sound::readIsLoopabe( nlohmann::ordered_json const& data )
    {
        m_IsLoopabe = Stream::Read<bool>( data );
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

        data["IsLoopabe"] = m_IsLoopabe;
        data["Filepath"] = m_Filepath;

        return data;
    }

    /// @brief  map of the SceneSystem read methods
    ReadMethodMap< Sound > const Sound::s_ReadMethods = {
        { "IsLoopabe", &readIsLoopabe },
        { "Filepath" , &readFilepath  }
    };


//-----------------------------------------------------------------------------
