#include "EnemyBehavior.h"
#include "BehaviorSystem.h"
#include "EntitySystem.h"
#include "Transform.h"
#include "Engine.h"
#include "Pathfinder.h"
#include "RigidBody.h"
#include "basics.h"
#include "Pool.h"
#include "Entity.h"

EnemyBehavior::EnemyBehavior() : Behavior(typeid(EnemyBehavior))
{
}

void EnemyBehavior::OnInit()
{
	BehaviorSystem<EnemyBehavior>::GetInstance()->AddBehavior(this);
}

void EnemyBehavior::OnExit()
{
	BehaviorSystem<EnemyBehavior>::GetInstance()->RemoveBehavior(this);
}

void EnemyBehavior::Inspector()
{
}

ReadMethodMap< EnemyBehavior > const EnemyBehavior::s_ReadMethods = {};

void EnemyBehavior::OnFixedUpdate()
{
	Pool<int>* pool = GetParent()->GetComponent<Pool<int>>();
    Pathfinder* pathfinder = GetParent()->GetComponent<Pathfinder>();

	if (!pool || pathfinder)
		return;

	ChaseTarget(pathfinder, Engine::GetInstance()->GetFixedFrameDuration());

	if (!pool->GetActive())
	{
       GetParent()->Destroy();
	}
}

nlohmann::ordered_json EnemyBehavior::Write() const
{
	nlohmann::ordered_json data;
	data["Active"] = GetParent()->GetComponent<Pool<int>>()->GetActive();

	return data;
}

void EnemyBehavior::ChaseTarget(Pathfinder* pathfinder, float dt)
{
    for (auto& enemy : Entities()->GetEntities())
    {
        if (enemy->GetName() != std::string("Enemy"))
            continue;

        Transform* transform = enemy->GetComponent<Transform>();

        if (!transform)
        {
            continue;
        }

        glm::vec2 pos = transform->GetTranslation();


        RigidBody* rigidBody = enemy->GetComponent<RigidBody>();

        // accelerate along path
        glm::vec3 moveDir = glm::vec3(pathfinder->GetDirectionAt(pos), 0);
        rigidBody->SetAcceleration(moveDir * 12.0f);

        // air friction or something
        float af = 5.5f;
        glm::vec2 vel = rigidBody->GetVelocity();
        rigidBody->SetVelocity(vel * (1.0f - af * dt));
    }
}

Component* EnemyBehavior::Clone() const
{
	return new EnemyBehavior(*this);
}

EnemyBehavior::EnemyBehavior(EnemyBehavior const& other) : Behavior(typeid(EnemyBehavior))
{
}
