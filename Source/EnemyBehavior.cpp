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
        BaseEntityBehavior( typeid( EnemyBehavior ) )
    {}

//-----------------------------------------------------------------------------
// private: virtual override methods
//-----------------------------------------------------------------------------

    /// @brief initializes the component
    void EnemyBehavior::OnInit()
    {
        Behaviors<Behavior>()->AddBehavior(this);
        GetHealth()->OnInit();

        m_Pathfinder = Entities()->GetEntity( m_PathfinderName )->GetComponent< Pathfinder >();
        m_RigidBody = GetParent()->GetComponent< RigidBody >();
        m_Transform = GetParent()->GetComponent< Transform >();
    }

    /// @brief Called at a fixed interval
    void EnemyBehavior::OnFixedUpdate()
    {
        ChaseTarget();
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
	    { "Health"        , &BaseEntityBehavior::readHealth    },
        { "PathfinderName", &EnemyBehavior::readPathfinderName },
        { "Speed"         , &EnemyBehavior::readSpeed          }
    };


///----------------------------------------------------------------------------
/// public: writing
///----------------------------------------------------------------------------


///----------------------------------------------------------------------------
/// private: copying
///----------------------------------------------------------------------------

    /// @brief  copy constructor
    /// @param  other   the other EnemyBehavior to copy
    EnemyBehavior::EnemyBehavior( EnemyBehavior const& other ) :
        BaseEntityBehavior( other ),
        m_PathfinderName( other.m_PathfinderName ),
        m_Speed( other.m_Speed )
    {}

///----------------------------------------------------------------------------
