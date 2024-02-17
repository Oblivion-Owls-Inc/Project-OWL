/// @file CollisionSystem.h
/// @author Jax Clayton (jax.clayton@digipen.edu), Steve Bukowinski (steve.bukowinski@digipen.edu)
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
#include <cmath>

#include "DebugSystem.h"
#include "RenderSystem.h"

#include "Inspection.h"

//-----------------------------------------------------------------------------
// public: methods
//-----------------------------------------------------------------------------


    /// @brief  adds a CircleCollider to the CollisionSystem
    /// @param  circleCollider  the collider to add
    void CollisionSystem::addCollider( CircleCollider* circleCollider )
    {
        if ( circleCollider->GetRadius() > m_GridSize )
        {
            m_LargeCircleColliders.push_back( circleCollider );
            return;
        }

        glm::vec2 pos = circleCollider->GetTransform()->GetTranslation();
        m_CircleCollidersGrid[ getGridCell( pos ) ].push_back( circleCollider );
    }

    /// @brief  removes a CircleCollider from the CollisionSystem
    /// @param  circleCollider  the collider to remove
    void CollisionSystem::removeCollider( CircleCollider* circleCollider )
    {
        std::vector< CircleCollider* >* container = &m_LargeCircleColliders;

        if ( circleCollider->GetRadius() <= m_GridSize )
        {
            glm::vec2 pos = circleCollider->GetTransform()->GetTranslation();

            auto it = m_CircleCollidersGrid.find( getGridCell( pos ) );
            if ( it == m_CircleCollidersGrid.end() )
            {
                Debug() << "ERROR: could not find cell to remove CircleCollider" << std::endl;
                return;
            }

            container = &it->second;
        }

        auto it = std::find( container->begin(), container->end(), circleCollider );
        if ( it == container->end() )
        {
            Debug() << "ERROR: could not find CircleCollider to remove" << std::endl;
            return;
        }

        container->erase( it );
    }


    /// @brief  adds a TilemapCollider to the CollisionSystem
    /// @param  tilemapCollider the collider to add
    void CollisionSystem::addCollider( TilemapCollider* tilemapCollider )
    {
        m_TilemapColliders.push_back( tilemapCollider );
    }

    /// @brief  removes a TilemapCollider from the CollisionSystem
    /// @param  tilemapCollider     the collider to remove
    void CollisionSystem::removeCollider( TilemapCollider* tilemapCollider )
    {
        auto it = std::find( m_TilemapColliders.begin(), m_TilemapColliders.end(), tilemapCollider );
        if ( it == m_TilemapColliders.end() )
        {
            Debug() << "ERRORL could not find TilemapCollider to remove" << std::endl;
            return;
        }

        m_TilemapColliders.erase( it );
    }

    /// @brief  gets the names of the layers in a CollisionLayerFlags
    /// @param  layerFlags  the layer flags to get the names of
    /// @return the names of the layers
    std::vector< std::string > CollisionSystem::GetLayerNames( CollisionLayerFlags layerFlags ) const
    {
        std::vector< std::string > layerNames;
        for ( unsigned i = 0; layerFlags != 0; ++i )
        {
            if ( layerFlags.Includes( i ) )
            {
                layerNames.push_back( GetLayerName( i ) );
                layerFlags = layerFlags ^ (1 << i);
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

        for ( CircleCollider* circleCollider : m_LargeCircleColliders )
        {
            checkRayCircle( origin, direction, circleCollider, &hit );
        }

        for ( TilemapCollider* tilemapCollider : m_TilemapColliders )
        {
            checkRayTilemap( origin, direction, tilemapCollider, &hit );
        }

        // TODO: check ray against grid circles

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

    /// @brief  creates the debug window for the CollisionSystem
    void CollisionSystem::DebugWindow()
    {
        static bool _open = true;
        if ( ImGui::Begin( "Collision System", &_open ) == false )
        {
            return;
        }

        Inspection::InspectArray< std::string >(
            "Collision Layer Names",
            &m_CollisionLayerNames,
            []( std::string* name ) -> bool
            {
                return ImGui::InputText( "", name );
            }
        );

        ImGui::End();
    }

//-----------------------------------------------------------------------------
// private: methods
//-----------------------------------------------------------------------------

    /// @brief Checks and handles all Collisions
    void CollisionSystem::checkCollisions()
    {
        for ( int i = 0; i < m_LargeCircleColliders.size(); ++i )
        {
            CircleCollider* circleCollider = m_LargeCircleColliders[ i ];

            for ( int j = 0; j < m_TilemapColliders.size(); ++i )
            {
                TilemapCollider* tilemapCollider = m_TilemapColliders[ i ];

                bool aCollidesB = circleCollider->GetCollisionLayerFlags().Includes( tilemapCollider->GetCollisionLayer() );


                CollisionData collisionData;

                if ( checkCircleTilemap( circleCollider, tilemapCollider, &collisionData ) )
                {
                    circleCollider->CallOnCollisionCallbacks( tilemapCollider, collisionData );
                    tilemapCollider->CallOnCollisionCallbacks( circleCollider, -collisionData );

                    if ( circleCollider->TryAddContact( tilemapCollider ) )
                    {

                    }

                }
                else
                {
                    
                }
            }
        }
    }


    /// @brief  gets the collision grid cell of a given world pos
    /// @param  worldPos    the world position to get the grid cell of
    /// @return the grid cell position containing the world pos
    glm::ivec2 CollisionSystem::getGridCell( glm::vec2 const& worldPos ) const
    {
        return glm::ivec2( worldPos / m_GridSize );
    }


//-----------------------------------------------------------------------------
// private: static methods
//-----------------------------------------------------------------------------

    // /// @brief  checks a collision between two colliders of unknown type
    // /// @param  colliderA       the first collider
    // /// @param  colliderB       the second collider
    // /// @param  collisionData   pointer to where to store additional data about the collision
    // /// @return whether or not the two colliders are colliding
    // void CollisionSystem::checkCollision( Collider* colliderA, Collider* colliderB )
    // {
    //     // check if the layers interact
    //     bool aCollidesB = colliderA->GetCollisionLayerFlags() & ( 1 << colliderB->GetCollisionLayer() );
    //     bool bCollidesA = colliderB->GetCollisionLayerFlags() & ( 1 << colliderA->GetCollisionLayer() );
    // 
    //     // if collision layers don't interact, don't test collision
    //     if ( (aCollidesB || bCollidesA) == false )
    //     {
    //         return;
    //     }
    // 
    //     // ensure that both colliders have Transforms
    //     if ( colliderA->GetTransform() == nullptr || colliderB->GetTransform() == nullptr )
    //     {
    //         Debug() << "WARNING: Collider component must always be accompanied by a Transform component" << std::endl;
    //         return;
    //     }
    // 
    //     // check type of each collider
    //     std::pair< std::type_index, std::type_index > colliderTypes = {
    //         colliderA->GetType(),
    //         colliderB->GetType()
    //     };
    // 
    //     // find the collision function between those two shapes
    //     auto checkFuncIt = s_CollisionFunctions.find( colliderTypes );
    //     if ( checkFuncIt == s_CollisionFunctions.end() )
    //     {
    //         // if no collision function found, swap the order of the colliders and search again
    //         colliderTypes = {
    //             colliderTypes.second,
    //             colliderTypes.first
    //         };
    // 
    //         Collider* tempPtr = colliderA;
    //         colliderA = colliderB;
    //         colliderB = tempPtr;
    // 
    //         bool tempBool = aCollidesB;
    //         aCollidesB = bCollidesA;
    //         bCollidesA = tempBool;
    // 
    //         checkFuncIt = s_CollisionFunctions.find( colliderTypes );
    //         if ( checkFuncIt == s_CollisionFunctions.end() )
    //         {
    //             Debug() <<
    //                 "WARNING: no collision function implemented between " <<
    //                 colliderTypes.first.name() <<
    //                 " and " << colliderTypes.second.name();
    // 
    //             return;
    //         }
    //     }
    // 
    //     // check the collision
    //     CollisionData collisionData;
    //     if ( (*checkFuncIt->second)( colliderA, colliderB, &collisionData ) == false )
    //     {
    //         // no collision happened
    //         if ( aCollidesB && colliderA->TryRemoveContact( colliderB ) )
    //         {
    //             colliderA->CallOnCollisionExitCallbacks( colliderB );
    //         }
    // 
    //         if ( bCollidesA && colliderB->TryRemoveContact( colliderA ) )
    //         {
    //             colliderB->CallOnCollisionExitCallbacks( colliderA );
    //         }
    // 
    //         return;
    //     }
    // 
    //     // a collision happened
    // 
    //     collisionData.depth += 0.001f;
    // 
    //     // call callbacks 
    //     if ( aCollidesB )
    //     {
    //         colliderA->CallOnCollisionCallbacks( colliderB, collisionData );
    // 
    //         if ( colliderA->TryAddContact( colliderB ) )
    //         {
    //             colliderA->CallOnCollisionEnterCallbacks( colliderB );
    //         }
    //     }
    // 
    //     if ( bCollidesA )
    //     {
    //         collisionData.normal *= -1;
    //         colliderB->CallOnCollisionCallbacks( colliderA, collisionData );
    // 
    //         if ( colliderB->TryAddContact( colliderA ) )
    //         {
    //             colliderB->CallOnCollisionEnterCallbacks( colliderA );
    //         }
    //     }
    // }

    /// @brief  checks a collision between two circle colliders
    /// @param  colliderA       the first collider
    /// @param  colliderB       the second collider
    /// @param  collisionData   pointer to where to store additional data about the collision
    /// @return whether or not the two colliders are colliding
    bool CollisionSystem::checkCircleCircle( CircleCollider const* colliderA, CircleCollider const* colliderB, CollisionData* collisionData )
    {
        if (collisionData)
            *collisionData = {};

        glm::vec2 posA = colliderA->GetTransform()->GetTranslation();
        glm::vec2 posB = colliderB->GetTransform()->GetTranslation();

        glm::vec2 displacement = posB - posA;
        float distance = glm::length( displacement );

        float minDistance = colliderA->GetRadius() + colliderB->GetRadius();
        
        if ( distance >= minDistance )
        {
            return false;
        }

        if ( collisionData != nullptr )
        {
            collisionData->normal = distance == 0 ? glm::vec2( 0 ) : - displacement / distance;
            collisionData->position = (
                posA - collisionData->normal * colliderA->GetRadius() +
                posB + collisionData->normal * colliderB->GetRadius()
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
    bool CollisionSystem::checkCircleTilemap( CircleCollider const* circleCollider, TilemapCollider const* tilemapCollider, CollisionData* collisionData )
    {
        if (collisionData)
            *collisionData = {};

        Tilemap< int > const* tilemap = (tilemapCollider)->GetTilemap();

        glm::mat4 const& worldToTile = tilemap->GetWorldToTilemapMatrix();
        glm::mat4 const& tileToWorld = tilemap->GetTilemapToWorldMatrix();
        glm::vec2 const& tileSize = tilemap->GetTileScale();

        glm::vec2 pos = circleCollider->GetTransform()->GetTranslation();
        float radius = circleCollider->GetRadius() / tileSize.x;

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

                if ( tilemap->GetTile( tilePos ) < 0 )
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
            rayCastHit->position = rayOrigin + rayDirection * distance;
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


        checkRayUnitGrid(
            tilePos, tileVel,
            [ rayOrigin, rayDirection, tilemapCollider, rayCastHit ]( glm::ivec2 cellPos, float distance, glm::ivec2 stepDir, int stepAxis ) -> bool
            {
                Tilemap<int> const* tilemap = tilemapCollider->GetTilemap();

                // ensure within bounds of tilemap
                if (
                    cellPos.x < 0 || cellPos.x >= tilemap->GetDimensions().x ||
                    cellPos.y < 0 || cellPos.y >= tilemap->GetDimensions().y
                )
                {
                    return (
                        ( cellPos.x < 0 && stepDir.x <= 0 ) ||
                        ( cellPos.y < 0 && stepDir.y <= 0 ) ||
                        ( cellPos.x >= tilemap->GetDimensions().x && stepDir.x >= 0 ) ||
                        ( cellPos.y >= tilemap->GetDimensions().y && stepDir.y >= 0 )
                    );
                }
                
                // check tile at current position
                if ( tilemap->GetTile( cellPos ) >= 0 )
                {
                    rayCastHit->distance = distance;
                    rayCastHit->colliderHit = tilemapCollider;
                
                    rayCastHit->normal = stepDir;
                    rayCastHit->normal[ (int)!stepAxis ] = 0;
                
                    rayCastHit->position = rayOrigin + rayDirection * rayCastHit->distance;
                    rayCastHit->tilePos = cellPos;
                
                    return true;
                }
            }
        );

        // glm::ivec2 tile = tilePos;
        // // direction of the step in each axis
        // glm::ivec2 stepDir = {
        //     tileVel.x > 0 ? 1 : ( tileVel.x < 0 ? -1 : 0 ),
        //     tileVel.y > 0 ? 1 : ( tileVel.y < 0 ? -1 : 0 )
        // };
        // 
        // // length of the step in each axis
        // glm::vec2 deltaT = {
        //     tileVel.x == 0 ? INFINITY : 1 / std::abs( tileVel.x ),
        //     tileVel.y == 0 ? INFINITY : 1 / std::abs( tileVel.y )
        // };
        // 
        // // current position in each axis
        // glm::vec2 t = tilePos - (glm::vec2)tile;
        // if ( stepDir.x == 1 ) { t.x = 1 - t.x; }
        // if ( stepDir.y == 1 ) { t.y = 1 - t.y; }
        // 
        // // can't just multiply because sometimes deltaT is infinity
        // t.x = t.x == 0 ? 0 : t.x * deltaT.x;
        // t.y = t.y == 0 ? 0 : t.y * deltaT.y;
        // 
        // // loop until max distance reached
        // while ( t.x < rayCastHit->distance || t.y < rayCastHit->distance )
        // {
        //     // step in the closest direction
        //     int stepAxis;
        //     if ( t.x < t.y )
        //     {
        //         tile.x += stepDir.x;
        //         t.x += deltaT.x;
        //         stepAxis = 0;
        //     }
        //     else
        //     {
        //         tile.y += stepDir.y;
        //         t.y += deltaT.y;
        //         stepAxis = 1;
        //     }
        // 
        //     // ensure within bounds of tilemap
        //     if (
        //         tile.x < 0 || tile.x >= tilemap->GetDimensions().x ||
        //         tile.y < 0 || tile.y >= tilemap->GetDimensions().y
        //     )
        //     {
        //         if (
        //             ( tile.x < 0 && stepDir.x <= 0 ) ||
        //             ( tile.y < 0 && stepDir.y <= 0 ) ||
        //             ( tile.x >= tilemap->GetDimensions().x && stepDir.x >= 0 ) ||
        //             ( tile.y >= tilemap->GetDimensions().y && stepDir.y >= 0 )
        //         )
        //         {
        //             // don't infinite infinite loop upon exiting tilemap bounds
        //             break;
        //         }
        // 
        //         continue;
        //     }
        // 
        //     // check tile at current position
        //     if ( tilemap->GetTile( tile ) >= 0 )
        //     {
        //         rayCastHit->distance = t[ stepAxis ] - deltaT[ stepAxis ];
        //         rayCastHit->colliderHit = tilemapCollider;
        // 
        //         rayCastHit->normal = stepDir;
        //         rayCastHit->normal[ (int)!stepAxis ] = 0;
        // 
        //         rayCastHit->position = rayOrigin + rayDirection * rayCastHit->distance;
        //         rayCastHit->tilePos = tile;
        // 
        //         return;
        //     }
        // }
    }


    /// @brief  checks if a ray hits a unit grid
    /// @param  rayOrigin           the origin of the ray in grid space
    /// @param  rayDirection        the direction of the ray in grid space
    /// @param  gridCellCallback    callback that returns true when the ray should stop. argument is the current cell of the grid the ray is in.
    void CollisionSystem::checkRayUnitGrid(
        glm::vec2 const& rayOrigin,
        glm::vec2 const& rayDirection,
        std::function< bool ( glm::ivec2 cellPos, float distance, glm::ivec2 stepDir, int stepAxis ) > gridCellCallback
    )
    {

        glm::ivec2 tile = rayOrigin;

        // direction of the step in each axis
        glm::ivec2 stepDir = {
            rayDirection.x > 0 ? 1 : ( rayDirection.x < 0 ? -1 : 0 ),
            rayDirection.y > 0 ? 1 : ( rayDirection.y < 0 ? -1 : 0 )
        };

        // length of the step in each axis
        glm::vec2 deltaT = {
            rayDirection.x == 0 ? INFINITY : 1 / std::abs( rayDirection.x ),
            rayDirection.y == 0 ? INFINITY : 1 / std::abs( rayDirection.y )
        };

        // current position in each axis
        glm::vec2 t = rayOrigin - (glm::vec2)tile;
        if ( stepDir.x == 1 ) { t.x = 1 - t.x; }
        if ( stepDir.y == 1 ) { t.y = 1 - t.y; }

        // can't just multiply because sometimes deltaT is infinity
        t.x = t.x == 0 ? 0 : t.x * deltaT.x;
        t.y = t.y == 0 ? 0 : t.y * deltaT.y;

        // loop until max distance reached
        while ( true )
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

            // check tile at current position
            if ( gridCellCallback( tile, t[ stepAxis ] - deltaT[ stepAxis ], stepDir, stepAxis ) )
            {
                return;
            }
        }
    }


//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------


    /// @brief  reads the collision layer names from json
    /// @param  data    the json data to read from
    void CollisionSystem::readCollisionLayerNames( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_CollisionLayerNames, data );
    }

    /// @brief  reads the number of collision steps each frame
    /// @param  data    the json data to read from
    void CollisionSystem::readCollisionSteps( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_CollisionSteps, data );
    }

    /// @brief  reads the size of each cell of the collision grid
    /// @param  data    the json data to read from
    void CollisionSystem::readGridSize( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_GridSize, data );
    }
    
//-----------------------------------------------------------------------------
// public: reading / writing
//-----------------------------------------------------------------------------


    /// @brief  gets this System's read methods
    /// @return this System's read methods
    ReadMethodMap< ISerializable > const& CollisionSystem::GetReadMethods() const
    {
        static ReadMethodMap< CollisionSystem > const readMethods = {
            { "CollisionLayerNames", &CollisionSystem::readCollisionLayerNames },
            { "CollisionSteps"     , &CollisionSystem::readCollisionSteps      },
            { "GridSize"           , &CollisionSystem::readGridSize            }
        };

        return (ReadMethodMap< ISerializable > const&)readMethods;
    }


    /// @brief  writes the CollisionSystem config to json
    /// @return the written json data
    nlohmann::ordered_json CollisionSystem::Write() const
    {
        nlohmann::ordered_json json;

        json[ "CollisionLayerNames" ] = Stream::Write( m_CollisionLayerNames );
        json[ "CollisionSteps"      ] = Stream::Write( m_CollisionSteps      );
        json[ "GridSize"            ] = Stream::Write( m_GridSize            );

        return json;
    }


//-----------------------------------------------------------------------------
// public: singleton stuff
//-----------------------------------------------------------------------------


    /// @brief gets the instance of CollisionSystem
    /// @return the instance of the CollisionSystem
    CollisionSystem* CollisionSystem::GetInstance()
    {
        static CollisionSystem* instance = nullptr;
        if ( instance == nullptr )
        {
            instance = new CollisionSystem();
        }
        return instance;
    }

    
//-----------------------------------------------------------------------------
// private: singleton stuff
//-----------------------------------------------------------------------------


    /// @brief Constructs the CollisionSystem
    CollisionSystem::CollisionSystem() :
        System( "CollisionSystem" )
    {}


//-----------------------------------------------------------------------------
