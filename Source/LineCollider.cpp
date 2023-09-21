#include "LineCollider.h"

LineCollider::LineCollider(): Collider()
{
	setColliderType(ColliderType::ColliderTypeLine);
}

Component* LineCollider::Clone() const
{
	return nullptr;
}

void LineCollider::AddLineSegment(const vec2* p0, const vec2* p1)
{
}

bool LineCollider::IsColliding(const Collider* other)
{
	return false;
}

void LineCollider::addLineSegment(const vec2* p0, const vec2* p1)
{
}

void LineCollider::OnFixedUpdate()
{
}
