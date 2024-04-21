/// @file       CircleCollider.cpp
/// @author     Jax Clayton (jax.clayton@digipen.edu)
/// @brief      Circle-shaped collider
/// @version    0.1
/// @date       2023-09-13
/// 
/// @copyright  Copyright (c) 2024 Digipen Institute of Technology

#pragma once


#include "pch.h" // precompiled header has to be included first
#include "CircleCollider.h"

#include "ComponentReference.t.h"
#include "Transform.h"
#include "RigidBody.h"
#include "StaticBody.h"

#include "DebugSystem.h"
#include "RenderSystem.h"
#include "CollisionSystem.h"

//-----------------------------------------------------------------------------
// public: constructor
//-----------------------------------------------------------------------------


    /// @brief  default constructor
    CircleCollider::CircleCollider() :
        Collider( typeid( CircleCollider ) )
    {}

    
//-----------------------------------------------------------------------------
// public: accessors
//-----------------------------------------------------------------------------


    /// @brief  gets this CircleCollider's radius
    /// @return this CircleCollider's radius
    float CircleCollider::GetRadius() const
    {
        return m_Radius;
    }

    /// @brief  sets this CircleCollider's radius
    /// @param  radius  the radius
    void CircleCollider::SetRadius( float radius )
    {
        m_HasChanged = true;

        m_Radius = radius;
    }


    /// @brief  gets whether this CircleCollider has changed and its position in the CollisionSystem needs to update
    /// @return whether this CircleCollider has changed and its position in the CollisionSystem needs to update
    bool CircleCollider::GetHasChanged() const
    {
        return m_HasChanged;
    }

    /// @brief  clears the HasChanged flag
    /// @brief  SHOULD ONLY BE CALLED BY COLLISIONSYSTEM
    void CircleCollider::ClearHasChanged()
    {
        m_HasChanged = false;
    }


//-----------------------------------------------------------------------------
// public: virtual overrides
//-----------------------------------------------------------------------------


    /// @brief  called when this Component's Entity enters the Scene
    void CircleCollider::OnInit()
    {
        m_Transform.SetOnConnectCallback(
            [ this ]()
            {
                m_Transform->AddOnTransformChangedCallback(
                    GetId(),
                    [ this ]()
                    {
                        m_HasChanged = true;
                    }
                );

                Collisions()->AddCollider( this );
            }
        );
        m_Transform.SetOnDisconnectCallback(
            [ this ]()
            {
                Collisions()->RemoveCollider( this );

                m_Transform->RemoveOnTransformChangedCallback( GetId() );
            }
        );

        m_Transform .Init( GetEntity() );
        m_RigidBody .Init( GetEntity() );
        m_StaticBody.Init( GetEntity() );
    }

    /// @brief  called when this Component's Entity is removed from the Scene
    void CircleCollider::OnExit()
    {
        m_Transform .Exit();
        m_RigidBody .Exit();
        m_StaticBody.Exit();
    }


    /// @brief  draws the collision shape of this collider for debug purposes
    void CircleCollider::DebugDraw() const
    {
        if ( m_Transform == nullptr )
        {
            return;
        }

        Renderer()->DrawCircle( m_Transform->GetTranslation(), m_Radius );
    }
    

//-----------------------------------------------------------------------------
// public: inspection
//-----------------------------------------------------------------------------


    /// @brief  inspector for this CircleCollider
    void CircleCollider::Inspector()
    {
        if ( m_Transform == nullptr )
        {
            ImGui::Text( "WARNING: no Transform component attached" );
        }

        if ( ImGui::DragFloat( "Radius", &m_Radius, 0.05f, 0.0f, INFINITY ) )
        {
            m_HasChanged = true;
        }

        Collider::Inspector();
    }

//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------


    /// @brief  Reads the radius
    /// @param  stream  The json data to read from
    void CircleCollider::readRadius( nlohmann::ordered_json const& data )
    {
        m_Radius = data;
    }
    

//-----------------------------------------------------------------------------
// public: reading / writing
//-----------------------------------------------------------------------------


    /// @brief gets the map of read methods for this Component
    /// @return the map of read methods for this Component
    ReadMethodMap< ISerializable > const& CircleCollider::GetReadMethods() const
    {
        static ReadMethodMap< CircleCollider > const readMethods = {
            { "Radius"             , &CircleCollider::readRadius              },
            { "CollisionLayer"     , &CircleCollider::readCollisionLayer      },
            { "CollisionLayerFlags", &CircleCollider::readCollisionLayerFlags }
        };

        return (ReadMethodMap< ISerializable > const&)readMethods;
    }



    /// @brief Write all CircleCollider component data to a JSON file.
    /// @return The JSON file containing the CircleCollider component data.
    nlohmann::ordered_json CircleCollider::Write() const
    {
        nlohmann::ordered_json data;

        data[ "Radius"              ] = Stream::Write( m_Radius                 );
        data[ "CollisionLayer"      ] = Stream::Write( GetCollisionLayer()      );
        data[ "CollisionLayerFlags" ] = Stream::Write( GetCollisionLayerFlags() );

        return data;
    }


//-----------------------------------------------------------------------------
// public: copying
//-----------------------------------------------------------------------------


    /// @brief  virtual component clone function
    /// @return new clone of component
    CircleCollider* CircleCollider::Clone() const
    {
        return new CircleCollider( *this );
    }

    
//-----------------------------------------------------------------------------
// private: copying
//-----------------------------------------------------------------------------


    /// @brief  copy-constructor
    /// @param  other   the collider to copy
    CircleCollider::CircleCollider( CircleCollider const& other ) :
        Collider( other ),
        m_Radius( other.m_Radius )
    {}


//-----------------------------------------------------------------------------
