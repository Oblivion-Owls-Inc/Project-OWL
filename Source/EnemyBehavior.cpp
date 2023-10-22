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


	//ChaseTarget(pathfinder, Engine::GetInstance()->GetFixedFrameDuration());

	if (!m_Health.GetActive())
	{
       GetParent()->Destroy();
	}
}

nlohmann::ordered_json EnemyBehavior::Write() const
{
	nlohmann::ordered_json data;
    data["Health"] = m_Health.Write();
	return data;
}

void EnemyBehavior::readHealth(nlohmann::ordered_json const& data)
{
    Stream::Read<ISerializable>(&m_Health, data);
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

EnemyBehavior::EnemyBehavior(EnemyBehavior const& other) : Behavior(typeid(EnemyBehavior)), m_Health(other.m_Health)
{
}
