/// @file       CircleCollider.cpp
/// @author     Jax Clayton (jax.clayton@digipen.edu)
/// @brief      Circle-shaped collider
/// @version    0.1
/// @date       2023-09-13
/// 
/// @copyright  Copyright (c) 2023 Digipen Institute of Technology

#pragma once

#include "CircleCollider.h"
#include "DebugSystem.h"

//-----------------------------------------------------------------------------
// public: constructor
//-----------------------------------------------------------------------------

    /// @brief  default constructor
    CircleCollider::CircleCollider() :
        Collider( typeid( CircleCollider ) ),
        m_Radius( 1.0f )
    {}

    void CircleCollider::Inspector()
    {
        ImGui::DragFloat( "Radius", &m_Radius);
    }

//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------

    /// @brief  Reads the radius
    /// @param  stream  The json data to read from
    void CircleCollider::readRadius( Stream stream )
    {
        m_Radius = stream.Read<float>();
    }

    /// @brief map of the read methods for this Component
    ReadMethodMap< CircleCollider > CircleCollider::s_ReadMethods = {
        { "Radius", &readRadius }
    };

    /// @brief gets the map of read methods for this Component
    /// @return the map of read methods for this Component
    ReadMethodMap< Component > const& CircleCollider::GetReadMethods() const
    {
        return (ReadMethodMap< Component > const&)s_ReadMethods;
    }

//-----------------------------------------------------------------------------
// copying
//-----------------------------------------------------------------------------

    /// @brief  copy-constructor
    /// @param  other   the collider to copy
    CircleCollider::CircleCollider( CircleCollider const& other ) :
        Collider( other ),
        m_Radius( other.m_Radius )
    {}

    /// @brief  virtual component clone function
    /// @return new clone of component
    CircleCollider* CircleCollider::Clone() const
    {
        return new CircleCollider( *this );
    }

//-----------------------------------------------------------------------------
