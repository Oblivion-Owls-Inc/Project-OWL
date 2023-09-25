///@file Collider.cpp
///@Author Jax Clayton (jax.clayton@digipen.edu)

#include "Collider.h"
#include "CollisionSystem.h"

Collider::Collider(): Component(typeid(Collider)), mtype(), mIsColliding(false)
{
	CollisionSystem::GetInstance()->addCollider(this);

}

Collider::Collider(const Collider& other) : Component(other), mtype(other.mtype)
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
	mOther = other;
}

Collider* Collider::getOtherCollider()
{
	return mOther;
}

bool Collider::isColliding()
{
	return mIsColliding;
}

void Collider::isColliding(bool colliding)
{
	mIsColliding = colliding;
}

void Collider::setColliderType(ColliderType cType)
{
	mtype = cType;
}


/// @brief the map of read methods for Collider
ReadMethodMap< Collider > Collider::readMethods = 
{
	{ "ColliderType",		&ReadColliderType },

};

std::map<std::string, ReadMethod<Component>> const& Collider::getReadMethods()
{
	return (ReadMethodMap< Component > const&)readMethods;
}

void Collider::ReadColliderType(Stream data)
{
	mtype = (ColliderType)data.Read<int>();
}
