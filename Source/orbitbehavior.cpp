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


/// @brief  Called at a fixed framerate
void OrbitBehavior::OnFixedUpdate()
{
    if (!m_Transform || !m_OrbitPoint || !m_Sprite)
    {
        return;
    }

    float deltaTime = GameEngine()->GetFixedFrameDuration();

    // Update the orbit angle based on the orbit speed
    m_Angle += m_Frequency * deltaTime;

    // Calculate the new position
    float x = m_OrbitPoint->GetTranslation().x + m_Radius.x * glm::cos( m_Angle );
    float y = m_OrbitPoint->GetTranslation().y + m_Radius.y * glm::sin( m_Angle );

    // Set the new position for diagonal movement
    m_Transform->SetTranslation( glm::vec2(x, y) );

     
    float Oscillation = 0.2f * glm::cos( glm::abs(m_Angle) );

    // Set sprite layer based on the phase of oscillation
    if (Oscillation > 0)
    {
        m_Sprite->SetLayer(3);
    }
    else
    {
        m_Sprite->SetLayer(1);
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
    ImGui::DragFloat("Frequency", &m_Frequency, 0.1f);
    ImGui::DragFloat("Angle", &m_Angle, 0.01f);
    ImGui::DragFloat2("Radius", &m_Radius[0], 0.1f);
}

//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------

void OrbitBehavior::readFrequency(nlohmann::ordered_json const& data)
{
    Stream::Read(m_Frequency, data);
}

void OrbitBehavior::readAngle(nlohmann::ordered_json const& data)
{
    Stream::Read(m_Angle, data);
}

void OrbitBehavior::readRadius(nlohmann::ordered_json const& data)
{
    m_Radius = Stream::Read< 2, float >(data);
}

//-----------------------------------------------------------------------------
// public: reading / writing
//-----------------------------------------------------------------------------


/// @brief  gets the map of read methods for this OrbitBehavior
/// @return the map of read methods for this OrbitBehavior
ReadMethodMap< ISerializable > const& OrbitBehavior::GetReadMethods() const
{
    static ReadMethodMap< OrbitBehavior > const readMethods = {
        { "Frequency", &OrbitBehavior::readFrequency },
        { "Angle",     &OrbitBehavior::readAngle     },
        { "Radius",    &OrbitBehavior::readRadius    }

    };

    return (ReadMethodMap< ISerializable > const&)readMethods;
}


/// @brief  Writes all OrbitBehavior data to a JSON file.
/// @return The JSON file containing the data.
nlohmann::ordered_json OrbitBehavior::Write() const
{
    nlohmann::ordered_json json;

    json["Frequency"] = m_Frequency;
    json["Angle"]     = m_Angle;
    json["Radius"]    = Stream::Write(m_Radius);

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
    Behavior(other), m_Frequency(other.m_Frequency), m_Angle(other.m_Angle), m_Radius(other.m_Radius)
{}


//-----------------------------------------------------------------------------
