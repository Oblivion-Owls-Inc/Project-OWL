///*****************************************************************/
/// @file	    Bullet.cpp
/// @author     Jax Clayton (jax.clayton@digipen.edu)
/// @date	    9/15/2021
/// @brief      Component that destroys itself and deals damage on contact
/// 
/// @copyright  Copyright (c) 2024 Digipen Institute of Technology
///*****************************************************************/

#include "pch.h" // precompiled header has to be included first
#include "Bullet.h"

#include "ComponentReference.t.h"
#include "Collider.h"

#include "Health.h"

//-----------------------------------------------------------------------------
// public: constructor / destructors
//-----------------------------------------------------------------------------


    /// @brief  constructor
    Bullet::Bullet() :
        Component( typeid( Bullet ) )
    {}


//-----------------------------------------------------------------------------
// public: accessors
//-----------------------------------------------------------------------------


    /// @brief Set the damage the bullet will do
    /// @param damage - the damage the bullet will do
    void Bullet::SetDamage( int damage )
    {
        m_Damage = damage;
    }

    /// @brief Get the damage the bullet will do
    int Bullet::GetDamage() const
    {
        return m_Damage;
    }


//-----------------------------------------------------------------------------
// public: virtual override methods
//-----------------------------------------------------------------------------


    /// @brief Default constructor for the Bullet class.
    void Bullet::OnInit()
    {
        m_Collider.SetOnConnectCallback(
            [ this ]()
            {
                m_Collider->AddOnCollisionEnterCallback(
                    GetId(),
                    std::bind( &Bullet::onCollisionEnter, this, std::placeholders::_1 )
                );
            }
        );
        m_Collider.SetOnDisconnectCallback(
            [ this ]()
            {
                m_Collider->RemoveOnCollisionEnterCallback( GetId() );
            }
        );

        m_Collider.Init( GetEntity() );
    }

    /// @brief  called when this Component's Entity is removed from the Scene
    void Bullet::OnExit()
    {
        m_Collider.Exit();
    }


//-----------------------------------------------------------------------------
// private: methods
//-----------------------------------------------------------------------------


    /// @brief  called whenever this Entity's Collider enters a collision
    /// @param  other   the collider that was collided with
    void Bullet::onCollisionEnter( Collider* other )
    {
        Health* health = other->GetEntity()->GetComponent< Health >();
        if ( health != nullptr )
        {
            health->TakeDamage( m_Damage );
        }

        GetEntity()->Destroy();
    }

    
//-----------------------------------------------------------------------------
// public: inspection
//-----------------------------------------------------------------------------


    /// @brief Used by the Debug System to display information about this Component
    void Bullet::Inspector()
    {
        if ( m_Collider == nullptr )
        {
            ImGui::Text( "WARNING: no Collider component attached" );
        }

        ImGui::DragInt( "damage", &m_Damage, 0.05f, 0, INT_MAX );
    }


//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------


    /// @brief  reads this Bullet's damage
    /// @param  data    the json data to read from
    void Bullet::readDamage( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_Damage, data );
    }


//-----------------------------------------------------------------------------
// public: reading / writing
//-----------------------------------------------------------------------------


    /// @brief gets the map of read methods for this Component
    /// @return the map of read methods for this Component
    ReadMethodMap< ISerializable > const& Bullet::GetReadMethods() const
    {
        static ReadMethodMap< Bullet > const readMethods = {
            { "Damage", &Bullet::readDamage }
        };

        return (ReadMethodMap< ISerializable > const&)readMethods;
    }

    /// @brief  writes this Bullet to json
    /// @return the written json data
    nlohmann::ordered_json Bullet::Write() const
    {
        nlohmann::ordered_json json;

        json[ "Damage" ] = Stream::Write( m_Damage );

        return json;
    }

    
//-----------------------------------------------------------------------------
// public: copying
//-----------------------------------------------------------------------------


    /// @brief  clones this RigidBody
    /// @return the newly created clone of this RigidBody
    Bullet* Bullet::Clone() const
    {
        return new Bullet( *this );
    }


//-----------------------------------------------------------------------------
// private: copying
//-----------------------------------------------------------------------------


    /// @brief  copy-constructor for the RigidBody
    /// @param  other   the other RigidBody to copy
    Bullet::Bullet( const Bullet& other ) :
        Component( other ),
        m_Damage( other.m_Damage )
    {}


//-----------------------------------------------------------------------------
