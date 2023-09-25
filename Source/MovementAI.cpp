#include "MovementAI.h"
#include "Entity.h"
#include "BehaviorSystem.h"
#include "DebugSystem.h"


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

/// @brief the map of read methods for this Component
ReadMethodMap< MovementAI > const MovementAI::readMethods = {};

ReadMethodMap<Component> const& MovementAI::GetReadMethods()
{
    return (std::map< std::string, ReadMethod< Component > > const&)readMethods;
}

void MovementAI::MovementAISpawnBullet()
{
}

void MovementAI::OnFixedUpdate()
{
}