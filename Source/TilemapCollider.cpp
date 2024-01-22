/// @file       TilemapCollider.cpp
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      tilemap collider
/// @version    0.1
/// @date       2023-10-16
/// 
/// @copyright  Copyright (c) 2023 Digipen Institute of Technology

#pragma once

#include "TilemapCollider.h"

#include "Tilemap.h"

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
        Collider::OnInit();

        m_Tilemap = GetEntity()->GetComponent< Tilemap< int > >();
    }

//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------

    /// @brief Write all TilemapCollider component data to a JSON file.
    /// @return The JSON file containing the TilemapCollider component data.
    nlohmann::ordered_json TilemapCollider::Write() const
    {
        nlohmann::ordered_json data;

        data[ "CollisionLayer" ] = GetCollisionLayer();
        data[ "CollisionLayerFlags" ] = GetCollisionLayerFlags();

        return data;
    }

    /// @brief map of the read methods for this Component
    ReadMethodMap< TilemapCollider > TilemapCollider::s_ReadMethods = {
        { "CollisionLayer"     , &readCollisionLayer      },
        { "CollisionLayerFlags", &readCollisionLayerFlags }
    };

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
