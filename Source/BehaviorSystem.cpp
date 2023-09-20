/// @file     BehaviorSystem.cpp
/// @author   Name (first.last@digipen.edu)
/// 
/// @brief    Example System meant to be copy-pasted when creating new Systems

#define BEHAVIORSYSTEM_C

#ifndef BEHAVIORSYSTEM_H
#include "BehaviorSystem.h"
#endif

#include "Entity.h"
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
	for(auto behavior : behaviorsList)
	{
		behavior->OnFixedUpdate();
		if (behavior->Parent()->isColliding())
		{
			behavior->OnCollisionEvent();
		}
	}
}

template<typename BehaviorType>
void BehaviorSystem<BehaviorType>::OnUpdate(float dt)
{
    for(auto behavior : behaviorsList)
	{
		behavior->OnUpdate(dt);
		if (behavior->Parent()->isColliding())
		{
			behavior->OnCollisionEvent();
		}
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
	behaviorsList.erase(std::remove(behaviorsList.begin(), behaviorsList.end(), behavior), behaviorsList.end());
}

// Define the getInstance function for the templated BehaviorSystem
template < typename BehaviorType >
BehaviorSystem< BehaviorType >* BehaviorSystem< BehaviorType >::getInstance()
{
    if ( instance == nullptr )
    {
        instance = new BehaviorSystem< BehaviorType >();
    }
    return instance;
}

template<typename BehaviorType>
std::vector<BehaviorType*>& BehaviorSystem<BehaviorType>::getBehaviors() const
{
    return &behaviorsList;
}

