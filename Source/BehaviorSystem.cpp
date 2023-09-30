///*****************************************************************/
/// @file	 BehaviorSystem.cpp
/// @Author  Jax Clayton (jax.clayton@digipen.edu)
/// @date	 9/15/2021
/// @brief   BehaviorSystem  
/// @details This class handles all behaviors for entities
///*****************************************************************/
#define BEHAVIORSYSTEM_C

#ifndef BEHAVIORSYSTEM_H
#include "BehaviorSystem.h"
#endif

#include "Entity.h"
#include "Collider.h"
#include "Behavior.h"
// Define the static instance variable for the templated BehaviorSystem
template < typename BehaviorType >
BehaviorSystem< BehaviorType >* BehaviorSystem< BehaviorType >::instance = nullptr;

template<typename BehaviorType>
void BehaviorSystem<BehaviorType>::OnInit()
{
}

template<typename BehaviorType>
void BehaviorSystem<BehaviorType>::OnFixedUpdate()
{
	for (auto behavior : behaviorsList)
	{
		behavior->OnFixedUpdate();
	}

}

template<typename BehaviorType>
void BehaviorSystem<BehaviorType>::OnUpdate(float dt)
{
	for (auto behavior : behaviorsList)
	{
		behavior->OnUpdate(dt);
	}
}

template<typename BehaviorType>
void BehaviorSystem<BehaviorType>::OnExit()
{
}

template<typename BehaviorType>
void BehaviorSystem<BehaviorType>::OnSceneExit()
{
}


template<typename BehaviorType>
void BehaviorSystem<BehaviorType>::AddBehavior(BehaviorType* behavior)
{
	behaviorsList.push_back(behavior);
}

template<typename BehaviorType>
void BehaviorSystem<BehaviorType>::RemoveBehavior(BehaviorType* behavior)
{
	behaviorsList.erase(
        std::remove(behaviorsList.begin(), behaviorsList.end(), behavior), behaviorsList.end()
    );
}

// Define the GetInstance function for the templated BehaviorSystem
template < typename BehaviorType >
BehaviorSystem< BehaviorType >* BehaviorSystem< BehaviorType >::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new BehaviorSystem< BehaviorType >();
	}
	return instance;
}

template<typename BehaviorType>
std::vector<BehaviorType*>& BehaviorSystem<BehaviorType>::GetBehaviors() const
{
	return &behaviorsList;
}

//-----------------------------------------------------------------------------
// private: reading
//----------------------------------------------------------------------------- 

	/// @brief                  the read methods of a BehaviorSystem
	/// @tparam BehaviorType    the type of behavior this BehaviorSystem manages
template<typename BehaviorType>
ReadMethodMap< BehaviorSystem< BehaviorType > > const BehaviorSystem< BehaviorType >::s_ReadMethods = {};

/// @brief                  gets the read methods for this System
/// @tparam BehaviorType    the type of behavior this BehaviorSystem manages
/// @return                 the read methods for this System
template<typename BehaviorType>
ReadMethodMap< System > const& BehaviorSystem< BehaviorType >::GetReadMethods() const
{
	return (ReadMethodMap< System > const&)s_ReadMethods;
}
