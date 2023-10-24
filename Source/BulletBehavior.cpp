#include "BulletBehavior.h"
#include "BehaviorSystem.h"
#include "Transform.h"
#include "EnemyBehavior.h"
#include "StaticBody.h"
#include "Engine.h"
#include "Entity.h"
#include "CollisionSystem.h"

BulletBehavior::BulletBehavior() :
    Behavior( typeid( BulletBehavior ) ),
    m_BulletDamage( 1.0f ),
    m_BulletSpeed( 1.0f ),
    m_BulletLifeTime( std::string( "BulletLifeTime" ) )
{}

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
	if ( other->GetCollisionLayerId() == Collisions()-> )
		other->GetParent()->GetComponent<EnemyBehavior>()->TakeDamage(m_BulletDamage);

	GetParent()->Destroy();
}

void BulletBehavior::OnUpdate(float dt)
{
	
}

void BulletBehavior::OnFixedUpdate()
{
	float dt = Engine::GetInstance()->GetFixedFrameDuration();

	m_BulletLifeTime -= dt;
	std::cout << m_BulletLifeTime.GetCurrent() << std::endl;

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
