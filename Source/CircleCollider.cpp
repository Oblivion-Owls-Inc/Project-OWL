#include "CircleCollider.h"
#include "Collider.h"

CircleCollider::CircleCollider(): Collider(), radius(0.0f)
{
	setColliderType(ColliderType::ColliderTypeCircle);
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

void CircleCollider::OnFixedUpdate()
{
}
