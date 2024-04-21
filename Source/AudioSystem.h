/// @file       AudioSystem.h
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      System that implements FMOD and allows the loading and playing of audio
/// @version    0.1
/// @date       2023-09-12
/// 
/// @copyright  Copyright (c) 2024 Digipen Institute of Technology

#pragma once

#include "System.h"
#include <fmod.hpp>


class AudioListener;


/// @brief  Example System meant to be copy-pasted when creating new Systems
class AudioSystem : public System
{
//-----------------------------------------------------------------------------
public: // accessors
//-----------------------------------------------------------------------------


    /// @brief  gets the internal FMOD::System
    /// @return the FMOD::System
    FMOD::System* GetFMOD();


    /// @brief  gets the currently active AudioListener
    /// @return the currently active AudioListener
    AudioListener* GetActiveListener() const;


//-----------------------------------------------------------------------------
public: // methods
//-----------------------------------------------------------------------------


    /// @brief  gets the currently active AudioListener
    /// @param  listener    the AudioListener to make active
    void SetActiveListener( AudioListener* listener );


    /// @brief  gets the channel group with the specified name
    /// @param  name    the name of the channel group to get
    /// @return the FMOD::ChannelGroup with the specified name
    FMOD::ChannelGroup* GetChannelGroup( std::string const& name );


    /// @brief  gets the volume of the master channel
    /// @return the volume of the master channel
    float GetVolume() const;

    /// @brief  sets the volume of the master channel
    /// @param  volume  the volume to set the master channel to
    void SetVolume( float volume );


    /// @brief  gets the volume of the specified channel
    /// @param  name    the name of the channel to get the volume of
    /// @return the volume of the channel
    float GetVolume( std::string const& name ) const;

    /// @brief  sets the volume of the specified channel
    /// @param  name    the name of the channel to set the volume of
    /// @param  volume  the volume to set the channel to
    void SetVolume( std::string const& name, float volume );


//-----------------------------------------------------------------------------
public: // virtual override methods
//-----------------------------------------------------------------------------


    /// @brief  Gets called once when this System is added to the Engine
    virtual void OnInit() override;

    /// @brief  Gets called once before the Engine closes
    virtual void OnExit() override;


    /// @brief  Gets called once every graphics frame. Do not use this function for anything that affects the simulation.
    /// @param  dt  the elapsed time in seconds since the previous frame
    virtual void OnUpdate( float dt ) override;

    
//-----------------------------------------------------------------------------
private: // struct AudioGroup
//-----------------------------------------------------------------------------


    /// @brief  struct used to load and save data about audio groups
    struct AudioGroup : public ISerializable
    {
    //-----------------------------------------------------------------------------
    public: // members
    //-----------------------------------------------------------------------------

        
        /// @brief  the name of this AudioGroup
        std::string M_Name = "";


        /// @brief  the volume of this AudioGroup
        float M_Volume = 1.0f;

        /// @brief  whether this AudioGroup is muted
        bool M_Mute = false;


        /// @brief  the underlying FMOD ChannelGroup
        FMOD::ChannelGroup* M_Group = nullptr;
        
        
    //-----------------------------------------------------------------------------
    public: // methods
    //-----------------------------------------------------------------------------
    
        
        /// @brief  initializes this AudioGroup
        /// @param  system  the FMOD system to initialize the AudioGroup in
        void Init( FMOD::System* system );


        /// @brief  exits this AudioGroup;
        void Exit();


    //-----------------------------------------------------------------------------
    public: // inspection
    //-----------------------------------------------------------------------------
    

        /// @brief  inspects this ChannelGroup
        /// @return whether this ChannelGroup was changed
        bool Inspect();

        
    //-----------------------------------------------------------------------------
    public: // reading / writing
    //-----------------------------------------------------------------------------
    

        /// @brief  gets this System's read methods
        /// @return this System's read methods
        virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override;


        /// @brief  writes this AudioSystem to json
        /// @return the json data of the AudioSystem
        virtual nlohmann::ordered_json Write() const override;

        
    //-----------------------------------------------------------------------------
    private: // reading
    //-----------------------------------------------------------------------------
    

