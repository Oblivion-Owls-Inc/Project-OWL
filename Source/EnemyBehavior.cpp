#include "EnemyBehavior.h"
#include "BehaviorSystem.h"
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

	if (!pool)
		return;

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

Component* EnemyBehavior::Clone() const
{
	return new EnemyBehavior(*this);
}

EnemyBehavior::EnemyBehavior(EnemyBehavior const& other) : Behavior(typeid(EnemyBehavior))
{
}
