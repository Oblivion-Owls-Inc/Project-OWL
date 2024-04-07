/*********************************************************************
* \file   JetpackBoost.cpp
* \brief  s
*
* \author Eli Tsereteli
*********************************************************************/

#include "pch.h" // precompiled header has to be included first
#include "BehaviorSystem.h"
#include "JetpackBoost.h"
#include "ComponentReference.t.h"

#include "Entity.h"
#include "Emitter.h"
#include "Transform.h"
#include "RigidBody.h"
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

    //  own components
    m_Transform.Init( GetEntity() );
    m_Flame.Init( GetEntity() );

    //  parent components
    Entity* parent = GetEntity()->GetParent();
    if (parent)
    {
        m_PTransform.Init(parent);
        m_RBody.Init(parent);
    }

    //  input
    m_InputYAxis.SetOwnerName( GetName() );
    m_InputXAxis.SetOwnerName( GetName() );
    m_InputYAxis.Init();
    m_InputXAxis.Init();
}

/// @brief  Removes itself from behavior system
void JetpackBoost::OnExit() 
{ 
    Behaviors< Behavior >()->RemoveComponent(this);
    m_Transform.Exit();
    m_Flame.Exit();
    m_PTransform.Exit();
    m_RBody.Exit();
    m_InputXAxis.Exit();
    m_InputYAxis.Exit();
}


/// @brief  
void JetpackBoost::OnUpdate(float dt)
{
    if (!m_Transform || !m_Flame || !m_RBody || !m_PTransform)
        return;

    // which way is player facing   TODO: does x axis input do the same thing?
    float xscale = m_PTransform->GetScale().x;

    // goin up
    if (m_InputYAxis->GetAxis() > 0.0f)
    {
        m_Flame->SetContinuous(true);

        // tilt when trying to move left/right   TODO: velocity threshold?
        if (m_InputXAxis->GetAxis())
        {
            if (m_Angle < 0.5f)     m_Angle += dt*2;
            else                    m_Angle = 0.5f;
        }
        else
        {
            if (m_Angle > 0.0f)     m_Angle -= dt*2;
            else                    m_Angle = 0.0f;
        }
    }
    // not goin up
    else
    {
        m_Flame->SetContinuous(false);
        if (m_Angle > 0.0f)     m_Angle -= dt*2;
        else                    m_Angle = 0.0f;
    }

    // rotate player and particle direction
    float angle = m_Angle * xscale;
    m_PTransform->SetRotation( angle );
    ParticleSystem::EmitData fdata = m_Flame->GetEmitData();
    fdata.direction = downAngle + angle;
    m_Flame->SetEmitData(fdata);

    // align particles with jetpack
    glm::vec2 pos = m_PTransform->GetTranslation();
    pos.x += xscale * 0.1f;
    m_Transform->SetTranslation(pos);
}


/// @brief  Tweak properties in debug window
void JetpackBoost::Inspector()
{
    
}


/// @brief  What to do when entity is re-parented
void JetpackBoost::OnHierarchyChange(Entity* previousParent)
{
    // re-init
    //OnExit();
    //OnInit();
}


//-----------------------------------------------------------------------------
//              Reading / Writing
//-----------------------------------------------------------------------------

/// @brief read method map
ReadMethodMap<JetpackBoost> const JetpackBoost::s_ReadMethods =
{
    { "XAxisInputAction"               , &JetpackBoost::readXAxisInput },
    { "YAxisInputAction"               , &JetpackBoost::readYAxisInput }
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


/// @brief	write to json
nlohmann::ordered_json JetpackBoost::Write() const
{
    nlohmann::ordered_json data;

    data[ "YAxisInputAction" ] = Stream::Write( m_InputYAxis );
    data[ "XAxisInputAction" ] = Stream::Write( m_InputXAxis );

    return data;
}

