/// @file       TilemapCollider.cpp
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      tilemap collider
/// @version    0.1
/// @date       2023-10-16
/// 
/// @copyright  Copyright (c) 2023 Digipen Institute of Technology

#pragma once


#include "pch.h" // precompiled header has to be included first
#include "TilemapCollider.h"

#include "ComponentReference.t.h"
#include "Transform.h"
#include "RigidBody.h"
#include "StaticBody.h"

#include "CollisionSystem.h"

#include "CameraSystem.h"
#include "RenderSystem.h"

//-----------------------------------------------------------------------------
// public: constructor
//-----------------------------------------------------------------------------


    /// @brief  default constructor
    TilemapCollider::TilemapCollider() :
        Collider( typeid( TilemapCollider ) )
    {}


//-----------------------------------------------------------------------------
// public: virtual overrides
//-----------------------------------------------------------------------------


    /// @brief  called once when entering the scene
    void TilemapCollider::OnInit()
    {
        m_Transform.SetOnConnectCallback(
            [ this ]()
            {
                Collisions()->AddCollider( this );
            }
        );
        m_Transform.SetOnDisconnectCallback(
            [ this ]()
            {
                Collisions()->RemoveCollider( this );
            }
        );

        m_RigidBody .Init( GetEntity() );
        m_StaticBody.Init( GetEntity() );
        m_Tilemap   .Init( GetEntity() );
        m_Transform .Init( GetEntity() );
    }


    /// @brief  called once when exiting the scene
    void TilemapCollider::OnExit()
    {
        m_Transform .Exit();
        m_RigidBody .Exit();
        m_StaticBody.Exit();
        m_Tilemap   .Exit();
    }


    /// @brief  draws the collision shape of this collider for debug purposes
    void TilemapCollider::DebugDraw() const
    {
        if ( m_Tilemap == nullptr || m_Transform == nullptr )
        {
            return;
        }

        auto [ minWorldPos, maxWorldPos ] = Cameras()->GetCameraWorldBounds();

        glm::mat4 const& toTileMat = m_Tilemap->GetWorldToTilemapMatrix();

        glm::ivec2 minTile = toTileMat * glm::vec4( minWorldPos, 0.0f, 1.0f );
        glm::ivec2 maxTile = toTileMat * glm::vec4( maxWorldPos, 0.0f, 1.0f );

        if ( minTile.x > maxTile.x )
        {
            std::swap( minTile.x, maxTile.x );
        }
        if ( minTile.y > maxTile.y )
        {
            std::swap( minTile.y, maxTile.y );
        }

        minTile.x = std::max( minTile.x, 0 );
        minTile.y = std::max( minTile.y, 0 );

        glm::ivec2 const& dimensions = m_Tilemap->GetDimensions();

        maxTile.x = std::min( maxTile.x, dimensions.x - 1 );
        maxTile.y = std::min( maxTile.y, dimensions.y - 1 );

        if (
            minTile.x >= dimensions.x || maxTile.x < 0 ||
            minTile.y >= dimensions.y || maxTile.y < 0
        )
        {
            return;
        }

        glm::ivec2 tilePos;
        for ( tilePos.x = minTile.x; tilePos.x <= maxTile.x; ++tilePos.x )
        {
            for ( tilePos.y = minTile.y; tilePos.y <= maxTile.y; ++tilePos.y )
            {
                if ( m_Tilemap->GetTile( tilePos ) == -1 )
                {
                    continue;
                }

                Renderer()->DrawRect( m_Tilemap->TileCoordToWorldPos( tilePos ), m_Tilemap->GetTileScale() * m_Transform->GetScale() );
            }
        }
    }


//-----------------------------------------------------------------------------
// public: reading / writing
//-----------------------------------------------------------------------------

    
    /// @brief gets the map of read methods for this Component
    /// @return the map of read methods for this Component
    ReadMethodMap< ISerializable > const& TilemapCollider::GetReadMethods() const
    {
        static ReadMethodMap< TilemapCollider > const readMethods = {
            { "CollisionLayer"     , &TilemapCollider::readCollisionLayer      },
            { "CollisionLayerFlags", &TilemapCollider::readCollisionLayerFlags }
        };
        
        return (ReadMethodMap< ISerializable > const&)readMethods;
    }


    /// @brief Write all TilemapCollider component data to a JSON file.
    /// @return The JSON file containing the TilemapCollider component data.
    nlohmann::ordered_json TilemapCollider::Write() const
    {
        nlohmann::ordered_json data;

        data[ "CollisionLayer"      ] = Stream::Write( GetCollisionLayer()      );
        data[ "CollisionLayerFlags" ] = Stream::Write( GetCollisionLayerFlags() );

        return data;
    }


//-----------------------------------------------------------------------------
// public: copying
//-----------------------------------------------------------------------------


    /// @brief  virtual component clone function
    /// @return new clone of component
    TilemapCollider* TilemapCollider::Clone() const
    {
        return new TilemapCollider( *this );
    }


//-----------------------------------------------------------------------------
// private: copying
//-----------------------------------------------------------------------------


    /// @brief  copy-constructor
    /// @param  other   the collider to copy
    TilemapCollider::TilemapCollider( TilemapCollider const& other ) :
        Collider( other )
    {}


//-----------------------------------------------------------------------------
