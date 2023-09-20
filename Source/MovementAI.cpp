#include "MovementAI.h"
#include "Entity.h"
#include "BehaviorSystem.h"
#include "DebugSystem.h"

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

void MovementAI::CollisionEvent(Entity* other)
{
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
/// <summary>
/// UNUSED
/// </summary>
void MovementAI::OnFixedUpdate()
{
}