///--------------------------------------------------------------------------//
/// @file   PlayerController.h
/// @brief  PlayerController Behaviour Class
/// 
/// @author Aidan Straker (aidan.straker)
/// @date   October 2023
///
/// @copyright (c) 2023 DigiPen (USA) Corporation.
///--------------------------------------------------------------------------//

#pragma once
//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------
#include "Behavior.h"
#include <glm/glm.hpp> // glm::vec2
#include <string>


#include "ComponentReference.h"
#include "RigidBody.h"
#include "Animation.h"
#include "AudioPlayer.h"
#include "Health.h"
#include "Transform.h"

#include "EntityReference.h"
#include "MiningLaser.h"

class AnimationAsset;


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
public: // methods
//-----------------------------------------------------------------------------
	    
	/// @brief Update method called per frame.
	virtual void OnFixedUpdate() override;

    /// @brief called when this Component's Entity is added to the Scene
    virtual void OnInit() override;

    /// @brief  called when this Component's Entity is removed from the Scene
    /// @note   NOT CALLED WHEN THE SCENE IS EXITED - that should be handled by this Component's System
    virtual void OnExit() override;

    /// @brief Used by the Debug System to display information about this Component
    virtual void Inspector() override;

//-----------------------------------------------------------------------------
private: // methods
//-----------------------------------------------------------------------------

    /// @brief private helper function to move the player
    /// @return if the player moved Right or not
    bool moveRight();

    /// @brief protected helper function to move the player
    /// @return if the player moved Left or not
    bool moveLeft();

    /// @brief protected helper function to move the player
    /// @return if the player Jumped or not
    bool moveUp();

    /// @brief protected helper function to move the player
    /// @return if the player moved Down or not
    bool moveDown();


    /// @brief  updates the mining laser
    void updateMiningLaser();


    /// @brief Check if player heal is 0, then respawn them.
    void playerRespawn();

//-----------------------------------------------------------------------------
private: // member variables
//-----------------------------------------------------------------------------
   

    // Define the maximum speed for smooth movement.
    float m_MaxSpeed = 1.0f;

    // Player respawn location
    glm::vec2 m_PlayerRespawnLocation = { -15.0f, 5.0f };

    // All the names of the animations for the player.
    std::string m_AnimationNames[ 4 ] = { "", "", "", "" };

    

    // All the animations for the player.
    AnimationAsset const* m_PlayerAnimations[4] = { nullptr, nullptr, nullptr, nullptr };


    /// @brief  a cached instance of the parent's Rigidbody.
    ComponentReference< RigidBody > m_RigidBody;

    /// @brief  a cached instance of the parent's animation.
    ComponentReference< Animation > m_Animation;

    /// @brief  a cached instance of the parent's AudioPlayer.
    ComponentReference< AudioPlayer > m_AudioPlayer;

    /// @brief  the Transform attached to this PlayerController
    ComponentReference< Transform > m_Transform;

    /// @brief  a cached instance of the parent's Health.
    ComponentReference< Health > m_Health;

    

    /// @brief  the miningLaser this PlayerController uses
    ComponentReference< MiningLaser > m_MiningLaser;
    /// @brief  the Entity of the MiningLaser this PlayerController uses
    EntityReference m_MiningLaserEntity = EntityReference( { &m_MiningLaser } );


//-----------------------------------------------------------------------------
private: // reading
//-----------------------------------------------------------------------------


    /// @brief Read in the max speed for the player.
    /// @param data The JSON file to read from.
    void readMaxSpeed(nlohmann::ordered_json const& data);

    /// @brief Read in the respawn location for the player.
    /// @param data - the JSON file to read from.
    void readRespawnLocation(nlohmann::ordered_json const& data);

    /// @brief Read in the animation names for the player.
    /// @param data The JSON file to read from.
    void readAnimationNames(nlohmann::ordered_json const& data);

    /// @brief  reads the name of the MiningLaser entity this PlayerController uses
    /// @param  data    the JSON data to read from
    void readMiningLaserEntity( nlohmann::ordered_json const& data );


    /// @brief the map of read methods for this Component
    static ReadMethodMap< PlayerController > const  s_ReadMethods;


//-----------------------------------------------------------------------------
public: // reading / writing
//-----------------------------------------------------------------------------


    /// @brief gets the map of read methods for this Component
    /// @return the map of read methods for this Component
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override
    {
        return (ReadMethodMap< ISerializable > const&)s_ReadMethods;
    }


    /// @brief  Write all PlayerController data to a JSON file.
    /// @return The JSON file containing the PlayerController data.
    virtual nlohmann::ordered_json Write() const override;


//-----------------------------------------------------------------------------
private: // copying
//-----------------------------------------------------------------------------

    /// @brief  Creates a new copy of this Component.
    /// @return The newly created component
    virtual Component* Clone() const override;

    /// @brief Copy constructor.
    PlayerController(PlayerController const& other);


//-----------------------------------------------------------------------------
private: // inspector helpers
//-----------------------------------------------------------------------------

    /// @brief Allows all vector attributes to be accessed by the editor.
    void vectorInspector();
    /// @brief Allows all animation attributes to be accessed by the editor.
    void animationInspector();

    /// @brief  What to do when the player has been hit.
    /// @param  other   - the collider of the other entity.
    void onCollisionEnter( Collider* other );

};