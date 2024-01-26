
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
Component* CameraBehavior::Clone() const {return new CameraBehavior(*this); }

/// @brief  copy ctor
CameraBehavior::CameraBehavior(const CameraBehavior& other) : Behavior(other) {}


void CameraBehavior::OnInit() { Behaviors< Behavior >()->AddComponent(this); }

void CameraBehavior::OnExit() { Behaviors< Behavior >()->RemoveComponent(this); }

void CameraBehavior::OnUpdate(float dt)
{
	// Acquire: camera, transform, and target transform.
	// Slight overhead, less bugs to worry about.
	Camera* cam = GetEntity()->GetComponent<Camera>();
	Transform* tr = GetEntity()->GetComponent<Transform>();
	if (!cam || !tr)	return;
	Entity* target = Entities()->GetEntity(m_TargetEntityName);
	if (!target)		return;
	Transform* target_tr = target->GetComponent<Transform>();
	if (!target_tr)		return;

	// move
	glm::vec2 pos = tr->GetTranslation(), prev_pos = pos,
			  tpos = target_tr->GetTranslation();

	pos += (tpos - pos) * dt;

	// clamp to bounds
	clampOrCenter(pos.y, m_yBounds[0], m_yBounds[1], cam->GetHeight());
	clampOrCenter(pos.x, m_xBounds[0], m_xBounds[1], cam->GetWidth());

	// don't move if really close already. Moving camera too slowly causes texture 
	// flickering due to aliasing
	glm::vec2 change = prev_pos - pos;
	if (glm::dot(change, change) > dt*dt)
		tr->SetTranslation( pos );
}


void CameraBehavior::Inspector()
{
	ImGui::InputText("Target Entity", &m_TargetEntityName);
	ImGui::DragFloat2("X bounds", &m_xBounds[0], 0.01f);
	ImGui::DragFloat2("Y bounds", &m_yBounds[0], 0.01f);
}

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


/// @brief read method map
ReadMethodMap<CameraBehavior> const CameraBehavior::s_ReadMethods =
{
	{ "TargetEntityName",	&readTargetEntityName},
	{ "XBounds",			&readXBounds		 },
	{ "YBounds",			&readYBounds		 },
};


void CameraBehavior::readTargetEntityName(nlohmann::ordered_json const& data)
{
	Stream::Read(m_TargetEntityName, data);
}

void CameraBehavior::readXBounds(nlohmann::ordered_json const& data)
{
	m_xBounds[0] = data[0];
	m_xBounds[1] = data[1];
}

void CameraBehavior::readYBounds(nlohmann::ordered_json const& data)
{
	m_yBounds[0] = data[0];
	m_yBounds[1] = data[1];
}


/// @brief	write to json
nlohmann::ordered_json CameraBehavior::Write() const
{
	nlohmann::ordered_json data;



	return data;
}


