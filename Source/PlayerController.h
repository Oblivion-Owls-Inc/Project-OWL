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
        PlayerController();
        PlayerController(const PlayerController& other);
        ~PlayerController();
        Component* Clone() const override;

    public:
	    void OnUpdate(float dt) override;
        void OnCollisionEvent() override {};

    private:    
        bool MoveRight();
        bool MoveLeft();
        bool Jump();
        bool MoveDown();
    private:
        // Define acceleration and deceleration factors.
        float acceleration = 1.0f;
        float deceleration = 2.0f;

        // Define the maximum speed for smooth movement.
        float maxSpeed = 5.0f;

    private: // reading

        /// @brief the map of read methods for this Component
        static ReadMethodMap< PlayerController > const readMethods;

        /// @brief gets the map of read methods for this Component
        /// @return the map of read methods for this Component
        virtual ReadMethodMap< Component > const& GetReadMethods() const override;
};

