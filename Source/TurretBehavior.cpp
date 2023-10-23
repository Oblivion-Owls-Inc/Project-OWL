/// @file       TurretBehavior.cpp
/// @author     Jax Clayton (jax.clayton@digipen.edu)
/// @brief      Behavior Compenent for Basic Turret  
/// @version    0.1
/// @date       2023-10-06
/// 
/// @copyright  Copyright (c) 2023 Digipen Institute of Technology



#include "TurretBehavior.h"
#include "BehaviorSystem.h"
#include "EntitySystem.h"
#include "Transform.h"
#include "BulletBehavior.h"
#include "CircleCollider.h"
#include "AssetLibrarySystem.h"
#include "CollisionSystem.h"
#include "EnemyBehavior.h"
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
					m_BulletPrefab(other.m_BulletPrefab),
					m_TargetName(other.m_TargetName)
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
	Behaviors<Behavior>()->AddBehavior(this);
}

void TurretBehavior::OnExit()
{
	/// Remove this behavior from the behavior system
	Behaviors<Behavior>()->RemoveBehavior(this);
}	



void TurretBehavior::OnUpdate(float dt)
{

}

void TurretBehavior::OnFixedUpdate()
{
	float dt = Engine::GetInstance()->GetFixedFrameDuration();
	CheckIfBulletChanged();

	/// Check for a target
	RayCastHit Target = CheckForTarget();

	if (Target)
	{
		/// Fire a bullet at the target
		FireBullet(Target, dt);
	}

}

void TurretBehavior::Inspector()
{
	// Edit the Behavior of the Turret 
	ImGui::InputFloat("Range", &m_Range, 0.5f, 1.0f);
	ImGui::InputFloat("Fire Rate", &m_FireRate, 0.5f, 1.0f);
	ImGui::InputFloat("Bullet Damage", &m_BulletDamage, 0.5f, 1.0f);
	ImGui::InputFloat("Bullet Speed", &m_BulletSpeed, 0.5f, 1.0f);
	ImGui::InputFloat("Bullet Size", &m_BulletSize, 0.5f, 1.0f);
	ImGui::Text("Target Name: %s", m_TargetName.c_str());
}



void TurretBehavior::FireBullet(RayCastHit Target, float dt)
{
	m_LastFireTime += dt;  // Increment the time since the last bullet was fired.

	if (m_LastFireTime < 1.0f / m_FireRate)
		return;

	m_LastFireTime = 0.0f;  // Reset the timer since we're firing a bullet.

	Entity* bullet = new Entity;

	if (!bullet)
		return;

	*bullet = *m_BulletPrefab;
	
	bullet->SetName("Bullet");

	glm::vec2 turretPos = GetParent()->GetComponent<Transform>()->GetTranslation();

	bullet->GetComponent<CircleCollider>()->AddOnCollisionCallback(
		GetParent()->GetId(),
		[bulletBehavior = bullet->GetComponent<BulletBehavior>()](Collider* collider, CollisionData const& collisionData) {
			bulletBehavior->OnCollision(collider, collisionData);
		}
	);

	bullet->GetComponent<Transform>()->SetTranslation(turretPos );

	bullet->GetComponent<BulletBehavior>()->SetTarget(Target);
	bullet->GetComponent<BulletBehavior>()->SetBulletDamage(m_BulletDamage);
	bullet->GetComponent<BulletBehavior>()->SetBulletSpeed(m_BulletSpeed);
	
	bullet->GetComponent<Transform>()->SetScale(glm::vec2(m_BulletSize));

	Entities()->AddEntity(bullet);

}

RayCastHit TurretBehavior::CheckForTarget()
{
	RayCastHit hit;
    for (auto& entity : Entities()->GetEntities())
    {
		/// Skip the entities that don't match the target name		
		if (entity->GetName() != m_TargetName)
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
        hit = CollisionSystem::GetInstance()->RayCast(
			turretPosition, directionToEntity, m_Range, collider->GetCollisionLayerFlags()
		);

		return hit;
    }
	return hit;
}

void TurretBehavior::CheckIfBulletChanged()
{

	/// Check if the bullet prefab has changed
	if (m_BulletName != m_BulletPrefab->GetName())
	{
		m_BulletName = m_BulletPrefab->GetName();
		/// Get the new bullet prefab
		m_BulletPrefab = AssetLibrarySystem<Entity>::GetInstance()->GetAsset(m_BulletName);

	}
}

void TurretBehavior::readBulletName(nlohmann::ordered_json const& data)
{
	m_BulletName = Stream::Read<std::string>(data);
	m_BulletPrefab = AssetLibrarySystem<Entity>::GetInstance()->GetAsset(m_BulletName);
}

void TurretBehavior::readTargetName(nlohmann::ordered_json const& data)
{
	m_TargetName = Stream::Read<std::string>(data);
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
	data["bulletname"] = m_BulletName;
	data["Target"] = m_TargetName;

	return data;
}

ReadMethodMap<TurretBehavior> const TurretBehavior::s_ReadMethods =
{
	{ "fireRate",			  &readFireRate },
	{ "range",					 &readRange },
	{ "bulletName",		   &readBulletName  },
	{ "bulletdamage",	  &readBulletDamage },
	{ "bulletspeed",	   &readBulletSpeed },
	{ "bulletsize",		    &readBulletSize },
	{ "Target",				&readTargetName }
};
#pragma	warning(pop)
