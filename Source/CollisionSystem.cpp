///*****************************************************************/
/// @file	 CollisionSystem.cpp
/// @Author  Jax Clayton (jax.clayton@digipen.edu)
/// @date	 9/15/2021
/// @brief   CollisionSystem  
/// @details This class handles all collisions between colliders
///*****************************************************************/

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

/// @brief The singleton s_Instance of CollisionSystem
CollisionSystem* CollisionSystem::s_Instance = nullptr;

/// @brief gets the instance of CollisionSystem
/// @return the instance of the CollisionSystem
CollisionSystem* CollisionSystem::GetInstance()
{
    if (s_Instance == nullptr)
    {
        s_Instance = new CollisionSystem();
    }
    return s_Instance;
}

void CollisionSystem::addCollider(Collider* collider)
{
    m_ColliderList.push_back(collider);
}

void CollisionSystem::removeCollider(Collider* collider)
{
    m_ColliderList.erase(std::remove(m_ColliderList.begin(), m_ColliderList.end(), collider), m_ColliderList.end());
}

void CollisionSystem::OnFixedUpdate()
{
    checkCollisions();
    for (auto collider : m_ColliderList)
    {
        if (collider->isColliding())
        {
            Collider* other = collider->getOtherCollider();

            DebugConsole output( *DebugSystem::GetInstance() );
            output << "Collider: " << collider->Parent()->GetName().c_str()
                << " is colliding with: "
                << other->Parent()->GetName() << "\n";
        }
	}

}

void CollisionSystem::checkCollisions()
{
    for (auto collider : m_ColliderList)
    {
        for (auto otherCollider : m_ColliderList)
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
