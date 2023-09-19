#include "RigidBody.h"
#include "BehaviorSystem.h"

RigidBody::RigidBody() : 
	_velocity(vec3(0, 0, 0)),
	_acceleration(vec3(1, 1, 0)),
	_oldTranslation(vec3(0, 0, 0)),
	_rotationalVelocity(0), 
	Behavior(typeid(RigidBody))
{
	BehaviorSystem<RigidBody>::getInstance()->AddBehavior(this);
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
	BehaviorSystem<RigidBody>::getInstance()->RemoveBehavior(this);
}

void RigidBody::OnUpdate(float dt)
{
	vec3 temptranslation(0);
	Transform* transform = (Transform *)Parent()->HasComponent(typeid(Transform));

	_oldTranslation = *transform->getTranslation();
	_velocity += _acceleration * dt;
	temptranslation = *transform->getTranslation() + (_velocity * dt);

	float rotation = transform->getRotation();
	rotation += _rotationalVelocity * dt;

	transform->setRotation(rotation);
	transform->setTranslation(temptranslation);

}

Component* RigidBody::Clone() const
{
	return (Component*)new RigidBody(*this);
}

vec3* RigidBody::getAcceleration()
{
	return (vec3*) &_acceleration;
}

vec3* RigidBody::getVelocity()
{
	return (vec3*)&_velocity;
}

vec3* RigidBody::getOldTranslation()
{
	return (vec3*)&_oldTranslation;
}

float RigidBody::getRotationalVelocity()
{
	return _rotationalVelocity;
}

void RigidBody::setAcceleration(const vec3* Acceleration)
{
	Acceleration = &_acceleration;
}

void RigidBody::setVelocity(const vec3* Velocity)
{
	Velocity = &_velocity;
}

void RigidBody::setOldTranslation(const vec3* OldTranslation)
{
	OldTranslation = &_oldTranslation;
}


void RigidBody::SetRotationalVelocity(float rotational_velocity)
{
	_rotationalVelocity = rotational_velocity;
}

void RigidBody::OnCollision(Entity* other)
{
	(void)other;
}

//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------

/// @brief reads the velocity from json
/// @param data the json data
void RigidBody::ReadVelocity( Stream data )
{
    _velocity = data.Read< glm::vec3 >();
}

/// @brief reads the acceleration from json
/// @param data the json data
void RigidBody::ReadAcceleration( Stream data )
{
    _acceleration = data.Read< glm::vec3 >();
}

/// @brief reads the rotationalVelocity from json
/// @param data the json data
void RigidBody::ReadRotationalVelocity( Stream data )
{
    _rotationalVelocity = data.Read< float >();
}

/// @brief the map of read methods for RigidBodys
ReadMethodMap< RigidBody > RigidBody::readMethods = {
    { "velocity",           &ReadVelocity           },
    { "acceleration",       &ReadAcceleration       },
    { "rotationalVelocity", &ReadRotationalVelocity }
};

/// @brief gets the map of read methods for this Component
/// @return the map of read methods for this Component
ReadMethodMap< Component > const& RigidBody::getReadMethods()
{
    return (ReadMethodMap< Component > const&)readMethods;
}

//-----------------------------------------------------------------------------
