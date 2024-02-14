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
#include "Health.h"

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


//-----------------------------------------------------------------------------
// private: virtual override methods
//-----------------------------------------------------------------------------


    /// @brief initializes the component
    void EnemyBehavior::OnInit()
    {
        Behaviors< Behavior >()->AddComponent(this);

        m_Health.SetOnConnectCallback(
            [ this ]( Health* health )
            {
                health->AddOnHealthChangedCallback(
                    GetId(),
                    std::bind(
                        &EnemyBehavior::onDamageTaken,
                        this
                    )
                );
            }
        );
        m_Health.SetOnDisconnectCallback(
            [ this ]( Health* health )
            {
                health->RemoveOnHealthChangedCallback( GetId() );
            }
        );

        m_RigidBody  .Init( GetEntity() );
        m_Transform  .Init( GetEntity() );
        m_AudioPlayer.Init( GetEntity() );
        m_Health     .Init( GetEntity() );

        m_PathfinderEntity.SetOwnerName( GetName() );
        m_PathfinderEntity.Init();
    }

    /// @brief Removes this behavior from the behavior system on exit
    void EnemyBehavior::OnExit()
    {
        Behaviors<Behavior>()->RemoveComponent(this);

        m_RigidBody  .Exit( GetEntity() );
        m_Transform  .Exit( GetEntity() );
        m_AudioPlayer.Exit( GetEntity() );
        m_Health     .Exit( GetEntity() );

        m_PathfinderEntity.Exit();
    }


    /// @brief Called at a fixed interval
    void EnemyBehavior::OnFixedUpdate()
    {
        if (
            m_Transform == nullptr ||
            m_RigidBody == nullptr ||
            m_Pathfinder == nullptr
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
        glm::vec2 moveDir = m_Pathfinder->GetDirectionAt( m_Transform->GetTranslation() );
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

        m_PathfinderEntity.Inspect( "pathfinder entity" );
    }


//-----------------------------------------------------------------------------
// private: Reading
//-----------------------------------------------------------------------------

    /// @brief  reads the name of the pathfinder entity
    /// @param  data    the json data to read from
    void EnemyBehavior::readPathfinderEntity( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_PathfinderEntity, data );
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


//-----------------------------------------------------------------------------
// public: reading / writing
//-----------------------------------------------------------------------------


    /// @brief  gets the map of read methods
    /// @return the map of read methods
    ReadMethodMap< ISerializable > const& EnemyBehavior::GetReadMethods() const
    {
        static ReadMethodMap< EnemyBehavior > const readMethods = {
            { "PathfinderEntity", &EnemyBehavior::readPathfinderEntity },
            { "Speed"           , &EnemyBehavior::readSpeed            },
            { "Damage"          , &EnemyBehavior::readDamage           }
        };

        return (ReadMethodMap< ISerializable > const&)readMethods;
    }


    /// @brief  write all component data to a JSON object
    nlohmann::ordered_json EnemyBehavior::Write() const
    {
        nlohmann::ordered_json data;

        data[ "PathfinderEntity" ] = Stream::Write( m_PathfinderEntity );
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
        m_Damage( other.m_Damage ),

        m_PathfinderEntity( other.m_PathfinderEntity, { &m_Pathfinder } )
    {}


//-----------------------------------------------------------------------------
