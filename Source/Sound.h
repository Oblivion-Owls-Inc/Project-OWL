/// @file       Sound.h
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      Wrapper class for FMOD sounds
/// @version    0.1
/// @date       2023-09-13
/// 
/// @copyright  Copyright (c) 2023 Digipen Institute of Technology

#pragma once

#include <fmod.hpp>

#include "ISerializable.h"

/// @brief Example System meant to be copy-pasted when creating new Systems
class Sound : public ISerializable
{

//-----------------------------------------------------------------------------
public: // constructor / destructor
//-----------------------------------------------------------------------------

    /// @brief default constructor
    Sound();

    /// @brief  Constructs a new Sound
    /// @param  filepath    the filepath of the sound to load
    /// @param  looping     whether or not the sound should loop
    Sound( char const* filepath, bool looping );

    /// @brief Destroys this Sound
    ~Sound();

//-----------------------------------------------------------------------------
public: // methods
//-----------------------------------------------------------------------------

    /// @brief  Plays this sound
    /// @param  volume  the relative volume to play this sound at
    /// @param  pitch   the relative pitch to play this sound at
    /// @return The channel that this sound is being played on
    FMOD::Channel* Play(
        FMOD::ChannelGroup* group = nullptr,
        float volume = 1.0f,
        float pitch = 1.0f
    ) const;

//-----------------------------------------------------------------------------
public: // accessors
//-----------------------------------------------------------------------------

    /// @brief  gets the length of this sound
    /// @return the length of this sound in seconds
    float GetLength() const;

    /// @brief  gets whether this Sound is looping
    /// @return whether this Sound is looping
    __inline bool GetLooping() const { return m_IsLooping; }

    /// @brief  gets this Sound's filepath
    /// @return this Sound's filepath
    __inline std::string const& GetFilepath() const { return m_Filepath; }

    /// @brief Used by the Debug System to display information about this Sound
    void Inspect();


//-----------------------------------------------------------------------------
private: // reading
//-----------------------------------------------------------------------------

    /// @brief  reads filepath
    /// @param  data    the JSON data to read from
    void readFilepath( nlohmann::ordered_json const& data );

    /// @brief  reads isLooping
    /// @param  data    the JSON data to read from
    void readIsLooping( nlohmann::ordered_json const& data );

    /// @brief  runs after Sound has been loaded 
    void AfterLoad();

    /// @brief  map of the SceneSystem read methods
    static ReadMethodMap< Sound > const s_ReadMethods;

    /// @brief  gets the Sound read method map
    /// @return the read method map
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override
    {
        return (ReadMethodMap< ISerializable > const&)s_ReadMethods;
    }

//-----------------------------------------------------------------------------
private: // member variables
//-----------------------------------------------------------------------------

    /// @brief  the actual FMOD::Sound
    FMOD::Sound* m_Sound;

    /// @brief  whether this Sound is looping
    bool m_IsLooping;

    /// @brief  The filepath of this Sound
    std::string m_Filepath;

//-----------------------------------------------------------------------------
};
