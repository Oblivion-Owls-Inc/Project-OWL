/// @file       TurretBehavior.cpp
/// @author     Jax Clayton (jax.clayton@digipen.edu)
/// @brief      Behavior Compenent for Basic Turret  
/// @version    0.1
/// @date       2023-10-06
/// 
/// @copyright  Copyright (c) 2023 Digipen Institute of Technology


#include "pch.h" // precompiled header has to be included first
#include "TurretBehavior.h"

#include "BehaviorSystem.h"

#include "Bullet.h"
#include "BulletAoePulse.h"
#include "CircleCollider.h"
#include "RigidBody.h"

#include "EnemyBehavior.h"
#include "Generator.h"

#include "Engine.h"
#include "CollisionSystem.h"
#include "DebugSystem.h"
#include "RenderSystem.h"


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
        Behaviors< Behavior >()->AddComponent( this );

        m_Transform  .Init( GetEntity() );
        m_AudioPlayer.Init( GetEntity() );

        m_BulletPrefab.SetOwnerName( GetName() );
        m_BulletPrefab.Init();
    }

    /// @brief  called when this Component's Entity is removed from the Scene
    void TurretBehavior::OnExit()
    {
        Behaviors< Behavior >()->RemoveComponent( this );

        m_Transform  .Exit();
        m_AudioPlayer.Exit();
    }

    /// @brief Called Every Fixed Frame by the system
    void TurretBehavior::OnFixedUpdate()
    {
        if ( m_Transform == nullptr )
        {
            return;
        }

        checkActive();
        if ( m_IsActive )
        {
            float dt = GameEngine()->GetFixedFrameDuration();


            if (m_FireCooldown > 0.0f)
            {
                m_FireCooldown -= dt;
                if (m_FireCooldown > 0.0f) { return; }
            }
            else
            {
                m_FireCooldown = 0.0f;
            }

            /// Check for a target
            glm::vec2 direction = checkForTarget();

            if (direction != glm::vec2(0))
            {
                /// Fire a bullet at the target
                fireBullet(direction);
            }
        }
    }


//-----------------------------------------------------------------------------
// private: methods
//-----------------------------------------------------------------------------


    /// @brief  creates a bullet and fires it at the target
    /// @param  direction the direction to fire the bullet in
    void TurretBehavior::fireBullet( glm::vec2 const& direction )
    {
        // Create a new bullet entity
        if (m_BulletPrefab)
        {
            Entity* bullet = m_BulletPrefab->Clone();

            // Sets the data within the bullet
            Transform* bulletTransform = bullet->GetComponent<Transform>();
            if (bulletTransform)
            {
                bulletTransform->SetTranslation(m_Transform->GetTranslation());
                bulletTransform->SetScale(glm::vec2(m_BulletSize));
            }
            
            bullet->GetComponent< RigidBody >()->SetVelocity(direction * m_BulletSpeed);
            
            if (bullet->GetComponent< Bullet >())
            {
                bullet->GetComponent< Bullet >()->SetDamage(m_BulletDamage);
            }
            if (bullet->GetComponent< CircleCollider >())
            {
                bullet->GetComponent< CircleCollider >()->SetRadius(0.5f * m_BulletSize);
            }
            BulletAoePulse* pulse = bullet->GetComponent< BulletAoePulse >();
            if (pulse)
            {
                pulse->SetDamage(m_BulletDamage);
                pulse->SetRadius(m_BulletSize);
            }
            
            // Add the bullet to the entity system
            bullet->AddToScene();
            
            

            // Play turret shoot sound
            if (m_AudioPlayer != nullptr)
            {
                m_AudioPlayer->Play();
            }
        }

        m_FireCooldown += 1.0f / m_FireRate;
    }


    /// @brief  checks if this Turret is in range of a Generator and updates it's active state
    void TurretBehavior::checkActive()
    {
        for ( Generator* generator : Behaviors< Generator >()->GetComponents() )
        {
            if ( generator->GetActive() == false )
            {
                continue;
            }

            float distance = glm::distance(
                generator->GetTransform()->GetTranslation(),
                m_Transform->GetTranslation()
            );
            if ( distance <= generator->GetPowerRadius() )
            {
                m_IsActive = true;
                return;
            }
        }

        m_IsActive = false;
    }


    /// @brief  Uses Raycasting to check for a target on the same Collision Layer
    /// @return the direction towards the target, or (0, 0) if no valid target found
    glm::vec2 TurretBehavior::checkForTarget()
    {
        for ( EnemyBehavior const* enemy : Behaviors< EnemyBehavior >()->GetComponents() )
        {

            /// Get the position of the turret
            glm::vec2 turretPosition = m_Transform->GetTranslation();  

            /// Get the position of the entity
            glm::vec2 enemyPosition = enemy->GetTransform()->GetTranslation();

            /// Calculate the direction from the turret to the entity
            glm::vec2 offsetToEntity = enemyPosition - turretPosition;

            // check if the target is within range
            float distanceSquared = glm::dot( offsetToEntity, offsetToEntity );
            if ( distanceSquared > m_Range * m_Range )
            {
                continue;
            }

            // Cast a ray from the turret towards the entity to see if there are any walls in the way
            if (
                Collisions()->RayCast(
                    turretPosition, offsetToEntity, 1.0f, m_ObstructionLayers
                )
            )
            {
                continue;
            }

            // return the direction to the target
            return offsetToEntity / std::sqrt( distanceSquared );
        }

        // return zero vector to signify no valid enemies
        return glm::vec2( 0 );
    }

    
