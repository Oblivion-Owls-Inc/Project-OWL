///@file Collider.cpp
///@Author Jax Clayton (jax.clayton@digipen.edu)

#include "Collider.h"

Collider::Collider(): Component(typeid(Collider)), mtype(), mIsColliding(false)
{
}

Collider::Collider(const Collider& other) : Component(other), mtype(other.mtype)
{
}

Component* Collider::Clone() const
{
	return nullptr;
}
void Collider::OnFixedUpdate()
{
}

bool Collider::checkCollisionType(const Collider* other)
{
	return false;
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

