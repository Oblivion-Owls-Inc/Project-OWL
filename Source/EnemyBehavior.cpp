///*****************************************************************/
/// @file	    EnemyBehavior.h
/// @author     Jax Clayton (jax.clayton@digipen.edu)
/// @date	    10/1/2023
/// @brief      EnemyBehavior class header
/// @copyright  Digipen LLC (c) 2023
///*****************************************************************/

#include "EnemyBehavior.h"

#include "BehaviorSystem.h"
#include "EntitySystem.h"
#include "Transform.h"
#include "Engine.h"
#include "Pathfinder.h"
#include "RigidBody.h"
#include "basics.h"
#include "Pool.h"
#include "Entity.h"

//-----------------------------------------------------------------------------
// public: constructor / destructor
//-----------------------------------------------------------------------------

    EnemyBehavior::EnemyBehavior() :
        Behavior( typeid( EnemyBehavior ) ),
        m_Health("Health")
    {}

///-----------------------------------------------------------------------------
// public: methods
///-----------------------------------------------------------------------------

    /// @brief Apply damage to the enemy 
    /// @param damage - the amount of damage to enemy
    void EnemyBehavior::TakeDamage(int damage)
    {
        m_Health -= damage;
        if ( !m_Health )
        {
		    GetParent()->Destroy();
	    }
    }

//-----------------------------------------------------------------------------
// private: virtual override methods
//-----------------------------------------------------------------------------

    /// @brief initializes the component
    void EnemyBehavior::OnInit()
    {
        Behaviors<Behavior>()->AddBehavior(this);
        m_Health.OnInit();

        m_Pathfinder = Entities()->GetEntity( m_PathfinderName )->GetComponent< Pathfinder >();
        m_RigidBody = GetParent()->GetComponent< RigidBody >();
        m_Transform = GetParent()->GetComponent< Transform >();
    }

    /// @brief Called when the component is destroyed
    void EnemyBehavior::OnExit()
    {
        Behaviors<Behavior>()->RemoveBehavior(this);
    }

    /// @brief Called at a fixed interval
    void EnemyBehavior::OnFixedUpdate()
    {
        ChaseTarget();
    }

    /// @brief  inspector for this component
    void EnemyBehavior::Inspector()
    {
        m_Health.Inspector();
    }

//-----------------------------------------------------------------------------
// private: methods
//-----------------------------------------------------------------------------

    ///  @brief moves towards the target
    void EnemyBehavior::ChaseTarget()
    {
        // accelerate along path
        glm::vec2 moveDir = m_Pathfinder->GetDirectionAt( m_Transform->GetTranslation() );
        m_RigidBody->ApplyAcceleration( moveDir * m_Speed );
    }

///-----------------------------------------------------------------------------
// private: Reading
///-----------------------------------------------------------------------------

    /// @brief  reads the health of the enemy from json
    /// @param  data    the json data to read from
    void EnemyBehavior::readHealth(nlohmann::ordered_json const& data)
    {
        Stream::Read( m_Health, data );
    }

    /// @brief  reads the name of the pathfinder entity
    /// @param  data    the json data to read from
    void EnemyBehavior::readPathfinderName( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_PathfinderName, data );
    }

    /// @brief  reads the speed
    /// @param  data    the json data to read from
    void EnemyBehavior::readSpeed( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_Speed, data );
    }

    /// @brief  map of read methods
    ReadMethodMap< EnemyBehavior > const EnemyBehavior::s_ReadMethods = {
	    { "Health"        , &EnemyBehavior::readHealth         },
        { "PathfinderName", &EnemyBehavior::readPathfinderName },
        { "Speed"         , &EnemyBehavior::readSpeed          }
    };


///----------------------------------------------------------------------------
// public: writing
///----------------------------------------------------------------------------

    /// @brief  write all component data to a JSON object
    /// @return the JSON object containing the component data
    nlohmann::ordered_json EnemyBehavior::Write() const
    {
	    nlohmann::ordered_json data;
        data["Health"] = m_Health.Write();
	    return data;
    }

///----------------------------------------------------------------------------
// private: copying
///----------------------------------------------------------------------------

    /// @brief  copy constructor
    /// @param  other   the other EnemyBehavior to copy
    EnemyBehavior::EnemyBehavior( EnemyBehavior const& other ) :
        Behavior( other ),
        m_Health( other.m_Health ),
        m_PathfinderName( other.m_PathfinderName ),
        m_Speed( other.m_Speed )
    {}

///----------------------------------------------------------------------------
