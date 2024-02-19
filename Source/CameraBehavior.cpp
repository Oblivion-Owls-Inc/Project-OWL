/*********************************************************************
* \file   CameraBehavior.cpp
* \brief  Camera that smoothly follows specified entity.
*
* \author Eli Tsereteli
*********************************************************************/
#include "BehaviorSystem.h"
#include "CameraBehavior.h"
#include "CameraSystem.h"
#include "EntitySystem.h"
#include "Entity.h"
#include "Transform.h"

#include "InputSystem.h"

/// @brief  Default constructor
CameraBehavior::CameraBehavior() : Behavior(typeid(CameraBehavior)) {}

/// @brief  Return a copy of this component
CameraBehavior* CameraBehavior::Clone() const { return new CameraBehavior(*this); }

/// @brief  copy ctor
CameraBehavior::CameraBehavior(const CameraBehavior& other) : Behavior(other) {}



//-----------------------------------------------------------------------------
//              Overrides
//-----------------------------------------------------------------------------

/// @brief  Adds itself to behavior system, caches the transform
void CameraBehavior::OnInit() 
{
	Behaviors< Behavior >()->AddComponent(this);

	m_Cam.Init( GetEntity() );
	m_Transform.Init( GetEntity() );

	m_ParentTransform.Init( GetEntity()->GetParent() );
}


/// @brief  Removes itself from behavior system
void CameraBehavior::OnExit() 
{ 
	m_Cam      .Exit();
	m_Transform.Exit();

	m_ParentTransform.Exit();

	Behaviors< Behavior >()->RemoveComponent(this); 
}


/// @brief  Performs the smooth following
void CameraBehavior::OnUpdate(float dt)
{
	if (!m_Cam || !m_Transform || !m_ParentTransform)
		return;

	// move
	glm::vec2 pos = m_Transform->GetTranslation(), prev_pos = pos,
			  tpos = m_ParentTransform->GetTranslation();

	pos += (tpos - pos) * dt * m_Factor;

	// clamp to bounds
	clampOrCenter(pos.y, m_yBounds[0], m_yBounds[1], m_Cam->GetHeight());
	clampOrCenter(pos.x, m_xBounds[0], m_xBounds[1], m_Cam->GetWidth());

	// This shouldn't be customizable. It's here to prevent aliasing.
	// However, combining it with 'speed' determines how lazy it is
	// with following the target. So I call it 'follow factor'.
	glm::vec2 change = prev_pos - pos;
	if (glm::dot(change, change) > dt*dt)
		m_Transform->SetTranslation( pos );
}


/// @brief  Tweak properties in debug window
void CameraBehavior::Inspector()
{
	ImGui::DragFloat2("X bounds", &m_xBounds[0], 0.01f);
	ImGui::DragFloat2("Y bounds", &m_yBounds[0], 0.01f);
	ImGui::Spacing();
	ImGui::SliderFloat("Follow factor", &m_Factor, 0.0f, 5.0f, "%.2f");
}


/// @brief  What to do when entity is re-parented
void CameraBehavior::OnHierarchyChange(Entity * previousParent)
{
	// re-init parent's transform
	m_ParentTransform.Exit();
	m_ParentTransform.Init( GetEntity()->GetParent() );
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
	{ "XBounds", &readXBounds },
	{ "YBounds", &readYBounds },
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


/// @brief	write to json
nlohmann::ordered_json CameraBehavior::Write() const
{
	nlohmann::ordered_json data;

	data["XBounds"] = m_xBounds;
	data["YBounds"] = m_yBounds;

	return data;
}


