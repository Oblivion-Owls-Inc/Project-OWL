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
#include "TilemapCollider.h"
#include "Tilemap.h"

#include "CollisionData.h"

#include "Entity.h"
#include "Behavior.h"
#include "Transform.h"

#include <sstream>
#include <algorithm>

#include "DebugSystem.h"
#include "RenderSystem.h"

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

        return (int)(it - m_CollisionLayerNames.begin());
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


    /// @brief  casts a ray in the scene
    /// @param  origin      the origin of the cast ray
    /// @param  direction   the direction of the ray
    /// @param  maxDistance the maximum distance of the raycast
    /// @param  layers      the layers for the raycast to collide with
    /// @return RayCastHit struct containing information about the result of the raycast
    RayCastHit CollisionSystem::RayCast( glm::vec2 const& origin, glm::vec2 const& direction, float maxDistance, CollisionLayerFlags layers ) const
    {
        RayCastHit hit;
        hit.distance = maxDistance;
        for ( Collider* collider : m_Colliders )
        {
            if ( !(layers & (1 << collider->GetCollisionLayerId())) )
            {
                continue;
            }

            if ( collider->GetType() == typeid( CircleCollider ) )
            {
                checkRayCircle( origin, direction, (CircleCollider*)collider, &hit );
            }
            else if ( collider->GetType() == typeid( TilemapCollider ) )
            {
                checkRayTilemap( origin, direction, (TilemapCollider*)collider, &hit );
            }
        }
        return hit;
    }

