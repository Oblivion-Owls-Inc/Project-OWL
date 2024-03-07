/////////////////////////////////////////////////////////////////////////////////
/// @file       OrbitBehavior.h
/// @author     Jax Clayton (jax.clayton@digipen.edu)
/// @brief      Component orbits the entity around a point
/// @version    0.1
/// @date       3/6/2024
/// 
/// @copyright  Copyright (c) 2024 Digipen Institute of Technology
/////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "Behavior.h"
#include "EntityReference.h"
#include "ComponentReference.h"

class Transform;
class Sprite;

/// @brief  This Behavior orbits the entity around a point
class OrbitBehavior : public Behavior
{
    //-----------------------------------------------------------------------------
public: // constructor / Destructor
    //-----------------------------------------------------------------------------


        /// @brief  default constructor
    OrbitBehavior();


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

    /// @brief  called At a fixed framerate
    virtual void OnFixedUpdate();


    /// @brief  called once when exiting the scene
    virtual void OnExit() override;


    //-----------------------------------------------------------------------------
private: // members
    //-----------------------------------------------------------------------------


    /// @brief the Transform of the entity
    ComponentReference< Transform > m_Transform;

    ComponentReference< Transform > m_OrbitPoint;

    /// @brief the Sprite to Set the Layer of
    ComponentReference< Sprite > m_Sprite;

    /// @brief how fast the entity orbits in a direction
    float m_Frequency = 1.0f;

    /// @brief  the angle of the orbit
    float m_Angle = 5.0f;

    /// @brief the radius of the orbit
    glm::vec2 m_Radius = {0.48,0.48};
  

    //-----------------------------------------------------------------------------
private: // methods
    //-----------------------------------------------------------------------------


    //-----------------------------------------------------------------------------
public: // inspection
    //-----------------------------------------------------------------------------


        /// @brief  shows the inspector for OrbitBehavior
    virtual void Inspector() override;



    //-----------------------------------------------------------------------------
private: // reading
    //-----------------------------------------------------------------------------

    /// @brief Reads the Frequency from JSON
    /// @param data - the JSON data to read from
    void readFrequency(nlohmann::ordered_json const& data);

    /// @brief Reads the Angle from JSON
    /// @param data - the JSON data to read from
    void readAngle(nlohmann::ordered_json const& data);

    /// @brief Reads the Radius from JSON
    /// @param data - the JSON data to read from
    void readRadius(nlohmann::ordered_json const& data);

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
    virtual OrbitBehavior* Clone() const override;


    //-----------------------------------------------------------------------------
private: // copying
    //-----------------------------------------------------------------------------


        /// @brief  copy-constructor for the OrbitBehavior
        /// @param  other   the other OrbitBehavior to copy
    OrbitBehavior(OrbitBehavior const& other);


    // disable assignment operator
    void operator =(OrbitBehavior const&) = delete;


    //-----------------------------------------------------------------------------
};
