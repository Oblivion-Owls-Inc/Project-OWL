/*********************************************************************
* \file         JetpackBoost.cpp
* \author       Eli Tsereteli (ilya.tsereteli@digipen.edu)
* \date         April 2024
* \copyright    Copyright (c) 2024 Digipen Institute of Technology
* 
* \brief        Controls jetpack visuals: particles + tilt.
*********************************************************************/

#include "pch.h" // precompiled header has to be included first
#include "BehaviorSystem.h"
#include "JetpackBoost.h"
#include "ComponentReference.t.h"



#include "Emitter.h"

#include "AudioPlayer.h"
#include "InputSystem.h"

static const float downAngle = 4.712389f;

/// @brief  Default constructor
JetpackBoost::JetpackBoost() :
    Behavior(typeid(JetpackBoost))
{}

/// @brief  copy ctor
JetpackBoost::JetpackBoost(const JetpackBoost& other) :
    Behavior(other)
{}

/// @brief  Return a copy of this component
JetpackBoost* JetpackBoost::Clone() const { return new JetpackBoost(*this); }



//-----------------------------------------------------------------------------
//              Overrides
//-----------------------------------------------------------------------------

/// @brief  Adds itself to behavior system
void JetpackBoost::OnInit() 
{ 
    Behaviors< Behavior >()->AddComponent(this);

    // init:

    m_PTransform.SetOnConnectCallback( [ this ]()
    {
        // execute this here to prevent using old translation
        m_PTransform->AddOnTransformChangedCallback(GetId(), [ this ]()
        {
            // which way is player facing
            float xscale = m_PTransform->GetScale().x;

            // align particles with jetpack
            glm::mat4 parentt = m_PTransform->GetMatrix();
            glm::vec2 offset = parentt * m_Offset;
            m_Transform->SetTranslation(offset);
        } );
    } );

    m_PTransform.SetOnDisconnectCallback([this]()
    {
        m_PTransform->RemoveOnTransformChangedCallback( GetId() );
    } );

    //  own components
    m_Transform.Init( GetEntity() );
    m_Flame.Init( GetEntity() );
    m_Sound.Init( GetEntity() );

    //  parent component(s?)
    Entity* parent = GetEntity()->GetParent();
    if (parent)
    {
        m_PTransform.Init(parent);
        m_PController.Init(parent);
    }

    //  input
    m_InputYAxis.SetOwnerName( GetName() );
    m_InputXAxis.SetOwnerName( GetName() );
    m_InputYAxis.Init();
    m_InputXAxis.Init();

    if (m_Transform && m_PTransform && m_Sound && m_Flame && m_InputXAxis && m_InputYAxis)
        m_Initialized = true;
}

/// @brief  Removes itself from behavior system
void JetpackBoost::OnExit() 
{ 
    Behaviors< Behavior >()->RemoveComponent(this);
    m_Transform.Exit();
    m_Flame.Exit();
    m_PTransform.Exit();
    m_PController.Exit();
    m_InputXAxis.Exit();
    m_InputYAxis.Exit();
}


/// @brief  
void JetpackBoost::OnUpdate(float dt)
{
    if (!m_Initialized || !m_PController->GetActive())
        return;

    float targetAngle = 0.0f;

    // goin up
    if (m_InputYAxis->GetAxis() > 0.0f)
    {
        m_Flame->SetContinuous(true);
        m_Sound->Play();

        targetAngle = m_MaxAngle * -Sign( m_InputXAxis->GetAxis() );
    }
    // not goin up
    else
    {
        m_Flame->SetContinuous(false);
        m_Sound->Stop();
    }

    // rotate towards target
    if ( std::abs( m_Angle - targetAngle ) < m_AngleSpeed * dt )
    {
        m_Angle = targetAngle;
    }
    else if ( m_Angle < targetAngle )
    {
        m_Angle += m_AngleSpeed * dt;
    }
    else
    {
        m_Angle -= m_AngleSpeed * dt;
    }

    // which way is player facing
    float xscale = m_PTransform->GetScale().x;

    // rotate player and particle direction
    m_PTransform->SetRotation(m_Angle);
    ParticleSystem::EmitData fdata = m_Flame->GetEmitData();
    fdata.direction = downAngle + m_Angle;
    m_Flame->SetEmitData(fdata);
}


/// @brief  Tweak properties in debug window
void JetpackBoost::Inspector()
{
    ImGui::DragFloat2("Flame Offset", &m_Offset[0], 0.005f);
    ImGui::DragFloat("Max Tilt (radians)", &m_MaxAngle, 0.005f);
    ImGui::DragFloat("Tilt speed (radians/sec)", &m_AngleSpeed, 0.005f);
}


/// @brief  What to do when entity is re-parented
void JetpackBoost::OnHierarchyChange(Entity* previousParent)
{
    // re-init
    if (m_Initialized)
    {
        OnExit();
        OnInit();
    }
}


//-----------------------------------------------------------------------------
//              Reading / Writing
//-----------------------------------------------------------------------------

/// @brief read method map
ReadMethodMap<JetpackBoost> const JetpackBoost::s_ReadMethods =
{
    { "XAxisInputAction"               , &JetpackBoost::readXAxisInput },
    { "YAxisInputAction"               , &JetpackBoost::readYAxisInput },
    { "FlameOffset"                    , &JetpackBoost::readOffset     },
    { "MaxTiltRad"                     , &JetpackBoost::readMaxAngle   },
    { "TiltSpeed"                      , &JetpackBoost::readAngleSpeed }
};


/// @brief       Reads horizontal input axis
/// @param data  json to read from
void JetpackBoost::readXAxisInput(nlohmann::ordered_json const& data)
{
    Stream::Read(m_InputXAxis, data);
}


/// @brief       Reads vertical input axis
/// @param data  json to read from
void JetpackBoost::readYAxisInput(nlohmann::ordered_json const& data)
{
    Stream::Read(m_InputYAxis, data);
}


/// @brief       Reads the flame offset
/// @param data  json to read from
void JetpackBoost::readOffset(nlohmann::ordered_json const& data)
{
    m_Offset = glm::vec4( Stream::Read<2, float>(data), 0, 1 );
}


/// @brief       Reads maximum tilt angle
/// @param data  json to read from
void JetpackBoost::readMaxAngle(nlohmann::ordered_json const& data)
{
    m_MaxAngle = Stream::Read<float>(data);
}


/// @brief       Reads tilt angle speed
/// @param data  json to read from
void JetpackBoost::readAngleSpeed(nlohmann::ordered_json const& data)
{
    m_AngleSpeed = Stream::Read<float>(data);
}


/// @brief	write to json
nlohmann::ordered_json JetpackBoost::Write() const
{
    nlohmann::ordered_json data;

    data[ "YAxisInputAction" ] = Stream::Write( m_InputYAxis );
    data[ "XAxisInputAction" ] = Stream::Write( m_InputXAxis );
    data[ "FlameOffset" ] = Stream::Write( glm::vec2(m_Offset) );
    data[ "MaxTiltRad" ] = Stream::Write( m_MaxAngle );
    data[ "TiltSpeed" ] = Stream::Write( m_AngleSpeed );

    return data;
}

