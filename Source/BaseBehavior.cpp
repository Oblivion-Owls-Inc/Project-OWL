///*****************************************************************/
/// @file	    BaseBehavior.cpp
/// @author     Tyler Birdsall (tyler.birdsall@digipen.edu)
/// @date	    10/27/2023
/// @brief      BaseBehavior class implimentation
/// @copyright  Digipen LLC (c) 2023
///*****************************************************************/

#include "BaseBehavior.h"
#include "Entity.h"
#include "CircleCollider.h"
#include "CollisionSystem.h"
#include "EnemyBehavior.h"

BaseBehavior::BaseBehavior() : BasicEntityBehavior(typeid(BaseBehavior))
{
}

BaseBehavior::BaseBehavior(const BaseBehavior& other) : BasicEntityBehavior(other)
{
}

BaseBehavior::~BaseBehavior()
{
}

Component* BaseBehavior::Clone() const
{
	return new BaseBehavior(*this);
}

void BaseBehavior::OnInit()
{
	BasicEntityBehavior::OnInit();

	GetParent()->GetComponent< CircleCollider >()->AddOnCollisionCallback(
		GetId(),
		std::bind(&BaseBehavior::onCollision, this, std::placeholders::_1, std::placeholders::_2)
	);
}

void BaseBehavior::OnExit()
{
	BasicEntityBehavior::OnExit();
	std::cout << "we died!";
}

void BaseBehavior::onCollision(Collider* other, CollisionData const& collisionData)
{
	EnemyBehavior* enemy = other->GetParent()->GetComponent<EnemyBehavior>();
	if (!enemy)
	{
		return;
	}
	
	BasicEntityBehavior::TakeDamage(enemy->GetDamage());
	enemy->GetParent()->Destroy();
}

/// @brief read method map
ReadMethodMap<BaseBehavior> const BaseBehavior::s_ReadMethods =
{
	{ "Health",	  &readHealth},
	
};

nlohmann::ordered_json BaseBehavior::Write() const
{
	nlohmann::ordered_json data;

	data["Health"] = m_Health.Write();


	return data;
}


