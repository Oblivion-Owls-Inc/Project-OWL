///*****************************************************************/
/// @file	    EnemyBehavior.h
/// @author     Jax Clayton (jax.clayton@digipen.edu)
/// @date	    10/1/2023
/// @brief      EnemyBehavior class header
/// @copyright  Digipen LLC (c) 2023
///*****************************************************************/


#include "pch.h" // precompiled header has to be included first
#include "EnemyBehavior.h"

#include "ComponentReference.t.h"

#include "BehaviorSystem.h"
#include "EntitySystem.h"
#include "Transform.h"
#include "Engine.h"
#include "RigidBody.h"
#include "basics.h"
#include "Pool.h"
#include "Entity.h"
#include "AudioPlayer.h"
#include "Health.h"
#include "PathfindSystem.h"

//-----------------------------------------------------------------------------
// public: constructor / destructor
//-----------------------------------------------------------------------------

    
    /// @brief  default constructor
    EnemyBehavior::EnemyBehavior() :
        Behavior( typeid( EnemyBehavior ) )
    {}


//-----------------------------------------------------------------------------
// public: accessors
//-----------------------------------------------------------------------------


    /// @brief  gets the amount of damage that this EnemyBehavior deals
    /// @return the amount of damage that this EnemyBehavior deals
    int EnemyBehavior::GetDamage() const
    {
        return m_Damage;
    }


    /// @brief  gets this EnemyBehavior's attached Transform Component
    /// @return this EnemyBehavior's attached Transform Component
    Transform const* EnemyBehavior::GetTransform() const
    {
        return m_Transform;
    }


//-----------------------------------------------------------------------------
// private: virtual override methods
//-----------------------------------------------------------------------------


    /// @brief initializes the component
    void EnemyBehavior::OnInit()
    {
        Behaviors< EnemyBehavior >()->AddComponent( this );

        m_Health.SetOnConnectCallback(
            [ this ]()
            {
                m_Health->AddOnHealthChangedCallback(
                    GetId(),
                    std::bind(
                        &EnemyBehavior::onDamageTaken,
                        this
                    )
                );
            }
        );

        m_Health.SetOnDisconnectCallback(
            [ this ]()
            {
                m_Health->RemoveOnHealthChangedCallback( GetId() );
            }
        );

        m_RigidBody  .Init( GetEntity() );
        m_Transform  .Init( GetEntity() );
        m_AudioPlayer.Init( GetEntity() );
        m_Health     .Init( GetEntity() );
    }

    /// @brief Removes this behavior from the behavior system on exit
    void EnemyBehavior::OnExit()
    {
        Behaviors< EnemyBehavior >()->RemoveComponent( this );

        m_RigidBody  .Exit();
        m_Transform  .Exit();
        m_AudioPlayer.Exit();
        m_Health     .Exit();
    }


    /// @brief Called at a fixed interval
    void EnemyBehavior::OnFixedUpdate()
    {
        if (
            m_Transform == nullptr ||
            m_RigidBody == nullptr 
        )
        {
            return;
        }

        if ( m_AudioPlayer != nullptr )
        {
            m_AudioPlayer->Play();
        }

        chaseTarget();
    }


//-----------------------------------------------------------------------------
// private: methods
//-----------------------------------------------------------------------------


    ///  @brief moves towards the target
    void EnemyBehavior::chaseTarget()
    {
        // accelerate along path
        glm::vec2 moveDir = Pathfinder()->GetDirectionAt(m_Transform->GetTranslation()); 
        m_RigidBody->ApplyAcceleration( moveDir * m_Speed );
    }

    /// @brief What to do when the enemy gets damaged.
    void EnemyBehavior::onDamageTaken()
    {
        if (m_Health->GetHealth()->GetCurrent() <= 0)
        {
            GetEntity()->Destroy();
        }
    }

    
//-----------------------------------------------------------------------------
// public: inspection
//-----------------------------------------------------------------------------


    /// @brief  inspector for this component
    void EnemyBehavior::Inspector()
    {
        if ( m_Transform == nullptr )
        {
            ImGui::Text( "WARNING: no Trasform attached" );
        }
        if ( m_RigidBody == nullptr )
        {
            ImGui::Text( "WARNING: no RigidBody attached" );
        }
        if ( m_AudioPlayer == nullptr )
        {
            ImGui::Text( "WARNING: no AudioPlayer attached" );
        }
        if ( m_Health == nullptr )
        {
            ImGui::Text( "WARNING: no Health attached" );
        }

        ImGui::DragFloat( "Speed", &m_Speed, 0.05f, 0.0f, INFINITY );

        ImGui::DragInt( "Damage", &m_Damage, 0.05f, 0, INT_MAX );
    }


//-----------------------------------------------------------------------------
// private: Reading
//-----------------------------------------------------------------------------

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


//-----------------------------------------------------------------------------
// public: reading / writing
//-----------------------------------------------------------------------------


    /// @brief  gets the map of read methods
    /// @return the map of read methods
    ReadMethodMap< ISerializable > const& EnemyBehavior::GetReadMethods() const
    {
        static ReadMethodMap< EnemyBehavior > const readMethods = {
            { "Speed"           , &EnemyBehavior::readSpeed            },
            { "Damage"          , &EnemyBehavior::readDamage           }
        };

        return (ReadMethodMap< ISerializable > const&)readMethods;
    }


    /// @brief  write all component data to a JSON object
    nlohmann::ordered_json EnemyBehavior::Write() const
    {
        nlohmann::ordered_json data;

        data[ "Speed"            ] = Stream::Write( m_Speed            );
        data[ "Damage"           ] = Stream::Write( m_Damage           );

        return data;
    }

    
//-----------------------------------------------------------------------------
// public: copying
//-----------------------------------------------------------------------------


    /// @brief  creates a new copy of this Component
    /// @return the newly created component
    EnemyBehavior* EnemyBehavior::Clone() const
    {
        return new EnemyBehavior( *this );
    }


//-----------------------------------------------------------------------------
// private: copying
//-----------------------------------------------------------------------------


    /// @brief  copy constructor
    /// @param  other   the other EnemyBehavior to copy
    EnemyBehavior::EnemyBehavior( EnemyBehavior const& other ) :
        Behavior( other ),
        m_Speed ( other.m_Speed  ),
        m_Damage( other.m_Damage )
    {}


//-----------------------------------------------------------------------------