//-----------------------------------------------------------------------------
// public: virtual override methods
//-----------------------------------------------------------------------------

    /// @brief  Gets called once every simulation frame. Use this function for anything that affects the simulation.
    void CollisionSystem::OnFixedUpdate()
    {
        for ( unsigned i = 0; i < m_CollisionSteps; ++i )
        {
            checkCollisions();
        }
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
                checkCollision( m_Colliders[i], m_Colliders[j] );
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
    void CollisionSystem::checkCollision( Collider* colliderA, Collider* colliderB )
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
    bool CollisionSystem::checkCircleCircle( Collider const* colliderA, Collider const* colliderB, CollisionData* collisionData )
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
            collisionData->normal = distance == 0 ? glm::vec2( 0 ) : - displacement / distance;
            collisionData->position = (
                posA - collisionData->normal * circleA->GetRadius() +
                posB + collisionData->normal * circleB->GetRadius()
            ) * 0.5f;
            collisionData->depth = minDistance - distance;
        }

        return true;
    }


    /// @brief  checks a collision between a circle and tilemap collider
    /// @param  colliderA       the first collider
    /// @param  colliderB       the second collider
    /// @param  collisionData   pointer to where to store additional data about the collision
    /// @return whether or not the two colliders are colliding
    bool CollisionSystem::checkCircleTilemap( Collider const* colliderA, Collider const* colliderB, CollisionData* collisionData )
    {
        CircleCollider const* circle = (CircleCollider const*)colliderA;
        Tilemap< int > const* tilemap = ((TilemapCollider const*)colliderB)->GetTilemap();

        glm::mat4 const& worldToTile = tilemap->GetWorldToTilemapMatrix();
        glm::mat4 const& tileToWorld = tilemap->GetTilemapToWorldMatrix();
        glm::vec2 const& tileSize = tilemap->GetTileScale();

        glm::vec2 pos = circle->GetTransform()->GetTranslation();
        float radius = circle->GetRadius() / tileSize.x;

        if ( std::abs( tileSize.x ) != std::abs( tileSize.y ) )
        {
            throw std::runtime_error(
                "Error: Tilemap must be uniformly scaled for collisions to work"
            );
        }

        // Renderer()->DrawRect( pos, glm::vec2( radius * 2.0f ) );

        pos = worldToTile * glm::vec4( pos, 0, 1 );
        glm::vec2 extents = glm::vec2( radius, radius );
        glm::ivec2 minTile = pos - extents;
        glm::ivec2 maxTile = pos + extents;



        bool collision = false;

        glm::ivec2 tilePos;
        for ( tilePos.y = minTile.y; tilePos.y <= maxTile.y; ++tilePos.y )
        {
            for ( tilePos.x = minTile.x; tilePos.x <= maxTile.x; ++tilePos.x )
            {
                if (
                    tilePos.x < 0 || tilePos.x >= tilemap->GetDimensions().x ||
                    tilePos.y < 0 || tilePos.y >= tilemap->GetDimensions().y
                )
                {
                    continue;
                }

                if ( tilemap->GetTile( tilePos ) == 0 )
                {
                    continue;
                }

                // Renderer()->DrawRect( tileToWorld * glm::vec4( (glm::vec2)tilePos + glm::vec2( 0.5f, 0.5f), 0, 1 ), tileToWorld * glm::vec4( 1, 1, 0, 0 ) );

                CollisionData tempCollisionData;
                if ( !checkCircleAABB( pos, radius, tilePos, tilePos + glm::ivec2( 1, 1 ), collisionData ? &tempCollisionData : nullptr) )
                {
                    continue;
                }

                collision = true;
                if ( collisionData && tempCollisionData.depth > collisionData->depth )
                {
                    *collisionData = tempCollisionData;
                }
            }
        }

        if ( collisionData )
        {
            collisionData->normal = tileToWorld * glm::vec4( collisionData->normal, 0, 0 );
            collisionData->position = tileToWorld * glm::vec4( collisionData->position, 0, 1 );
        }

        return collision;

    }


    /// @brief  helper function which checks a circle against an axis aligned rectangle
    /// @param  circlePos       the position of the circle
    /// @param  circleRadius    the radius of the circle
    /// @param  aabbMin         the min pos of the AABB
    /// @param  aabbMax         the max pos of the AABB
    /// @param  collisionData   pointer to where to store additional data about the collision
    /// @return whether or not the two shapes are colliding
    /// @note   ASSUMES THAT THE AABB OF THE CIRCLE IS KNOWN TO OVERLAP THE RECTANGLE
    bool CollisionSystem::checkCircleAABB( glm::vec2 const& circlePos, float circleRadius, glm::vec2 const& aabbMin, glm::vec2 const& aabbMax, CollisionData* collisionData )
    {
        if ( circlePos.x >= aabbMax.x )
        {
            if ( circlePos.y >= aabbMax.y )
            { // top right corner
                return checkCirclePoint( circlePos, circleRadius, aabbMax, collisionData );
            }
            else if ( circlePos.y <= aabbMin.y )
            { // bottom right corner
                return checkCirclePoint( circlePos, circleRadius, glm::vec2( aabbMax.x, aabbMin.y ), collisionData );
            }
            else
            { // right edge
                if ( collisionData != nullptr )
                {
                    collisionData->depth = aabbMax.x - (circlePos.x - circleRadius);
                    collisionData->position = circlePos - glm::vec2( circleRadius, 0 );
                    collisionData->normal = glm::vec2( 1, 0 );
                }

                return true;
            }
        }
        else if ( circlePos.x <= aabbMin.x )
        {
            if ( circlePos.y >= aabbMax.y )
            { // top left corner
                return checkCirclePoint( circlePos, circleRadius, glm::vec2( aabbMin.x, aabbMax.y ), collisionData);
            }
            else if ( circlePos.y <= aabbMin.y )
            { // bottom left corner
                return checkCirclePoint( circlePos, circleRadius, aabbMin , collisionData );
            }
            else
            { // left edge
                if ( collisionData != nullptr )
                {
                    collisionData->depth = (circlePos.x + circleRadius) - aabbMin.x;
                    collisionData->position = circlePos + glm::vec2( circleRadius, 0 );
                    collisionData->normal = glm::vec2( -1, 0 );
                }

                return true;
            }
        }
        else
        {
            if ( circlePos.y >= aabbMax.y )
            { // top edge
                if ( collisionData != nullptr )
                {
                    collisionData->depth = aabbMax.y - (circlePos.y - circleRadius);
                    collisionData->position = circlePos - glm::vec2( 0, circleRadius );
                    collisionData->normal = glm::vec2( 0, 1 );
                }

                return true;
            }
            else if ( circlePos.y <= aabbMin.y )
            { // bottom edge
                if ( collisionData != nullptr )
                {
                    collisionData->depth = (circlePos.y + circleRadius) - aabbMin.y;
                    collisionData->position = circlePos + glm::vec2( 0, circleRadius );
                    collisionData->normal = glm::vec2( 0, -1 );
                }

                return true;
            }
            else
            { // interior
                return false;
            }
        }
    }


    /// @brief  helper function which checks a circle against a point
    /// @param  circlePos       the position of the circle
    /// @param  circleRadius    the radius of the circle
    /// @param  point           the pos position of the point
    /// @param  collisionData   pointer to where to store additional data about the collision
    /// @return whether or not the two shapes are colliding
    bool CollisionSystem::checkCirclePoint( glm::vec2 const& circlePos, float circleRadius, glm::vec2 const& point, CollisionData* collisionData )
    {
        glm::vec2 offset = circlePos - point;

        float distanceSquared = glm::dot( offset, offset );

        if ( distanceSquared >= circleRadius * circleRadius )
        {
            return false;
        }

        if ( collisionData != nullptr )
        {
            float distance = std::sqrt( distanceSquared );
            collisionData->depth = circleRadius - distance;
            collisionData->normal = distance == 0 ? glm::vec2( 0 ) : offset / distance;
            collisionData->position = point;
        }

        return true;
    }




    /// @brief  checks if a raycast hits a Circle
    /// @param  rayOrigin       the origin of the cast ray
    /// @param  rayDirection    the direction of the cast ray
    /// @param  circle          the CircleCollider to test the ray against
    /// @param  rayCastHit      information on the current best hit, to be overridden if this hit is better
    void CollisionSystem::checkRayCircle( glm::vec2 const& rayOrigin, glm::vec2 const& rayDirection, CircleCollider* circle, RayCastHit* rayCastHit )
    {
        glm::vec2 offset = rayOrigin - circle->GetTransform()->GetTranslation();
        
        // quadratic coefficients
        float a = glm::dot( rayDirection, rayDirection );
        float b = 2 * glm::dot( rayDirection, offset );
        float c = glm::dot( offset, offset ) - circle->GetRadius() * circle->GetRadius();

        // if part inside of radical would be negative, there's no collision
        float radical = b * b - 4 * a * c;
        if ( radical < 0 )
        {
            return;
        }
        radical = std::sqrt( radical );

        // get the smaller of the two results (only collide when entering circle, not when exiting)
        float distance = ( -b - radical ) / ( 2 * a );

        if ( distance >= 0 && distance < rayCastHit->distance )
        {
            rayCastHit->distance = distance;
            rayCastHit->colliderHit = circle;
            rayCastHit->normal = glm::normalize( offset );
        }
    }


    /// @brief  checks if a raycast hits a Circle
    /// @param  rayOrigin       the origin of the cast ray
    /// @param  rayDirection    the direction of the cast ray
    /// @param  tilemapCollider the TilemapCollider to test the ray against
    /// @param  rayCastHit      information on the current best hit, to be overridden if this hit is better
    void CollisionSystem::checkRayTilemap( glm::vec2 const& rayOrigin, glm::vec2 const& rayDirection, TilemapCollider* tilemapCollider, RayCastHit* rayCastHit )
    {
        // get relevant variables from collider
        Tilemap<int> const* tilemap = tilemapCollider->GetTilemap();
        glm::mat4 const& worldToTile = tilemap->GetWorldToTilemapMatrix();

        // convert to tilemap space
        glm::vec2 tilePos = worldToTile * glm::vec4( rayOrigin, 0, 1 );
        glm::vec2 tileVel = worldToTile * glm::vec4( rayDirection, 0, 0 );

        glm::ivec2 tile = tilePos;
        // direction of the step in each axis
        glm::ivec2 stepDir = {
            tileVel.x > 0 ? 1 : ( tileVel.x < 0 ? -1 : 0 ),
            tileVel.y > 0 ? 1 : ( tileVel.y < 0 ? -1 : 0 )
        };

        // length of the step in each axis
        glm::vec2 deltaT = {
            tileVel.x == 0 ? INFINITY : 1 / std::abs( tileVel.x ),
            tileVel.y == 0 ? INFINITY : 1 / std::abs( tileVel.y )
        };

        // current position in each axis
        glm::vec2 t = tilePos - (glm::vec2)tile;
        if ( stepDir.x == 1 ) { t.x = 1 - t.x; }
        if ( stepDir.y == 1 ) { t.y = 1 - t.y; }
        t *= deltaT;

        // loop until max distance reached
        while ( t.x < rayCastHit->distance || t.y < rayCastHit->distance )
        {
            // step in the closest direction
            int stepAxis;
            if ( t.x < t.y )
            {
                tile.x += stepDir.x;
                t.x += deltaT.x;
                stepAxis = 0;
            }
            else
            {
                tile.y += stepDir.y;
                t.y += deltaT.y;
                stepAxis = 1;
            }

            // ensure within bounds of tilemap
            if (
                tile.x < 0 || tile.x >= tilemap->GetDimensions().x ||
                tile.y < 0 || tile.y >= tilemap->GetDimensions().y
            )
            {
                continue;
            }

            // check tile at current position
            if ( tilemap->GetTile( tile ) != 0 )
            {
                rayCastHit->distance = t[ stepAxis ] - deltaT[ stepAxis ];
                rayCastHit->colliderHit = tilemapCollider;

                rayCastHit->normal = stepDir;
                rayCastHit->normal[ (int)!stepAxis ] = 0;

                return;
            }
        }
    }


