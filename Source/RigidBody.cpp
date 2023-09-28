///*****************************************************************/
/// @file	 RigidBody.cpp
/// @Author  Jax Clayton (jax.clayton@digipen.edu)
/// @date	 9/15/2021
/// @brief   RigidBody  
/// @details Contains Basic Physics for the Entities
///*****************************************************************/
#include "RigidBody.h"
#include "BehaviorSystem.h"
#include "DebugSystem.h"

static int GravityForce = 1;

RigidBody::RigidBody() : 
	m_Velocity(vec3(1, 1, 1)),
	m_Acceleration(vec3(1, 1, 0)),
	m_OldTranslation(vec3(0, 0, 0)),
	m_RotationalVelocity(0), 
	Behavior(typeid(RigidBody))
{
	BehaviorSystem<RigidBody>::GetInstance()->AddBehavior(this);
}

RigidBody::RigidBody(const RigidBody& other) : Behavior(typeid(RigidBody))
{
	m_RotationalVelocity = other.m_RotationalVelocity;
	m_Acceleration = other.m_Acceleration;
	m_OldTranslation = other.m_OldTranslation;
	m_Velocity = other.m_Velocity;
}

RigidBody::~RigidBody()
{
	BehaviorSystem<RigidBody>::GetInstance()->RemoveBehavior(this);
}

void RigidBody::OnUpdate(float dt)
{
	Collider* collider = Parent()->GetComponent<Collider>();
	vec3 temptranslation(0);
	Transform* transform = (Transform *)Parent()->GetComponent<Transform>();

	m_OldTranslation = *transform->GetTranslation();
	temptranslation = *transform->GetTranslation();

	// Gravity
	static float gravity = 9.81f; // Adjust this value as needed
	m_Velocity.y -= gravity * dt;

#ifndef NDEBUG
	ImGui::Begin("Gravity");
	ImGui::InputFloat("Gravity", &gravity);
	ImGui::End();
#endif // !NDEBUG


	if (m_Velocity.x > 45 || m_Velocity.y > 45)
	{
		m_Velocity.x = 45;
		m_Velocity.y = 45;
	}
	temptranslation += m_Velocity * dt;
	temptranslation.z = 0;

	float rotation = transform->GetRotation();
	rotation += m_RotationalVelocity;

	transform->SetRotation(rotation);
	transform->SetTranslation(temptranslation);
}

Component* RigidBody::Clone() const
{
	return (Component*) new RigidBody(*this);
}

vec3* RigidBody::GetAcceleration()
{
	return (vec3*) &m_Acceleration;
}

vec3* RigidBody::GetVelocity()
{
	return (vec3*)&m_Velocity;
}

vec3* RigidBody::GetOldTranslation()
{
	return (vec3*)&m_OldTranslation;
}

float RigidBody::GetRotationalVelocity()
{
	return m_RotationalVelocity;
}

void RigidBody::SetAcceleration(const vec3* Acceleration)
{
	m_Acceleration = *Acceleration;
}

void RigidBody::SetVelocity(const vec3* Velocity)
{
		m_Velocity = *Velocity;
}

void RigidBody::SetOldTranslation(const vec3* OldTranslation)
{
	m_OldTranslation = *OldTranslation;
}


void RigidBody::SetRotationalVelocity(float rotational_velocity)
{
	m_RotationalVelocity = rotational_velocity;
}

void RigidBody::OnCollisionEvent()
{
	DebugConsole output(*DebugSystem::GetInstance());
	output << Parent()->GetName().c_str() << ":Collision Detected in RigidBody" << "\n";
}

//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------

/// @brief reads the velocity from json
/// @param data the json data
void RigidBody::ReadVelocity(Stream data)
{
	m_Velocity = data.Read< glm::vec3 >();
}

/// @brief reads the acceleration from json
/// @param data the json data
void RigidBody::ReadAcceleration(Stream data)
{
	m_Acceleration = data.Read< glm::vec3 >();
}

/// @brief reads the rotationalVelocity from json
/// @param data the json data
void RigidBody::ReadRotationalVelocity(Stream data)
{
	m_RotationalVelocity = data.Read< float >();
}

/// @brief the map of read methods for RigidBodys
ReadMethodMap< RigidBody > RigidBody::s_ReadMethods = {
	{ "velocity"           , &ReadVelocity           },
	{ "acceleration"       , &ReadAcceleration       },
	{ "rotationalVelocity" , &ReadRotationalVelocity }
};

/// @brief gets the map of read methods for this Component
/// @return the map of read methods for this Component
ReadMethodMap< Component > const& RigidBody::GetReadMethods() const
{
	return (ReadMethodMap< Component > const&)s_ReadMethods;
}