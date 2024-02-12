///-----------------------------------------------------------------------------//
/// @file   PlayerController.cpp
/// @brief  PlayerController Behaviour Class
/// 
/// @author Aidan Straker (aidan.straker)
/// @date   October 2023
///
/// @copyright (c) 2023 DigiPen (USA) Corporation.
///-----------------------------------------------------------------------------//
/// 
#include "PlayerController.h" 
#include "BehaviorSystem.h"     // GetInstance, AddBehavior, RemoveBehavior

#include "RigidBody.h"          // ApplyVelocity
#include "Animation.h"          // SetAsset
#include "AudioPlayer.h"
#include "Transform.h"
#include "MiningLaser.h"
#include "ConstructionBehavior.h"

#include "InputSystem.h"        // GetInstance, GetKeyDown
#include "AnimationAsset.h"
#include "AssetLibrarySystem.h" // GetAsset
#include "DebugSystem.h"
#include "EnemyBehavior.h"
#include "Health.h"              // TakeDamage
#include "Transform.h"
#include "CircleCollider.h"
#include "EntitySystem.h"
#include "glm/glm.hpp"
#include "GeneratorBehavior.h"

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


///----------------------------------------------------------------------------
/// Public: methods
///----------------------------------------------------------------------------

/// @brief Adds this behavior to the behavior system on init.
void PlayerController::OnInit()
{
	Behaviors< Behavior >()->AddComponent( this );
    
    m_RigidBody  .Init( GetEntity() );
    m_Animation  .Init( GetEntity() );
    m_AudioPlayer.Init( GetEntity() );
    m_Transform  .Init( GetEntity() );
    m_Health     .Init( GetEntity() );

    m_MiningLaserEntity.Init();


    GetEntity()->GetComponent< CircleCollider >()->AddOnCollisionEnterCallback(
        GetId(),
        std::bind( &PlayerController::onCollisionEnter, this, std::placeholders::_1 )
    );

    // Get all the player's animations
    for ( int i = 0; i < NUM_ANIMATIONS; ++i )
    {
        m_PlayerAnimations[ i ] = AssetLibrary< AnimationAsset >()->GetAsset( m_AnimationNames[ i ] );
    }


    

    // Set the callback for when the player takes damage.
    if(m_Health)
    {
        m_Health->AddOnHealthChangedCallback(
            GetId(),
            std::bind(
                &PlayerController::playerRespawn,
                this
            )
        );
    }
}


/// @brief Removes this behavior from the behavior system on exit
void PlayerController::OnExit()
{
    Behaviors<Behavior>()->RemoveComponent(this);

    if (m_Health != nullptr)
    {
        m_Health->RemoveOnHealthChangedCallback(GetId());
    }


    m_RigidBody  .Exit( GetEntity() );
    m_Animation  .Exit( GetEntity() );
    m_AudioPlayer.Exit( GetEntity() );
    m_Transform  .Exit( GetEntity() );
    m_Health     .Exit( GetEntity() );

    m_MiningLaserEntity.Exit();
}

