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

EnemyBehavior::EnemyBehavior() : Behavior(typeid(EnemyBehavior)), m_Health("Health")
{
}

void EnemyBehavior::TakeDamage(int damage)
{
    m_Health -= damage;
    if (!m_Health)
    {
		GetParent()->Destroy();
	}
}

void EnemyBehavior::TakeDamage(float damage)
{
    
    m_Health -= damage;

    ///if the pool is empty, destroy the enemy
    if (!m_Health)
    {
        GetParent()->Destroy();
    }
}

void EnemyBehavior::OnInit()
{
    Behaviors<Behavior>()->AddBehavior(this);
    m_Health.OnInit();
}

void EnemyBehavior::OnExit()
{
    Behaviors<Behavior>()->RemoveBehavior(this);
}

void EnemyBehavior::Inspector()
{
    m_Health.Inspector();
}

ReadMethodMap< EnemyBehavior > const EnemyBehavior::s_ReadMethods = {
	{ "Health", &EnemyBehavior::readHealth }
};

void EnemyBehavior::OnFixedUpdate()
{
}

nlohmann::ordered_json EnemyBehavior::Write() const
{
	nlohmann::ordered_json data;
    data["Health"] = m_Health.Write();
	return data;
}

void EnemyBehavior::readHealth(nlohmann::ordered_json const& data)
{
    Stream::Read(m_Health, data);
}

Component* EnemyBehavior::Clone() const
{
	return new EnemyBehavior(*this);
}

EnemyBehavior::EnemyBehavior(EnemyBehavior const& other) : Behavior(typeid(EnemyBehavior)), m_Health(other.m_Health)
{
}
