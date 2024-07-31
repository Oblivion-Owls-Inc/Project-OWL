///-----------------------------------------------------------------------------//
/// @file   PlayerController.cpp
/// @brief  PlayerController Behaviour Class
/// 
/// @author Aidan Straker (aidan.straker)
/// @date   October 2023
///
/// @copyright Copyright (c) 2024 Digipen Institute of Technology
///-----------------------------------------------------------------------------//

#include "pch.h" // precompiled header has to be included first
#include "PlayerController.h" 
#include "BehaviorSystem.h"     // GetInstance, AddBehavior, RemoveBehavior

#include "InputSystem.h"        // GetInstance, GetKeyDown
#include "CollisionSystem.h"    // GetInstance, RayCast
#include "Engine.h"             // GetInstance, GetFixedFrameDuration
#include "AnimationAsset.h"
#include "AssetLibrarySystem.h" // GetAsset
#include "DebugSystem.h"
#include "EnemyBehavior.h"
#include "EntitySystem.h"
#include "Generator.h"
#include "ItemStack.h"
#include "HomeBase.h"
#include "SceneTransition.h"
#include "EventSystem.h"
#include "Lifetime.h"


#include "ComponentReference.t.h"

#include "Inspection.h"

///----------------------------------------------------------------------------
/// Static Variables
///----------------------------------------------------------------------------


    // The amount of animations for the player character.


///----------------------------------------------------------------------------
/// Public: constructor / destructor
///----------------------------------------------------------------------------


    /// @brief Default Constructor.
    PlayerController::PlayerController() : 
        Behavior( typeid( PlayerController ) )
    {}


