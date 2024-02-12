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
BaseBehavior::BaseBehavior() : Behavior(typeid(BaseBehavior))
{
}

/// @brief  copy ctor
BaseBehavior::BaseBehavior(const BaseBehavior& other) : Behavior(other)
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
}

/// @brief	called on exit, handles loss state
void BaseBehavior::OnExit()
{
}

/// @brief	destroy the base
void BaseBehavior::Destroy()
{
	SceneSystem::GetInstance()->SetNextScene("Gameover");
}

Pool<int>* BaseBehavior::GetHealth()
{
	return GetEntity()->GetComponent<Health>()->GetHealth();
}

//-----------------------------------------------------------------------------
