#include "TurretBehavior.h"
#include "BehaviorSystem.h"
#include "EntitySystem.h"
#include "Transform.h"
#include "CircleCollider.h"
#include "CollisionSystem.h"
#include "Animation.h"
#include "Engine.h"
#include "Pool.h"
#include "DebugSystem.h"

#pragma warning(push, 0)
TurretBehavior::TurretBehavior(): 
	Behavior(typeid(TurretBehavior)),
	m_CollisionLayerFlags(0),
	m_FireRate(1.0f), 
	m_Range(5.0f), 
	m_BulletDamage(1.0f), 
	m_BulletSpeed(1.0f),
	m_BulletSize(1.0f), 
	m_LastFireTime(0.0f),
	m_BulletPrefab(nullptr)
{
}

TurretBehavior::TurretBehavior(const TurretBehavior& other)
	             : Behavior(typeid(TurretBehavior)), 
					m_CollisionLayerFlags(0),
					m_FireRate(other.m_FireRate), 
					m_Range(other.m_Range), 
					m_BulletDamage(other.m_BulletDamage), 
					m_BulletSpeed(other.m_BulletSpeed), 
					m_BulletSize(other.m_BulletSize),
					m_LastFireTime(other.m_LastFireTime),
					m_BulletPrefab(other.m_BulletPrefab)
{
}

TurretBehavior::~TurretBehavior()
{
}

Component* TurretBehavior::Clone() const
{
	return (Component*)new TurretBehavior(*this);
}

void TurretBehavior::OnInit()
{
	/// Add this behavior to the behavior system
	Behaviors<TurretBehavior>()->AddBehavior(this);
}

void TurretBehavior::OnExit()
{
	/// Remove this behavior from the behavior system
	Behaviors<TurretBehavior>()->RemoveBehavior(this);
}	



void TurretBehavior::OnUpdate(float dt)
{

}

void TurretBehavior::OnFixedUpdate()
{
	/// Check for a target
	Entity* target = CheckForTarget();

	if (target)
	{
		/// Fire a bullet at the target
		FireBullet(target);
	}

}

void TurretBehavior::Inspector()
{
	// Edit the Behavior of the Turret 
	ImGui::DragFloat("Range", &m_Range, 1.0f, 1.0f);
	ImGui::DragFloat("Fire Rate", &m_FireRate, 1.0f, 1.0f);
	ImGui::DragFloat("Bullet Damage", &m_BulletDamage, 1.0f, 1.0f);
	ImGui::DragFloat("Bullet Speed", &m_BulletSpeed, 1.0f, 1.0f);
	ImGui::DragFloat("Bullet Size", &m_BulletSize, 1.0f, 1.0f);
}



void TurretBehavior::FireBullet(Entity* Target)
{
	float currentTime = glfwGetTime();
	if (currentTime - m_LastFireTime < m_FireRate)
		return;

	Pool<int>* pool = Target->GetComponent<Pool<int>>();

	if (!pool || !pool->GetActive() || pool->GetName() != std::string("Health"))
		return;

	///@note this will be moved elsewhere
	pool->DecreasePoolTime(m_BulletDamage);

	m_LastFireTime = currentTime;  // Update the last fire time
}

Entity* TurretBehavior::CheckForTarget()
{
    for (auto& entity : Entities()->GetEntities())
    {
        if (entity == GetParent())
        {
            continue;  // Skip the parent entity
        }

		if (entity->GetName() == std::string("Bullet"))
		{
			continue;
		}

		/// Get the position of the turret
        glm::vec2 turretPosition = GetParent()->GetComponent<Transform>()->GetTranslation();  

		/// Get the position of the entity
		glm::vec2 enemyPosition = entity->GetComponent<Transform>()->GetTranslation();

		/// Calculate the direction from the turret to the entity
        glm::vec2 directionToEntity = enemyPosition - turretPosition;

		directionToEntity = glm::normalize(directionToEntity);

		CircleCollider* collider = (CircleCollider *)GetParent()->GetComponent<Collider>();
        // Cast a ray from the turret towards the entity
        RayCastHit hit = CollisionSystem::GetInstance()->RayCast(turretPosition, directionToEntity, m_Range, collider->GetCollisionLayerFlags());

        if (hit)  // If a collider was hit
        {
			return entity;  // Set the hit entity as the target
        }
    }

	return nullptr;
}

void TurretBehavior::readFireRate( nlohmann::ordered_json const& data )
{
	m_FireRate = Stream::Read<float>(data);
}

void TurretBehavior::readRange( nlohmann::ordered_json const& data )
{
	m_Range = Stream::Read<float>(data);
}

void TurretBehavior::readBulletDamage( nlohmann::ordered_json const& data )
{
	m_BulletDamage = Stream::Read<float>(data);
}

void TurretBehavior::readBulletSpeed( nlohmann::ordered_json const& data )
{
	m_BulletSpeed = Stream::Read<float>(data);
}

void TurretBehavior::readBulletSize( nlohmann::ordered_json const& data )
{
	m_BulletSize = Stream::Read<float>(data);
}

/// @brief Write all TurretBehavior data to a JSON file.
/// @return The JSON file containing the TurretBehavior data.
nlohmann::ordered_json TurretBehavior::Write() const
{
	nlohmann::ordered_json data;

	data["fireRate"] = m_FireRate;
	data["range"] = m_Range;
	data["bulletdamage"] = m_BulletDamage;
	data["bulletspeed"] = m_BulletSpeed;
	data["bulletsize"] = m_BulletSize;

	return data;
}

ReadMethodMap<TurretBehavior> const TurretBehavior::s_ReadMethods =
{
	{ "fireRate",			  &readFireRate },
	{ "range",					 &readRange },
	{ "bulletdamage",	  &readBulletDamage },
	{ "bulletspeed",	   &readBulletSpeed },
	{ "bulletsize",		    &readBulletSize }
};
#pragma	warning(pop)
