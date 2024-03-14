///-----------------------------------------------------------------------------//
/// @file   PlayerController.cpp
/// @brief  PlayerController Behaviour Class
/// 
/// @author Aidan Straker (aidan.straker)
/// @date   October 2023
///
/// @copyright (c) 2023 DigiPen (USA) Corporation.
///-----------------------------------------------------------------------------//

#include "pch.h" // precompiled header has to be included first
#include "PlayerController.h" 
#include "BehaviorSystem.h"     // GetInstance, AddBehavior, RemoveBehavior

#include "InputSystem.h"        // GetInstance, GetKeyDown
#include "AnimationAsset.h"
#include "AssetLibrarySystem.h" // GetAsset
#include "DebugSystem.h"
#include "EnemyBehavior.h"
#include "EntitySystem.h"
#include "Generator.h"

#include "ComponentReference.t.h"

#include "Inspection.h"

///----------------------------------------------------------------------------
/// Static Variables
///----------------------------------------------------------------------------


    // The amount of animations for the player character.
    #define NUM_ANIMATIONS 4


///----------------------------------------------------------------------------
/// Public: constructor / destructor
///----------------------------------------------------------------------------


    /// @brief Default Constructor.
    PlayerController::PlayerController() : 
        Behavior( typeid( PlayerController ) )
    {}


//-----------------------------------------------------------------------------
// public: accessors
//-----------------------------------------------------------------------------


    /// @brief  Gets the Health Component attached to this Entity.
    /// @return The Health Component attached to this Entity.
    Health* PlayerController::GetHealth() { return m_Health; }

    /// @brief  Gets the MiningLaser component attached to this Entity.
    /// @return The MiningLaser component attached to this Entity.
    MiningLaser* PlayerController::GetMiningLaser() { return m_MiningLaser; }


