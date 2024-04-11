/// @file       AudioPlayer.h
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      Component that plays audio
/// @version    0.1
/// @date       2023-09-13
/// 
/// @copyright  Copyright (c) 2023 Digipen Institute of Technology

#pragma once

#include "Behavior.h"

#include "AssetReference.h"
#include "Sound.h"

#include "ComponentReference.h"
#include "Transform.h"
#include "RigidBody.h"


/// @brief  Component that can play audio
class AudioPlayer : public Behavior
{
//-----------------------------------------------------------------------------
public: // constructor / Destructor
//-----------------------------------------------------------------------------


    /// @brief  constructs a new AudioPlayer
    AudioPlayer();


//-----------------------------------------------------------------------------
public: // methods
//-----------------------------------------------------------------------------


    /// @brief  Starts playing this AudioPlayer's sound
    void Play();


    /// @brief  Stops the currently playing channel
    /// @note   FULLY STOPS the channel, doesn't just pause it
    void Stop();


    /// @brief  adds a callback function to be called when the sound completes
    /// @param  ownerId     the ID of the owner of the callback
    /// @param  callback    the function to be called when the sound completes
    /// @note   YOU MUST REMOVE THE CALLBACK USING THE CALLBACK HANDLE WHEN YOU ARE DONE WITH IT
    void AddOnSoundCompleteCallback( unsigned ownerId, std::function< void() > callback );

    /// @brief  removes a callback function to be called when the sound completes
    /// @param  ownerId the ID of the owner of the callback to remove
    void RemoveOnSoundCompleteCallback( unsigned ownerId );


//-----------------------------------------------------------------------------
public: // accessors
//-----------------------------------------------------------------------------


    /// @brief  gets the Sound that this AudioPlayer plays
    /// @return the Sound that this AudioPlayer plays
    AssetReference< Sound > const& GetSound();

    /// @brief  sets the SOund that this AudioPlayer plays
    /// @param  sound   the sound that this AudioPlayer will play
    void SetSound( AssetReference< Sound > const& sound );


    /// @brief  gets whether this AudioPlayer is currently playing anything
    /// @return whether this AudioPlayer is currently playing anything
    bool GetIsPlaying() const;


    /// @brief  gets the current time of the currently playing sound
    /// @return the current time of the currently playing sound
    float GetTime() const;

    /// @brief  sets the current time of the currently playing sound
    /// @param  time    the current time of the currently playing sound
    void SetTime( float time );


    /// @brief  gets whether this AudioPlayer is paused
    /// @return whether this AudioPlayer is paused
    bool GetIsPaused() const;

    /// @brief  Sets whether or not this AudioPlayer is paused
    /// @param  paused   whether to pause or unpause the AudioPlayer
    void SetIsPaused( bool paused );


    /// @brief  gets the volume of this AudioPlayer
    /// @return the volume of this AudioPlayer
    float GetVolume() const;

    /// @brief  sets the volume of this AudioPlayer
    /// @param  volume  the volume for this AudioPlayer
    void SetVolume( float volume );


    /// @brief  gets the pitch of this AudioPlayer
    /// @return the pitch of this AudioPlayer
    float GetPitch() const;

    /// @brief  sets the pitch of this AudioPlayer
    /// @param  pitch  the pitch for this AudioPlayer
    void SetPitch( float pitch );


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


    /// @brief  gets the default loop count of this AudioPlayer
    /// @return the default loop count of this AudioPlayer
    int GetDefaultLoopCount() const;

    /// @brief  sets the default loop count of this AudioPlayer
    /// @param  defaultLoopCount    the default loop count of this AudioPlayer
    void SetDefaultLoopCount( int defaultLoopCount );


    /// @brief  gets the current loop count
    /// @return the current loop count
    int GetLoopCount() const;

    /// @brief  sets the current loop count
    /// @param  loopCount   the current loop count
    void SetLoopCount( int loopCount );


    /// @brief  sets whether the AudioPlayer is spatial
    /// @param  isSpatial   whether the AudioPlayer should be spatial
    void SetIsSpatial( bool isSpatial );


//-----------------------------------------------------------------------------
public: // virtual override methods
//-----------------------------------------------------------------------------


    /// @brief  called once when entering the scene
    virtual void OnInit() override;

    /// @brief  called once when exiting the scene
    virtual void OnExit() override;


    /// @brief  called once every graphics frame
    /// @param  dt  the duration of the frame in seconds
    virtual void OnUpdate( float dt ) override;


//-----------------------------------------------------------------------------
private: // members
//-----------------------------------------------------------------------------


    /// @brief  The sound that this AudioPlayer will play
    AssetReference< Sound > m_Sound;


    /// @brief  the relative volume this AudioPlayer will play at
    float m_Volume = 1.0f;

    /// @brief  the pitch this AudioPlayer will play at
    float m_Pitch = 1.0f;

    /// @brief  the maximum variation of the volume
    float m_VolumeVariance = 0.0f;

    /// @brief  the maxumum variation of the pitch
    float m_PitchVariance = 0.0f;


