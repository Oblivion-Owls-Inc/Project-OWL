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

#include "Engine.h"

#include "Inspection.h"

//-----------------------------------------------------------------------------
// public: methods
//-----------------------------------------------------------------------------


    /// @brief  adds a CircleCollider to the CollisionSystem
    /// @param  circleCollider  the collider to add
    void CollisionSystem::AddCollider( CircleCollider* circleCollider )
    {
        if ( 2.0f * circleCollider->GetRadius() > m_GridSize )
        {
            m_LargeCircleColliders.push_back( circleCollider );
            return;
        }

        Transform* transform = circleCollider->GetTransform();
        if ( transform == nullptr )
        {
            Debug() << "ERROR: cannot add a Collider without a Transform to CollisionSystem" << std::endl;
            return;
        }

        glm::vec2 pos = circleCollider->GetTransform()->GetTranslation();
        m_CircleCollidersGrid[ getGridCell( pos ) ].push_back( circleCollider );
    }

    /// @brief  removes a CircleCollider from the CollisionSystem
    /// @param  circleCollider  the collider to remove
    void CollisionSystem::RemoveCollider( CircleCollider* circleCollider )
    {
        std::vector< CircleCollider* >* container = &m_LargeCircleColliders;

        if ( 2.0f * circleCollider->GetRadius() <= m_GridSize )
        {
            glm::vec2 pos = circleCollider->GetTransform()->GetTranslation();

            auto it = m_CircleCollidersGrid.find( getGridCell( pos ) );
            if ( it == m_CircleCollidersGrid.end() )
            {
                Debug() << "ERROR: could not find cell to remove CircleCollider" << std::endl;
                return;
            }

            if ( circleCollider->GetHasChanged() )
            {
                updatePositionsInGrid();
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
    void CollisionSystem::AddCollider( TilemapCollider* tilemapCollider )
    {
        m_TilemapColliders.push_back( tilemapCollider );
    }

    /// @brief  removes a TilemapCollider from the CollisionSystem
    /// @param  tilemapCollider     the collider to remove
    void CollisionSystem::RemoveCollider( TilemapCollider* tilemapCollider )
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
            checkRayCircle( origin, direction, circleCollider, &hit, layers );
        }

        for ( TilemapCollider* tilemapCollider : m_TilemapColliders )
        {
            checkRayTilemap( origin, direction, tilemapCollider, &hit, layers );
        }

        glm::vec2 gridOrigin = origin * m_GridSize;
        checkRayUnitGrid(
            gridOrigin, direction,
            [ & ]( glm::ivec2 cellPos, float distance, glm::ivec2 stepDir, int stepAxis ) -> bool
            {
                if ( distance * m_GridSize >= hit.distance + m_GridSize )
                {
                    return true;
                }

                auto it = m_CircleCollidersGrid.find( cellPos );
                if ( it == m_CircleCollidersGrid.end() )
                {
                    return false;
                }

                for ( CircleCollider* collider : it->second )
                {
                    checkRayCircle( origin, direction, collider, &hit, layers );
                }

                return false;
            }
        );

        return hit;
    }

//-----------------------------------------------------------------------------
// public: virtual override methods
//-----------------------------------------------------------------------------

    /// @brief  Gets called once every simulation frame. Use this function for anything that affects the simulation.
    void CollisionSystem::OnFixedUpdate()
    {
        for ( int i = 0; i < m_CollisionSteps; ++i )
        {
            updatePositionsInGrid();
            checkCollisions();
        }

        removeOutdatedContacts();
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

        ImGui::NewLine();

        ImGui::DragFloat( "grid size", &m_GridSize, 0.05f, 0.01f, INFINITY );
        ImGui::Text( "If the grid size is changed, the scene must be reloaded" );

        ImGui::DragInt( "collision steps", &m_CollisionSteps, 0.05f, 1, INT_MAX );

        ImGui::End();
    }


//-----------------------------------------------------------------------------
// private: methods
//-----------------------------------------------------------------------------


    /// @brief Checks and handles all Collisions
    void CollisionSystem::checkCollisions()
    {
        // check collisions between large circle colliders and each other
        checkCollisions( &m_LargeCircleColliders );

        // check collisions between large circle colliders and tilemaps
        checkCollisions( &m_LargeCircleColliders, &m_TilemapColliders );


        // check large circle colliders against grid
        for ( CircleCollider* collider : m_LargeCircleColliders )
        {
            glm::vec2 pos = collider->GetTransform()->GetTranslation();

            glm::ivec2 minCell = getGridCell( pos - glm::vec2( collider->GetRadius() + m_GridSize ) );
            glm::ivec2 maxCell = getGridCell( pos + glm::vec2( collider->GetRadius() + m_GridSize ) );

            // for each row the collider overlaps
            for ( glm::ivec2 cell = minCell; cell.y <= maxCell.y; ++cell.y )
            {
                // find start of the row
                auto it = m_CircleCollidersGrid.lower_bound( cell );
                if ( it == m_CircleCollidersGrid.end() || it->first.y != cell.y )
                {
                    continue;
                }

                // loop through to end of row
                while ( it != m_CircleCollidersGrid.end() && it->first.x <= maxCell.x )
                {
                    // check against each collider in that cell
                    for ( CircleCollider* colliderB : it->second )
                    {
                        checkCollision( collider, colliderB );
                    }

                    ++it;
                }
            }
        }


        // loop through each cell with colliders in it
        for ( auto it = m_CircleCollidersGrid.begin(); it != m_CircleCollidersGrid.end(); ++it )
        {
            glm::ivec2 const& cellPos = it->first;
            std::vector< CircleCollider* >& colliders = it->second;

            // check collisions between multiple colliders in this cell
            checkCollisions( &colliders );

            // check small circle colliders against large circle colliders
            // checkCollisions( &colliders, &m_LargeCircleColliders );

            // check small circle colliders against tilemap colliders
            checkCollisions( &colliders, &m_TilemapColliders );

            // check collision against the next cell to the right
            auto next = it;
            ++next;
            if ( next != m_CircleCollidersGrid.end() && next->first == cellPos + glm::ivec2( 1, 0 ) )
            {
                checkCollisions( &colliders, &next->second );
            }

            // check collision against the cells above
            next = m_CircleCollidersGrid.lower_bound( cellPos + glm::ivec2( -1, 1 ) );
            while (
                next != m_CircleCollidersGrid.end() &&
                next->first.y == cellPos.y + 1 &&
                next->first.x <= cellPos.x + 1
            )
            {
                checkCollisions( &colliders, &next->second );
                ++next;
            }
        }
    }


    /// @brief  removes outdated contacts from all colliders
    void CollisionSystem::removeOutdatedContacts()
    {
        for ( CircleCollider* collider : m_LargeCircleColliders )
        {
            collider->RemoveOutdatedContacts();
        }

        for ( TilemapCollider* collider : m_TilemapColliders )
        {
            collider->RemoveOutdatedContacts();
        }

        for ( auto& [ cellPos, colliders ] : m_CircleCollidersGrid )
        {
            for ( CircleCollider* collider : colliders )
            {
                collider->RemoveOutdatedContacts();
            }
        }
    }


    /// @brief  updates the position of each Collider in the grid, if necessary
    void CollisionSystem::updatePositionsInGrid()
    {
        std::map< glm::ivec2, std::vector< CircleCollider* > > newCells = {};

        for ( auto& [ cellPos, colliders ] : m_CircleCollidersGrid )
        {
            // conditionally remove colliders from each cell
            std::erase_if(
                colliders,
                [ & ]( CircleCollider* collider ) -> bool
                {
                    // don't remove colliders that haven't changed
                    if ( collider->GetHasChanged() == false )
                    {
                        return false;
                    }

                    collider->ClearHasChanged();

                    // move colliders that've grown larger than the grid size to the large colliders array
                    if ( 2.0f * collider->GetRadius() > m_GridSize )
                    {
                        m_LargeCircleColliders.push_back( collider );
                        return true;
                    }

                    // move colliders that've moved into a different cell to that cell
                    glm::ivec2 newCell = getGridCell( collider->GetTransform()->GetTranslation() );
                    if ( newCell != cellPos )
                    {
                        auto it = m_CircleCollidersGrid.find( newCell );
                        if ( it == m_CircleCollidersGrid.end() )
                        {
                            // if the target cell doesn't exist yet, queue it to be added after we're done iterating through the cells
                            newCells[ newCell ].push_back( collider );
                        }
                        else
                        {
                            // if the target cell already exists, move directly there
                            it->second.push_back( collider );
                        }

                        return true;
                    }

                    return false;
                }
            );
        }

        // remove any empty cells
        std::erase_if(
            m_CircleCollidersGrid,
            []( std::pair< glm::ivec2, std::vector< CircleCollider* > > const& cell ) -> bool
            {
                return cell.second.empty();
            }
        );

        // move new cells into the grid
        m_CircleCollidersGrid.insert( newCells.begin(), newCells.end() );

        
        // move colliders that are now small enough to be in the grid into the grid
        std::erase_if(
            m_LargeCircleColliders,
            [ this ]( CircleCollider* collider ) -> bool
            {
                collider->ClearHasChanged();

                if ( 2.0f * collider->GetRadius() > m_GridSize )
                {
                    return false;
                }

                glm::vec2 pos = collider->GetTransform()->GetTranslation();
                m_CircleCollidersGrid[ getGridCell( pos ) ].push_back( collider );

                return true;
            }
        );
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

    

    /// @brief  checks collisions between Colliders of the same type in a container
    /// @tparam ColliderType    the Type of collider to check collisions between
    /// @param  colliders       the container of the Colliders to check between
    template < class ColliderType >
    void CollisionSystem::checkCollisions( std::vector< ColliderType* >* colliders )
    {
        for ( int i = 0; i < colliders->size(); ++i )
        {
            ColliderType* colliderA = (*colliders)[ i ];

            for ( int j = i + 1; j < colliders->size(); ++j )
            {
                ColliderType* colliderB = (*colliders)[ j ];

                checkCollision( colliderA, colliderB );
            }
        }
    }


    /// @brief  checks and updates collision between Colliders in two containters
    /// @tparam ColliderAType   the type of the first Collider
    /// @tparam ColliderBType   the type of the second Collider
    /// @param  collidersA      the first container of Colliders
    /// @param  collidersB      the second container of Colliders
    template < class ColliderAType, class ColliderBType >
    void CollisionSystem::checkCollisions( std::vector< ColliderAType* >* collidersA, std::vector< ColliderBType* >* collidersB )
    {
        for ( ColliderAType* colliderA : *collidersA )
        {
            for ( ColliderBType* colliderB : *collidersB )
            {
                checkCollision( colliderA, colliderB );
            }
        }
    }


    
    /// @brief  checks and updates collision between two collider of any known type
    /// @tparam ColliderAType   the type of the first Collider
    /// @tparam ColliderBType   the type of the second Collider
    /// @param  colliderA       the first Collider
    /// @param  colliderB       the second Collider
    template < class ColliderAType, class ColliderBType >
    void CollisionSystem::checkCollision( ColliderAType* colliderA, ColliderBType* colliderB )
    {
        bool aCollidesB = colliderA->GetCollisionLayerFlags().Includes( colliderB->GetCollisionLayer() );
        bool bCollidesA = colliderB->GetCollisionLayerFlags().Includes( colliderA->GetCollisionLayer() );

        // if neither Collider can collide with the other based on flags, return
        if ( !(aCollidesB || bCollidesA) )
        {
            return;
        }

        // check the collision
        CollisionData collisionData;
        bool touching = checkCollision( colliderA, colliderB, &collisionData );

        if ( touching == false )
        {
            return;
        }

        // handle callbacks
        if ( aCollidesB )
        {
            colliderA->CallOnCollisionCallbacks( colliderB, collisionData );
            colliderA->TryAddContact( colliderB, GameEngine()->GetFixedFrameCount() );
        }

        if ( bCollidesA )
        {
            colliderB->CallOnCollisionCallbacks( colliderA, -collisionData );
            colliderB->TryAddContact( colliderA, GameEngine()->GetFixedFrameCount() );
        }
    }


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
    bool CollisionSystem::checkCollision( CircleCollider const* colliderA, CircleCollider const* colliderB, CollisionData* collisionData )
    {
        glm::vec2 posA = colliderA->GetTransform()->GetTranslation();
        glm::vec2 posB = colliderB->GetTransform()->GetTranslation();

        glm::vec2 displacement = posB - posA;
        float squareDistance = glm::dot( displacement, displacement );

        float minDistance = colliderA->GetRadius() + colliderB->GetRadius();
        
        if ( squareDistance >= minDistance * minDistance )
        {
            return false;
        }

        if ( collisionData != nullptr )
        {
            float distance = std::sqrt( squareDistance );
            collisionData->normal = distance == 0.0f ? glm::vec2( 0 ) : - displacement / distance;
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
    bool CollisionSystem::checkCollision( CircleCollider const* circleCollider, TilemapCollider const* tilemapCollider, CollisionData* collisionData )
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
                if ( tilemap->IsPositionWithinBounds( tilePos ) == false )
                {
                    continue;
                }

                if ( tilemap->GetTile( tilePos ) < 0 )
                {
                    continue;
                }

                // get which edges of the AABB are enabled
                int enabledEdges = 0;
                glm::ivec2 offsets[] = {
                    glm::ivec2( -1,  0 ),
                    glm::ivec2( +1,  0 ),
                    glm::ivec2(  0, -1 ),
                    glm::ivec2(  0, +1 )
                };
                for ( int i = 0; i < 4; ++i )
                {
                    glm::ivec2 offsetTile = tilePos + offsets[ i ];
                    if ( tilemap->IsPositionWithinBounds( offsetTile ) && tilemap->GetTile( offsetTile ) < 0 )
                    {
                        enabledEdges |= 1 << i;
                    }
                }

                CollisionData tempCollisionData;
                if ( !checkCircleAABB( pos, radius, tilePos, tilePos + glm::ivec2( 1, 1 ), collisionData ? &tempCollisionData : nullptr, enabledEdges ) )
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
    /// @param  enabledEdges    flags of which edges of the AABB are enabled
    /// @return whether or not the two shapes are colliding
    /// @note   ASSUMES THAT THE AABB OF THE CIRCLE IS KNOWN TO OVERLAP THE RECTANGLE
    bool CollisionSystem::checkCircleAABB(
        glm::vec2 const& circlePos, float circleRadius,
        glm::vec2 const& aabbMin, glm::vec2 const& aabbMax,
        CollisionData* collisionData,
        int enabledEdges
    )
    {
        if ( enabledEdges == 0 )
        {
            return false;
        }

        if ( circlePos.x >= aabbMax.x && (enabledEdges & s_EdgeRight) )
        {
            if ( circlePos.y >= aabbMax.y && (enabledEdges & s_EdgeUp) )
            { // top right corner
                return checkCirclePoint( circlePos, circleRadius, aabbMax, collisionData );
            }
            else if ( circlePos.y <= aabbMin.y && (enabledEdges & s_EdgeDown) )
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
        else if ( circlePos.x <= aabbMin.x && (enabledEdges & s_EdgeLeft) )
        {
            if ( circlePos.y >= aabbMax.y && (enabledEdges & s_EdgeUp) )
            { // top left corner
                return checkCirclePoint( circlePos, circleRadius, glm::vec2( aabbMin.x, aabbMax.y ), collisionData);
            }
            else if ( circlePos.y <= aabbMin.y && (enabledEdges & s_EdgeDown) )
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
            if ( circlePos.y >= aabbMax.y && (enabledEdges & s_EdgeUp) )
            { // top edge
                if ( collisionData != nullptr )
                {
                    collisionData->depth = aabbMax.y - (circlePos.y - circleRadius);
                    collisionData->position = circlePos - glm::vec2( 0, circleRadius );
                    collisionData->normal = glm::vec2( 0, 1 );
                }

                return true;
            }
            else if ( circlePos.y <= aabbMin.y && (enabledEdges & s_EdgeDown) )
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
                if ( collisionData == nullptr )
                {
                    return true;
                }

                collisionData->depth = -INFINITY;
                if ( enabledEdges & s_EdgeLeft )
                {
                    collisionData->depth = (circlePos.x + circleRadius) - aabbMin.x;
                    collisionData->position = circlePos + glm::vec2( circleRadius, 0 );
                    collisionData->normal = glm::vec2( -1, 0 );
                }
                if ( enabledEdges & s_EdgeRight )
                {
                    float depth = aabbMax.x - (circlePos.x - circleRadius);
                    if ( depth > collisionData->depth )
                    {
                        collisionData->depth = depth;
                        collisionData->position = circlePos - glm::vec2( circleRadius, 0 );
                        collisionData->normal = glm::vec2( 1, 0 );
                    }
                }
                if ( enabledEdges & s_EdgeDown )
                {
                    float depth = (circlePos.y + circleRadius) - aabbMin.y;
                    if ( depth > collisionData->depth )
                    {
                        collisionData->depth = depth;
                        collisionData->position = circlePos + glm::vec2( 0, circleRadius );
                        collisionData->normal = glm::vec2( 0, -1 );
                    }
                }
                if ( enabledEdges & s_EdgeUp )
                {
                    float depth = aabbMax.y - (circlePos.y - circleRadius);
                    if ( depth > collisionData->depth )
                    {
                        collisionData->depth = depth;
                        collisionData->position = circlePos - glm::vec2( 0, circleRadius );
                        collisionData->normal = glm::vec2( 0, 1 );
                    }
                }

                return true;
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
    /// @param  layers          the Collision layers the raycast interacts with
    void CollisionSystem::checkRayCircle( glm::vec2 const& rayOrigin, glm::vec2 const& rayDirection, CircleCollider* circle, RayCastHit* rayCastHit, CollisionLayerFlags layers )
    {
        if ( layers.Includes( circle->GetCollisionLayer() ) == false )
        {
            return;
        }

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
    /// @param  layers          the Collision layers the raycast interacts with
    void CollisionSystem::checkRayTilemap( glm::vec2 const& rayOrigin, glm::vec2 const& rayDirection, TilemapCollider* tilemapCollider, RayCastHit* rayCastHit, CollisionLayerFlags layers )
    {
        if ( layers.Includes( tilemapCollider->GetCollisionLayer() ) == false )
        {
            return;
        }

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

                distance *= tilemap->GetTileScale().x;

                if ( distance >= rayCastHit->distance )
                {
                    return true;
                }

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

                return false;
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
