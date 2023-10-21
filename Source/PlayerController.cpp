///--------------------------------------------------------------------------//
/// @file   PlayerController.cpp
/// @brief  PlayerController Bheaviour Class
/// 
/// @author Aidan Straker (aidan.straker)
/// @date   October 2023
///
/// @copyright © 2023 DigiPen (USA) Corporation.
///--------------------------------------------------------------------------//
#include "PlayerController.h" 
#include "BehaviorSystem.h" // GetInstance, AddBehavior, RemoveBehavior
#include "InputSystem.h"    // GetInstance, GetKeyDown
#include "RigidBody.h"      // ApplyVelocity
#include "Animation.h"      // SetAsset
#include "AnimationAsset.h"
#include "AssetLibrarySystem.h"

///----------------------------------------------------------------------------
/// Static Variables
///----------------------------------------------------------------------------

// Get an instance of the input system.
static InputSystem * input = InputSystem::GetInstance();

#define NUM_ANIMATIONS 4

///----------------------------------------------------------------------------
/// Public: constructor / destructor
///----------------------------------------------------------------------------

/// @brief Default Constructor.
PlayerController::PlayerController() : 
    Behavior(typeid(PlayerController)),
    m_leftDirection(0.0f, 0.0f),
    m_rightDirection(0.0f, 0.0f),
    m_upwardDirection(0.0f, 0.0f),
    m_downwardDirection(0.0f, 0.0f),
    m_maxSpeed(0.0f),
    m_RigidBody(nullptr),
    m_Animation(nullptr),
    m_animationNames(),
    m_playerAnimations()
{}

/// @brief Destructor
PlayerController::~PlayerController()
{
}

///----------------------------------------------------------------------------
/// Public: methods
///----------------------------------------------------------------------------

/// @brief Adds this behavior to the behavior system on init.
void PlayerController::OnInit()
{
	BehaviorSystem<PlayerController>::GetInstance()->AddBehavior(this);
    // Get the parent's rigidbody component.
    m_RigidBody = GetParent()->GetComponent<RigidBody>();
    // Get the parent' animation component.
    m_Animation = GetParent()->GetComponent<Animation>();

    // Get all the player's animations
    for (int i = 0; i < NUM_ANIMATIONS; i++)
    {
        m_playerAnimations[i] = AssetLibrary<AnimationAsset>()->GetAsset(m_animationNames[i]);
    }
}

/// @brief Removes this behavior from the behavior system on exit
void PlayerController::OnExit()
{
	BehaviorSystem< PlayerController >::GetInstance()->RemoveBehavior(this);
}

/// @brief on fixed update check which input is being pressed.
void PlayerController::OnFixedUpdate()
{
    // The normlaised direction vector.
    glm::vec2 direction = { 0.0f, 0.0f };

    if (MoveRight())
    {
        // 0 is right.
        m_Animation->SetAsset(m_playerAnimations[0]);
        direction += m_rightDirection;
    }
    if (MoveLeft())
    {
        // 1 is left
        m_Animation->SetAsset(m_playerAnimations[1]);
        direction += m_leftDirection;
    }
    if (MoveUp())
    {
        // 2 is up.
        m_Animation->SetAsset(m_playerAnimations[2]);
        direction += m_upwardDirection;
    }
	if (MoveDown())
	{
        // 3 is down.
        m_Animation->SetAsset(m_playerAnimations[3]);
        direction += m_downwardDirection;
	}

    if ( direction != glm::vec2( 0 ) )
    {
        direction = glm::normalize( direction );
    }

    m_RigidBody->ApplyVelocity(direction * m_maxSpeed);
}

//-----------------------------------------------------------------------------
// player movement
//-----------------------------------------------------------------------------

/// @brief  Check if the 'D' key is being pressed.
/// @return Is the 'D' key being pressed?
bool PlayerController::MoveRight()
{
	return input->GetKeyDown(GLFW_KEY_D);
}

/// @brief  Check if the 'A' key is being pressed.
/// @return Is the 'A' key being pressed?
bool PlayerController::MoveLeft()
{
	return input->GetKeyDown(GLFW_KEY_A);
}

