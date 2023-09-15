/// @file     BehaviorSystem.cpp
/// @author   Name (first.last@digipen.edu)
/// 
/// @brief    Example System meant to be copy-pasted when creating new Systems
#include "BehaviorSystem.h"

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
		behavior->FixedUpdate();
	}
}

template<typename BehaviorType>
void BehaviorSystem<BehaviorType>::OnUpdate(float dt)
{
    for(auto behavior : behaviorsList)
	{
		behavior->Update(dt);
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

// Define the constructor for the templated BehaviorSystem
template < typename BehaviorType >
BehaviorSystem< BehaviorType >::BehaviorSystem()
{

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

