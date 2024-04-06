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

    m_Transform.Init( GetEntity() );
    m_Flame.Init( GetEntity() );
    m_PTransform.Init( GetEntity()->GetParent() );
    m_RBody.Init( GetEntity()->GetParent() );
}

/// @brief  Removes itself from behavior system
void JetpackBoost::OnExit() 
{ 
    Behaviors< Behavior >()->RemoveComponent(this);
    m_Transform.Exit();
    m_Flame.Exit();
    m_PTransform.Exit();
    m_RBody.Exit();
}


/// @brief  
void JetpackBoost::OnUpdate(float dt)
{
    
}


/// @brief  Tweak properties in debug window
void JetpackBoost::Inspector()
{
    
}


/// @brief  What to do when entity is re-parented
void JetpackBoost::OnHierarchyChange(Entity* previousParent)
{
    // re-init
    OnExit();
    OnInit();
}


//-----------------------------------------------------------------------------
//              Reading / Writing
//-----------------------------------------------------------------------------

/// @brief read method map
ReadMethodMap<JetpackBoost> const JetpackBoost::s_ReadMethods =
{
//    { "Thing"               , &JetpackBoost::readThing }
};

#if 0
/// @brief		 Reads thing
/// @param data  json data to read
void JetpackBoost::readThing(nlohmann::ordered_json const& data)
{
    Stream::Read(m_Thing, data);
}
#endif


/// @brief	write to json
nlohmann::ordered_json JetpackBoost::Write() const
{
    nlohmann::ordered_json data;

    //data[ "Thing" ] = Stream::Write( m_Thing );

    return data;
}