        /// @brief  reads the name of this AudioGroup
        /// @param  data    the JSON data to read from
        void readName( nlohmann::ordered_json const& data );


        /// @brief  reads the volume of this AudioGroup
        /// @param  data    the JSON data to read from
        void readVolume( nlohmann::ordered_json const& data );

        /// @brief  reads whether this AudioGroup is muted
        /// @param  data    the JSON data to read from
        void readMute( nlohmann::ordered_json const& data );


    //-----------------------------------------------------------------------------
    };


//-----------------------------------------------------------------------------
private: // member variables
//-----------------------------------------------------------------------------


    /// @brief  The FMOD system
    FMOD::System* m_System = nullptr;

    /// @brief  The maximum number of FMOD audio channels
    int m_MaxChannels = 256;


    /// @brief  the master volume of the AudioSystem
    float m_Volume = 1.0f;


    /// @brief  the currently active AudioListener
    AudioListener* m_ActiveListener = nullptr;


    /// @brief  all of the AudioGroups
    std::vector< AudioGroup > m_Groups = {};


//-----------------------------------------------------------------------------
private: // static methods
//-----------------------------------------------------------------------------


    /// @brief  FMOD callback function for error handling
    /// @param  system          handle to the FMOD system
    /// @param  type            the type of callback
    /// @param  commandData1    first callback parameter, dependent on callback type
    /// @param  commandData2    second callback parameter, dependent on callback type
    /// @param  userData        user data associated with the FMOD system
    /// @return FMOD_RESULT
    static FMOD_RESULT fmodCallback(
        FMOD_SYSTEM* system,
        FMOD_SYSTEM_CALLBACK_TYPE m_Type,
        void* commandData1,
        void* commandData2,
        void* userData
    );
    

//-----------------------------------------------------------------------------
public: // inspection
//-----------------------------------------------------------------------------
    

    /// @brief  inspects a reference to a channel group
    /// @param  label           the label of the channel group
    /// @param  groupName       the name of the channel group reference being inspected
    /// @param  channelGroup    the channel group reference being inspeced
    /// @return whether the reference to the channel group was changed
    bool InspectChannelGroup( char const* label, std::string* groupName, FMOD::ChannelGroup** channelGroup );


    /// @brief  shows this System's DebugWindow
    virtual void DebugWindow() override;


//-----------------------------------------------------------------------------
private: // reading
//-----------------------------------------------------------------------------


    /// @brief  reads the max channels
    /// @param  data  the data to read from
    void readMaxChannels( nlohmann::ordered_json const& data );

    /// @brief  reads the master volume of the AudioSystem
    /// @param  data    the JSON data to read from
    void readVolume( nlohmann::ordered_json const& data );

    /// @brief  reads the groups in the AudioSystem
    /// @param  data    the JSON data to read from
    void readGroups( nlohmann::ordered_json const& data );


//-----------------------------------------------------------------------------
public: // reading / writing
//-----------------------------------------------------------------------------


    /// @brief  gets this System's read methods
    /// @return this System's read methods
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override;


    /// @brief  writes this AudioSystem to json
    /// @return the json data of the AudioSystem
    virtual nlohmann::ordered_json Write() const override;

    
//-----------------------------------------------------------------------------
public: // singleton stuff
//-----------------------------------------------------------------------------


    /// @brief  gets the instance of AudioSystem
    /// @return the instance of the AudioSystem
    static AudioSystem* GetInstance();


//-----------------------------------------------------------------------------
private: // singleton stuff
//-----------------------------------------------------------------------------


    /// @brief  Constructs the AudioSystem
    AudioSystem();


    // Prevent copying
    AudioSystem( AudioSystem const& ) = delete;
    void operator =( AudioSystem const& ) = delete;


//-----------------------------------------------------------------------------
};


/// @brief  shorthand method for AudioSystem::GetInstance()
/// @return the AudioSystem instance
__inline AudioSystem* Audio()
{
    return AudioSystem::GetInstance();
}
