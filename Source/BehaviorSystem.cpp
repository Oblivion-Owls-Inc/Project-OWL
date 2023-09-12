/// @file     BehaviorSystem.cpp
/// @author   Name (first.last@digipen.edu)
/// 
/// @brief    Example System meant to be copy-pasted when creating new Systems
#include "BehaviorSystem.h"

// Define the static instance variable for the templated BehaviorSystem
template <typename BehaviorType>
BehaviorSystem<BehaviorType>* BehaviorSystem<BehaviorType>::instance = nullptr;

// Define the constructor for the templated BehaviorSystem
template <typename BehaviorType>
BehaviorSystem<BehaviorType>::BehaviorSystem() {}

// Define the getInstance function for the templated BehaviorSystem
template <typename BehaviorType>
BehaviorSystem<BehaviorType>* BehaviorSystem<BehaviorType>::getInstance()
{
    if (instance == nullptr)
    {
        instance = new BehaviorSystem<BehaviorType>();
    }
    return instance;
}

