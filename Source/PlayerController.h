///*****************************************************************/
/// @file	 PlayerController.h
/// @Author  Jax Clayton (jax.clayton@digipen.edu)
/// @date	 9/15/2021
/// @brief   PlayerController class header
/// @details This class contains the PlayerController component
///*****************************************************************/

#pragma once
///*****************************************************************/
/// Includes
///*****************************************************************/
#include "Behavior.h"

class PlayerController :
    public Behavior
{
    public:
        /// @brief Default constructor for the PlayerController class.
        PlayerController();

        /// @brief Copy constructor for the PlayerController class.
        /// @param other PlayerController object to copy.
        PlayerController(const PlayerController& other);
        /// @brief Destructor for the PlayerController class.
        ~PlayerController();
        /// @brief Clone method for the PlayerController class.
        /// @return A component pointer to a copy of the PlayerController.
        Component* Clone() const override;

    public:
	    /// @brief Update method called per frame.
	    virtual void OnFixedUpdate() override;

        /// @brief called when this Component's Entity is added to the Scene
        virtual void OnInit();

        /// @brief  called when this Component's Entity is removed from the Scene
        /// @note   NOT CALLED WHEN THE SCENE IS EXITED - that should be handled by this Component's System
        virtual void OnExit();


    private:    /// movement

        /// @brief private helper function to move the player
        /// @return if the player moved Right or not
        bool MoveRight();

        /// @brief protected helper function to move the player
        /// @return if the player moved Left or not
        bool MoveLeft();

        /// @brief protected helper function to move the player
        /// @return if the player Jumped or not
        bool Jump();

        /// @brief protected helper function to move the player
        /// @return if the player moved Down or not
        bool MoveDown();

    private:
        // Define acceleration and deceleration factors.
        float acceleration = 1.0f;
        float deceleration = 2.0f;

        // Define the maximum speed for smooth movement.
        float maxSpeed = 5.0f;

};

