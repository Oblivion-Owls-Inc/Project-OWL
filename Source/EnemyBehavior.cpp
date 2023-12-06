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
#include "AudioPlayer.h"

//-----------------------------------------------------------------------------
// public: constructor / destructor
//-----------------------------------------------------------------------------

    EnemyBehavior::EnemyBehavior() :
        BasicEntityBehavior( typeid( EnemyBehavior ) )
    {}

//-----------------------------------------------------------------------------
// private: virtual override methods
//-----------------------------------------------------------------------------

    /// @brief initializes the component
    void EnemyBehavior::OnInit()
    {
        BasicEntityBehavior::OnInit();

        m_Pathfinder = Entities()->GetEntity( m_PathfinderName )->GetComponent< Pathfinder >();
        m_RigidBody = GetEntity()->GetComponent< RigidBody >();
        m_Transform = GetEntity()->GetComponent< Transform >();
        m_AudioPlayer = GetEntity()->GetComponent<AudioPlayer>();
    }

    /// @brief Called at a fixed interval
    void EnemyBehavior::OnFixedUpdate()
    {
        ChaseTarget();
    }

    void EnemyBehavior::OnUpdate(float)
    {
    }

    void EnemyBehavior::Inspector()
    {
        ImGui::VSliderFloat("Speed", ImVec2(18, 160), &m_Speed, 0, 100);
        ImGui::SameLine();
        ImGui::VSliderInt("Damage", ImVec2(18, 160), &m_Damage, 0, 100);

        BasicEntityBehavior::Inspector();
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

        // Changes the animation based on the direction
        glm::vec2 scale = m_Transform->GetScale();
        if ( (scale.x > 0) != (moveDir.x > 0) )
        {
            scale.x *= -1;
            m_Transform->SetScale( scale );
        }
        m_AudioPlayer->Play();
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

    void EnemyBehavior::readDamage(nlohmann::ordered_json const& data)
    {
        Stream::Read( m_Damage, data );
    }

    /// @brief  map of read methods
    ReadMethodMap< EnemyBehavior > const EnemyBehavior::s_ReadMethods = {
	    { "Health"        , &BasicEntityBehavior::readHealth    },
        { "PathfinderName", &EnemyBehavior::readPathfinderName  },
        { "Speed"         , &EnemyBehavior::readSpeed           },
        { "Damage"        , &EnemyBehavior::readDamage          }
    };


///----------------------------------------------------------------------------
/// public: writing
///----------------------------------------------------------------------------

    int EnemyBehavior::GetDamage()
    {
        return m_Damage;
    }

    /// @brief  write all component data to a JSON object
    nlohmann::ordered_json EnemyBehavior::Write() const
    {
        nlohmann::ordered_json data;

        data["Health"] = m_Health.Write();
        data["PathfinderName"] = m_PathfinderName;
        data["Speed"] = m_Speed;
        data["Damage"] = m_Damage;

        return data;
    }


///----------------------------------------------------------------------------
/// private: copying
///----------------------------------------------------------------------------

    /// @brief  copy constructor
    /// @param  other   the other EnemyBehavior to copy
    EnemyBehavior::EnemyBehavior( EnemyBehavior const& other ) :
        BasicEntityBehavior( other ),
        m_PathfinderName( other.m_PathfinderName ),
        m_Speed( other.m_Speed ),
        m_RigidBody(nullptr),
        m_AudioPlayer(nullptr)
    {}

///----------------------------------------------------------------------------
