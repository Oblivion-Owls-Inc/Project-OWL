///*****************************************************************/
/// @file	    BulletAoe.cpp
/// @author     Tyler Birdsall (tyler.birdsall@digipen.edu)
/// @date	    3//2024
/// @brief      Component that destroys itself and deals aoe damage on contact
/// 
/// @copyright  Copyright (c) 2024 Digipen Institute of Technology
///*****************************************************************/


#include "BulletAoe.h"

#include "ComponentReference.t.h"
#include "Collider.h"

#include "Health.h"

//-----------------------------------------------------------------------------
// public: constructor / destructors
//-----------------------------------------------------------------------------


    /// @brief  constructor
    BulletAoe::BulletAoe()
    {}


//-----------------------------------------------------------------------------
// public: virtual override methods
//-----------------------------------------------------------------------------


    /// @brief Default constructor for the BulletAoe class.
    /*void BulletAoe::OnInit()
    {
        m_Collider.SetOnConnectCallback(
            [ this ]()
            {
                m_Collider->AddOnCollisionEnterCallback(
                    GetId(),
                    std::bind( &BulletAoe::onCollisionEnter, this, std::placeholders::_1 )
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
    }*/




//-----------------------------------------------------------------------------
// private: methods
//-----------------------------------------------------------------------------


    /// @brief  called whenever this Entity's Collider enters a collision
    /// @param  other   the collider that was collided with
    void BulletAoe::onCollisionEnter( Collider* other )
    {
        Health* health = other->GetEntity()->GetComponent< Health >();
        if ( health != nullptr )
        {
            health->TakeDamage( GetDamage() );
        }

        GetEntity()->Destroy();
    }

//-----------------------------------------------------------------------------
// public: inspection
//-----------------------------------------------------------------------------

    /// @brief Used by the Debug System to display information about this Component
    void BulletAoe::Inspector()
    {
        ImGui::Text("Aoe Bullet");
        Bullet::Inspector();
    }


//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------


    /// @brief  reads this BulletAoe's damage
    /// @param  data    the json data to read from
    void BulletAoe::readDamage( nlohmann::ordered_json const& data )
    {
        int m_Damage = 0;
        Stream::Read( m_Damage, data );
        SetDamage(m_Damage);
    }


//-----------------------------------------------------------------------------
// public: reading / writing
//-----------------------------------------------------------------------------


    /// @brief gets the map of read methods for this Component
    /// @return the map of read methods for this Component
    ReadMethodMap< ISerializable > const& BulletAoe::GetReadMethods() const
    {
        static ReadMethodMap< BulletAoe > const readMethods = {
            { "Damage", &BulletAoe::readDamage }
        };

        return (ReadMethodMap< ISerializable > const&)readMethods;
    }

    /// @brief  writes this BulletAoe to json
    /// @return the written json data
    nlohmann::ordered_json BulletAoe::Write() const
    {
        nlohmann::ordered_json json;

        json[ "Damage" ] = Stream::Write( GetDamage() );

        return json;
    }

    
//-----------------------------------------------------------------------------
// public: copying
//-----------------------------------------------------------------------------


    /// @brief  clones this RigidBody
    /// @return the newly created clone of this RigidBody
    BulletAoe* BulletAoe::Clone() const
    {
        return new BulletAoe( *this );
    }


//-----------------------------------------------------------------------------
// private: copying
//-----------------------------------------------------------------------------


    /// @brief  copy-constructor for the RigidBody
    /// @param  other   the other RigidBody to copy
    BulletAoe::BulletAoe( const BulletAoe& other )
    {}


//-----------------------------------------------------------------------------
