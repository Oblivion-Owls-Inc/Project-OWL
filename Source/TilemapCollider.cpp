/// @file       TilemapCollider.cpp
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      tilemap collider
/// @version    0.1
/// @date       2023-10-16
/// 
/// @copyright  Copyright (c) 2023 Digipen Institute of Technology

#pragma once

#include "TilemapCollider.h"

#include "ComponentReference.t.h"
#include "Transform.h"
#include "RigidBody.h"
#include "StaticBody.h"

#include "CollisionSystem.h"

//-----------------------------------------------------------------------------
// public: constructor
//-----------------------------------------------------------------------------


    /// @brief  default constructor
    TilemapCollider::TilemapCollider() :
        Collider( typeid( TilemapCollider ) )
    {}


//-----------------------------------------------------------------------------
// private: virtual overrides
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

        m_Transform .Init( GetEntity() );
        m_RigidBody .Init( GetEntity() );
        m_StaticBody.Init( GetEntity() );
        m_Tilemap   .Init( GetEntity() );
    }


    /// @brief  called once when exiting the scene
    void TilemapCollider::OnExit()
    {
        m_Transform .Exit( GetEntity() );
        m_RigidBody .Exit( GetEntity() );
        m_StaticBody.Exit( GetEntity() );
        m_Tilemap   .Exit( GetEntity() );
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
