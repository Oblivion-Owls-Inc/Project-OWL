/////////////////////////////////////////////////////////////////////////////////
/// @file       OrbitBehavior.h
/// @author     Jax Clayton (jax.clayton@digipen.edu)
/// @brief      Component orbits the entity around a point
/// @version    0.1
/// @date       3/6/2024
/// 
/// @copyright  Copyright (c) 2024 Digipen Institute of Technology
/////////////////////////////////////////////////////////////////////////////////

#include "pch.h" // precompiled header has to be included first
#include "OrbitBehavior.h"
#include "Transform.h"
#include "Sprite.h"
#include "Engine.h"
#include "BehaviorSystem.h"

//-----------------------------------------------------------------------------
// public: constructor / Destructor
//-----------------------------------------------------------------------------


/// @brief  default constructor
OrbitBehavior::OrbitBehavior() :
    Behavior(typeid(OrbitBehavior))
{}


//-----------------------------------------------------------------------------
// public: methods
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// public: accessors
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// public: virtual override methods
//-----------------------------------------------------------------------------


/// @brief  called once when entering the scene
void OrbitBehavior::OnInit()
{
    BehaviorSystem< Behavior >::GetInstance()->AddComponent(this);
    
    m_Sprite.   Init( GetEntity() );
    m_Transform.Init( GetEntity() );
    m_OrbitPoint.Init(GetEntity()->GetParent());
}


//void OrbitBehavior::OnUpdate(float deltaTime)
//{
//    if (!m_Transform || !m_OrbitPoint || !m_Sprite)
//    {
//        return; // Exit if any component is missing
//    }
//
//    // Update and clamp the orbit angle
//    m_Angle += m_RotationSpeed * deltaTime;
//    m_Angle = glm::mod(m_Angle, 2 * glm::pi<float>());
//
//    // Calculate new position
//    float x = m_OrbitPoint->GetTranslation().x + m_Radius.x * glm::cos(m_Angle);
//    float y = m_OrbitPoint->GetTranslation().y + m_Radius.y * glm::sin(m_Angle);
//
//    // Set the new position
//    m_Transform->SetTranslation(glm::vec2(x, y));
//
//    // Oscillation for scale modulation
//    float Oscillation = 0.2f * glm::cos(glm::abs(m_Angle));
//
//    if (m_FlipGrowth)
//    {
//        Oscillation *= -1.0f;
//    }
//
//    glm::vec2 newScale = m_BaseScale * (1.0f + Oscillation);
//    m_Transform->SetScale(newScale);
//
//    // Determine the current angle relative to orbit center for layer change
//    float currentAngle = atan2(y - m_OrbitPoint->GetTranslation().y, x - m_OrbitPoint->GetTranslation().x);
//    float thresholdAngle = glm::radians(m_ThresholdAngle);
//
//    // Set sprite layer based on current angle
//    if (glm::abs(currentAngle) < thresholdAngle)
//    {
//        m_Sprite->SetLayer(m_FirstLayer);
//    }
//    else
//    {
//        m_Sprite->SetLayer(m_SecondLayer);
//    }
//}


void OrbitBehavior::OnUpdate(float deltaTime)
{
    if (!m_Transform || !m_OrbitPoint || !m_Sprite)
    {
        return; // Exit if any component is missing
    }

    // Calculate the maximum angle
    constexpr float MaxAngle = 2 * glm::pi<float>();


    // Update and clamp the orbit angle
    m_Angle += m_RotationSpeed * deltaTime;
    m_Angle = glm::mod(m_Angle, MaxAngle);

    // Calculate new position
    float x = m_OrbitPoint->GetTranslation().x + m_Radius.x * glm::cos(m_Angle);
    float y = m_OrbitPoint->GetTranslation().y + m_Radius.y * glm::sin(m_Angle);

    // Set the new position
    m_Transform->SetTranslation(glm::vec2(x, y));

    // Oscillation for scale modulation

    float calculatedAngle = m_Angle - m_ScaleExtremesAngle;


    float Oscillation = ( m_ScaleOscillationMagnitude * glm::cos(MaxAngle * calculatedAngle / MaxAngle) ) + 1.0f;

    glm::vec2 newScale = m_BaseScale * Oscillation;
    m_Transform->SetScale(newScale);

    // Determine the current angle relative to orbit center for layer change
    float currentAngle = glm::mod( atan2( y - m_OrbitPoint->GetTranslation().y, x - m_OrbitPoint->GetTranslation().x ) - m_LayerSwitchAngle, MaxAngle);

    // Set sprite layer based on current angle
    if (currentAngle < glm::pi<float>())
    {
        m_Sprite->SetLayer(m_FirstLayer);
    }
    else
    {
        m_Sprite->SetLayer(m_SecondLayer);
    }
}



/// @brief  called once when exiting the scene
void OrbitBehavior::OnExit()
{
    m_Transform.Exit();
    m_Sprite.Exit();
    m_OrbitPoint.Exit();

    BehaviorSystem< Behavior >::GetInstance()->RemoveComponent(this);
}


//-----------------------------------------------------------------------------
// private: methods
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// public: inspection
//-----------------------------------------------------------------------------