//-----------------------------------------------------------------------------
// public: inspection 
//-----------------------------------------------------------------------------


    /// @brief Used by the Debug System to display information about this Component
    void TurretBehavior::Inspector()
    {
        if ( m_Transform == nullptr )
        {
            ImGui::Text( "WARNING: no Transform Component attached" );
        }
        if ( m_AudioPlayer == nullptr )
        {
            ImGui::Text( "WARNING: no AudioPlayer Component attached" );
        }

        m_BulletPrefab.Inspect( "bullet prefab" );

        ImGui::DragFloat( "Range", &m_Range, 0.05f, 0.0f, INFINITY );
        if ( m_Transform != nullptr && ImGui::IsItemHovered() )
        {
            Renderer()->DrawCircle( m_Transform->GetTranslation(), m_Range );
        }

        ImGui::DragFloat( "Fire Rate", &m_FireRate, 0.05f, 0.0f, INFINITY );

        ImGui::InputInt( "Bullet Damage", &m_BulletDamage, 1, 5 );

        ImGui::DragFloat( "Bullet Speed", &m_BulletSpeed, 0.05f, 0.0f, INFINITY );

        ImGui::DragFloat( "Bullet Size", &m_BulletSize, 0.05f, 0.0f, INFINITY );

        m_ObstructionLayers.Inspect( "obstruction layers" );

        if ( m_IsActive ){ ImGui::Text( "Active: True" ); }
        else { ImGui::Text( "Active: False" ); }
    }


//-----------------------------------------------------------------------------
// private: reading 
//-----------------------------------------------------------------------------


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

    /// @brief  reads layers that can obstruct line of sight between this Turret and its target
    /// @param  data - the json data to read from
    void TurretBehavior::readObstructionLayers(nlohmann::ordered_json const& data)
    {
        Stream::Read( m_ObstructionLayers, data );
    }

    /// @brief Reads the name of the bullet prefab to grab from AssetLib
    /// @param data - the json data to read from
    void TurretBehavior::readBulletPrefab(nlohmann::ordered_json const& data)
    {
        Stream::Read( m_BulletPrefab, data );
    }

    
//-----------------------------------------------------------------------------
// public: reading / writing
//-----------------------------------------------------------------------------


    /// @brief gets the map of read methods for this Component
    /// @return the map of read methods for this Component
    ReadMethodMap< ISerializable > const& TurretBehavior::GetReadMethods() const
    {
        static ReadMethodMap< TurretBehavior > const readMethods = {
            { "FireRate"         , &TurretBehavior::readFireRate          },
            { "Range"            , &TurretBehavior::readRange             },
            { "BulletDamage"     , &TurretBehavior::readBulletDamage      },
            { "BulletSpeed"      , &TurretBehavior::readBulletSpeed       },
            { "BulletSize"       , &TurretBehavior::readBulletSize        },
            { "ObstructionLayers", &TurretBehavior::readObstructionLayers },
            { "BulletPrefab"     , &TurretBehavior::readBulletPrefab      }
        };

        return (ReadMethodMap< ISerializable > const&)readMethods;
    }

    /// @brief  Write all TurretBehavior data to a JSON file.
    /// @return The JSON data containing the TurretBehavior data.
    nlohmann::ordered_json TurretBehavior::Write() const
    {
        nlohmann::ordered_json json;

        json[ "FireRate"          ] = Stream::Write( m_FireRate          );
        json[ "Range"             ] = Stream::Write( m_Range             );
        json[ "BulletDamage"      ] = Stream::Write( m_BulletDamage      );
        json[ "BulletSpeed"       ] = Stream::Write( m_BulletSpeed       );
        json[ "BulletSize"        ] = Stream::Write( m_BulletSize        );
        json[ "ObstructionLayers" ] = Stream::Write( m_ObstructionLayers );
        json[ "BulletPrefab"      ] = Stream::Write( m_BulletPrefab      );

        return json;
    }

    
//-----------------------------------------------------------------------------
// public: copying
//-----------------------------------------------------------------------------


    /// @brief  creates a new clone of this Component
    /// @return the newly created clone
    TurretBehavior* TurretBehavior::Clone() const
    {
        return new TurretBehavior( *this );
    }


//-----------------------------------------------------------------------------
// private: copying
//-----------------------------------------------------------------------------


    TurretBehavior::TurretBehavior(const TurretBehavior& other) :
        Behavior( other ), 
        m_FireRate         ( other.m_FireRate          ), 
        m_Range            ( other.m_Range             ), 
        m_BulletDamage     ( other.m_BulletDamage      ), 
        m_BulletSpeed      ( other.m_BulletSpeed       ), 
        m_BulletSize       ( other.m_BulletSize        ),
        m_ObstructionLayers( other.m_ObstructionLayers ),
        m_BulletPrefab     ( other.m_BulletPrefab      )
    {}

    
//-----------------------------------------------------------------------------