/// @brief Used by the Debug System to display information about this Component.
void PlayerController::Inspector()
{
    vectorInspector();
    animationInspector();

    m_MiningLaserEntity.Inspect( "Mining Laser Entity" );
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


    if (Input()->GetKeyDown(GLFW_KEY_E))
    {
        for (auto& generator : Behaviors<GeneratorBehavior>()->GetComponents())
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
    
    if ( moveRight() )
    {
        // 0 is right.
        m_Animation->SetAsset( m_PlayerAnimations[ 0 ] );
        m_Animation->SetIsRunning( true );
        direction.x += 1.0f;
        m_AudioPlayer->Play();
    }
    if ( moveLeft() )
    {
        // 1 is left
        m_Animation->SetAsset( m_PlayerAnimations[ 1 ] );
        m_Animation->SetIsRunning( true );
        direction.x -= 1.0f;
        m_AudioPlayer->Play();
    }
    if ( moveUp() )
    {
        // 2 is up.
        m_Animation->SetAsset( m_PlayerAnimations[ 2 ] );
        m_Animation->SetIsRunning( true );
        direction.y += 1.0f;
        m_AudioPlayer->Play();
    }
    if ( moveDown() )
    {
        // 3 is down.
        m_Animation->SetAsset( m_PlayerAnimations[ 3 ] );
        m_Animation->SetIsRunning( true );
        direction.y -= 1.0f;
        m_AudioPlayer->Play();
	}

    

    if ( direction != glm::vec2( 0 ) )
    {
        direction = glm::normalize( direction );
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

    /// @brief  Check if the 'D' key is being pressed.
    /// @return Is the 'D' key being pressed?
    bool PlayerController::moveRight()
    {
        
        return Input()->GetKeyDown(GLFW_KEY_D) || 
              (Input()->GetGamepadAxisState(GLFW_JOYSTICK_1, GLFW_GAMEPAD_AXIS_LEFT_X) >= 1.0f);
    }

    /// @brief  Check if the 'A' key is being pressed.
    /// @return Is the 'A' key being pressed?
    bool PlayerController::moveLeft()
    {
	    return Input()->GetKeyDown(GLFW_KEY_A) || 
               (Input()->GetGamepadAxisState(GLFW_JOYSTICK_1, GLFW_GAMEPAD_AXIS_LEFT_X) <= -1.0f);
    }

    /// @brief  Check if the 'W' key is being pressed.
    /// @return Is the 'W' key being pressed?
    bool PlayerController::moveUp()
    {
	    return Input()->GetKeyDown(GLFW_KEY_W) || 
               (Input()->GetGamepadAxisState(GLFW_JOYSTICK_1, GLFW_GAMEPAD_AXIS_LEFT_Y) <= -1.0f);
    }

    /// @brief  Check if the 'S' key is being pressed.
    /// @return Is the 'S' key being pressed?
    bool PlayerController::moveDown()
    {
	    return Input()->GetKeyDown(GLFW_KEY_S) || 
               (Input()->GetGamepadAxisState(GLFW_JOYSTICK_1, GLFW_GAMEPAD_AXIS_LEFT_Y) >= 1.0f);
    }


    /// @brief  updates the mining laser
    void PlayerController::updateMiningLaser()
    {
        if ( m_MiningLaser == nullptr )
        {
            return;
        }

        m_MiningLaser->GetTransform()->SetTranslation( m_Transform->GetTranslation() );

        if (Input()->GetMouseDown(GLFW_MOUSE_BUTTON_1) || 
           (Input()->GetGamepadAxisState(GLFW_JOYSTICK_1,GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER) >= 1.0f))
        {
            m_MiningLaser->SetIsFiring( true );
            if (Input()->GetGamepadAxisState(GLFW_JOYSTICK_1, GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER) >= 1.0f)
            {
                // Get the data from the right thumbstick.
                float xAxis = Input()->GetGamepadAxisState(GLFW_JOYSTICK_1, GLFW_GAMEPAD_AXIS_RIGHT_X);
                float yAxis = Input()->GetGamepadAxisState(GLFW_JOYSTICK_1, GLFW_GAMEPAD_AXIS_RIGHT_Y);
                glm::vec2 controllerAxis(xAxis, -yAxis);
                glm::vec2 controllerDirection = glm::normalize(controllerAxis);
                m_MiningLaser->SetDirection(controllerDirection);
            }
            else
            {
                glm::vec2 direction = glm::normalize(Input()->GetMousePosWorld() - m_Transform->GetTranslation());
                m_MiningLaser->SetDirection(direction);
            }
            
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
    void PlayerController::readAnimationNames(nlohmann::ordered_json const& data)
    {
        for (int i = 0; i < NUM_ANIMATIONS; i++)
        {
            Stream::Read( m_AnimationNames[i], data[i] );
        }
    }

    /// @brief  reads the name of the MiningLaser entity this PlayerController uses
    /// @param  data    the JSON data to read from
    void PlayerController::readMiningLaserEntity( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_MiningLaserEntity, data );
    }


    // Map of all the read methods for the PlayerController component.
    ReadMethodMap< PlayerController > const PlayerController::s_ReadMethods = {
        { "MaxSpeed"        , &readMaxSpeed         },
        { "RespawnLocation" , &readRespawnLocation  },
        { "AnimationNames"  , &readAnimationNames   },
        { "MiningLaserEnitity" , &readMiningLaserEntity }
    };

//-----------------------------------------------------------------------------
// public: writing
//-----------------------------------------------------------------------------

    /// @brief  Write all PlayerController data to a JSON file.
    /// @return The JSON file containing the TurretBehavior data.
    nlohmann::ordered_json PlayerController::Write() const
    {
        nlohmann::ordered_json data;

        nlohmann::ordered_json& animationNames = data[ "AnimationNames" ];
        for ( std::string const& animationName : m_AnimationNames )
        {
            animationNames.push_back( Stream::Write( animationName ) );
        }

        data[ "MaxSpeed"          ] = Stream::Write( m_MaxSpeed              );
        data[ "MiningLaserEntity" ] = Stream::Write( m_MiningLaserEntity     );
        data[ "RespawnLocation"   ] = Stream::Write( m_PlayerRespawnLocation );

        return data;
    }


//--------------------------------------------------------------------------------
//  Copying/Cloning
//--------------------------------------------------------------------------------

/// @brief Copy Constructor
/// @param other A PlayerController to copy.
PlayerController::PlayerController(PlayerController const& other):
    Behavior( other ),
    m_MaxSpeed( other.m_MaxSpeed ),
    m_PlayerRespawnLocation( other.m_PlayerRespawnLocation )
{
    // Copy the animations
    for (int i = 0; i < NUM_ANIMATIONS; i++)
    {
        m_AnimationNames[i] = other.m_AnimationNames[i];
        m_PlayerAnimations[i] = other.m_PlayerAnimations[i];
    }
}

/// @brief  Clones the current PlayerController and returns a copy.
/// @return A copy of the current PlayerController.
Component* PlayerController::Clone() const
{
    return (Component*) new PlayerController(*this);
}
//--------------------------------------------------------------------------------

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
    std::string animNames[NUM_ANIMATIONS] = { "Right Animation", "Left Animation", "Up Animation", "Down Animation" };

    for(int i = 0; i < NUM_ANIMATIONS; i++)
    {
        if(ImGui::BeginCombo(animNames[i].c_str(), AssetLibrary<AnimationAsset>()->GetAssetName(m_PlayerAnimations[i])))
        {
            for ( auto& [ name, animation ] : AssetLibrary<AnimationAsset>()->GetAssets() )
            {
                if (ImGui::Selectable(name.c_str(), m_PlayerAnimations[i] == animation))
                {
                    m_PlayerAnimations[i] = animation;
                }
            }
            ImGui::EndCombo();
        }
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
