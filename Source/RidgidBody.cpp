#include "RidgidBody.h"
#include "PhysicsSystem.h"

RidgidBody::RidgidBody() : 
	_velocity(vec3(0, 0, 0)),
	_acceleration(vec3(1, 1, 0)),
	_oldTranslation(vec3(0, 0, 0)),
	_rotationalVelocity(0), 
	Behavior(typeid(RidgidBody))
{
	PhysicsSystem::getInstance()->AddBehavior(this);
}

RidgidBody::RidgidBody(const RidgidBody& other) : Behavior(typeid(RidgidBody))
{
	_rotationalVelocity = other._rotationalVelocity;
	_acceleration = other._acceleration;
	_oldTranslation = other._oldTranslation;
	_velocity = other._velocity;
}

RidgidBody::~RidgidBody()
{
	PhysicsSystem::getInstance()->RemoveBehavior(this);
}

void RidgidBody::OnUpdate(float dt)
{
	//vec3 temptranslation(0);
	//Transform* transform = (Transform *)Parent()->HasComponent(typeid(Transform));

	//_oldTranslation = *transform->getTranslation();
	//_velocity += _acceleration * dt;
	//temptranslation = *transform->getTranslation() + (_velocity * dt);

	//float rotation = transform->getRotation();
	//rotation += _rotationalVelocity * dt;

	//transform->setRotation(rotation);
	//transform->setTranslation(temptranslation);

}

Component* RidgidBody::Clone() const
{
	return (Component*)new RidgidBody(*this);
}

vec3* RidgidBody::getAcceleration()
{
	return (vec3*) &_acceleration;
}

vec3* RidgidBody::getVelocity()
{
	return (vec3*)&_velocity;
}

vec3* RidgidBody::getOldTranslation()
{
	return (vec3*)&_oldTranslation;
}

float RidgidBody::getRotationalVelocity()
{
	return _rotationalVelocity;
}

void RidgidBody::setAcceleration(const vec3* Acceleration)
{
	Acceleration = &_acceleration;
}

void RidgidBody::setVelocity(const vec3* Velocity)
{
	Velocity = &_velocity;
}

void RidgidBody::setOldTranslation(const vec3* OldTranslation)
{
	OldTranslation = &_oldTranslation;
}


void RidgidBody::SetRotationalVelocity(float rotational_velocity)
{
	_rotationalVelocity = rotational_velocity;
}

void RidgidBody::CollisionEvent(Entity* other)
{
	(void)other;
}

