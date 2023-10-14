#include "TurretBehavior.h"
#include "BehaviorSystem.h"
#include "EntitySystem.h"
#include "Transform.h"
#include "Animation.h"
#include "DebugSystem.h"

TurretBehavior::TurretBehavior(): 
	Behavior(typeid(TurretBehavior)),
	m_FireRate(1.0f), 
	m_Range(5.0f), 
	m_BulletDamage(1.0f), 
	m_BulletSpeed(1.0f),
	m_BulletSize(1.0f), 
	m_BulletPrefab(nullptr)
{
}

TurretBehavior::TurretBehavior(const TurretBehavior& other)
	             : Behavior(typeid(TurretBehavior)), 
					m_FireRate(other.m_FireRate), 
					m_Range(other.m_Range), 
					m_BulletDamage(other.m_BulletDamage), 
					m_BulletSpeed(other.m_BulletSpeed), 
					m_BulletSize(other.m_BulletSize), 
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
		BehaviorSystem<TurretBehavior>::GetInstance()->AddBehavior(this);
}

void TurretBehavior::OnExit()
{
	/// Remove this behavior from the behavior system
	BehaviorSystem<TurretBehavior>::GetInstance()->RemoveBehavior(this);
}



void TurretBehavior::OnUpdate(float dt)
{
 /// Do Turret Debug Stuff
}

void TurretBehavior::OnFixedUpdate()
{
	///Check if any entities are in range
	for (auto entity : EntitySystem::GetInstance()->GetEntities())
	{
		if (entity == GetParent())
			continue;
		Transform* m_Transform = entity->GetComponent<Transform>();
		if (m_Transform)
		{
			glm::vec2 entityPOS = m_Transform->GetTranslation(); 
			glm::vec2 turretPOS = GetParent()->GetComponent<Transform>()->GetTranslation();

			/// Caclulate the area of attack for the turret
			float AOA = glm::length(entityPOS - turretPOS);
			/// Check if the entity is in range
			if (AOA <= m_Range)
			{
				FireBullet();
				//entity->Destroy();
			}
		}
	}
}

void TurretBehavior::Inspector()
{
	///Edit the Behavior of the Turret 
	ImGui::DragFloat("Range", &m_Range);
	ImGui::DragFloat("Fire Rate", &m_FireRate);
	ImGui::DragFloat("Bullet Damage", &m_BulletDamage);
	ImGui::DragFloat("Bullet Speed", &m_BulletSpeed);
	ImGui::DragFloat("Bullet Size", &m_BulletSize);
}


void TurretBehavior::FireBullet()
{
	/// Todo: Make this actually work
}

void TurretBehavior::readFireRate( nlohmann::json const& data )
{
	m_FireRate = jsonValue.Read<float>();
}

void TurretBehavior::readRange( nlohmann::json const& data )
{
	m_Range = jsonValue.Read<float>();
}

void TurretBehavior::readBulletDamage( nlohmann::json const& data )
{
	m_BulletDamage = jsonValue.Read<float>();
}

void TurretBehavior::readBulletSpeed( nlohmann::json const& data )
{
	m_BulletSpeed = jsonValue.Read<float>();
}

void TurretBehavior::readBulletSize( nlohmann::json const& data )
{
	m_BulletSize = jsonValue.Read<float>();
}

ReadMethodMap<TurretBehavior> const TurretBehavior::s_ReadMethods =
{
	{ "fireRate",			  &readFireRate },
	{ "range",					 &readRange },
	{ "bulletdamage",	  &readBulletDamage },
	{ "bulletspeed",	   &readBulletSpeed },
	{ "bulletsize",		    &readBulletSize }
};
