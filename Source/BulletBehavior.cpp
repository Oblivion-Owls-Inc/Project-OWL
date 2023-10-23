#include "BulletBehavior.h"
#include "BehaviorSystem.h"
#include "Transform.h"
#include "EnemyBehavior.h"
#include "StaticBody.h"
#include "Engine.h"
#include "Entity.h"

BulletBehavior::BulletBehavior() : Behavior(typeid(BulletBehavior)), m_BulletDamage(1.0f), m_BulletSpeed(1.0f), m_BulletLifeTime(std::string("BulletLifeTime"))
{
}

void BulletBehavior::SetTarget(RayCastHit target)
{
	m_Target = target;
}

void BulletBehavior::OnInit()
{
	Behaviors<BulletBehavior>()->AddBehavior(this);
}

void BulletBehavior::OnExit()
{
	Behaviors<BulletBehavior>()->RemoveBehavior(this);
}

void BulletBehavior::OnCollision(Collider* other, CollisionData const& collisionData)
{
	///If the bullet hits an enemy, deal damage to it
	if (other->GetParent()->GetName() == std::string("Enemy"))
		other->GetParent()->GetComponent<EnemyBehavior>()->TakeDamage(m_BulletDamage);

	///The bullet is destroyed on collision
	GetParent()->Destroy();
}

void BulletBehavior::OnUpdate(float dt)
{
	
}

void BulletBehavior::OnFixedUpdate()
{
	float dt = Engine::GetInstance()->GetFixedFrameDuration();

	///Update the bullet's life time
	m_BulletLifeTime -= dt;

	///If the bullet has a target, update it's position towards the target
	if (m_Target)
	{
		glm::vec2 bulletPos = GetParent()->GetComponent<Transform>()->GetTranslation();

		/// Update the bullet's position using the normal of the raycast hit
		glm::vec2 newPos = bulletPos - m_Target.normal * m_BulletSpeed * dt;

		GetParent()->GetComponent<Transform>()->SetTranslation(newPos);
	}

	///Destroy the bullet if it's life time is over
	if (m_BulletLifeTime <= 0.0f)
	{
		GetParent()->Destroy();
	}
}

void BulletBehavior::Inspector()
{
	m_BulletLifeTime.Inspector();
	ImGui::InputFloat("Bullet Damage", &m_BulletDamage, 1.0f, 1.0f);
	ImGui::InputFloat("Bullet Speed", &m_BulletSpeed, 1.0f, 1.0f);
}

void BulletBehavior::readPool(nlohmann::ordered_json const& data)
{
	Stream::Read(m_BulletLifeTime, data);
}

Component* BulletBehavior::Clone() const
{
	return new BulletBehavior(*this);
}


ReadMethodMap<BulletBehavior> const BulletBehavior::s_ReadMethods =
{
	{ "Pool", &BulletBehavior::readPool }
};



BulletBehavior::BulletBehavior(const BulletBehavior& other)
	: Behavior(typeid(BulletBehavior)), m_Target(other.m_Target), m_BulletDamage(other.m_BulletDamage), m_BulletSpeed(other.m_BulletSpeed), m_BulletLifeTime(other.m_BulletLifeTime)
{
}
