#include "LinesCollider.h"

LinesCollider::LinesCollider()
{
}

Component* LinesCollider::Clone() const
{
	return nullptr;
}

void LinesCollider::AddLineSegment(const vec2* p0, const vec2* p1)
{
}

bool LinesCollider::IsColliding(const Collider* other)
{
	return false;
}
