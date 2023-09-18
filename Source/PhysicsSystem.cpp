///@file Physics.cpp
/// @brief Physics class 
/// @author Jax Clayton (jax.clayton)
/// 
/// @details This class is used to handle all physics related calculations
///

#include "PhysicsSystem.h"

PhysicsSystem * PhysicsSystem::instance = nullptr;

PhysicsSystem::PhysicsSystem()
{
}

void PhysicsSystem::OnInit()
{
}

void PhysicsSystem::OnFixedUpdate()
{
}

void PhysicsSystem::OnSceneExit()
{
}

void PhysicsSystem::OnUpdate(float dt)
{
}

void PhysicsSystem::OnExit()
{
}


/// @brief the Read Methods used in this System
std::map< std::string, ReadMethod< PhysicsSystem > > const PhysicsSystem::ReadMethods = {};

/// @brief Gets the read methods of this System
/// @return the map of read methods of this System
std::map< std::string, ReadMethod< System > > const& PhysicsSystem::GetReadMethods()
{
    return (std::map< std::string, ReadMethod< System > > const&)ReadMethods;
}


PhysicsSystem* PhysicsSystem::getInstance()
{
    if (instance == nullptr)
    {
        instance = new PhysicsSystem();
    }

    return instance;
}
