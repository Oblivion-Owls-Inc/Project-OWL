///*****************************************************************/
/// @file	    BulletAoePulse.cpp
/// @author     Tyler Birdsall (tyler.birdsall@digipen.edu)
/// @date	    3//2024
/// @brief      Component that destroys itself and deals aoe damage on contact
/// 
/// @copyright  Copyright (c) 2024 Digipen Institute of Technology
///*****************************************************************/

#include "pch.h"
#include "BulletAoePulse.h"

#include "ComponentReference.t.h"
#include "Collider.h"
#include "EnemyBehavior.h"
#include "BehaviorSystem.h"

#include "Health.h"

//-----------------------------------------------------------------------------
// public: constructor / destructors
//-----------------------------------------------------------------------------


    /// @brief  constructor
    BulletAoePulse::BulletAoePulse() : Bullet(typeid(BulletAoePulse))
    {}


//-----------------------------------------------------------------------------
// public: virtual override methods
//-----------------------------------------------------------------------------


    /// @brief Default constructor for the BulletAoePulse class.
    void BulletAoePulse::OnInit()
    {
        for (EnemyBehavior const* enemy : Behaviors< EnemyBehavior >()->GetComponents())
        {
            /// Get the position of the turret
            glm::vec2 turretPosition = GetEntity()->GetComponent<Transform>()->GetTranslation();

            /// Get the position of the entity
            glm::vec2 enemyPosition = enemy->GetTransform()->GetTranslation();

            /// Calculate the direction from the turret to the entity
            glm::vec2 offsetToEntity = enemyPosition - turretPosition;

            // check if the target is within range
            float distanceSquared = glm::dot(offsetToEntity, offsetToEntity);
            if (distanceSquared > m_Radius * m_Radius)
            {
                continue;
            }

            Health* health = enemy->GetEntity()->GetComponent< Health >();
            if (health != nullptr)
            {
                health->TakeDamage(m_Damage);
            }
        }
        GetEntity()->Destroy();
    }

//-----------------------------------------------------------------------------
// public: accessors
//-----------------------------------------------------------------------------

    /// @brief Set the damage the bullet will do
    /// @param damage - the damage the bullet will do
    void BulletAoePulse::SetDamage(int damage)
    {
        m_Damage = damage;
    }

    /// @brief Set the damage the bullet will do
    int BulletAoePulse::GetDamage()
    {
        return m_Damage;
    }

    /// @brief Set the radius of the bullet pulse
    /// @param radius - the radius of the bullet pulse
    void BulletAoePulse::SetRadius(float radius)
    {
        m_Radius = radius;
    }

    /// @brief Get the radius of the bullet pulse
    float BulletAoePulse::GetRadius()
    {
        return m_Radius;
    }

//-----------------------------------------------------------------------------
// public: inspection
//-----------------------------------------------------------------------------

    /// @brief Used by the Debug System to display information about this Component
    void BulletAoePulse::Inspector()
    {
        ImGui::Text("Aoe Pulse");
        //ImGui::DragInt("damage", &m_Damage, 0.05f, 0, INT_MAX);
        ImGui::DragFloat("radius", &m_Radius, 0.5f);
    }


//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------

    /// @brief  reads this BulletAoePulse's radius
    /// @param  data    the json data to read from
    void BulletAoePulse::readRadius(nlohmann::ordered_json const& data)
    {
        Stream::Read( m_Radius, data );
    }

//-----------------------------------------------------------------------------
// public: reading / writing
//-----------------------------------------------------------------------------


    /// @brief gets the map of read methods for this Component
    /// @return the map of read methods for this Component
    ReadMethodMap< ISerializable > const& BulletAoePulse::GetReadMethods() const
    {
        static ReadMethodMap< BulletAoePulse > const readMethods = {
            { "Radius", &BulletAoePulse::readRadius }
        };

        return (ReadMethodMap< ISerializable > const&)readMethods;
    }

    /// @brief  writes this BulletAoePulse to json
    /// @return the written json data
    nlohmann::ordered_json BulletAoePulse::Write() const
    {
        nlohmann::ordered_json json;

        json[ "Radius" ] = Stream::Write( m_Radius );

        return json;
    }

    
//-----------------------------------------------------------------------------
// public: copying
//-----------------------------------------------------------------------------


    /// @brief  clones this RigidBody
    /// @return the newly created clone of this RigidBody
    BulletAoePulse* BulletAoePulse::Clone() const
    {
        return new BulletAoePulse( *this );
    }


//-----------------------------------------------------------------------------
// private: copying
//-----------------------------------------------------------------------------


    /// @brief  copy-constructor for the RigidBody
    /// @param  other   the other RigidBody to copy
    BulletAoePulse::BulletAoePulse( const BulletAoePulse& other ) :
        m_Damage(other.m_Damage),
        m_Radius(other.m_Radius)
    {}


//-----------------------------------------------------------------------------
