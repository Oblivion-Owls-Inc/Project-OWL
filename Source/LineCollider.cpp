#include "glm/glm.hpp"
#include "Entity.h"
#include "Sprite.h"
#include "RigidBody.h"
#include "Transform.h"

#include "LineCollider.h"
#include "CircleCollider.h"

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
	ColliderLineSegment temp{};
	temp.point[0] = *p0;
	temp.point[1] = *p1;
	m_lineSegments.push_back(temp);
	lineCount++;
}

void LineCollider::AddLineSegment(vec2 p0, vec2 p1)
{
	ColliderLineSegment temp{};
	temp.point[0] = p0;
	temp.point[1] = p1;
	m_lineSegments.push_back(temp);
    lineCount++;
}

bool LineCollider::CheckIfColliding(const Collider* other)
{
    switch (other->getColliderType())
    {
        case ColliderType::ColliderTypeCircle:
        {
            const CircleCollider* circleCollider = dynamic_cast<const CircleCollider*>(other);

            if (circleCollider)
            {
                Transform* lineTransform = Parent()->GetComponent<Transform>();
                Transform* circleTransform = other->Parent()->GetComponent<Transform>();
            }

            return false;
		}

        case ColliderType::ColliderTypeLine:
        {
            return true; // Placeholder
        }

        default:
        {
            return false; // No collision detected
        }
    }
    
}

//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------

    /// @brief map of the read methods for this Component
    ReadMethodMap< LineCollider > LineCollider::s_ReadMethods = {};

    /// @brief gets the map of read methods for this Component
    /// @return the map of read methods for this Component
    ReadMethodMap< Component > const& LineCollider::GetReadMethods() const
    {
        return (ReadMethodMap< Component > const&)s_ReadMethods;
    }

//-----------------------------------------------------------------------------
