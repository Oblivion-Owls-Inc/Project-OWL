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

void PhysicsSystem::AddBehavior(Behavior* behavior)
{
    if (behavior)
	{
		m_behaviors.push_back(behavior);
	}
}

void PhysicsSystem::RemoveBehavior(Behavior* behavior)
{
    auto it = std::find(m_behaviors.begin(), m_behaviors.end(), behavior);

    // Check if the behavior was found
    if (it != m_behaviors.end())
    {
        // Remove the behavior from the vector
        m_behaviors.erase(it);
    }
	
}

void PhysicsSystem::AddCollider(Collider* collider)
{
    if (collider)
    {
        m_colliders.push_back(collider);
    }
}

void PhysicsSystem::RemoveCollider(Collider* collider)
{
    // Use an iterator to find the collider in the vector
    auto it = std::find(m_colliders.begin(), m_colliders.end(), collider);

    // Check if the collider was found
    if (it != m_colliders.end())
    {
        // Remove the collider from the vector
        m_colliders.erase(it);
    }
}
