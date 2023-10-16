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

#include <sstream>
#include <algorithm>

#include "DebugSystem.h"

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


    /// @brief  makes a CollisionLayerFlags for a set of layer names
    /// @param  layerNames  the names of the layers to include in the flags
    CollisionLayerFlags CollisionSystem::GetLayerFlags( std::vector< std::string > const& layerNames ) const
    {
        CollisionLayerFlags flags = 0;
        for ( std::string const& name : layerNames )
        {
            flags |= 1 << GetCollisionLayerId( name );
        }
        return flags;
    }

    /// @brief  gets the collision layer ID with the specified name
    /// @param  layerName   the name of the layer to get
    /// @return the collision layer ID
    unsigned CollisionSystem::GetCollisionLayerId( std::string const& layerName ) const
    {
        auto it = std::find( m_CollisionLayerNames.begin(), m_CollisionLayerNames.end(), layerName );
        if ( it == m_CollisionLayerNames.end() )
        {
            throw std::runtime_error(
                std::string() + "Error: \"" + layerName + "\" is not a recognized collision layer name"
            );
        }

        return it - m_CollisionLayerNames.begin();
    }

    /// @brief  gets the names of the layers in a CollisionLayerFlags
    /// @param  layerFlags  the layer flags to get the names of
    /// @return the names of the layers
    std::vector< std::string > CollisionSystem::GetLayerNames( CollisionLayerFlags layerFlags ) const
    {
        std::vector< std::string > layerNames;
        for ( unsigned i = 0; layerFlags > 0; ++i )
        {
            if ( layerFlags & (1 << i) )
            {
                layerNames.push_back( GetLayerName( i ) );
                layerFlags ^= 1 << i;
            }
        }
        return layerNames;
    }

    /// @brief  gets the name of the specified layer
    /// @param  layerId the ID of the layer to get the name of
    /// @return the name of the layer
    std::string const& CollisionSystem::GetLayerName( unsigned layerId ) const
    {
        return m_CollisionLayerNames[ layerId ];
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

    /// @brief  creates the debug window for the CollisionSystem
    void CollisionSystem::DebugWindow()
    {
        if ( !ImGui::BeginListBox("Collision Layers") )
        {
            return;
        }

        for (int i = 0; i < m_CollisionLayerNames.size(); ++i )
        {
            char buffer[ 64 ];
            strncpy_s( buffer, IM_ARRAYSIZE( buffer ), m_CollisionLayerNames[i].c_str(), m_CollisionLayerNames[i].size());
            ImGui::InputText( (std::string() + "Layer Name##" + std::to_string( i )).c_str(), buffer, IM_ARRAYSIZE(buffer));
            m_CollisionLayerNames[ i ] = buffer;
        }

        ImGui::EndListBox();

        if ( ImGui::Button( "Add Layer" ) )
        {
            m_CollisionLayerNames.push_back( "new_layer" );
        }

        if ( ImGui::Button( "Remove Layer" ) )
        {
            m_CollisionLayerNames.pop_back();
        }
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
    void CollisionSystem::CheckCollision( Collider* colliderA, Collider* colliderB )
    {
        // check if the layers interact
        bool aCollidesB = colliderA->GetCollisionLayerFlags() & ( 1 << colliderB->GetCollisionLayerId() );
        bool bCollidesA = colliderB->GetCollisionLayerFlags() & ( 1 << colliderA->GetCollisionLayerId() );

        // if collision layers don't interact, don't test collision
        if ( !aCollidesB && !bCollidesA )
        {
            return;
        }

        // ensure that both colliders have Transforms
        if ( colliderA->GetTransform() == nullptr || colliderB->GetTransform() == nullptr )
		{
            throw std::runtime_error(
                "Error: Collider component must always be accompanied by a Transform component"
            );
		}

        // check type of each collider
        std::pair< std::type_index, std::type_index > colliderTypes = {
            colliderA->GetType(),
            colliderB->GetType()
        };

        // find the collision function between those two shapes
        auto checkFuncIt = s_CollisionFunctions.find( colliderTypes );
        if ( checkFuncIt == s_CollisionFunctions.end() )
        {
            // if no collision function found, swap the order of the colliders and search again
            colliderTypes = {
                colliderTypes.second,
                colliderTypes.first
            };

            Collider* tempPtr = colliderA;
            colliderA = colliderB;
            colliderB = tempPtr;

            bool tempBool = aCollidesB;
            aCollidesB = bCollidesA;
            bCollidesA = tempBool;

            checkFuncIt = s_CollisionFunctions.find( colliderTypes );
            if ( checkFuncIt == s_CollisionFunctions.end() )
            {
                // if still no function found, fail
                #ifndef NDEBUG // Let it silently fail in release mode
                    throw std::runtime_error(
                        std::string() +
                        "Error: no collision function implemented between " +
                        colliderTypes.first.name() + " and " +
                        colliderTypes.second.name()
                    );
                #endif // !NDEBUG

                return;
            }
        }

        // check the collision
        CollisionData collisionData;
        if ( (*checkFuncIt->second)( colliderA, colliderB, &collisionData ) )
        {
            // call callbacks 
            if ( aCollidesB )
            {
                colliderA->CallOnCollisionCallbacks( colliderB, collisionData );
            }

            if ( bCollidesA )
            {
                collisionData.normal *= -1;
                colliderB->CallOnCollisionCallbacks( colliderA, collisionData );
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

    /// @brief  reads the collision layer names from json
    /// @param  data    the json data to read from
    void CollisionSystem::readCollisionLayerNames( nlohmann::ordered_json const& data )
    {
        m_CollisionLayerNames = data;
    }

    /// @brief map of the CollisionSystem read methods
    ReadMethodMap< CollisionSystem > const CollisionSystem::s_ReadMethods = {
        { "CollisionLayerNames", &readCollisionLayerNames }
    };

    /// @brief  writes the CollisionSystem config to json
    /// @return the written json data
    nlohmann::ordered_json CollisionSystem::Write() const
    {
        nlohmann::ordered_json json;

        json[ "CollisionLayerNames" ] = m_CollisionLayerNames;

        return json;
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
