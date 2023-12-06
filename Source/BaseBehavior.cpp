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
#include "SceneSystem.h"
#include "AudioPlayer.h"

//-----------------------------------------------------------------------------
// constructor / destructor 
//-----------------------------------------------------------------------------

/// @brief  constructor
BaseBehavior::BaseBehavior() : BasicEntityBehavior(typeid(BaseBehavior))
{
}

/// @brief  copy ctor
BaseBehavior::BaseBehavior(const BaseBehavior& other) : BasicEntityBehavior(other)
{
}

/// @brief  dtor
BaseBehavior::~BaseBehavior()
{
}

/// @brief  clone
Component* BaseBehavior::Clone() const
{
	return new BaseBehavior(*this);
}

//-----------------------------------------------------------------------------
// virtual override methods
//-----------------------------------------------------------------------------

/// @brief	initialize base
void BaseBehavior::OnInit()
{
	BasicEntityBehavior::OnInit();

	GetEntity()->GetComponent< CircleCollider >()->AddOnCollisionCallback(
		GetId(),
		std::bind(&BaseBehavior::onCollision, this, std::placeholders::_1, std::placeholders::_2)
	);

	m_AudioPlayer = GetEntity()->GetComponent<AudioPlayer>();
}

/// @brief	called on exit, handles loss state
void BaseBehavior::OnExit()
{
	BasicEntityBehavior::OnExit();
}

void BaseBehavior::onCollision(Collider* other, CollisionData const& collisionData)
{
	EnemyBehavior* enemy = other->GetEntity()->GetComponent<EnemyBehavior>();
	if (!enemy)
	{
		return;
	}
	
	BasicEntityBehavior::TakeDamage(enemy->GetDamage());
	if(m_AudioPlayer)
	{
		m_AudioPlayer->Play();
	}
	enemy->GetEntity()->Destroy();

	if (BasicEntityBehavior::GetHealth()->GetCurrent() <= 0)
	{
		SceneSystem::GetInstance()->SetNextScene("Gameover");
	}
}

//-----------------------------------------------------------------------------
// reading
//-----------------------------------------------------------------------------

/// @brief read method map
ReadMethodMap<BaseBehavior> const BaseBehavior::s_ReadMethods =
{
	{ "Health",	  &readHealth}
};

//-----------------------------------------------------------------------------
// writing
//-----------------------------------------------------------------------------

/// @brief	write to json
nlohmann::ordered_json BaseBehavior::Write() const
{
	nlohmann::ordered_json data;

	data["Health"] = m_Health.Write();


	return data;
}


