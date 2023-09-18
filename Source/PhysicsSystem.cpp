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

void PhysicsSystem::Load(rapidjson::Value const& configData)
{
}

PhysicsSystem* PhysicsSystem::getInstance()
{
    if (instance == nullptr)
    {
        instance = new PhysicsSystem();
    }

    return instance;
}
