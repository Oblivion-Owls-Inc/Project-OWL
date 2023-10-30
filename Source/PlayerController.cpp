///-----------------------------------------------------------------------------//
/// @file   PlayerController.cpp
/// @brief  PlayerController Behaviour Class
/// 
/// @author Aidan Straker (aidan.straker)
/// @date   October 2023
///
/// @copyright (c) 2023 DigiPen (USA) Corporation.
///-----------------------------------------------------------------------------//
#include "PlayerController.h" 
#include "BehaviorSystem.h"     // GetInstance, AddBehavior, RemoveBehavior
#include "InputSystem.h"        // GetInstance, GetKeyDown
#include "RigidBody.h"          // ApplyVelocity
#include "Animation.h"          // SetAsset
#include "AudioPlayer.h"
#include "AnimationAsset.h"
#include "AssetLibrarySystem.h" // GetAsset
#include "DebugSystem.h"

///----------------------------------------------------------------------------
/// Static Variables
///----------------------------------------------------------------------------

// Get an instance of the input system.
static InputSystem * input = InputSystem::GetInstance();
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
	BehaviorSystem<Behavior>::GetInstance()->AddBehavior(this);
    // Get the parent's RigidBody component.
    m_RigidBody = GetParent()->GetComponent<RigidBody>();
    // Get the parent's Animation component.
    m_Animation = GetParent()->GetComponent<Animation>();
    // Get the parent's AudioPlayer component.
    m_AudioPlayer = GetParent()->GetComponent<AudioPlayer>();

    // Get all the player's animations
    for ( int i = 0; i < NUM_ANIMATIONS; ++i )
    {
        m_PlayerAnimations[ i ] = AssetLibrary< AnimationAsset >()->GetAsset( m_AnimationNames[ i ] );
    }
}

/// @brief Removes this behavior from the behavior system on exit
void PlayerController::OnExit()
{
    Behaviors<Behavior>()->RemoveBehavior(this);
}

/// @brief Used by the Debug System to display information about this Component.
void PlayerController::Inspector()
{
    vectorInspector();
    animationInspector();
}

/// @brief on fixed update check which input is being pressed.
void PlayerController::OnFixedUpdate()
{
    // The normalised direction vector.
    glm::vec2 direction = { 0.0f, 0.0f };

    if ( moveRight() )
    {
        // 0 is right.
        m_Animation->SetAsset( m_PlayerAnimations[ 0 ] );
        m_Animation->SetIsRunning( true );
        direction.x += 1.0f;
    }
    if ( moveLeft() )
    {
        // 1 is left
        m_Animation->SetAsset( m_PlayerAnimations[ 1 ] );
        m_Animation->SetIsRunning( true );
        direction.x -= 1.0f;
    }
    if ( moveUp() )
    {
        // 2 is up.
        m_Animation->SetAsset( m_PlayerAnimations[ 2 ] );
        m_Animation->SetIsRunning( true );
        direction.y += 1.0f;
    }
    if ( moveDown() )
    {
        // 3 is down.
        m_Animation->SetAsset( m_PlayerAnimations[ 3 ] );
        m_Animation->SetIsRunning( true );
        direction.y -= 1.0f;
	}

    if ( direction != glm::vec2( 0 ) )
    {
        direction = glm::normalize( direction );
    }
    else
    {
        m_Animation->SetIsRunning( false );
        m_Animation->SetFrameIndex( 0, true );
    }

    m_RigidBody->ApplyVelocity( direction * m_MaxSpeed );
}

//-----------------------------------------------------------------------------
// player movement
//-----------------------------------------------------------------------------

/// @brief  Check if the 'D' key is being pressed.
/// @return Is the 'D' key being pressed?
bool PlayerController::moveRight()
{
	return input->GetKeyDown(GLFW_KEY_D);
}

/// @brief  Check if the 'A' key is being pressed.
/// @return Is the 'A' key being pressed?
bool PlayerController::moveLeft()
{
	return input->GetKeyDown(GLFW_KEY_A);
}

/// @brief  Check if the 'W' key is being pressed.
/// @return Is the 'W' key being pressed?
bool PlayerController::moveUp()
{
	return input->GetKeyDown(GLFW_KEY_W);
}

/// @brief  Check if the 'S' key is being pressed.
/// @return Is the 'S' key being pressed?
bool PlayerController::moveDown()
{
	return input->GetKeyDown(GLFW_KEY_S);
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

    /// @brief Read in the names of the player animations.
    /// @param data The JSON file to read from.
    void PlayerController::readAnimationNames(nlohmann::ordered_json const& data)
    {
        for (int i = 0; i < NUM_ANIMATIONS; i++)
        {
            Stream::Read( m_AnimationNames[i], data[i] );
        }
    }


    // Map of all the read methods for the PlayerController component.
    ReadMethodMap< PlayerController > const PlayerController::s_ReadMethods = {
        { "MaxSpeed"      , &readMaxSpeed       },
        { "AnimationNames", &readAnimationNames }
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

        data[ "MaxSpeed" ] = m_MaxSpeed;

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
    m_RigidBody( nullptr ),
    m_Animation( nullptr )
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
    ImGui::InputFloat("Max Speed", &m_MaxSpeed, 0.1f, 1.0f);
}

/// @brief Helper function for inspector.
void PlayerController::animationInspector()
{
    std::string animNames[NUM_ANIMATIONS] = { "Right Animation", "Left Animation", "Up Animation", "Down Animation" };

    for(int i = 0; i < NUM_ANIMATIONS; i++)
    {
        if(ImGui::BeginCombo(animNames[i].c_str(), AssetLibrary<AnimationAsset>()->GetAssetName(m_playerAnimations[i])))
        {
            for ( auto& [ name, animation ] : AssetLibrary<AnimationAsset>()->GetAssets() )
            {
                if (ImGui::Selectable(name.c_str(), m_playerAnimations[i] == animation))
                {
                    m_playerAnimations[i] = animation;
                }
            }
            ImGui::EndCombo();
        }
    }
}
