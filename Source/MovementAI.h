///*****************************************************************/
/// @file	 MovementAI.h
/// @Author  Jax Clayton (jax.clayton@digipen.edu)
/// @date	 9/15/2021
/// @brief   MovementAI  
/// @details 
/// 
///* ****************************************************************/

///------------------------------------------------------------------------------
/// Include Files:
///------------------------------------------------------------------------------
#pragma once
#include "Behavior.h"
#include "RigidBody.h"
#include "Transform.h"

#define _USE_MATH_DEFINES
#include <math.h>

///------------------------------------------------------------------------------
/// @brief This is the MovementAI class this is a temporary class for the movement
///------------------------------------------------------------------------------
class MovementAI :
    public Behavior
{
    public:
        /// @brief Construct a new MovementAI object
        MovementAI();

        MovementAI(const MovementAI& other);

        /// @brief Clones the Component 
        /// @return A pointer to a newly allocated Component
        Component* Clone() const override;

        /// @brief Called on a Fixed Update 
        void OnFixedUpdate() override;

        /// @brief called when this Component's Entity is added to the Scene
        virtual void OnInit() override;

        /// @brief  called when this Component's Entity is removed from the Scene
        /// @note   NOT CALLED WHEN THE SCENE IS EXITED - that should be handled by this Component's System
        virtual void OnExit() override;

        /// @brief  Called whenever a Collider on this Behavior's Entity collides
        /// @param  other           the entity that was collided with
        /// @param  collisionData   additional data about the collision
        virtual void OnCollision( Entity* other, CollisionData const& collisionData ) override;

private: // reading

    /// @brief the map of read methods for this Component
    static ReadMethodMap< MovementAI > const readMethods;

    /// @brief gets the map of read methods for this Component
    /// @return the map of read methods for this Component
    virtual ReadMethodMap< Component > const& GetReadMethods() const override;
};

