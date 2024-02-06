///*****************************************************************/
/// @file	 BulletBehavior.cpp
/// @author  Jax Clayton (jax.clayton@digipen.edu)
/// @date	 9/15/2021
/// @brief   Bullet behavior class 
/// @details Game Behavior that handles bullet movement and collision
///*****************************************************************/

#include "BulletBehavior.h"
#include "BehaviorSystem.h"
#include "Transform.h"
#include "EnemyBehavior.h"
#include "Engine.h"
#include "Entity.h"
#include "CollisionSystem.h"
#include "CircleCollider.h"
#include "Health.h"

//-----------------------------------------------------------------------------
// public: constructor / destructors
//-----------------------------------------------------------------------------

    /// @brief  constructor
    BulletBehavior::BulletBehavior() :
        Behavior( typeid( BulletBehavior ) ),
        m_LifeTime()
    {}

//-----------------------------------------------------------------------------
// public: virtual override methods
//-----------------------------------------------------------------------------

    /// @brief Default constructor for the BulletBehavior class.
    void BulletBehavior::OnInit()
    {
	    Behaviors< Behavior >()->AddComponent( this );
        GetEntity()->GetComponent< CircleCollider >()->AddOnCollisionEnterCallback(
            GetId(),
            std::bind( &BulletBehavior::onCollisionEnter, this, std::placeholders::_1 )
        );
    }

    /// @brief  called when this Component's Entity is removed from the Scene
    /// @note   NOT CALLED WHEN THE SCENE IS EXITED - that should be handled by this Component's System
    void BulletBehavior::OnExit()
    {
	    Behaviors< Behavior >()->RemoveComponent( this );
    }

    /// @brief Fixed update method called at a fixed time step.
    void BulletBehavior::OnFixedUpdate()
    {
	    float dt = Engine::GetInstance()->GetFixedFrameDuration();

	    // Update the bullet's life time
	    m_LifeTime -= dt;

	    // Destroy the bullet if it's life time is over
	    if ( m_LifeTime.GetCurrent() <= 0.0f )
	    {
		    GetEntity()->Destroy();
	    }
    }

    /// @brief Used by the Debug System to display information about this Component
    void BulletBehavior::Inspector()
    {
	    m_LifeTime.Inspect();
	    ImGui::InputInt( "Bullet Damage", &m_Damage, 1, 5 );
    }

//-----------------------------------------------------------------------------
// private: methods
//-----------------------------------------------------------------------------

    /// @brief  called whenever this Entity's Collider enters a collision
    /// @param  other   the collider that was collided with
    void BulletBehavior::onCollisionEnter( Collider* other )
    {
        // If the bullet hits an enemy, deal damage to it
        if ( other->GetCollisionLayer() == Collisions()->GetCollisionLayerId( "Enemies" ) )
        {
            other->GetEntity()->GetComponent< Health >()->TakeDamage( m_Damage );
        }

        // The bullet is destroyed on collision
        GetEntity()->Destroy();
    }

//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------

    /// @brief  reads how long this Bullet will last
    /// @param  data    the json data to read from
    void BulletBehavior::readLifetime( nlohmann::ordered_json const& data )
    {
	    Stream::Read( m_LifeTime, data );
    }

    /// @brief  reads this Bullet's damage
    /// @param  data    the json data to read from
    void BulletBehavior::readDamage( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_Damage, data );
    }

    ReadMethodMap<BulletBehavior> const BulletBehavior::s_ReadMethods = {
        { "Lifetime", &readLifetime },
        { "Damage"  , &readDamage   }
    };

//-----------------------------------------------------------------------------
// public: writing
//-----------------------------------------------------------------------------

    /// @brief  writes this BulletBehavior to json
    /// @return the written json data
    nlohmann::ordered_json BulletBehavior::Write() const
    {
        nlohmann::ordered_json json;

        json[ "Lifetime" ] = Stream::Write( m_LifeTime );
        json[ "Damage" ] = Stream::Write( m_Damage );

        return json;
    }

//-----------------------------------------------------------------------------
// private: copying
//-----------------------------------------------------------------------------

    /// @brief  copy-constructor for the RigidBody
    /// @param  other   the other RigidBody to copy
    BulletBehavior::BulletBehavior(const BulletBehavior& other) :
        Behavior( other ),
        m_Damage( other.m_Damage ),
        m_LifeTime( other.m_LifeTime )
    {}

//-----------------------------------------------------------------------------
