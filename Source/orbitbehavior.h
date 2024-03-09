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

    /// @brief Updates the OrbitBehavior every frame
    /// @param deltaTime - the time since the last frame
    virtual void OnUpdate(float deltaTime) override;


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
    float m_RotationSpeed = 1.0f;

    /// @brief  the angle of the orbit
    float m_Angle = 5.0f;

    /// @brief the radius of the orbit
    glm::vec2 m_Radius = { 0.48 , 0.48 };

    /// @brief The first layer to set the sprite to
    int m_FirstLayer = 3;
    
    /// @brief The second layer to set the sprite to
    int m_SecondLayer = 1;

    /// @brief The Base Scale of the sprite
    glm::vec2 m_BaseScale = { .5f, .5f };

    /// Controls the magnitude of scale oscillation
    float m_ScaleOscillationMagnitude = 0.2f; 

    /// Angle at which the object is largest or smallest
    float m_ScaleExtremesAngle = 0.0f; 

    /// Angle in orbit to switch layers (radians)
    float m_LayerSwitchAngle = glm::pi<float>(); 



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

    /// @brief Reads the Rotation Speed from JSON
    /// @param data - the JSON data to read from
    void readRotationSpeed(nlohmann::ordered_json const& data);

    /// @brief Reads the Angle from JSON
    /// @param data - the JSON data to read from
    void readAngle(nlohmann::ordered_json const& data);

    /// @brief Reads the Radius from JSON
    /// @param data - the JSON data to read from
    void readRadius(nlohmann::ordered_json const& data);

    /// @brief Reads the First Layer from JSON
    /// @brief data - the JSON data to read from
    void readFirstLayer(nlohmann::ordered_json const& data);

    /// @brief Reads the Second Layer from JSON
    /// @param data - the JSON data to read from
    void readSecondLayer(nlohmann::ordered_json const& data);

    /// @brief Reads the Base Scale from JSON
    /// @param data - the JSON data to read from
    void readBaseScale(nlohmann::ordered_json const& data);

    void readScaleOscillationMagnitude(nlohmann::ordered_json const& data);

    void readScaleExtremesAngle(nlohmann::ordered_json const& data);

    void readLayerSwitchAngle(nlohmann::ordered_json const& data);


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
