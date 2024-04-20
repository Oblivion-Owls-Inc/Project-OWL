/*********************************************************************
* \file         JetpackBoost.h
* \author       Eli Tsereteli
* \date         April 2024
* \copyright    Copyright (c) 2023 Digipen Institute of Technology
*
* \brief        Controls jetpack visuals: particles + tilt.
*********************************************************************/
#pragma once
#include "Behavior.h"
#include "ComponentReference.h"
#include "ActionReference.h"
#include "PlayerController.h"

class Emitter;
class Transform;
class RigidBody;
class AudioPlayer;

class JetpackBoost : public Behavior
{
//-----------------------------------------------------------------------------
public: // constructors / destructors
//-----------------------------------------------------------------------------

    /// @brief  Default constructor
    JetpackBoost();

    /// @brief  Returns copy of this behavior
    JetpackBoost* Clone() const override;
    
private:
    /// @brief  copy ctor
    JetpackBoost(const JetpackBoost& other);



//-----------------------------------------------------------------------------
//              Overrides
//-----------------------------------------------------------------------------
private:
    /// @brief  Adds itself to behavior system
    virtual void OnInit() override;

    /// @brief  
    virtual void OnUpdate(float dt) override;

    /// @brief  Removes itself from behavior system
    virtual void OnExit() override;
    
    /// @brief  Tweak properties in debug window
    virtual void Inspector() override;

    /// @brief  What to do when entity is re-parented
    virtual void OnHierarchyChange(Entity* previousParent);


//-----------------------------------------------------------------------------
//              Public methods
//-----------------------------------------------------------------------------
public:

    /// @brief   Sets maximum tilt angle to given value (rad)
    __inline void SetMaxTilt(float angle) { m_MaxAngle = angle; }

    /// @return  Maximum tilt angle (rad)
    __inline float GetMaxTilt() const { return m_MaxAngle; }

    /// @brief   Sets tilt speed to given value (rad/sec)
    __inline void SetTiltSpeed(float rad_per_sec) { m_AngleSpeed = rad_per_sec; }

    /// @return  How fast player tilts (rad/sec)
    __inline float GetTiltSpeed() const { return m_AngleSpeed; }


//-----------------------------------------------------------------------------
//              Data
//-----------------------------------------------------------------------------
private:
    
    /// @brief  This entity's transform
    ComponentReference< Transform > m_Transform;

    /// @brief  Player's transsform
    ComponentReference< Transform > m_PTransform;

    ComponentReference< PlayerController > m_PController;

    /// @brief  Flame emitter
    ComponentReference< Emitter > m_Flame;

    /// @brief  Jetpack sound
    ComponentReference< AudioPlayer > m_Sound;

    /// @brief  up/down input
    ActionReference m_InputYAxis;

    /// @brief  left/right input
    ActionReference m_InputXAxis;

    /// @brief  True when all references are present/initialized
    bool m_Initialized = false;

    /// @brief  Keep track of player angle to change it gradually
    float m_Angle = 0.0f;

    /// @brief  Speed at which angle changes (per sec)
    float m_AngleSpeed = 2.0f;

    /// @brief  Maximum tilt angle (in radians)
    float m_MaxAngle = 0.5f;

    /// @brief  Jetpack flame offset
    glm::vec4 m_Offset = { 0,0,0,1 };


//-----------------------------------------------------------------------------
//              Reading / Writing
//-----------------------------------------------------------------------------
private:
    /// @brief   the map of read methods for this Component
    static ReadMethodMap< JetpackBoost > const s_ReadMethods;

    /// @brief       Reads horizontal input axis
    /// @param data  json to read from
    void readXAxisInput(nlohmann::ordered_json const& data);

    /// @brief       Reads vertical input axis
    /// @param data  json to read from
    void readYAxisInput(nlohmann::ordered_json const& data);

    /// @brief       Reads the flame offset
    /// @param data  json to read from
    void readOffset(nlohmann::ordered_json const& data);

    /// @brief       Reads maximum tilt angle
    /// @param data  json to read from
    void readMaxAngle(nlohmann::ordered_json const& data);

    /// @brief       Reads speed of tilt angle
    /// @param data  json to read from
    void readAngleSpeed(nlohmann::ordered_json const& data);
    

public:

    /// @brief   gets the map of read methods for this Component
    /// @return  the map of read methods for this Component
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override
    {
        return (ReadMethodMap< ISerializable > const&)s_ReadMethods;
    }

    /// @brief      Write all JetpackBoost component data to a JSON file.
    /// @return     The JSON file containing the JetpackBoost component data.
    virtual nlohmann::ordered_json Write() const override;
};

