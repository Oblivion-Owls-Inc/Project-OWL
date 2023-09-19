#include "RigidBody.h"
#include "Engine.h"


RigidBody::RigidBody() : 
	_velocity(glm::vec3(0, 0, 0)),
	_acceleration(glm::vec3(0, 0, 0)),
	_translation(glm::vec3(0, 0, 0)),
	_oldTranslation(glm::vec3(0, 0, 0)),
	_rotationalVelocity(0), 
	Behavior(typeid(RigidBody))
{}

RigidBody::RigidBody(const RigidBody& other) : Behavior(typeid(RigidBody))
{
	_rotationalVelocity = other._rotationalVelocity;
	_acceleration = other._acceleration;
	_translation = other._translation;
	_oldTranslation = other._oldTranslation;
	_velocity = other._velocity;
}

RigidBody::~RigidBody()
{

}

void RigidBody::OnFixedUpdate()
{
    float dt = Engine::getInstance()->getFixedFrameDuration();

	glm::vec3 temptranslation(0);
	Transform* transform = (Transform *)Parent()->HasComponent(typeid(Transform));

	_oldTranslation = *transform->getTranslation();
	_velocity += _acceleration * dt;
	temptranslation = _translation + (_velocity * dt);

	float rotation = transform->getRotation();
	rotation += _rotationalVelocity * dt;

	transform->setRotation(rotation);
	transform->setTranslation(temptranslation);

}

Component* RigidBody::Clone() const
{
	return (Component *)new RigidBody(*this);
}

glm::vec3* RigidBody::getAcceleration()
{
	return (glm::vec3*) &_acceleration;
}

glm::vec3* RigidBody::getVelocity()
{
	return (glm::vec3*)&_velocity;
}

glm::vec3* RigidBody::getOldTranslation()
{
	return (glm::vec3*)&_oldTranslation;
}

glm::vec3* RigidBody::getTranslation()
{
	return (glm::vec3*)&_translation;
}

float RigidBody::getRotationalVelocity()
{
	return _rotationalVelocity;
}

void RigidBody::setAcceleration(const glm::vec3* Acceleration)
{
	Acceleration = &_acceleration;
}

void RigidBody::setVelocity(const glm::vec3* Velocity)
{
	Velocity = &_velocity;
}

void RigidBody::setOldTranslation(const glm::vec3* OldTranslation)
{
	OldTranslation = &_oldTranslation;
}

void RigidBody::setTranslation(const glm::vec3* Translation)
{
	Translation = &_translation;
}

void RigidBody::SetRotationalVelocity(float rotational_velocity)
{
	_rotationalVelocity = rotational_velocity;
}

void RigidBody::CollisionEvent( Entity* other )
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
