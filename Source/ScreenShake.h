/////////////////////////////////////////////////////////////////////////////////
/// @file       ScreenShake.h
/// @author     Jax Clayton (jax.clayton@digipen.edu)
/// @brief      
/// @version    0.1
/// @date       3/6/2024
/// 
/// @copyright  Copyright (c) 2024 Digipen Institute of Technology
/////////////////////////////////////////////////////////////////////////////////

#pragma once


#include "EventListener.h"


class ScreenShake :
    public Behavior
{
//-----------------------------------------------------------------------------
public: // constructor / Destructor
//-----------------------------------------------------------------------------


    /// @brief  default constructor
    ScreenShake();


//-----------------------------------------------------------------------------
public: // methods
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
public: // accessors
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
public: // virtual override methods
//-----------------------------------------------------------------------------

    /// @brief  called once when entering the scene
    virtual void OnInit() override;

    virtual void OnUpdate(float deltaTime) override;

    /// @brief Called at a fixed interval
    virtual void OnFixedUpdate() override;
    
    /// @brief  called once when exiting the scene
    virtual void OnExit() override;

//-----------------------------------------------------------------------------
private: // members
//-----------------------------------------------------------------------------

    /// @brief the Transform of the entity
    ComponentReference< Transform > m_Transform;

    /// @brief Listener for the event
    EventListener<std::string> m_Listener;

    /// @brief the name of the event
    std::string m_EventName;

    /// @brief the duration of the shake
    float m_ShakeDuration = 0.0f;

    /// @brief the distance of the shake from the center on the x-axis
    glm::vec2 m_ShakeXRange = glm::vec2(0.0f, 0.0f);

    /// @brief the distance of the shake from the center on the y-axis
    glm::vec2 m_ShakeYRange = glm::vec2(0.0f, 0.0f);

    glm::vec2 m_ShakeRotationRange = glm::vec2(0.0f, 0.0f);

    /// @brief if the shake is active
    bool m_Active = false;

//-----------------------------------------------------------------------------
private: // methods
//-----------------------------------------------------------------------------

    /// @brief  shakes the screen
    /// @param deltaTime - the time since the last frame
    void ShakeScreen(float deltaTime);

//-----------------------------------------------------------------------------
public: // inspection
//-----------------------------------------------------------------------------

    /// @brief  shows the inspector for OrbitBehavior
    virtual void Inspector() override;
//-----------------------------------------------------------------------------
private: // reading
//-----------------------------------------------------------------------------
    
    /// @brief  reads the event name from the JSON data
    /// @param data    the JSON data to read from
    void readEventName(nlohmann::ordered_json const& data);

    /// @brief reads the shake duration from the JSON data
    /// @param data - the JSON data to read from
    void readShakeDuration(nlohmann::ordered_json const& data);

    /// @brief reads the shake x range from the JSON data
    /// @param data - the JSON data to read from
    void readShakeXRange(nlohmann::ordered_json const& data);

    /// @brief reads the shake y range from the JSON data
    /// @param data - the JSON data to read froma
    void readShakeYRange(nlohmann::ordered_json const& data);

    /// @brief reads the shake rotation range from the JSON data
    /// @param data - the JSON data to read from
    void readShakeRotationRange(nlohmann::ordered_json const& data);

//-----------------------------------------------------------------------------
public: // reading / writing
//-----------------------------------------------------------------------------


    /// @brief  gets the map of read methods for this OrbitBehavior
    /// @return the map of read methods for this OrbitBehavior
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override;


    /// @brief  writes this OrbitBehavior to JSON
    /// @return the JSON data of this OrbitBehavior
    virtual nlohmann::ordered_json Write() const override;

//-----------------------------------------------------------------------------
public: // copying
//-----------------------------------------------------------------------------


    /// @brief  clones this OrbitBehavior
    /// @return the newly created clone of this OrbitBehavior
    virtual ScreenShake* Clone() const override;

//-----------------------------------------------------------------------------
private: // copying
//-----------------------------------------------------------------------------


    /// @brief  copy-constructor for the OrbitBehavior
    /// @param  other   the other OrbitBehavior to copy
    ScreenShake(ScreenShake const& other);


    /// disable assignment operator
    void operator =(ScreenShake const&) = delete;


//-----------------------------------------------------------------------------
};

