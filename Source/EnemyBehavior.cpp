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
#include "AssetLibrarySystem.h"
#include "Animation.h"
#include "AnimationAsset.h"

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
        m_RigidBody = GetParent()->GetComponent< RigidBody >();
        m_Transform = GetParent()->GetComponent< Transform >();
        m_Animation = GetParent()->GetComponent< Animation >();

        // Get all the enemy's animations
        for (int i = 0; i < 2; ++i)
        {
            m_EnemyAnimations[i] = AssetLibrary< AnimationAsset >()->GetInstance()->GetAsset(m_AnimationNames[i]);
        }
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

        // Changes the animation based on the direction
        if (moveDir.x > 0.0f)
        {
            m_Animation->SetAsset(m_EnemyAnimations[0]);
            m_Animation->SetIsRunning(true);
        }
        else
        {
            m_Animation->SetAsset(m_EnemyAnimations[1]);
            m_Animation->SetIsRunning(true);
        }
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

    /// @brief Read in the names of the player animations.
    /// @param data The JSON file to read from.
    void EnemyBehavior::readAnimationNames(nlohmann::ordered_json const& data)
    {
        for (int i = 0; i < 2; i++)
        {
            Stream::Read(m_AnimationNames[i], data[i]);
        }
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
        { "Damage"        , &EnemyBehavior::readDamage          },
        { "AnimationNames", &EnemyBehavior::readAnimationNames  }
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

        // Write the names of the current enemy animations to a JSON
        nlohmann::ordered_json& animationNames = data["AnimationNames"];
        for (std::string const& animationName : m_AnimationNames)
        {
            animationNames.push_back(Stream::Write(animationName));
        }

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
        m_Animation(nullptr)
    {
        // Copy the animations
        for (int i = 0; i < 2; i++)
        {
            m_AnimationNames[i] = other.m_AnimationNames[i];
            m_EnemyAnimations[i] = other.m_EnemyAnimations[i];
        }
    }

///----------------------------------------------------------------------------
