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

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------
class RigidBody;
class Animation;
class AnimationAsset;
class AudioPlayer;

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
private: // player movement
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

//-----------------------------------------------------------------------------
private: // member variables
//-----------------------------------------------------------------------------
   

    // Define the maximum speed for smooth movement.
    float m_MaxSpeed = 1.0f;


    // All the names of the animations for the player.
    std::string m_AnimationNames[4] = { "", "", "", "" };

    // All the animations for the player.
    AnimationAsset const* m_PlayerAnimations[4] = { nullptr, nullptr, nullptr, nullptr };


    // A cached instance of the parent's Rigidbody.
    RigidBody* m_RigidBody = nullptr;

    // A cached instance of the parent's animation.
    Animation* m_Animation = nullptr;

    // A cached instance of the parent's AudioPlayer.
    AudioPlayer* m_AudioPlayer = nullptr;


//-----------------------------------------------------------------------------
private: // reading
//-----------------------------------------------------------------------------


    /// @brief Read in the max speed for the player.
    /// @param data The JSON file to read from.
    void readMaxSpeed(nlohmann::ordered_json const& data);

    /// @brief Read in the animation names for the player.
    /// @param data The JSON file to read from.
    void readAnimationNames(nlohmann::ordered_json const& data);


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
private: // Helper Functions
//-----------------------------------------------------------------------------

    /// @brief Allows all vector attributes to be accessed by the editor.
    void vectorInspector();
    /// @brief Allows all animation attributes to be accessed by the editor.
    void animationInspector();

};