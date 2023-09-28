///@file Collider.cpp
///@Author Jax Clayton (jax.clayton@digipen.edu)

#include "Collider.h"
#include "CollisionSystem.h"

Collider::Collider(): Component(typeid(Collider)), m_Type(), m_IsColliding(false)
{
	CollisionSystem::GetInstance()->addCollider(this);

}

Collider::Collider(const Collider& other) : Component(other), m_Type(other.m_Type)
{
	CollisionSystem::GetInstance()->removeCollider(this);
}

Component* Collider::Clone() const
{
	return nullptr;
}

void Collider::OnFixedUpdate()
{
}


void Collider::setOtherCollider(Collider* other)
{
	m_Other = other;
}

Collider* Collider::getOtherCollider()
{
	return m_Other;
}

bool Collider::isColliding()
{
	return m_IsColliding;
}

void Collider::isColliding(bool colliding)
{
	m_IsColliding = colliding;
}

void Collider::setColliderType(ColliderType cType)
{
	m_Type = cType;
}


/// @brief the map of read methods for Collider
ReadMethodMap< Collider > Collider::s_ReadMethods = 
{
	{ "ColliderType",		&ReadColliderType },

};

std::map<std::string, ReadMethod<Component>> const& Collider::getReadMethods()
{
	return (ReadMethodMap< Component > const&)s_ReadMethods;
}

void Collider::ReadColliderType(Stream data)
{
	m_Type = (ColliderType)data.Read<int>();
}
