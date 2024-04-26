/// @file       AudioListener.h
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      Component that picks up spatial audio
/// @version    0.1
/// @date       2023-10-20
/// 
/// @copyright  Copyright (c) 2024 Digipen Institute of Technology

#pragma once




class Transform;
class RigidBody;


/// @brief  component that picks up spatial audio
class AudioListener : public Behavior
{
//-----------------------------------------------------------------------------
public: // types
//-----------------------------------------------------------------------------

    
//-----------------------------------------------------------------------------
private: // types
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
public: // accessors
//-----------------------------------------------------------------------------


    /// @brief  sets whether this AudioListener is the active listener in the scene
    /// @param  isActive    whether this AudioListener is the active listener in the scene
    /// @note   SHOULD ONLY BE USED BY AUDIOSYSTEM - use the MakeActive() function instead
    void SetIsActive( bool isActive );


//-----------------------------------------------------------------------------
public: //  methods
//-----------------------------------------------------------------------------


    /// @brief  makes this the active AudioListener in the scene
    void MakeActive();


//-----------------------------------------------------------------------------
public: // virtual override methods
//-----------------------------------------------------------------------------


    /// @brief  called once when entering the scene
    virtual void OnInit() override;

    /// @brief  called once when exiting the scene
    virtual void OnExit() override;


    /// @brief  gets called every graphics frame
    /// @param  dt  the duration of the frame in seconds
    virtual void OnUpdate( float dt ) override;


//-----------------------------------------------------------------------------
private: // members
//-----------------------------------------------------------------------------


    /// @brief  the z-axis offset out of the screen to calculate spatial audio with
    float m_ZOffset = 0;

    /// @brief  whether this AudioListener is the active listener in the scene
    bool m_IsActive = true;


    /// @brief  the Transform attached to this AudioListener
    ComponentReference< Transform, false > m_Transform;

    /// @brief  the RigidBody attached to this AudioListener
    ComponentReference< RigidBody, false > m_RigidBody;


//-----------------------------------------------------------------------------
private: // helper methods
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
private: // inspection
//-----------------------------------------------------------------------------

    
    /// @brief  displays this AudioListener in the Inspector
    virtual void Inspector() override;


//-----------------------------------------------------------------------------
private: // reading
//-----------------------------------------------------------------------------


    /// @brief  reads the z-axis offset out of the screen to calculate spatial audio with
    /// @param  data    the json data to read from
    void readZOffset( nlohmann::ordered_json const& data );

    /// @brief  reads whether this AudioListener is the active listener in the scene
    /// @param  data    the json data to read from
    void readIsActive( nlohmann::ordered_json const& data );


//-----------------------------------------------------------------------------
public: // reading / writing
//-----------------------------------------------------------------------------


    /// @brief  gets the map of read methods for this Component
    /// @return the map of read methods for this Component
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override;


    /// @brief  Write all AudioListener data to a JSON file.
    /// @return The JSON file containing the AudioListener data.
    virtual nlohmann::ordered_json Write() const override;


//-----------------------------------------------------------------------------
public: // constructor / Destructor
//-----------------------------------------------------------------------------


    /// @brief  constructor
    AudioListener();


//-----------------------------------------------------------------------------
public: // copying
//-----------------------------------------------------------------------------


    /// @brief  clones this AudioListener
    /// @return the newly created clone of this AudioListener
    virtual AudioListener* Clone() const override;


//-----------------------------------------------------------------------------
private: // copying
//-----------------------------------------------------------------------------


    /// @brief  copy-constructor for the AudioListener
    /// @param  other   the other AudioListener to copy
    AudioListener( AudioListener const& other );

    // diable = operator
    void operator =( AudioListener const& ) = delete;


//-----------------------------------------------------------------------------
};
