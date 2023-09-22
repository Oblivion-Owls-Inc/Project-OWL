#include "CircleCollider.h"
#include "Collider.h"

CircleCollider::CircleCollider(): Collider(), radius(0.0f)
{
	setColliderType(ColliderType::ColliderTypeCircle);
}

CircleCollider::CircleCollider(const CircleCollider& other) : Collider(other), radius(other.radius)
{
	setColliderType(ColliderType::ColliderTypeCircle);
}

Component* CircleCollider::Clone() const
{
	return (Component *) new CircleCollider(*this);
}

bool CircleCollider::CheckIfColliding(const Collider* other)
{
	switch (other->getColliderType())
	{
		case ColliderType::ColliderTypeCircle:
		{
			return false;
		}
		case ColliderType::ColliderTypeLine:
		{
			return false;
		}
		default:
		{
			return false; // No collision detected
		}
	}
}

void CircleCollider::SetRadius(float _radius)
{
	radius = _radius;
}

float CircleCollider::GetRadius() const
{
	return radius;
}

void CircleCollider::OnFixedUpdate()
{
}

