///@file Collider.cpp
///@Author Jax Clayton (jax.clayton@digipen.edu)

#include "Collider.h"

Collider::Collider(): Component(typeid(Collider)), type()
{
    // PhysicsSystem::GetInstance()->AddCollider(this);
}

Collider::Collider(const Collider& other) : Component(other), type(other.type)
{
}

Collider::~Collider()
{
	// PhysicsSystem::GetInstance()->RemoveCollider(this);
}

Component* Collider::Clone() const
{
	return nullptr;
}
void Collider::OnUpdate(float dt)
{
}

void Collider::Check(const Collider* other)
{
  
}


///This is not complete and will need to be finished
bool Collider::IsColliding(const Collider* other)
{
    switch (type)
    {
        case ColliderTypeCircle:

            switch (other->type)
            {
                case ColliderTypeCircle:

                case ColliderTypeLine:

                default:
                    return false;
            }

            break;

        case ColliderTypeLine:

            switch (other->type)
            {
                case ColliderTypeCircle:

                case ColliderTypeLine:

                default:
                    return false;
            }

            break;

        default:

            return false;
    }

}

void Collider::SetCollider(Collider* collider)
{
}

Collider* Collider::GetCollider()
{
	return nullptr;
}