/// @brief  Check if the 'W' key is being pressed.
/// @return Is the 'W' key being pressed?
bool PlayerController::MoveUp()
{
	return input->GetKeyDown(GLFW_KEY_W);
}

/// @brief  Check if the 'S' key is being pressed.
/// @return Is the 'S' key being pressed?
bool PlayerController::MoveDown()
{
	return input->GetKeyDown(GLFW_KEY_S);
}

//-----------------------------------------------------------------------------
//  reading/writing
//-----------------------------------------------------------------------------

/// @brief Read in a glm::vec2 for the player's left movement.
/// @param data The JSON file to read from.
void PlayerController::readLeftDirection(nlohmann::ordered_json const& data)
{
    m_leftDirection = Stream::Read < 2, float >(data);
}

/// @brief Read in a glm::vec2 for the player's right movement.
/// @param data The JSON file to read from.
void PlayerController::readRightDirection(nlohmann::ordered_json const& data)
{
    m_rightDirection = Stream::Read < 2, float >(data);
}

/// @brief Read in a glm::vec2 for the player's upward movement.
/// @param data The JSON file to read from.
void PlayerController::readUpDirection(nlohmann::ordered_json const& data)
{
    m_upwardDirection = Stream::Read < 2, float >(data);
}

/// @brief Read in a glm::vec2 for the player's downward movement.
/// @param data The JSON file to read from.
void PlayerController::readDownDirection(nlohmann::ordered_json const& data)
{
    m_downwardDirection = Stream::Read < 2, float >(data);
}

/// @brief Read in the max speed for the player.
/// @param data The JSON file to read from.
void PlayerController::readMaxSpeed(nlohmann::ordered_json const& data)
{
    m_maxSpeed = Stream::Read <float>(data);
}

/// @brief Read in the names of the player animations.
/// @param data The JSON file to read from.
void PlayerController::readAnimationNames(nlohmann::ordered_json const& data)
{
    for (int i = 0; i < NUM_ANIMATIONS; i++)
    {
        m_animationNames[i] = data[i];
    }
}

/// @brief  Write all PlayerController data to a JSON file.
/// @return The JSON file containing the TurretBehavior data.
nlohmann::ordered_json PlayerController::Write() const
{
    nlohmann::ordered_json data;

    data["left"] = Stream::Write(m_leftDirection);
    data["right"] = Stream::Write(m_rightDirection);
    data["up"] = Stream::Write(m_upwardDirection);
    data["down"] = Stream::Write(m_downwardDirection);
    data["maxSpeed"] = m_maxSpeed;

    return data;
}

// Map of all the read methods for the PlayerController component.
ReadMethodMap< PlayerController > PlayerController::s_ReadMethods = {
    { "left"          , &readLeftDirection  },
    { "right"	      , &readRightDirection },
    { "up"		      , &readUpDirection    },
    { "down"	      , &readDownDirection  },
    { "maxSpeed"      , &readMaxSpeed       },
    { "animationName" , &readAnimationNames }
};

//-----------------------------------------------------------------------------
//  Copying/Cloning
//-----------------------------------------------------------------------------

/// @brief Copy Constructor
/// @param other A PlayerController to copy.
PlayerController::PlayerController(PlayerController const& other):
    Behavior(typeid(PlayerController)),
    m_leftDirection(other.m_leftDirection),
    m_rightDirection(other.m_rightDirection),
    m_upwardDirection(other.m_upwardDirection),
    m_downwardDirection(other.m_downwardDirection),
    m_maxSpeed(other.m_maxSpeed),
    m_RigidBody(nullptr),
    m_Animation(nullptr)
{
    // Copy the animations
    for (int i = 0; i < NUM_ANIMATIONS; i++)
    {
        m_animationNames[i] = other.m_animationNames[i];
    }
}

/// @brief  Clones the current PlayerController and returns a copy.
/// @return A copy of the current PlayerController.
Component* PlayerController::Clone() const
{
    return (Component*) new PlayerController(*this);
}