///----------------------------------------------------------------------------
/// Public: methods
///----------------------------------------------------------------------------

    /// @brief Adds this behavior to the behavior system on init.
    void PlayerController::OnInit()
    {
        Behaviors< PlayerController >()->AddComponent( this );


        m_Health.SetOnConnectCallback( [ this ]()
        {
            m_Health->AddOnHealthChangedCallback(
                GetId(),
                std::bind( &PlayerController::playerRespawn, this )
            );
        } );
        m_Health.SetOnDisconnectCallback( [ this ]()
        {
            m_Health->RemoveOnHealthChangedCallback( GetId() );
        } );

        m_Collider.SetOnConnectCallback( [ this ]()
        {
            m_Collider->AddOnCollisionEnterCallback(
                GetId(),
                std::bind( &PlayerController::onCollisionEnter, this, std::placeholders::_1 )
            );


            m_Collider->AddOnCollisionCallback(GetId(),
                [this](Collider* collider, CollisionData const& collisionData)
                {
                    if (collider->GetRigidBody() == nullptr && collider->GetStaticBody() == nullptr)
                    {
                        return;
                    }
                    const glm::vec2 VerticleDirection(0.0f, 1.0f);

                    float dotProduct = glm::dot(collisionData.normal, VerticleDirection);

                    if (dotProduct > m_GroundCollisionThreshold) // If the player is colliding with the ground
                    {
                        m_IsJumping = false;

                        m_CurrentCoyoteTime = 0.0f;
                    }
                }
            );
        } );
        m_Collider.SetOnDisconnectCallback( [ this ]()
        {
            m_Collider->RemoveOnCollisionEnterCallback( GetId() );
            m_Collider->RemoveOnCollisionCallback     ( GetId() );
        } );

        m_Transform.SetOnConnectCallback( [ this ]()
        {
            m_Transform->AddOnTransformChangedCallback( GetId(), [ this ]()
            {
                if ( m_MiningLaser != nullptr && m_MiningLaser->GetTransform() != nullptr )
                {
                    m_MiningLaser->GetTransform()->SetTranslation( glm::vec2(m_Transform->GetMatrix() * glm::vec4( m_MiningLaserOffset, 0.0f, 1.0f )) );
                }
            } );
        } );
        m_Transform.SetOnDisconnectCallback( [ this ]()
        {
            m_Transform->RemoveOnTransformChangedCallback( GetId() );
        } );

        m_RigidBody     .Init( GetEntity() );
        m_AudioPlayer   .Init( GetEntity() );
        m_Transform     .Init( GetEntity() );
        m_Health        .Init( GetEntity() );
        m_Collider      .Init( GetEntity() );
        m_Inventory     .Init( GetEntity() );
        m_EffectAnimator.Init( GetEntity() );
        m_Sprite        .Init(GetEntity());
        
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
        m_AimHorizontal .Init();
        m_AimVertical   .Init();
        m_Interact      .Init();

        /// Set the filter function for the listener
        m_ListenerBegin.SetFilterFunction([&](std::string const& EventNameBegin) -> bool
        {
            if (EventNameBegin == m_EventNameHide)
            {
                return true;
            }
            if (EventNameBegin == m_EventNameShow)
            {
                return true;
            }
            return EventNameBegin == m_EventNameBegin;
        });

        /// Set the Callback function for the listener
        m_ListenerBegin.SetResponseFunction([&](std::string const& EventNameBegin)
        {
            if (EventNameBegin == m_EventNameHide)
            {
                m_Sprite->SetOpacity(0.0f);
                return;
            }
            if (EventNameBegin == m_EventNameShow)
            {
                m_EffectAnimator->SetIsPlaying(false);
                return;
            }
            // do thing on start
            m_EffectAnimator->SetIsPlaying(false);
            m_RigidBody->ApplyVelocity(glm::vec2(1, 10));
            m_CanMove = true;
        });

        m_ListenerBegin.Init();
    }


    /// @brief Removes this behavior from the behavior system on exit
    void PlayerController::OnExit()
    {
        Behaviors< PlayerController >()->RemoveComponent( this );

        m_RigidBody     .Exit();
        m_AudioPlayer   .Exit();
        m_Transform     .Exit();
        m_Health        .Exit();
        m_Collider      .Exit();
        m_Inventory     .Exit();
        m_EffectAnimator.Exit();
        m_Sprite        .Exit();

        m_MiningLaserEntity.Exit();

        m_MoveHorizontal.Exit();
        m_MoveVertical  .Exit();
        m_FireLaser     .Exit();
        m_Interact      .Exit();
        m_AimHorizontal .Exit();
        m_AimVertical   .Exit();
        m_ListenerBegin .Exit();
    }

   

    /// @brief on fixed update check which input is being pressed.
    void PlayerController::OnFixedUpdate()
    {
        if ( m_RigidBody == nullptr || m_Transform == nullptr || !m_CanMove)
        {
            return;
        }

        // The normalised direction vector.
        glm::vec2 direction = { 0.0f, 0.0f };

        direction.x = m_MoveHorizontal != nullptr ? m_MoveHorizontal->GetAxis() : 0.0f;
        direction.y = m_MoveVertical   != nullptr ? m_MoveVertical  ->GetAxis() : 0.0f;

        if ( direction != glm::vec2( 0.0f ) )
        {
            if ( direction.x > 0 )
            {
                direction.x *= m_HorizontalMoveforce[ 0 ];

                m_Transform->SetScale( glm::vec2( -1.0f, 1.0f ) );
            }
            else if ( direction.x < 0 )
            {
                direction.x *= m_HorizontalMoveforce[ 1 ];

                m_Transform->SetScale( glm::vec2( 1.0f, 1.0f ) );
            }

            if ( direction.y > 0 )
            {
                direction.y *= m_VerticalMoveforce[ 0 ];

                if ( m_IsJumping == false || m_CurrentCoyoteTime <= m_MaxCoyoteTime )
                {
                    m_RigidBody->ApplyVelocity( glm::vec2( 0, m_JumpSpeed ) );
                    m_IsJumping = true;
                    m_CurrentCoyoteTime = m_MaxCoyoteTime + 1; // Make sure that the player can't jump again
                    // until they hit the ground.
                }
            }
            else
            {
                direction.y *= m_VerticalMoveforce[ 1 ];
            }

            m_RigidBody->ApplyAcceleration( direction );

            if ( m_AudioPlayer != nullptr )
            {
                m_AudioPlayer->Play();
            }
        }
        else
        {
            if ( m_AudioPlayer != nullptr )
            {
                m_AudioPlayer->Stop();
            }
        }

        updateMiningLaser();
    }