//-----------------------------------------------------------------------------
// private: static members
//-----------------------------------------------------------------------------

    /// @brief map that stores the CollisionCheckMethods between each Collider type
    CollisionFunctionMap const CollisionSystem::s_CollisionFunctions = {
        { { typeid( CircleCollider ), typeid( CircleCollider )  }, &checkCircleCircle  },
        { { typeid( CircleCollider ), typeid( TilemapCollider ) }, &checkCircleTilemap }
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

    /// @brief  reads the number of collision steps each frame
    /// @param  data    the json data to read from
    void CollisionSystem::readCollisionSteps( nlohmann::ordered_json const& data )
    {
        m_CollisionSteps = data;
    }

    /// @brief map of the CollisionSystem read methods
    ReadMethodMap< CollisionSystem > const CollisionSystem::s_ReadMethods = {
        { "CollisionLayerNames", &readCollisionLayerNames },
        { "CollisionSteps"     , &readCollisionSteps      }
    };

    /// @brief  writes the CollisionSystem config to json
    /// @return the written json data
    nlohmann::ordered_json CollisionSystem::Write() const
    {
        nlohmann::ordered_json json;

        json[ "CollisionLayerNames" ] = m_CollisionLayerNames;
        json[ "CollisionSteps" ] = m_CollisionSteps;

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