    /// @brief  allows the AudioPlayer to start playing a new sound before another one is finished,
    /// @brief  but can no longer control sounds after they start playing
    bool m_AllowMultipleSounds = false;


    /// @brief  whether the AudioPlayer should start playing on init
    bool m_PlayOnInit = false;


    /// @brief  the number of loops to play
    int m_DefaultLoopCount = 0;


    /// @brief  whether the sound exists in 3D space
    bool m_IsSpatial = false;


    /// @brief  the name of the channelGroup to play sounds in
    std::string m_ChannelGroupName = "";
    /// @brief  The channelGroup to play sounds in
    FMOD::ChannelGroup* m_ChannelGroup = nullptr;


    /// @brief  the Transform attached to this AudioPlayer
    ComponentReference< Transform, false > m_Transform;

    /// @brief  the RigidBody attached to this AudioPlayer
    ComponentReference< RigidBody, false > m_RigidBody;


    /// @brief  The channel currently being used by this AudioPlayer
    FMOD::Channel* m_Channel = nullptr;

    /// @brief  whether to stay paused when the window regains focus
    bool m_KeepPausedOnFocus = false;


    /// @brief  callbacks to call whenever a sound finishes playing
    std::map< unsigned, std::function< void() > > m_OnSoundCompleteCallbacks = {};


//-----------------------------------------------------------------------------
private: // methods
//-----------------------------------------------------------------------------
    

    /// @brief  callback called when an fmod channel finishes playing
    /// @param  channelControl  the channel the callback is from
    /// @param  controlType     identifier to distinguish between channel and channelgroup
    /// @param  callbackType    the type of callback
    /// @param  commandData1    first callback parameter
    /// @param  commandData2    second callback parameter
    static FMOD_RESULT F_CALLBACK onFmodChannelCallback(
        FMOD_CHANNELCONTROL* channelControl,
        FMOD_CHANNELCONTROL_TYPE controlType,
        FMOD_CHANNELCONTROL_CALLBACK_TYPE callbackType,
        void* commandData1,
        void* commandData2
    );

    /// @brief  callback to call when the window focus changes
    /// @param  focused whether the window is focused
    void onWindowFocusChangedCallback( bool focused );


    /// @brief  sets the spatial attributes of the current channel
    void setSpatialAttributes();


//-----------------------------------------------------------------------------
public: // inspection
//-----------------------------------------------------------------------------


    /// @brief  shows the inspector for AudioPlayer
    virtual void Inspector() override;


//-----------------------------------------------------------------------------
private: // reading
//-----------------------------------------------------------------------------

    /// @brief  read the sound of this component from json
    /// @param  data    the json data
    void readSound( nlohmann::ordered_json const& data );

    /// @brief  read the volume of this component from json
    /// @param  data    the json data
    void readVolume( nlohmann::ordered_json const& data );

    /// @brief  read the pitch of this component from json
    /// @param  data    the json data
    void readPitch( nlohmann::ordered_json const& data );

    /// @brief  read the volumeVariance of this component from json
    /// @param  data    the json data
    void readVolumeVariance( nlohmann::ordered_json const& data );

    /// @brief  read the pitchVariance of this component from json
    /// @param  data    the json data
    void readPitchVariance( nlohmann::ordered_json const& data );

    /// @brief  read DefaultLoopCount of this component from json
    /// @param  data    the json data
    void readDefaultLoopCount( nlohmann::ordered_json const& data );

    /// @brief  read PlayOnInit of this component from json
    /// @param  data    the json data
    void readPlayOnInit( nlohmann::ordered_json const& data );

    /// @brief  read IsSpatial of this component from json
    /// @param  data    the json data
    void readIsSpatial( nlohmann::ordered_json const& data );

    /// @brief  read AllowMultipleSounds of this component from json
    /// @param  data    the json data
    void readAllowMultipleSounds( nlohmann::ordered_json const& data );


    /// @brief  reads the name of the channelGroup to play sounds in
    /// @param  data    the JSON data to read from
    void readChannelGroupName( nlohmann::ordered_json const& data );


//-----------------------------------------------------------------------------
public: // reading / writing
//-----------------------------------------------------------------------------


    /// @brief  gets the map of read methods for this Component
    /// @return the map of read methods for this Component
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override;


    /// @brief  Writes all AudioPlayr data to a JSON file.
    /// @return The JSON file containing the data.
    virtual nlohmann::ordered_json Write() const override;

    
//-----------------------------------------------------------------------------
public: // copying
//-----------------------------------------------------------------------------


    /// @brief  clones this AudioPlayer
    /// @return the newly created clone of this AudioPlayer
    virtual AudioPlayer* Clone() const override;


//-----------------------------------------------------------------------------
private: // copying
//-----------------------------------------------------------------------------


    /// @brief  copy-constructor for the AudioPlayer
    /// @param  other   the other AudioPlayer to copy
    AudioPlayer( AudioPlayer const& other );


    // diable = operator
    void operator =( AudioPlayer const& ) = delete;


//-----------------------------------------------------------------------------

};
