///*****************************************************************/
/// @file	 MovementAI.cpp
/// @Author  Jax Clayton (jax.clayton@digipen.edu)
/// @date	 9/15/2021
/// @brief   MovementAI  
/// @details 
/// 
///* ****************************************************************/

#include "MovementAI.h"
#include "Entity.h"
#include "BehaviorSystem.h"
#include "PlatformSystem.h"
#include "CircleCollider.h"
#include "DebugSystem.h"
#include <string>

static float timer = 0.0f;

MovementAI::MovementAI() :
    Behavior( typeid( MovementAI ) )
{}

Component* MovementAI::Clone() const
{
	return new MovementAI( *this );
}
MovementAI::MovementAI( MovementAI const& other ) :
    Behavior( other )
{}


void MovementAI::OnInit()
{
    Behaviors<Behavior>()->AddBehavior(this);
}
void MovementAI::OnExit()
{
    Behaviors<Behavior>()->RemoveBehavior(this);
}

void MovementAI::OnUpdate(float dt)
{
}

void MovementAI::OnFixedUpdate()
{
    glm::vec2 WindowSize = glm::vec2(10.0f, 8.0f);
    Transform* ballTransform = GetEntity()->GetComponent<Transform>();
    RigidBody* ballRigidBody = GetEntity()->GetComponent<RigidBody>();
    CircleCollider* ballCollider = (CircleCollider*)GetEntity()->GetComponent<CircleCollider>();

    if (ballTransform == nullptr || ballRigidBody == nullptr || ballCollider == nullptr)
	{
		return;
	}

    float radius = ballCollider->GetRadius();

    glm::vec2 pos = ballTransform->GetTranslation();
    glm::vec2 velocity = ballRigidBody->GetVelocity();

    // Check if the ball is going to exceed the screen bounds
    if ((pos.x - radius) < -WindowSize.x / 2)
    {
        pos.x = -WindowSize.x / 2 + radius;
        velocity.x = -velocity.x;
    }
    else if ((pos.x + radius) > WindowSize.x / 2)
    {
        pos.x = WindowSize.x / 2 - radius;
        velocity.x = -velocity.x;
    }

    if ((pos.y - radius) < -WindowSize.y / 2)
    {
        pos.y = -WindowSize.y / 2 + radius;
        velocity.y = -velocity.y;
    }
    else if ((pos.y + radius) > WindowSize.y / 2)
    {
        pos.y = WindowSize.y / 2 - radius;
        velocity.y = -velocity.y;
    }

    // Update the ball's position and velocity
    ballTransform->SetTranslation(pos);
    ballRigidBody->SetVelocity(velocity);
}