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
#include "GeneratorBehavior.h"
#include "Health.h"

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

	GetEntity()->GetComponent< CircleCollider >()->AddOnCollisionEnterCallback(
		GetId(),
		std::bind( &BaseBehavior::onCollisionEnter, this, std::placeholders::_1 )
	);

	m_AudioPlayer = GetEntity()->GetComponent<AudioPlayer>();
}

/// @brief	called on exit, handles loss state
void BaseBehavior::OnExit()
{
	BasicEntityBehavior::OnExit();
}

void BaseBehavior::Inspector()
{

}

void BaseBehavior::Destroy()
{
	SceneSystem::GetInstance()->SetNextScene("Gameover");
}


/// @brief  called whenever the base's Collider enters a collision
/// @param  other   the collider that was collided with
void BaseBehavior::onCollisionEnter( Collider* other )
{
	//EnemyBehavior* enemy = other->GetEntity()->GetComponent<EnemyBehavior>();
	//if (!enemy)
	//{
		//return;
	//}
	//BasicEntityBehavior::GetHealth()->SetCurrent
	//(GetEntity()->GetComponent<GeneratorBehavior>()->GetHealth()->GetCurrent());
	//BasicEntityBehavior::TakeDamage(enemy->GetDamage());
	//if(m_AudioPlayer)
	//{
		//m_AudioPlayer->Play();
	//}
	//enemy->GetEntity()->Destroy();

	//if (GetEntity()->GetComponent<Health>()->GetHealth()->GetCurrent() <= 0)
	//{
		//SceneSystem::GetInstance()->SetNextScene("Gameover");
	//}

	
}

//-----------------------------------------------------------------------------
// reading
//-----------------------------------------------------------------------------

/// @brief read method map
ReadMethodMap<BaseBehavior> const BaseBehavior::s_ReadMethods =
{
	
};

//-----------------------------------------------------------------------------
// writing
//-----------------------------------------------------------------------------

/// @brief	write to json
nlohmann::ordered_json BaseBehavior::Write() const
{
	nlohmann::ordered_json data;



	return data;
}


