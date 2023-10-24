/// @file       TurretBehavior.cpp
/// @author     Jax Clayton (jax.clayton@digipen.edu)
/// @brief      Behavior Compenent for Basic Turret  
/// @version    0.1
/// @date       2023-10-06
/// 
/// @copyright  Copyright (c) 2023 Digipen Institute of Technology



#include "TurretBehavior.h"
#include "BehaviorSystem.h"
#include "EntitySystem.h"
#include "Transform.h"
#include "BulletBehavior.h"
#include "CircleCollider.h"
#include "AssetLibrarySystem.h"
#include "CollisionSystem.h"
#include "EnemyBehavior.h"
#include "Animation.h"
#include "Engine.h"
#include "Pool.h"
#include "DebugSystem.h"
#include "RigidBody.h"

//-------------------------------------------------------------------------------------------
// public: constructor
//-------------------------------------------------------------------------------------------

    /// @brief  constructor
    TurretBehavior::TurretBehavior(): 
	    Behavior( typeid( TurretBehavior ) )
    {}

//-------------------------------------------------------------------------------------------
// private: virtual override methods
//-------------------------------------------------------------------------------------------

    /// @brief  called once when entering the scene
    void TurretBehavior::OnInit()
    {
        /// Add this behavior to the behavior system
        Behaviors<Behavior>()->AddBehavior(this);
        m_Transform = GetParent()->GetComponent< Transform >();
    }

    /// @brief  called when this Component's Entity is removed from the Scene
    /// @note   NOT CALLED WHEN THE SCENE IS EXITED - that should be handled by this Component's System
    void TurretBehavior::OnExit()
    {
        /// Remove this behavior from the behavior system
        Behaviors<Behavior>()->RemoveBehavior(this);
    }

    /// @brief Called Every Fixed Frame by the system
    void TurretBehavior::OnFixedUpdate()
    {
        float dt = Engine::GetInstance()->GetFixedFrameDuration();


        if ( m_FireCooldown > 0.0f )
        {
            m_FireCooldown -= dt;
            if ( m_FireCooldown > 0.0f ) { return; }
        }
        else
        {
            m_FireCooldown = 0.0f;
        }

        /// Check for a target
        glm::vec2 direction = CheckForTarget();

        if ( direction != glm::vec2( 0 ) )
        {
            /// Fire a bullet at the target
            FireBullet( direction );
        }
    }

    /// @brief Used by the Debug System to display information about this Component
    void TurretBehavior::Inspector()
    {
        // Edit the Behavior of the Turret 
        ImGui::InputFloat("Range", &m_Range, 0.5f, 1.0f);
        ImGui::InputFloat("Fire Rate", &m_FireRate, 0.5f, 1.0f);
        ImGui::InputInt("Bullet Damage", &m_BulletDamage, 1, 5);
        ImGui::InputFloat("Bullet Speed", &m_BulletSpeed, 0.5f, 1.0f);
        ImGui::InputFloat("Bullet Size", &m_BulletSize, 0.5f, 1.0f);
        ImGui::Text("Target Name: %s", m_TargetName.c_str());
    }

///-------------------------------------------------------------------------------------------
// private: methods
///-------------------------------------------------------------------------------------------

    /// @brief  creates a bullet and fires it at the target
    /// @param  direction the direction to fire the bullet in
    void TurretBehavior::FireBullet( glm::vec2 const& direction )
    {
        // Create a new bullet entity
        Entity* bullet = m_BulletPrefab->Clone();

        /// Sets the data within the bullet
        Transform* bulletTransform = bullet->GetComponent<Transform>();
        bulletTransform->SetTranslation( m_Transform->GetTranslation() );
        bulletTransform->SetScale( glm::vec2( m_BulletSize ) );

        bullet->GetComponent< BulletBehavior >()->SetDamage( m_BulletDamage );
        bullet->GetComponent< RigidBody >()->SetVelocity( direction * m_BulletSpeed );

        /// Add the bullet to the entity system
        Entities()->AddEntity(bullet);

        m_FireCooldown += 1.0f / m_FireRate;
    }

    /// @brief  Uses Raycasting to check for a target on the same Collision Layer
    /// @return the direction towards the target, or (0, 0) if no valid target found
    glm::vec2 TurretBehavior::CheckForTarget()
    {
        for ( auto& entity : Entities()->GetEntities() )
        {
            /// Skip the entities that don't match the target name		
            if (entity->GetName() != m_TargetName)
            {
                continue;
            }

            /// Get the position of the turret
            glm::vec2 turretPosition = m_Transform->GetTranslation();  

            /// Get the position of the entity
            glm::vec2 enemyPosition = entity->GetComponent<Transform>()->GetTranslation();

            /// Calculate the direction from the turret to the entity
            glm::vec2 directionToEntity = enemyPosition - turretPosition;

            // check if the target is within range
            float distanceSquared = glm::dot( directionToEntity, directionToEntity );
            if ( distanceSquared > m_Range * m_Range )
            {
                continue;
            }

            // Cast a ray from the turret towards the entity to see if there are any walls in the way
            RayCastHit hitWall = Collisions()->RayCast(
                turretPosition, directionToEntity, 1.0f, Collisions()->GetLayerFlags( { "Terrain" } )
            );
            if ( hitWall )
            {
                continue;
            }

            /// Return the raycast hit
            return directionToEntity / std::sqrt( distanceSquared );
        }

        /// Return the raycast hit
        return glm::vec2( 0 );
    }

