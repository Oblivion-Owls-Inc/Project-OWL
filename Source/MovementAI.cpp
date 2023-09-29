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

MovementAI::MovementAI(): Behavior(typeid(MovementAI))
{
    BehaviorSystem<MovementAI>::GetInstance()->AddBehavior(this);
}

MovementAI::~MovementAI()
{
    BehaviorSystem< MovementAI >::GetInstance()->RemoveBehavior(this);
}

Component* MovementAI::Clone() const
{
	return nullptr;
}

void MovementAI::OnCollisionEvent()
{
    DebugConsole output2( *DebugSystem::GetInstance() );
    output2 << Parent()->GetName().c_str() <<":Collision Detected in Movement AI" << "\n";
    RigidBody* ballRigidBody = Parent()->GetComponent<RigidBody>();
    Transform* ballTransform = Parent()->GetComponent<Transform>();
    CircleCollider* ballCollider = (CircleCollider*)Parent()->GetComponent<Collider>();
    vec3 velocity = *ballRigidBody->GetVelocity();

    velocity.x = -velocity.x;
    velocity.y = -velocity.y;

    ballTransform->SetTranslation(*ballRigidBody->GetOldTranslation());
    ballRigidBody->SetVelocity(&velocity); 
    ballCollider->isColliding(false);
}

void MovementAI::OnUpdate(float dt)
{
}

void MovementAI::MovementAIUpdateRotation(float dt)
{
}

void MovementAI::OnFixedUpdate()
{

    glm::vec2 WindowSize = glm::vec2(10.0f, 8.0f);
    Transform* ballTransform = Parent()->GetComponent<Transform>();
    RigidBody* ballRigidBody = Parent()->GetComponent<RigidBody>();
    CircleCollider* ballCollider = (CircleCollider*)Parent()->GetComponent<Collider>();
    float radius = ballCollider->GetRadius();

    vec3 pos = *ballTransform->GetTranslation();
    vec3 velocity = *ballRigidBody->GetVelocity();

    // Check if the ball is going to exceed the screen bounds
    if ((pos.x - radius) <= -WindowSize.x / 2)
    {
        pos.x = -WindowSize.x / 2 + radius;
        velocity.x = -velocity.x;
    }
    else if ((pos.x + radius) >= WindowSize.x / 2)
    {
        pos.x = WindowSize.x / 2 - radius;
        velocity.x = -velocity.x;
    }

    if ((pos.y - radius) <= -WindowSize.y / 2)
    {
        pos.y = -WindowSize.y / 2 + radius;
        velocity.y = -velocity.y;
    }
    else if ((pos.y + radius) >= WindowSize.y / 2)
    {
        pos.y = WindowSize.y / 2 - radius;
        velocity.y = -velocity.y;
    }

    // Update the ball's position and velocity
    ballTransform->SetTranslation(pos);
    ballRigidBody->SetVelocity(&velocity);
}
void MovementAI::MovementAIUpdateVelocity(float dt)
{
}

void MovementAI::MovementAIUpdateWeapon(float dt)
{
}


void MovementAI::MovementAISpiral(float dt)
{
}

/// @brief the map of read methods for this Component
ReadMethodMap< MovementAI > const MovementAI::readMethods = {};

ReadMethodMap<Component> const& MovementAI::GetReadMethods() const
{
    return (ReadMethodMap< Component > const&)readMethods;
}

void MovementAI::MovementAISpawnBullet()
{
}
