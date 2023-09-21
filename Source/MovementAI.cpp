#include "MovementAI.h"
#include "Entity.h"
#include "BehaviorSystem.h"
#include "DebugSystem.h"


static float timer = 0.0f;

MovementAI::MovementAI(): Behavior(typeid(MovementAI))
{
    BehaviorSystem<MovementAI>::getInstance()->AddBehavior(this);
}

MovementAI::~MovementAI()
{
    BehaviorSystem< MovementAI >::getInstance()->RemoveBehavior(this);
}

Component* MovementAI::Clone() const
{
	return nullptr;
}

void MovementAI::OnCollisionEvent()
{
    DebugConsole output2(*DebugSystem::getInstance());
    output2 << Parent()->GetName().c_str() <<":Collision Detected in Movement AI" << "\n";
}

void MovementAI::OnUpdate(float dt)
{
}

void MovementAI::MovementAIUpdateRotation(float dt)
{
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

void MovementAI::MovementAISpawnBullet()
{
}

void MovementAI::OnFixedUpdate()
{
}