///----------------------------------------------------------------------------
/// Public: methods
///----------------------------------------------------------------------------

    /// @brief Adds this behavior to the behavior system on init.
    void PlayerController::OnInit()
    {
	    Behaviors< PlayerController >()->AddComponent( this );


        m_Health.SetOnConnectCallback(
            [ this ]()
            {
                m_Health->AddOnHealthChangedCallback(
                    GetId(),
                    std::bind( &PlayerController::playerRespawn, this )
                );
            }
        );
        m_Health.SetOnDisconnectCallback(
            [ this ]()
            {
                m_Health->RemoveOnHealthChangedCallback( GetId() );
            }
        );

        m_Collider.SetOnConnectCallback(
            [ this ]()
            {
                m_Collider->AddOnCollisionEnterCallback(
                    GetId(),
                    std::bind( &PlayerController::onCollisionEnter, this, std::placeholders::_1 )
                );
            }
        );
        m_Collider.SetOnDisconnectCallback(
            [ this ]()
            {
                m_Collider->RemoveOnCollisionEnterCallback( GetId() );
            }
        );
    
        m_RigidBody  .Init( GetEntity() );
        m_Animation  .Init( GetEntity() );
        m_AudioPlayer.Init( GetEntity() );
        m_Transform  .Init( GetEntity() );
        m_Health     .Init( GetEntity() );
        m_Collider   .Init( GetEntity() );

        m_MiningLaserEntity.SetOwnerName( GetName() );
        m_MiningLaserEntity.Init();



        m_MoveHorizontal.SetOwnerName( GetName() );
        m_MoveVertical  .SetOwnerName( GetName() );
        m_FireLaser     .SetOwnerName( GetName() );
        m_Interact      .SetOwnerName( GetName() );
        m_AimHorizontal .SetOwnerName( GetName() );
        m_AimVertical   .SetOwnerName( GetName() );
        m_MoveHorizontal.Init();
        m_MoveVertical  .Init();
        m_FireLaser     .Init();
        m_Interact      .Init();
        m_AimHorizontal .Init();
        m_AimVertical   .Init();

        for ( AssetReference< AnimationAsset >& assetReference : m_Animations )
        {
            assetReference.SetOwnerName( GetName() );
            assetReference.Init();
        }
    }


    /// @brief Removes this behavior from the behavior system on exit
    void PlayerController::OnExit()
    {
        Behaviors<PlayerController>()->RemoveComponent(this);

        m_RigidBody  .Exit();
        m_Animation  .Exit();
        m_AudioPlayer.Exit();
        m_Transform  .Exit();
        m_Health     .Exit();
        m_Collider   .Exit();

        m_MiningLaserEntity.Exit();

        m_MoveHorizontal.Exit();
        m_MoveVertical  .Exit();
        m_FireLaser     .Exit();
        m_Interact      .Exit();
        m_AimHorizontal .Exit();
        m_AimVertical   .Exit();
    }


    /// @brief on fixed update check which input is being pressed.
    void PlayerController::OnFixedUpdate()
    {
        if (
            m_Animation == nullptr ||
            m_AudioPlayer == nullptr ||
            m_RigidBody == nullptr
        )
        {
            return;
        }


        if ( m_Interact != nullptr && m_Interact->GetDown() )
        {
            for (auto& generator : Behaviors<Generator>()->GetComponents())
            {
                float distance = glm::distance<>(generator->GetTransform()->GetTranslation(),
                    GetEntity()->GetComponent<Transform>()->GetTranslation());
                if (generator->GetActivationRadius() > distance)
                {
                    generator->Activate();
                    return;
                }
            }
        }

        // The normalised direction vector.
        glm::vec2 direction = { 0.0f, 0.0f };

        direction.x = m_MoveHorizontal != nullptr ? m_MoveHorizontal->GetAxis() : 0.0f;
        direction.y = m_MoveVertical != nullptr ? m_MoveVertical->GetAxis() : 0.0f;
    

        if ( direction != glm::vec2( 0 ) )
        {
            direction = glm::normalize( direction );

            if ( std::abs( direction.x ) >= std::abs( direction.y ) )
            {
                if ( direction.x > 0 )
                {
                    // 0 is right
                    m_Animation->SetAsset( m_Animations[ 0 ] );
                }
                else
                {
                    // 1 is left
                    m_Animation->SetAsset( m_Animations[ 1 ] );
                }
            }
            else
            {
                if ( direction.y > 0 )
                {
                    // 2 is up
                    m_Animation->SetAsset( m_Animations[ 2 ] );
                }
                else
                {
                    // 3 is down
                    m_Animation->SetAsset( m_Animations[ 3 ] );
                }
            }

            m_AudioPlayer->Play();
            m_Animation->SetIsRunning( true );
        }
        else
        {
            m_Animation->SetIsRunning( false );
            m_Animation->SetFrameIndex( 0, true );
            m_AudioPlayer->Stop();
        }

        m_RigidBody->ApplyVelocity( direction * m_MaxSpeed );


        updateMiningLaser();
    }


//-----------------------------------------------------------------------------
// private: methods
//-----------------------------------------------------------------------------


    /// @brief  updates the mining laser
    void PlayerController::updateMiningLaser()
    {
        if ( m_MiningLaser == nullptr )
        {
            return;
        }

        m_MiningLaser->GetTransform()->SetTranslation( m_Transform->GetTranslation() );

        if ( m_FireLaser != nullptr && m_FireLaser->GetDown() )
        {
            m_MiningLaser->SetIsFiring( true );

            glm::vec2 direction;
            if ( Input()->IsControllerMostRecentInput() && m_AimHorizontal != nullptr && m_AimVertical != nullptr )
            {
                // Get the data from the right thumbstick.
                direction.x = m_AimHorizontal->GetAxis();
                direction.y = m_AimVertical  ->GetAxis();
            }
            else
            {
                direction = Input()->GetMousePosWorld() - m_Transform->GetTranslation();
            }

            m_MiningLaser->SetDirection(
                direction == glm::vec2( 0.0f ) ?
                    glm::vec2( 1.0f, 0.0f ) :
                    glm::normalize( direction )
            );
        }
        else
        {
            m_MiningLaser->SetIsFiring( false );
        }

    }


    /// @brief Check if player heal is 0, then respawn them. 
    void PlayerController::playerRespawn()
    {
        // If the player is dead
        if (m_Health->GetHealth()->GetCurrent() <= 0)
        {
            m_Transform->SetTranslation(m_PlayerRespawnLocation);
            m_Health->GetHealth()->Reset();
        }
    }


    /// @brief  What to do when the player has been hit.
    /// @param  other   - the collider of the other entity.
    void PlayerController::onCollisionEnter( Collider* other )
    {
        // Get the enemy behaviour component.
        EnemyBehavior* enemy = other->GetEntity()->GetComponent<EnemyBehavior>();
        if (!enemy)
        {
            return;
        }

        if(m_Health)
        {
            // If the enemy collides with player, damage the player
            m_Health->TakeDamage(enemy->GetDamage());
        }

        // Add physical reaction to enemy.
    }


