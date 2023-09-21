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

                for (unsigned int i = 0; i < lineCount; i++)
                {
                    ColliderLineSegment* segment = &m_lineSegments[i];

                    // Define the line segment's endpoints
                    glm::vec2 lineStart = segment->point[0];
                    glm::vec2 lineEnd = segment->point[1];
                    


                    // Check if the distance is less than the circle's radius
                    if (distance <= circleCollider->GetRadius())
                    {
                        return true; // Collision detected
                    }
                }
                
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