/// @brief  shows the inspector for OrbitBehavior
void OrbitBehavior::Inspector()
{
    ImGui::DragFloat("RotationSpeed", &m_RotationSpeed, 0.1f);
    ImGui::DragFloat("Angle", &m_Angle, 0.01f);
    ImGui::DragFloat("Threshold Angle", &m_ThresholdAngle, 0.1f);
    ImGui::DragFloat2("Radius", &m_Radius[0], 0.1f);
    ImGui::DragInt("First Layer", &m_FirstLayer, 1);
    ImGui::DragInt("Second Layer", &m_SecondLayer, 1);
    ImGui::DragFloat2("Base Scale", &m_BaseScale[0], 0.1f);
    ImGui::DragFloat("Scale Oscillation Magnitude", &m_ScaleOscillationMagnitude, 0.1f);
    ImGui::DragFloat("Scale Extremes Angle", &m_ScaleExtremesAngle, 0.1f);
    ImGui::DragFloat("Layer Switch Angle", &m_LayerSwitchAngle, 0.1f);
}

//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------


/// @brief Reads the Rotation Speed from JSON
/// @param data - the JSON data to read from
void OrbitBehavior::readRotationSpeed(nlohmann::ordered_json const& data)
{
    Stream::Read(m_RotationSpeed, data);
}

/// @brief Reads the Angle from JSON
/// @param data - the JSON data to read from
void OrbitBehavior::readAngle(nlohmann::ordered_json const& data)
{
    Stream::Read(m_Angle, data);
}

/// @brief Reads the Radius from JSON
/// @param data - the JSON data to read from
void OrbitBehavior::readRadius(nlohmann::ordered_json const& data)
{
    m_Radius = Stream::Read< 2, float >(data);
}

/// @brief Reads the First Layer from JSON
/// @param data - the JSON data to read from
void OrbitBehavior::readFirstLayer(nlohmann::ordered_json const& data)
{
    Stream::Read(m_FirstLayer, data);
}

/// @brief Reads the Second Layer from JSON
/// @param data - the JSON data to read from
void OrbitBehavior::readSecondLayer(nlohmann::ordered_json const& data)
{
    Stream::Read(m_SecondLayer, data);
}

/// @brief Reads the Threshold Angle from JSON
/// @param data - the JSON data to read from
void OrbitBehavior::readThresholdAngle(nlohmann::ordered_json const& data)
{
    Stream::Read(m_ThresholdAngle, data);
}

/// @brief Reads the Base Scale from JSON
/// @param data - the JSON data to read from
void OrbitBehavior::readBaseScale(nlohmann::ordered_json const& data)
{
    m_BaseScale = Stream::Read< 2, float >(data);
}

//-----------------------------------------------------------------------------
// public: reading / writing
//-----------------------------------------------------------------------------


/// @brief  gets the map of read methods for this OrbitBehavior
/// @return the map of read methods for this OrbitBehavior
ReadMethodMap< ISerializable > const& OrbitBehavior::GetReadMethods() const
{
    static ReadMethodMap< OrbitBehavior > const readMethods = {
        { "RotationSpeed",   &OrbitBehavior::readRotationSpeed  },
        { "Angle",           &OrbitBehavior::readAngle          },
        { "Radius",          &OrbitBehavior::readRadius         },
        { "First Layer",     &OrbitBehavior::readFirstLayer     },
        { "Second Layer",    &OrbitBehavior::readSecondLayer    },
        { "Threshold Angle", &OrbitBehavior::readThresholdAngle },
        { "Base Scale",      &OrbitBehavior::readBaseScale      },

    };

    return (ReadMethodMap< ISerializable > const&)readMethods;
}


/// @brief  Writes all OrbitBehavior data to a JSON file.
/// @return The JSON file containing the data.
nlohmann::ordered_json OrbitBehavior::Write() const
{
    nlohmann::ordered_json json;

    json["RotationSpeed"]   = m_RotationSpeed;
    json["Angle"]           = m_Angle;
    json["Radius"]          = Stream::Write(m_Radius);
    json["First Layer"]     = m_FirstLayer;
    json["Second Layer"]    = m_SecondLayer;
    json["Threshold Angle"] = m_ThresholdAngle;
    json["Base Scale"]      = Stream::Write(m_BaseScale);

    return json;
}


//-----------------------------------------------------------------------------
// public: copying
//-----------------------------------------------------------------------------


/// @brief  clones this OrbitBehavior
/// @return the newly created clone of this OrbitBehavior
OrbitBehavior* OrbitBehavior::Clone() const
{
    return new OrbitBehavior(*this);
}


//-----------------------------------------------------------------------------
// private: copying
//-----------------------------------------------------------------------------


/// @brief  copy-constructor for the OrbitBehavior
/// @param  other   the other OrbitBehavior to copy
OrbitBehavior::OrbitBehavior(OrbitBehavior const& other) :
    Behavior(other), 
    m_RotationSpeed(other.m_RotationSpeed),
    m_Angle(other.m_Angle), 
    m_Radius(other.m_Radius),
    m_FirstLayer(other.m_FirstLayer),
    m_SecondLayer(other.m_SecondLayer),
    m_ThresholdAngle(other.m_ThresholdAngle),
    m_BaseScale(other.m_BaseScale)
{}


//-----------------------------------------------------------------------------