//-----------------------------------------------------------------------------
// public: inspection
//-----------------------------------------------------------------------------


    /// @brief Used by the Debug System to display information about this Component.
    void PlayerController::Inspector()
    {
        vectorInspector();
        animationInspector();

        m_MiningLaserEntity.Inspect( "Mining Laser Entity" );

        m_MoveHorizontal.Inspect( "Horizontal Control Action" );
        m_MoveVertical  .Inspect( "Vertical Control Action"   );
        m_FireLaser     .Inspect( "Fire Laser Control Action" );
        m_Interact      .Inspect( "Interact Control Action"   );
        m_AimHorizontal .Inspect( "Horizontal Aim Action"     );
        m_AimVertical   .Inspect( "Vertical Aim Action"       );
    }


//-----------------------------------------------------------------------------
// private: inspection
//-----------------------------------------------------------------------------


    /// @brief Helper function for inspector.
    void PlayerController::vectorInspector()
    {
        ImGui::DragFloat( "Max Speed", &m_MaxSpeed, 0.05f, 0.0f, INFINITY );

        // Change the respawn location in the editor.
        ImGui::DragFloat2( "Respawn Location", &m_PlayerRespawnLocation[ 0 ], 0.05f );
    }

    /// @brief Helper function for inspector.
    void PlayerController::animationInspector()
    {
        std::string animNames[ NUM_ANIMATIONS ] = { "Right Animation", "Left Animation", "Up Animation", "Down Animation" };

        for ( int i = 0; i < NUM_ANIMATIONS; i++ )
        {
            m_Animations[ i ].Inspect( animNames[ i ].c_str() );
        }
    }


//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------


    /// @brief Read in the max speed for the player.
    /// @param data The JSON file to read from.
    void PlayerController::readMaxSpeed(nlohmann::ordered_json const& data)
    {
        Stream::Read( m_MaxSpeed, data );
    }

    /// @brief Read in the respawn location for the player.
    /// @param data - the JSON file to read from.
    void PlayerController::readRespawnLocation(nlohmann::ordered_json const& data)
    {
        m_PlayerRespawnLocation = Stream::Read<2, float>(data);
    }

    /// @brief Read in the names of the player animations.
    /// @param data The JSON file to read from.
    void PlayerController::readAnimations(nlohmann::ordered_json const& data)
    {
        for ( int i = 0; i < NUM_ANIMATIONS; ++i )
        {
            Stream::Read( m_Animations[ i ], data[ i ] );
        }
    }

    /// @brief  reads the name of the MiningLaser entity this PlayerController uses
    /// @param  data    the JSON data to read from
    void PlayerController::readMiningLaserEntity( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_MiningLaserEntity, data );
    }


    /// @brief  the control Action used for vertical movement
    /// @param  data    the JSON data to read from
    void PlayerController::readMoveVertical( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_MoveVertical, data );
    }

    /// @brief  the control Action used for vertical movement
    /// @param  data    the JSON data to read from
    void PlayerController::readMoveHorizontal( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_MoveHorizontal, data );
    }

    /// @brief  the control Action to fire the laser
    /// @param  data    the JSON data to read from
    void PlayerController::readFireLaser( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_FireLaser, data );
    }

    /// @brief  the control Action to interact with something
    /// @param  data    the JSON data to read from
    void PlayerController::readInteract( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_Interact, data );
    }

    /// @brief  reads the control action for horizontal aim
    /// @param  data    the JSON data to read from
    void PlayerController::readAimHorizontal( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_AimHorizontal, data );
    }

    /// @brief  reads the control action for vertical aim
    /// @param  data    the JSON data to read from
    void PlayerController::readAimVertical( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_AimVertical, data );
    }


