///--------------------------------------------------------------------------//
/// @file   PlayerController.h
/// @brief  PlayerController Behaviour Class
/// 
/// @author Aidan Straker (aidan.straker)
/// @date   October 2023
///
/// @copyright (c) 2024 DigiPen (USA) Corporation.
///--------------------------------------------------------------------------//

#pragma once
//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "pch.h" // precompiled header has to be included first
#include "Behavior.h"

#include "ComponentReference.h"
#include "RigidBody.h"
#include "Animation.h"
#include "AudioPlayer.h"
#include "Inventory.h"
#include "Health.h"
#include "Transform.h"
#include "CircleCollider.h"
#include "EffectAnimator.h"

#include "EntityReference.h"
#include "MiningLaser.h"

#include "AssetReference.h"
#include "AnimationAsset.h"

#include "ActionReference.h"
#include "EventListener.h"


class PlayerController : public Behavior
{
//-----------------------------------------------------------------------------
public: // constructor / destructor
//-----------------------------------------------------------------------------


    /// @brief Default constructor for the PlayerController class.
    PlayerController();

    /// @brief Destructor for the PlayerController class.
    ~PlayerController() = default;

    
//-----------------------------------------------------------------------------
public: // accessors
//-----------------------------------------------------------------------------


    /// @brief  gets the Health Component attached to this Entity
    /// @return the Health Component attached to this Entity
    Health* GetHealth() { return m_Health;  }

    /// @brief  Gets the MiningLaser component attached to this Entity.
    /// @return The MiningLaser component attached to this Entity.
    MiningLaser* GetMiningLaser() {  return m_MiningLaser;  }

    /// @brief  Gets the Transform component attached to this Entity.
    /// @return The Transform component attached to this Entity.
    Transform const* GetTransform() const { return m_Transform; }


    bool GetActive() const { return m_CanMove; }
//-----------------------------------------------------------------------------
public: // virtual override methods
//-----------------------------------------------------------------------------
	    

    /// @brief called when this Component's Entity is added to the Scene
    virtual void OnInit() override;

    /// @brief  called when this Component's Entity is removed from the Scene
    virtual void OnExit() override;


    /// @brief Update method called per frame.
    virtual void OnFixedUpdate() override;

//-----------------------------------------------------------------------------
private: // member variables
//-----------------------------------------------------------------------------


    /// @brief The amount of force to apply to the player when moving vertically
    glm::vec2 m_VerticalMoveforce = { 1.0f, 1.0f };

    /// @brief The amount of force to apply to the player when moving horizontally
    glm::vec2 m_HorizontalMoveforce = { 1.0f, 1.0f };


    /// @brief  Player respawn location
    glm::vec2 m_PlayerRespawnLocation = { -15.0f, 5.0f };


    /// @brief Jump force
    float m_JumpSpeed = 1.0f;

    /// @brief  if the player is currently jumping
    bool m_IsJumping = false;


    /// @breif  How closely aligned the collision normal must be with the vertical axis
    float m_GroundCollisionThreshold = 0.9f;

    /// @brief Allowed time to jump after falling off a platform
    float m_MaxCoyoteTime = 0.3f;

    /// @brief  The current time the player has to jump after falling off a platform
    float m_CurrentCoyoteTime = 0.0f;


    /// @brief  the offset from the player of the mining laser's position (when facing left)
    glm::vec2 m_MiningLaserOffset = glm::vec2( 0.0f, 0.0f );


    /// @brief  a cached instance of the parent's Rigidbody.
    ComponentReference< RigidBody > m_RigidBody;

    /// @brief  a cached instance of the parent's AudioPlayer.
    ComponentReference< AudioPlayer > m_AudioPlayer;

    /// @brief  the Transform attached to this PlayerController
    ComponentReference< Transform > m_Transform;

    /// @brief  a cached instance of the parent's Health.
    ComponentReference< Health > m_Health;

    /// @brief  a cached instance of the parent's collider.
    ComponentReference< Collider > m_Collider;

    /// @brief a cached instance of the parent's Inventory.
    ComponentReference< Inventory > m_Inventory;

    /// @brief a cached instance of the effect animator.
    ComponentReference< EffectAnimator > m_EffectAnimator;

    /// @brief a cached instance of the sprite.
    ComponentReference< Sprite > m_Sprite;
    

    /// @brief  the miningLaser this PlayerController uses
    ComponentReference< MiningLaser > m_MiningLaser;

    /// @brief  the Entity the MiningLaser this PlayerController uses is attached to
    EntityReference m_MiningLaserEntity = EntityReference( { &m_MiningLaser } );


    /// @brief  the control Action used for vertical movement
    ActionReference m_MoveVertical;

    /// @brief  the control Action used for vertical movement
    ActionReference m_MoveHorizontal;

    /// @brief  the control Action to fire the laser
    ActionReference m_FireLaser;

    /// @brief  the control Action to interact with something
    ActionReference m_Interact;

    /// @brief  the control action for horizontal aim
    ActionReference m_AimHorizontal;

    /// @brief  the control action for vertical aim
    ActionReference m_AimVertical;

    /// @brief Listener for the begin event
    EventListener<std::string> m_ListenerBegin;

    /// @brief  the name of the event to listen for
    std::string m_EventNameBegin;

