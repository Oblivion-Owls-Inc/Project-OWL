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
}

void MovementAI::OnUpdate(float dt)
{
    timer += dt;
	DebugConsole output(*DebugSystem::getInstance());
	if (timer > 1.0f)
	{
		timer = 0.0f;
	}
	else
	{
	}
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
/// <summary>
/// UNUSED
/// </summary>
void MovementAI::OnFixedUpdate()
{
}