//-----------------------------------------------------------------------------
// public: reading writing
//-----------------------------------------------------------------------------


    /// @brief gets the map of read methods for this Component
    /// @return the map of read methods for this Component
    ReadMethodMap< ISerializable > const& PlayerController::GetReadMethods() const
    {
        static ReadMethodMap< PlayerController > const readMethods = {
            { "MaxSpeed"         , &PlayerController::readMaxSpeed          },
            { "RespawnLocation"  , &PlayerController::readRespawnLocation   },
            { "Animations"       , &PlayerController::readAnimations        },
            { "MiningLaserEntity", &PlayerController::readMiningLaserEntity },
            { "MoveVertical"     , &PlayerController::readMoveVertical      },
            { "MoveHorizontal"   , &PlayerController::readMoveHorizontal    },
            { "FireLaser"        , &PlayerController::readFireLaser         },
            { "Interact"         , &PlayerController::readInteract          },
            { "AimVertical"      , &PlayerController::readAimVertical       },
            { "AimHorizontal"    , &PlayerController::readAimHorizontal     }
        };

        return (ReadMethodMap< ISerializable > const&)readMethods;
    }



    /// @brief  Write all PlayerController data to a JSON file.
    /// @return The JSON file containing the TurretBehavior data.
    nlohmann::ordered_json PlayerController::Write() const
    {
        nlohmann::ordered_json data;

        nlohmann::ordered_json& animationNames = data[ "Animations" ];
        for ( AssetReference< AnimationAsset > const& animation : m_Animations )
        {
            animationNames.push_back( Stream::Write( animation ) );
        }

        data[ "MaxSpeed"          ] = Stream::Write( m_MaxSpeed              );
        data[ "MiningLaserEntity" ] = Stream::Write( m_MiningLaserEntity     );
        data[ "RespawnLocation"   ] = Stream::Write( m_PlayerRespawnLocation );
        data[ "MoveVertical"      ] = Stream::Write( m_MoveVertical          );
        data[ "MoveHorizontal"    ] = Stream::Write( m_MoveHorizontal        );
        data[ "FireLaser"         ] = Stream::Write( m_FireLaser             );
        data[ "Interact"          ] = Stream::Write( m_Interact              );
        data[ "AimVertical"       ] = Stream::Write( m_AimVertical           );
        data[ "AimHorizontal"     ] = Stream::Write( m_AimHorizontal         );

        return data;
    }


//--------------------------------------------------------------------------------
// public: copying
//--------------------------------------------------------------------------------


    /// @brief  Clones the current PlayerController and returns a copy.
    /// @return A copy of the current PlayerController.
    PlayerController* PlayerController::Clone() const
    {
        return new PlayerController( *this );
    }


//--------------------------------------------------------------------------------
// private: copying
//--------------------------------------------------------------------------------


    /// @brief Copy Constructor
    /// @param other A PlayerController to copy.
    PlayerController::PlayerController(PlayerController const& other):
        Behavior( other ),
        m_MaxSpeed             ( other.m_MaxSpeed              ),
        m_PlayerRespawnLocation( other.m_PlayerRespawnLocation ),
        m_MoveVertical         ( other.m_MoveVertical          ),
        m_MoveHorizontal       ( other.m_MoveHorizontal        ),
        m_FireLaser            ( other.m_FireLaser             ),
        m_Interact             ( other.m_Interact              ),
        m_AimVertical          ( other.m_AimVertical           ),
        m_AimHorizontal        ( other.m_AimHorizontal         )
    {
        // Copy the animations
        for (int i = 0; i < NUM_ANIMATIONS; i++)
        {
            m_Animations[ i ] = other.m_Animations[ i ];
        }
    }


//--------------------------------------------------------------------------------    
