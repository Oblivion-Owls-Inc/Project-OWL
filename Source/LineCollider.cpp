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
	ColliderLineSegment temp;
	temp.point[0] = *p0;
	temp.point[1] = *p1;
	m_lineSegments.push_back(temp);
}

void LineCollider::AddLineSegment(vec2 p0, vec2 p1)
{
	ColliderLineSegment temp;
	temp.point[0] = p0;
	temp.point[1] = p1;
	m_lineSegments.push_back(temp);
}

bool LineCollider::CheckIfColliding(const Collider* other)
{
	return false;
}

void LineCollider::OnFixedUpdate()
{
}