///-------------------------------------------------------------------------------------------
// private: reading 
///-------------------------------------------------------------------------------------------

    /// @brief Reads the name of the bullet prefab to grab from AssetLib
    /// @param data - the json data to read from
    void TurretBehavior::readBulletName(nlohmann::ordered_json const& data)
    {
	    /// Get the bullet prefab
	    m_BulletPrefab = AssetLibrary< Entity >()->GetAsset( Stream::Read< std::string >( data ) );
    }

    /// @brief reads in the name of the target
    /// @param data - the json data to read from
    void TurretBehavior::readTargetName(nlohmann::ordered_json const& data)
    {
	    m_TargetName = Stream::Read<std::string>(data);
    }

    /// @brief reads the fire rate from the json file
    /// @param jsonValue  the json data
    void TurretBehavior::readFireRate( nlohmann::ordered_json const& data )
    {
	    m_FireRate = Stream::Read<float>(data);
    }

    /// @brief reads the range from the json file
    /// @param jsonValue the json data
    void TurretBehavior::readRange( nlohmann::ordered_json const& data )
    {
	    m_Range = Stream::Read<float>(data);
    }

    /// @brief reads the bullet damage from the json file
    /// @param jsonValue the json data
    void TurretBehavior::readBulletDamage( nlohmann::ordered_json const& data )
    {
	    m_BulletDamage = Stream::Read<int>(data);
    }

    /// @brief reads the Bullet Speed from the json file
    /// @param jsonValue the json data
    void TurretBehavior::readBulletSpeed( nlohmann::ordered_json const& data )
    {
	    m_BulletSpeed = Stream::Read<float>(data);
    }

    /// @brief reads the bullet size from the json file
    /// @param jsonValue the json data
    void TurretBehavior::readBulletSize( nlohmann::ordered_json const& data )
    {
	    m_BulletSize = Stream::Read<float>(data);
    }

    /// @brief the map of read methods for this Component
    ReadMethodMap<TurretBehavior> const TurretBehavior::s_ReadMethods =
    {
	    { "FireRate",			  &readFireRate },
	    { "Range",					 &readRange },
	    { "BulletName",		   &readBulletName  },
	    { "BulletDamage",	  &readBulletDamage },
	    { "BulletSpeed",	   &readBulletSpeed },
	    { "BulletSize",		    &readBulletSize },
	    { "Target",				&readTargetName },
    };

/// @brief Write all TurretBehavior data to a JSON file.
/// @return The JSON file containing the TurretBehavior data.
nlohmann::ordered_json TurretBehavior::Write() const
{
    nlohmann::ordered_json data;

    data["FireRate"] = m_FireRate;
    data["Range"] = m_Range;
    data["BulletDamage"] = m_BulletDamage;
    data["BulletSpeed"] = m_BulletSpeed;
    data["BulletSize"] = m_BulletSize;
    data["BulletName"] = AssetLibrary< Entity >()->GetAssetName( m_BulletPrefab );
    data["Target"] = m_TargetName;

    return data;
}

TurretBehavior::TurretBehavior(const TurretBehavior& other) :
    Behavior( other ), 
    m_FireRate(other.m_FireRate), 
    m_Range(other.m_Range), 
    m_BulletDamage(other.m_BulletDamage), 
    m_BulletSpeed(other.m_BulletSpeed), 
    m_BulletSize(other.m_BulletSize),
    m_FireCooldown(other.m_FireCooldown),
    m_BulletPrefab(other.m_BulletPrefab),
    m_TargetName(other.m_TargetName)
{}