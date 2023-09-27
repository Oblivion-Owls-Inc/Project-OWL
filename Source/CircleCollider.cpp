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
		radius = stream.Read<float>();
	}

	/// @brief gets the map of read methods for this Component
    /// @return the map of read methods for this Component
    ReadMethodMap< Component > const& CircleCollider::GetReadMethods() const
    {
        return (ReadMethodMap< Component > const&)s_ReadMethods;
    }

//-----------------------------------------------------------------------------
