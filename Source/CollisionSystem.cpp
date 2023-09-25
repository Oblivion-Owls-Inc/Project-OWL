/// @file 
/// @author 
/// @brief 
/// @version 0.1
/// @date 2023-09-05
/// 
/// @copyright Copyright (c) 2023

#include "CollisionSystem.h"
#include "DebugSystem.h"
#include "Entity.h"
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
CollisionSystem* CollisionSystem::GetInstance()
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
    checkCollisions();
    for (auto collider : colliderList)
    {
        if (collider->isColliding())
        {
            Collider* other = collider->getOtherCollider();

            DebugConsole output( *DebugSystem::GetInstance() );
            output << "Collider: " << collider->Parent()->GetName().c_str()
                << " is colliding with: "
                << other->Parent()->GetName() << "\n";
            collider->isColliding(false);
        }
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
                if (collider->CheckIfColliding(otherCollider))
                {
					collider->isColliding(true);
					collider->setOtherCollider(otherCollider);
                }
			}
		}
	}
}

//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------

    /// @brief map of the CollisionSystem read methods
    ReadMethodMap< CollisionSystem > const CollisionSystem::s_ReadMethods = {};

    /// @brief  gets this System's read methods
    /// @return this System's read methods
    ReadMethodMap< System > const& CollisionSystem::GetReadMethods() const
    {
        return (ReadMethodMap< System > const&)s_ReadMethods;
    }

//-----------------------------------------------------------------------------
