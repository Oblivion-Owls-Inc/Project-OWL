/// @file CollisionSystem.h
/// @author Jax Clayton  (jax.clayton@digipen.edu)
/// @brief The CollisionSystem is responsible for checking for collisions between Different Colliders
/// @version 0.1
/// @date 2023-09-15
/// 
/// @copyright  Digipen LLC (c) 2023

#pragma once

//-----------------------------------------------------------------------------
// Includes:
//-----------------------------------------------------------------------------

#include "CollisionSystem.h"

#include "Collider.h"
#include "CircleCollider.h"
#include "CollisionData.h"

#include "Entity.h"
#include "Behavior.h"
#include "Transform.h"

//-----------------------------------------------------------------------------
// public: methods
//-----------------------------------------------------------------------------

    /// @brief  adds a Collider to the CollisionSystem
    /// @param  collider    the collider to add
    void CollisionSystem::addCollider( Collider* collider )
    {
        m_Colliders.push_back( collider );
    }

    /// @brief  removes a Collider from this System
    /// @param  collider    the collider to remove
    void CollisionSystem::removeCollider( Collider* collider )
    {
        m_Colliders.erase(
            std::remove( m_Colliders.begin(), m_Colliders.end(), collider )
        );
    }

//-----------------------------------------------------------------------------
// public: virtual override methods
//-----------------------------------------------------------------------------

    /// @brief  Gets called once every simulation frame. Use this function for anything that affects the simulation.
    void CollisionSystem::OnFixedUpdate()
    {
        checkCollisions();
    }

    void CollisionSystem::OnSceneExit()
    {
        m_Colliders.clear();
    }

//-----------------------------------------------------------------------------
// private: methods
//-----------------------------------------------------------------------------

    /// @brief Checks and handles all Collisions
    void CollisionSystem::checkCollisions()
    {
        for ( int i = 0; i < (int)m_Colliders.size() - 1; ++i )
        {
            for ( int j = i + 1; j < m_Colliders.size(); ++j )
            {
                CheckCollision( m_Colliders[i], m_Colliders[j] );
            }
        }
    }

//-----------------------------------------------------------------------------
// private: static methods
//-----------------------------------------------------------------------------

    /// @brief  checks a collision between two colliders of unknown type
    /// @param  colliderA       the first collider
    /// @param  colliderB       the second collider
    /// @param  collisionData   pointer to where to store additional data about the collision
    /// @return whether or not the two colliders are colliding
    void CollisionSystem::CheckCollision( Collider const* colliderA, Collider const* colliderB )
    {
        std::pair< std::type_index, std::type_index > colliderTypes
            = { colliderA->GetType(), colliderB->GetType() };

        auto checkFuncIt = s_CollisionFunctions.find( colliderTypes );
        if ( checkFuncIt == s_CollisionFunctions.end() )
        {
            colliderTypes = {
                colliderTypes.second,
                colliderTypes.first
            };

            Collider const* temp = colliderA;
            colliderA = colliderB;
            colliderB = temp;

            checkFuncIt = s_CollisionFunctions.find( colliderTypes );
            if ( checkFuncIt == s_CollisionFunctions.end() )
            {
                std::stringstream errorMessage;
                errorMessage <<
                    "Error: no collision function implemented between " <<
                    colliderTypes.first.name() << " and " <<
                    colliderTypes.second.name();
                throw std::runtime_error( errorMessage.str() );
            }
        }

        CollisionData collisionData;
        if ( (*checkFuncIt->second)( colliderA, colliderB, &collisionData ) )
        {
            for ( Behavior* behavior : colliderA->GetParent()->GetComponentsOfType<Behavior>() )
            {
                behavior->OnCollision( colliderB->GetParent(), collisionData );
            }

            collisionData.normal *= -1;

            for ( Behavior* behavior : colliderB->GetParent()->GetComponentsOfType<Behavior>() )
            {
                behavior->OnCollision( colliderA->GetParent(), collisionData );
            }
        }
    }

    /// @brief  checks a collision between two circle colliders
    /// @param  colliderA       the first collider
    /// @param  colliderB       the second collider
    /// @param  collisionData   pointer to where to store additional data about the collision
    /// @return whether or not the two colliders are colliding
    bool CollisionSystem::CheckCircleCircle( Collider const* colliderA, Collider const* colliderB, CollisionData* collisionData )
    {
        CircleCollider const* circleA = (CircleCollider const*)colliderA;
        CircleCollider const* circleB = (CircleCollider const*)colliderB;

        glm::vec2 posA = colliderA->GetTransform()->GetTranslation();
        glm::vec2 posB = colliderB->GetTransform()->GetTranslation();

        glm::vec2 displacement = posB - posA;
        float distance = glm::length( displacement );

        float minDistance = circleA->GetRadius() + circleB->GetRadius();
        
        if ( distance >= minDistance )
        {
            return false;
        }

        if ( collisionData != nullptr )
        {
            collisionData->normal = -displacement / distance;
            collisionData->position = (
                posA - collisionData->normal * circleA->GetRadius() +
                posB + collisionData->normal * circleB->GetRadius()
            ) * 0.5f;
            collisionData->depth = minDistance - distance;
        }

        return true;
    }

//-----------------------------------------------------------------------------
// private: static members
//-----------------------------------------------------------------------------

    /// @brief map that stores the CollisionCheckMethods between each Collider type
    CollisionFunctionMap const CollisionSystem::s_CollisionFunctions = {
        { { typeid( CircleCollider ), typeid( CircleCollider ) }, &CheckCircleCircle }
    };

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
// singleton stuff
//-----------------------------------------------------------------------------

    /// @brief Constructs the CollisionSystem
    CollisionSystem::CollisionSystem() :
        System( "CollisionSystem" )
    {}

    /// @brief The singleton s_Instance of CollisionSystem
    CollisionSystem* CollisionSystem::s_Instance = nullptr;

    /// @brief gets the instance of CollisionSystem
    /// @return the instance of the CollisionSystem
    CollisionSystem* CollisionSystem::GetInstance()
    {
        if ( s_Instance == nullptr )
        {
            s_Instance = new CollisionSystem();
        }
        return s_Instance;
    }

//-----------------------------------------------------------------------------
