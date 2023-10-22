#include "BulletBehavior.h"
#include "BehaviorSystem.h"
#include "StaticBody.h"
#include "Entity.h"

BulletBehavior::BulletBehavior(): Behavior(typeid(BulletBehavior))
{
}

void BulletBehavior::SetTarget(RayCastHit target)
{
	m_Target = target;
}

void BulletBehavior::OnInit()
{
	Behaviors<Behavior>()->AddBehavior(this);
}

void BulletBehavior::OnExit()
{
	Behaviors<Behavior>()->RemoveBehavior(this);
}

void BulletBehavior::OnCollision(Entity* other, CollisionData const& collisionData)
{
	if (other->GetName() == "Enemy")
	{
		other->Destroy();
	}

	GetParent()->Destroy();
}

void BulletBehavior::CollideWithStatic(StaticBody const* other, CollisionData const& collisionData)
{
	if (other->GetParent()->GetName() == "Enemy")
	{
		other->GetParent()->Destroy();
	}

	GetParent()->Destroy();
}

void BulletBehavior::OnUpdate(float dt)
{
	if (m_Target)

}

void BulletBehavior::OnFixedUpdate()
{
}

void BulletBehavior::Inspector()
{
}

Component* BulletBehavior::Clone() const
{
	return nullptr;
}

BulletBehavior::BulletBehavior(const BulletBehavior& other)
	: Behavior(typeid(BulletBehavior))
{
}
