#include "RigidBody.h"
#include "BehaviorSystem.h"
#include "DebugSystem.h"

static int GravityForce = 1;

RigidBody::RigidBody() : 
	_velocity(vec3(0, 0, 0)),
	_acceleration(vec3(1, 1, 0)),
	_oldTranslation(vec3(0, 0, 0)),
	_rotationalVelocity(0), 
	Behavior(typeid(RigidBody))
{
	BehaviorSystem<RigidBody>::GetInstance()->AddBehavior(this);
}

RigidBody::RigidBody(const RigidBody& other) : Behavior(typeid(RigidBody))
{
	_rotationalVelocity = other._rotationalVelocity;
	_acceleration = other._acceleration;
	_oldTranslation = other._oldTranslation;
	_velocity = other._velocity;
}

RigidBody::~RigidBody()
{
	BehaviorSystem<RigidBody>::GetInstance()->RemoveBehavior(this);
}

void RigidBody::OnUpdate(float dt)
{
	vec3 temptranslation(0);
	Transform* transform = (Transform *)Parent()->HasComponent(typeid(Transform));

	_oldTranslation = *transform->GetTranslation();
	temptranslation = *transform->GetTranslation() + (_velocity);

	ImGui::Begin("RigidBody Editor");
	ImGui::DragInt("Gravity Force", &GravityForce, 1, 0, 100);
	ImGui::End();

	temptranslation.y = temptranslation.y + (_acceleration.y * dt * GravityForce);

	float rotation = transform->GetRotation();
	rotation += _rotationalVelocity;

	transform->SetRotation(rotation);
	transform->SetTranslation(temptranslation);

}

Component* RigidBody::Clone() const
{
	return (Component*) new RigidBody(*this);
}

vec3* RigidBody::GetAcceleration()
{
	return (vec3*) &_acceleration;
}

vec3* RigidBody::GetVelocity()
{
	return (vec3*)&_velocity;
}

vec3* RigidBody::GetOldTranslation()
{
	return (vec3*)&_oldTranslation;
}

float RigidBody::GetRotationalVelocity()
{
	return _rotationalVelocity;
}

void RigidBody::SetAcceleration(const vec3* Acceleration)
{
	Acceleration = &_acceleration;
}

void RigidBody::SetVelocity(const vec3* Velocity)
{
	Velocity = &_velocity;
}

void RigidBody::SetOldTranslation(const vec3* OldTranslation)
{
	OldTranslation = &_oldTranslation;
}


void RigidBody::SetRotationalVelocity(float rotational_velocity)
{
	_rotationalVelocity = rotational_velocity;
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
	_velocity = data.Read< glm::vec3 >();
}

/// @brief reads the acceleration from json
/// @param data the json data
void RigidBody::ReadAcceleration(Stream data)
{
	_acceleration = data.Read< glm::vec3 >();
}

/// @brief reads the rotationalVelocity from json
/// @param data the json data
void RigidBody::ReadRotationalVelocity(Stream data)
{
	_rotationalVelocity = data.Read< float >();
}

/// @brief the map of read methods for RigidBodys
ReadMethodMap< RigidBody > RigidBody::s_ReadMethods = {
	{ "velocity"           , &ReadVelocity           },
	{ "acceleration"       , &ReadAcceleration       },
	{ "rotationalVelocity" , &ReadRotationalVelocity }
};

/// @brief gets the map of read methods for this Component
/// @return the map of read methods for this Component
ReadMethodMap< Component > const& RigidBody::GetReadMethods()
{
	return (ReadMethodMap< Component > const&)s_ReadMethods;
}