//-----------------------------------------------------------------------------
// private: methods
//-----------------------------------------------------------------------------


    /// @brief  updates the mining laser
    void PlayerController::updateMiningLaser()
    {
        if ( m_Transform == nullptr || m_MiningLaser == nullptr || m_MiningLaser->GetTransform() == nullptr )
        {
            return;
        }

        if ( m_FireLaser == nullptr || m_FireLaser->GetDown() == false )
        {
            m_MiningLaser->SetIsFiring( false );
            return;
        }

        // get direction from player to aim laser
        glm::vec2 direction = { 1.0f, 0.0f };
        if ( m_AimHorizontal != nullptr && m_AimVertical != nullptr && Input()->IsControllerMostRecentInput() )
        {
            direction.x = m_AimHorizontal->GetAxis();
            direction.y = m_AimVertical  ->GetAxis();
        }
        else
        {
            direction = Input()->GetMousePosWorld() - m_Transform->GetTranslation();
        }


        if ( direction.x > 0 )
        {
            m_Transform->SetScale( glm::vec2( -1.0f, 1.0f ) );
        }
        else if ( direction.x < 0 )
        {
            m_Transform->SetScale( glm::vec2( 1.0f, 1.0f ) );
        }

        // adjust direction to mouse to include laser offset
        if ( m_AimHorizontal == nullptr || m_AimVertical == nullptr || Input()->IsControllerMostRecentInput() == false )
        {
            direction = Input()->GetMousePosWorld() - glm::vec2(m_Transform->GetMatrix() * glm::vec4( m_MiningLaserOffset, 0.0f, 1.0f ));
        }

        m_MiningLaser->GetTransform()->SetRotation( std::atan2( direction.y, direction.x ) );

        m_MiningLaser->SetIsFiring( true );
    }


    /// @brief Check if player heal is 0, then respawn them. 
    void PlayerController::playerRespawn()
    {
        // If the player is dead
        if ( m_Health->GetHealth()->GetCurrent() <= 0 )
        {
            m_Transform->SetTranslation(m_PlayerRespawnLocation);
            m_Health->GetHealth()->Reset();
        }
    }


    /// @brief  What to do when the player has been hit.
    /// @param  other   - the collider of the other entity.
    void PlayerController::onCollisionEnter( Collider* other )
    {
        HomeBase* base = other->GetEntity()->GetComponent<HomeBase>();
        if (base)
        {
            if (base->CanWin())
            {
                Events()->BroadcastEvent< std::string >(m_EventNameWin);
                Debug() << "Event Emitted: " << m_EventNameWin << std::endl;
                base->GetEntity()->GetComponent<Lifetime>()->GetLifetime()->SetCurrent(1.0);
                base->PlayWinSound();
                m_Health->Reset();
            }
            return;
        }
        // Get the enemy behaviour component.
        EnemyBehavior* enemy = other->GetEntity()->GetComponent< EnemyBehavior >();
        if ( enemy == nullptr )
        {
            return;
        }
        if(m_Health)
        {
            // If the enemy collides with player, damage the player
            m_Health->TakeDamage( enemy->GetDamage() );
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

        ImGui::DragFloat( "Jump Force"                , &m_JumpSpeed               , 0.05f,  0.0f, INFINITY );
        ImGui::DragFloat( "Ground Collision Threshold", &m_GroundCollisionThreshold, 0.05f, -1.0f, 1.0f     );
        ImGui::DragFloat( "Max Coyote Time"           , &m_MaxCoyoteTime           , 0.05f,  0.0f, INFINITY );

        ImGui::DragFloat2( "mining laser offset", &m_MiningLaserOffset[ 0 ], 0.05f );

        m_MiningLaserEntity.Inspect( "Mining Laser Entity" );

        m_MoveVertical  .Inspect( "Vertical Control Action"   );
        m_MoveHorizontal.Inspect( "Horizontal Control Action" );
        m_FireLaser     .Inspect( "Fire Laser Control Action" );
        m_Interact      .Inspect( "Interact Control Action"   );
        m_AimHorizontal .Inspect( "Horizontal Aim Action"     );
        m_AimVertical   .Inspect( "Vertical Aim Action"       );
        ImGui::InputText("Event Name Begin", &m_EventNameBegin);
        ImGui::InputText("Event Name Win",   &m_EventNameWin);
        ImGui::InputText("Event Name Hide", &m_EventNameHide);
        ImGui::InputText("Event Name Show", &m_EventNameShow);
    }


//-----------------------------------------------------------------------------
// private: inspection
//-----------------------------------------------------------------------------


    /// @brief Helper function for inspector.
    void PlayerController::vectorInspector()
    {
        ImGui::DragFloat2( "Vertical Moveforce"  , &m_VerticalMoveforce  [ 0 ], 0.05f );
        ImGui::DragFloat2( "Horizontal Moveforce", &m_HorizontalMoveforce[ 0 ], 0.05f );

        // Change the respawn location in the editor.
        ImGui::DragFloat2( "Respawn Location", &m_PlayerRespawnLocation[ 0 ], 0.05f );
    }


//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------


    /// @brief Read in the amount of force to apply to the player when moving vertically.
    /// @param data - the JSON file to read from.
    void PlayerController::readVerticalMoveForce(nlohmann::ordered_json const& data)
    {
        m_VerticalMoveforce = Stream::Read<2, float>(data);
    }

    /// @brief Read in the amount of force to apply to the player when moving horizontally.
    /// @param data - the JSON file to read from.
    void PlayerController::readHorizontalMoveForce(nlohmann::ordered_json const& data)
    {
        m_HorizontalMoveforce = Stream::Read<2, float>(data);
    }


    /// @brief Read in the respawn location for the player.
    /// @param data - the JSON file to read from.
    void PlayerController::readRespawnLocation(nlohmann::ordered_json const& data)
    {
        m_PlayerRespawnLocation = Stream::Read<2, float>(data);
    }


    /// @brief  reads the JumpSpeed
    /// @param  data    the JSON data to read from
    void PlayerController::readJumpSpeed(nlohmann::ordered_json const& data)
    {
        Stream::Read(m_JumpSpeed, data);
    }

    /// @brief Read in the is jumping state.
    /// @param data - the JSON file to read from.
    void PlayerController::readIsJumping(nlohmann::ordered_json const& data)
    {
        Stream::Read(m_IsJumping, data);
    }


    /// @brief Read in the ground collision threshold.
    /// @param data - the JSON file to read from.
    void PlayerController::readGroundCollisionThreshold(nlohmann::ordered_json const& data)
    {
        Stream::Read(m_GroundCollisionThreshold, data);
    }

    /// @brief Read in the max coyote time.
    /// @param data - the JSON file to read from.
    void PlayerController::readMaxCoyoteTime(nlohmann::ordered_json const& data)
    {
        Stream::Read(m_MaxCoyoteTime, data);
    }


    /// @brief  reads the offset from the player of the mining laser's position (when facing left)
    /// @param  data    the JSON data to read from
    void PlayerController::readMiningLaserOffset( nlohmann::ordered_json const& data )
    {
        Stream::Read( &m_MiningLaserOffset, data );
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
    void PlayerController::readAimVertical(nlohmann::ordered_json const& data)
    {
        Stream::Read(m_AimVertical, data);
    }

    /// @brief  reads the EventNameBegin from a JSON file
    /// @param data    the JSON file to read from
    void PlayerController::readEventNameBegin(nlohmann::ordered_json const& data)
    {
        Stream::Read(m_EventNameBegin, data);
    }

    /// @brief  reads the EventNameWin from a JSON file
    /// @param data    the JSON file to read from
    void PlayerController::readEventNameWin(nlohmann::ordered_json const& data)
    {
        Stream::Read(m_EventNameWin, data);
    }

    /// @brief  reads the EventNameHide from a JSON file
    /// @param data    the JSON file to read from
    void PlayerController::readEventNameHide(nlohmann::ordered_json const& data)
    {
        Stream::Read(m_EventNameHide, data);
    }

    /// @brief  reads the EventNameShow from a JSON file
    /// @param data    the JSON file to read from
    void PlayerController::readEventNameShow(nlohmann::ordered_json const& data)
    {
        Stream::Read(m_EventNameShow, data);
    }

//-----------------------------------------------------------------------------
// public: reading writing
//-----------------------------------------------------------------------------


    /// @brief gets the map of read methods for this Component
    /// @return the map of read methods for this Component
    ReadMethodMap< ISerializable > const& PlayerController::GetReadMethods() const
    {
        static ReadMethodMap< PlayerController > const readMethods = {
            { "VerticalMoveforce"       , &PlayerController::readVerticalMoveForce        },
            { "HorizontalMoveforce"     , &PlayerController::readHorizontalMoveForce      },
            { "RespawnLocation"         , &PlayerController::readRespawnLocation          },
            { "JumpSpeed"               , &PlayerController::readJumpSpeed                },
            { "IsJumping"               , &PlayerController::readIsJumping                },
            { "GroundCollisionThreshold", &PlayerController::readGroundCollisionThreshold },
            { "MaxCoyoteTime"           , &PlayerController::readMaxCoyoteTime            },
            { "MiningLaserOffset"       , &PlayerController::readMiningLaserOffset        },
            { "MiningLaserEntity"       , &PlayerController::readMiningLaserEntity        },
            { "MoveVertical"            , &PlayerController::readMoveVertical             },
            { "MoveHorizontal"          , &PlayerController::readMoveHorizontal           },
            { "FireLaser"               , &PlayerController::readFireLaser                },
            { "Interact"                , &PlayerController::readInteract                 },
            { "AimVertical"             , &PlayerController::readAimVertical              },
            { "AimHorizontal"           , &PlayerController::readAimHorizontal            },
            { "EventNameBegin"          , &PlayerController::readEventNameBegin           },
            { "EventNameWin"            , &PlayerController::readEventNameWin             },
            { "EventNameHide"           , &PlayerController::readEventNameHide            },
            { "EventNameShow"           , &PlayerController::readEventNameShow            }
        };

        return (ReadMethodMap< ISerializable > const&)readMethods;
    }



    /// @brief  Write all PlayerController data to a JSON file.
    /// @return The JSON file containing the TurretBehavior data.
    nlohmann::ordered_json PlayerController::Write() const
    {
        nlohmann::ordered_json data;


        data[ "MiningLaserEntity"        ] = Stream::Write( m_MiningLaserEntity        );
        data[ "RespawnLocation"          ] = Stream::Write( m_PlayerRespawnLocation    );
        data[ "MoveVertical"             ] = Stream::Write( m_MoveVertical             );
        data[ "MoveHorizontal"           ] = Stream::Write( m_MoveHorizontal           );
        data[ "FireLaser"                ] = Stream::Write( m_FireLaser                );
        data[ "Interact"                 ] = Stream::Write( m_Interact                 );
        data[ "VerticalMoveforce"        ] = Stream::Write( m_VerticalMoveforce        );
        data[ "HorizontalMoveforce"      ] = Stream::Write( m_HorizontalMoveforce      );
        data[ "JumpSpeed"                ] = Stream::Write( m_JumpSpeed                );
        data[ "IsJumping"                ] = Stream::Write( m_IsJumping                );
        data[ "GroundCollisionThreshold" ] = Stream::Write( m_GroundCollisionThreshold );
        data[ "CoyoteTime"               ] = Stream::Write( m_MaxCoyoteTime            );
        data[ "MiningLaserOffset"        ] = Stream::Write( m_MiningLaserOffset        );
        data[ "AimVertical"              ] = Stream::Write( m_AimVertical              );
        data[ "AimHorizontal"            ] = Stream::Write( m_AimHorizontal            );
        data[ "EventNameBegin"           ] = Stream::Write( m_EventNameBegin           );
        data[ "EventNameWin"             ] = Stream::Write( m_EventNameWin             );
        data[ "EventNameHide"            ] = Stream::Write( m_EventNameHide            );
        data[ "EventNameShow"            ] = Stream::Write( m_EventNameShow            );

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
        m_VerticalMoveforce       ( other.m_VerticalMoveforce        ),
        m_HorizontalMoveforce     ( other.m_HorizontalMoveforce      ),
        m_PlayerRespawnLocation   ( other.m_PlayerRespawnLocation    ),
        m_JumpSpeed               ( other.m_JumpSpeed                ),
        m_IsJumping               ( other.m_IsJumping                ),
        m_GroundCollisionThreshold( other.m_GroundCollisionThreshold ),
        m_MaxCoyoteTime           ( other.m_MaxCoyoteTime            ),
        m_EventNameBegin          ( other.m_EventNameBegin           ),
        m_EventNameWin            ( other.m_EventNameWin             ),
        m_EventNameHide           ( other.m_EventNameHide            ),
        m_EventNameShow           ( other.m_EventNameShow            ),
        m_MiningLaserOffset       ( other.m_MiningLaserOffset        ),
        m_MoveVertical            ( other.m_MoveVertical             ),
        m_MoveHorizontal          ( other.m_MoveHorizontal           ),
        m_FireLaser               ( other.m_FireLaser                ),
        m_Interact                ( other.m_Interact                 ),
        m_AimVertical             ( other.m_AimVertical              ),
        m_AimHorizontal           ( other.m_AimHorizontal            ),
        m_MiningLaserEntity       ( other.m_MiningLaserEntity, { &m_MiningLaser } )
    {}


//--------------------------------------------------------------------------------    