    /// @brief  the name of the event to listen for
    std::string m_EventNameWin;

    /// @brief  the name of the event to listen for
    std::string m_EventNameHide;

    /// @brief  the name of the event to listen for
    std::string m_EventNameShow;

    bool m_CanMove = false;
//-----------------------------------------------------------------------------
private: // methods
//-----------------------------------------------------------------------------


    /// @brief  updates the mining laser
    void updateMiningLaser();


    /// @brief Check if player heal is 0, then respawn them.
    void playerRespawn();


    /// @brief  What to do when the player has been hit.
    /// @param  other   - the collider of the other entity.
    void onCollisionEnter( Collider* other );

    /// @brief  Makes sure the player stays within the bounds of the level.
    void playerBoundaryCheck();

//-----------------------------------------------------------------------------
public: // inspection
//-----------------------------------------------------------------------------


    /// @brief Used by the Debug System to display information about this Component
    virtual void Inspector() override;
    

//-----------------------------------------------------------------------------
private: // inspection
//-----------------------------------------------------------------------------


    /// @brief Allows all vector attributes to be accessed by the editor.
    void vectorInspector();


//-----------------------------------------------------------------------------
private: // reading
//-----------------------------------------------------------------------------



    /// @brief  reads in the amount of force to apply to the player when moving vertically
    /// @param  data    the JSON data to read from
    void readVerticalMoveForce( nlohmann::ordered_json const& data );

    /// @brief  reads in the amount of force to apply to the player when moving horizontally
    /// @param  data    the JSON data to read from
    void readHorizontalMoveForce( nlohmann::ordered_json const& data );


    /// @brief  reads in the respawn location for the player
    /// @param  data    the JSON data to read from
    void readRespawnLocation(nlohmann::ordered_json const& data);


    /// @brief  reads the JumpSpeed
    /// @param  data    the JSON data to read from
    void readJumpSpeed( nlohmann::ordered_json const& data );

    /// @brief Read in the is jumping state.
    /// @param  data    the JSON data to read from
    void readIsJumping( nlohmann::ordered_json const& data );


    /// @brief Read in the ground collision threshold.
    /// @param  data    the JSON data to read from
    void readGroundCollisionThreshold( nlohmann::ordered_json const& data );

    /// @brief Read in the max coyote time.
    /// @param  data    the JSON data to read from
    void readMaxCoyoteTime( nlohmann::ordered_json const& data );


    /// @brief  reads the offset from the player of the mining laser's position (when facing left)
    /// @param  data    the JSON data to read from
    void readMiningLaserOffset( nlohmann::ordered_json const& data );


    /// @brief  reads the name of the MiningLaser entity this PlayerController uses
    /// @param  data    the JSON data to read from
    void readMiningLaserEntity( nlohmann::ordered_json const& data );


    /// @brief  reads the control Action used for vertical movement
    /// @param  data    the JSON data to read from
    void readMoveVertical( nlohmann::ordered_json const& data );

    /// @brief  reads the control Action used for vertical movement
    /// @param  data    the JSON data to read from
    void readMoveHorizontal( nlohmann::ordered_json const& data );

    /// @brief  reads the control Action to fire the laser
    /// @param  data    the JSON data to read from
    void readFireLaser( nlohmann::ordered_json const& data );

    /// @brief  reads the control Action to interact with something
    /// @param  data    the JSON data to read from
    void readInteract( nlohmann::ordered_json const& data );

    /// @brief  reads the control action for horizontal aim
    /// @param  data    the JSON data to read from
    void readAimHorizontal( nlohmann::ordered_json const& data );

    /// @brief  reads the control action for vertical aim
    /// @param  data    the JSON data to read from
    void readAimVertical( nlohmann::ordered_json const& data );


    /// @brief  reads the EventNameBegin from a JSON file
    /// @param data    the JSON file to read from
    void readEventNameBegin(nlohmann::ordered_json const& data);

    /// @brief  reads the EventNameWin from a JSON file
    /// @param data    the JSON file to read from
    void readEventNameWin(nlohmann::ordered_json const& data);

    /// @brief  reads the EventNameWin from a JSON file
    /// @param data    the JSON file to read from
    void readEventNameHide(nlohmann::ordered_json const& data);

    /// @brief  reads the EventNameWin from a JSON file
    /// @param data    the JSON file to read from
    void readEventNameShow(nlohmann::ordered_json const& data);

//-----------------------------------------------------------------------------
public: // reading / writing
//-----------------------------------------------------------------------------


    /// @brief gets the map of read methods for this Component
    /// @return the map of read methods for this Component
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override;


    /// @brief  Write all PlayerController data to a JSON file.
    /// @return The JSON file containing the PlayerController data.
    virtual nlohmann::ordered_json Write() const override;


//-----------------------------------------------------------------------------
public: // copying
//-----------------------------------------------------------------------------


    /// @brief  Creates a new copy of this Component.
    /// @return The newly created component
    virtual PlayerController* Clone() const override;


//-----------------------------------------------------------------------------
private: // copying
//-----------------------------------------------------------------------------


    /// @brief  Copy constructor.
    PlayerController( PlayerController const& other );


    // disable assignment operator 
    void operator =( PlayerController const& ) = delete;


//-----------------------------------------------------------------------------
};