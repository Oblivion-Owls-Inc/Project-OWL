#include "CircleCollider.h"
#include "entity.h"
#include "Transform.h"
#include "Collider.h"

CircleCollider::CircleCollider(): Collider(), m_Radius(0.0f)
{
	setColliderType(ColliderType::ColliderTypeCircle);
}

CircleCollider::CircleCollider(const CircleCollider& other) : Collider(other), m_Radius(other.m_Radius)
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
			CircleCollider* otherCircle = (CircleCollider*)other;
			Transform* otherTransform = other->Parent()->GetComponent<Transform>();
			Transform* thisTransform = Parent()->GetComponent<Transform>();

			vec3 otherPos = *otherTransform->GetTranslation();
			vec3 thisPos = *thisTransform->GetTranslation();

			float otherRadius = otherCircle->GetRadius();
			float thisRadius = GetRadius();

			float distance = glm::length(otherPos - thisPos);
			if (distance <= (otherRadius + thisRadius))
			{
				return true;
			}
			else
			{
				return false;
			}
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

void CircleCollider::SetRadius(float _m_Radius)
{
	m_Radius = _m_Radius;
}

float CircleCollider::GetRadius() const
{
	return m_Radius;
}

void CircleCollider::OnFixedUpdate()
{
}

//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------


    /// @brief map of the read methods for this Component
    ReadMethodMap< CircleCollider > CircleCollider::s_ReadMethods = {
		{ "radius" , &ReadRadius }
	};

	/// @brief		  Read in the radius of the circle collider component.
	/// @param stream The JSON value to read from.
	void CircleCollider::ReadRadius(Stream stream)
	{
		m_Radius = stream.Read<float>();
	}

	/// @brief gets the map of read methods for this Component
    /// @return the map of read methods for this Component
    ReadMethodMap< Component > const& CircleCollider::GetReadMethods() const
    {
        return (ReadMethodMap< Component > const&)s_ReadMethods;
    }

//-----------------------------------------------------------------------------
