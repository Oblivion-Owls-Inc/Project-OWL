/*********************************************************************
* \file         CameraBehavior.cpp
* \author       Eli Tsereteli (ilya.tsereteli@digipen.edu)
* \date         April 2024
* \copyright    Copyright (c) 2024 Digipen Institute of Technology
*
* \brief        Makes camera smoothly follow specified entity.
*********************************************************************/


#include "pch.h" // precompiled header has to be included first

#include "BehaviorSystem.h"
#include "CameraBehavior.h"
#include "CameraSystem.h"
#include "EntitySystem.h"
#include "Entity.h"
#include "Transform.h"
#include "Engine.h"

#include "InputSystem.h"
#include "PlatformSystem.h"

#include "ComponentReference.t.h"

/// @brief  Default constructor
CameraBehavior::CameraBehavior() : Behavior(typeid(CameraBehavior)) {}

/// @brief  Return a copy of this component
CameraBehavior* CameraBehavior::Clone() const { return new CameraBehavior(*this); }

/// @brief  copy ctor
CameraBehavior::CameraBehavior( CameraBehavior const& other) :
    Behavior( other ),
    m_xBounds( other.m_xBounds ),
    m_yBounds( other.m_yBounds ),
    m_Snappiness ( other.m_Snappiness  )
{}



//-----------------------------------------------------------------------------
//              Overrides
//-----------------------------------------------------------------------------

/// @brief  Adds itself to behavior system, caches the transform
void CameraBehavior::OnInit() 
{
	Behaviors< Behavior >()->AddComponent(this);

	m_Cam.Init( GetEntity() );
	m_Transform.Init( GetEntity() );

	m_TargetTransform.Init( GetEntity()->GetParent() );

	if (m_TargetTransform)
		m_TargetOldPos = m_TargetTransform->GetTranslation();
}


/// @brief  Removes itself from behavior system
void CameraBehavior::OnExit() 
{ 
	m_Cam      .Exit();
	m_Transform.Exit();

	m_TargetTransform.Exit();

	Behaviors< Behavior >()->RemoveComponent(this); 
}


/// @brief  Calls update on fixed intervals
void CameraBehavior::OnFixedUpdate()
{
	if ( !Platform()->GetVsyncOn() )
		update( GameEngine()->GetFixedFrameDuration() );
}

/// @brief  Calls update on fixed intervals
void CameraBehavior::OnUpdate(float dt)
{
	if ( Platform()->GetVsyncOn() )
		update( dt );
}


/// @brief  Performs the smooth following. Executed by either OnUpdate or
///         OnFixedUpdate, depending on VSync status.
void CameraBehavior::update(float dt)
{
	if (!m_Cam || !m_Transform || !m_TargetTransform)
		return;

	// move
	glm::vec2 pos = m_Transform->GetTranslation(),
		tpos = m_TargetTransform->GetMatrix()[3];
	glm::vec2 vel = tpos - m_TargetOldPos;
	m_TargetOldPos = tpos;
	tpos += vel * m_Lead;

	// accurate smooth lerp equation
	pos = lerp(tpos, pos, std::exp2(dt * -m_Snappiness));

	// clamp to bounds
	clampOrCenter(pos.y, m_yBounds[0], m_yBounds[1], m_Cam->GetHeight());
	clampOrCenter(pos.x, m_xBounds[0], m_xBounds[1], m_Cam->GetWidth());

	m_Transform->SetTranslation(pos);
}


/// @brief  Tweak properties in debug window
void CameraBehavior::Inspector()
{
	ImGui::Text("Bounds:");
	ImGui::DragFloat2("left / right", &m_xBounds[0], 0.01f);
	ImGui::DragFloat2("bottom / top", &m_yBounds[0], 0.01f);
	ImGui::Spacing();
	ImGui::DragFloat( "Snappiness", &m_Snappiness, 0.05f, 0.0f, INFINITY );
	ImGui::DragFloat( "Lead" , &m_Lead, 0.05f, 0.0f, INFINITY );
}


/// @brief  What to do when entity is re-parented
void CameraBehavior::OnHierarchyChange(Entity * previousParent)
{
	// re-init parent's transform
	m_TargetTransform.Exit();
	m_TargetTransform.Init( GetEntity()->GetParent() );
}


//-----------------------------------------------------------------------------
//              Helpers
//-----------------------------------------------------------------------------

/// @brief		 Helper: clamps or centers coordinate between given bounds, depending
///			     on distance
/// @param val	 Value to clamp/center
/// @param lo	 Lower bound
/// @param hi	 Upper bound
/// @param dist  Distance to fit within bounds
void CameraBehavior::clampOrCenter(float& val, float lo, float hi, float dist)
{
	if (lo != hi)
	{
		if (hi - lo > dist)
		{
			// clamp by edges of the camera
			dist *= 0.5f;
			val = std::clamp(val, lo + dist, hi - dist);
		}
		else  // if camera doesn't fit within bounds, just center it between them
			val = (lo + hi) * 0.5f;
	}
}


//-----------------------------------------------------------------------------
//              Reading / Writing
//-----------------------------------------------------------------------------

/// @brief read method map
ReadMethodMap<CameraBehavior> const CameraBehavior::s_ReadMethods =
{
	{ "XBounds",     &readXBounds },
	{ "YBounds",     &readYBounds },
    { "Snappiness",  &readFactor  },
	{ "Lead",        &readLead    }
};

/// @brief		 Reads the horizontal bounds
/// @param data  json data to read
void CameraBehavior::readXBounds(nlohmann::ordered_json const& data)
{
	m_xBounds[0] = data[0];
	m_xBounds[1] = data[1];
}

/// @brief		 Reads the vertical bounds
/// @param data  json data to read
void CameraBehavior::readYBounds(nlohmann::ordered_json const& data)
{
	m_yBounds[0] = data[0];
	m_yBounds[1] = data[1];
}

/// @brief  reads the follow factor
/// @param  data    the JSON data to read from
void CameraBehavior::readFactor( nlohmann::ordered_json const& data )
{
    Stream::Read( m_Snappiness, data );
}

/// @brief  reads the follow factor
/// @param  data    the JSON data to read from
void CameraBehavior::readLead(nlohmann::ordered_json const& data)
{
	Stream::Read( m_Lead, data );
}


/// @brief	write to json
nlohmann::ordered_json CameraBehavior::Write() const
{
	nlohmann::ordered_json data;

	data["XBounds"] = m_xBounds;
	data["YBounds"] = m_yBounds;
    data["Snappiness"] = Stream::Write( m_Snappiness );
	data["Lead"] = Stream::Write( m_Lead );

	return data;
}


