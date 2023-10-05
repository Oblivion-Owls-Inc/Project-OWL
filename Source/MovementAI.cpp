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

MovementAI::~MovementAI() {}

Component* MovementAI::Clone() const
{
	return new MovementAI( *this );
}
MovementAI::MovementAI( MovementAI const& other ) :
    Behavior( other )
{}


void MovementAI::OnInit()
{
    BehaviorSystem<MovementAI>::GetInstance()->AddBehavior(this);
}
void MovementAI::OnExit()
{
    BehaviorSystem< MovementAI >::GetInstance()->RemoveBehavior(this);
}

/// @brief  Called whenever a Collider on this Behavior's Entity collides
/// @param  other           the entity that was collided with
/// @param  collisionData   additional data about the collision
void MovementAI::OnCollision( Entity* other, CollisionData const& collisionData )
{
    DebugConsole output2( *DebugSystem::GetInstance() );
    output2 << GetParent()->GetName().c_str() <<":Collision Detected in Movement AI" << "\n";
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
    Transform* ballTransform = GetParent()->GetComponent<Transform>();
    RigidBody* ballRigidBody = GetParent()->GetComponent<RigidBody>();
    CircleCollider* ballCollider = (CircleCollider*)GetParent()->GetComponent<CircleCollider>();
    float radius = ballCollider->GetRadius();

    glm::vec3 pos = ballTransform->GetTranslation();
    glm::vec3 velocity = ballRigidBody->GetVelocity();

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
    ballRigidBody->SetVelocity(velocity);
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
