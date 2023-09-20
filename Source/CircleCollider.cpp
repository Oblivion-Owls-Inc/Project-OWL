#include "CircleCollider.h"

CircleCollider::CircleCollider()
{
}

CircleCollider::CircleCollider(const CircleCollider& other)
{
}

CircleCollider::~CircleCollider()
{
}

Component* CircleCollider::Clone() const
{
	return nullptr;
}

bool CircleCollider::IsColliding(const Collider* other)
{
	return false;
}

void CircleCollider::SetRadius(float radius)
{
}

float CircleCollider::GetRadius() const
{
	return 0.0f;
}
