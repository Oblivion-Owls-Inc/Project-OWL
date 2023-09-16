#include "RidgedBody.h"


RidgedBody::RidgedBody() : 
	_velocity(vec3(0, 0, 0)),
	_acceleration(vec3(0, 0, 0)),
	_translation(vec3(0, 0, 0)),
	_oldTranslation(vec3(0, 0, 0)),
	_rotationalVelocity(0), 
	Behavior(typeid(RidgedBody))
{
}

RidgedBody::RidgedBody(const RidgedBody& other) : Behavior(typeid(RidgedBody))
{
	_rotationalVelocity = other._rotationalVelocity;
	_acceleration = other._acceleration;
	_translation = other._translation;
	_oldTranslation = other._oldTranslation;
	_velocity = other._velocity;
}

RidgedBody::~RidgedBody()
{

}

void RidgedBody::FixedUpdate(float dt)
{
	vec3 temptranslation(0);
	Transform* transform = (Transform *)Parent()->HasComponent(typeid(Transform));

	_oldTranslation = *transform->getTranslation();
	_velocity += _acceleration * dt;
	temptranslation = _translation + (_velocity * dt);

	float rotation = transform->getRotation();
	rotation += _rotationalVelocity * dt;

	transform->setRotation(rotation);
	transform->setTranslation(temptranslation);

}

Component* RidgedBody::Clone() const
{
	return (Component*)new RidgedBody(*this);
}

vec3* RidgedBody::getAcceleration()
{
	return (vec3*) &_acceleration;
}

vec3* RidgedBody::getVelocity()
{
	return (vec3*)&_velocity;
}

vec3* RidgedBody::getOldTranslation()
{
	return (vec3*)&_oldTranslation;
}

vec3* RidgedBody::getTranslation()
{
	return (vec3*)&_translation;
}

float RidgedBody::getRotationalVelocity()
{
	return _rotationalVelocity;
}

void RidgedBody::setAcceleration(const vec3* Acceleration)
{
	Acceleration = &_acceleration;
}

void RidgedBody::setVelocity(const vec3* Velocity)
{
	Velocity = &_velocity;
}

void RidgedBody::setOldTranslation(const vec3* OldTranslation)
{
	OldTranslation = &_oldTranslation;
}

void RidgedBody::setTranslation(const vec3* Translation)
{
	Translation = &_translation;
}

void RidgedBody::SetRotationalVelocity(float rotational_velocity)
{
	_rotationalVelocity = rotational_velocity;
}

void RidgedBody::CollisionEvent(Entity* other)
{
	(void)other;
}

