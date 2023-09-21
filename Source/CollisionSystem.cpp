/// @file 
/// @author 
/// @brief 
/// @version 0.1
/// @date 2023-09-05
/// 
/// @copyright Copyright (c) 2023

#include "CollisionSystem.h"
#include "Collider.h"
//-----------------------------------------------------------------------------
// virtual override methods
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// singleton stuff
//-----------------------------------------------------------------------------

    /// @brief Constructs the CollisionSystem
CollisionSystem::CollisionSystem() {}

/// @brief The singleton instance of CollisionSystem
CollisionSystem* CollisionSystem::instance = nullptr;

/// @brief gets the instance of CollisionSystem
/// @return the instance of the CollisionSystem
CollisionSystem* CollisionSystem::getInstance()
{
    if (instance == nullptr)
    {
        instance = new CollisionSystem();
    }
    return instance;
}

void CollisionSystem::addCollider(Collider* collider)
{
    colliderList.push_back(collider);
}

void CollisionSystem::removeCollider(Collider* collider)
{
    colliderList.erase(std::remove(colliderList.begin(), colliderList.end(), collider), colliderList.end());
}

void CollisionSystem::OnFixedUpdate()
{
    for (auto collider : colliderList)
    {
		collider->OnFixedUpdate();
	}
}

void CollisionSystem::checkCollisions()
{
    for (auto collider : colliderList)
    {
        for (auto otherCollider : colliderList)
        {
            if (collider != otherCollider)
            {
                if (collider->CheckCollision(otherCollider))
                {

                }
			}
		}
	}
}

//-----------------------------------------------------------------------------
