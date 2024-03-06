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

    private:
        virtual Component* Clone() const override;
        MovementAI( MovementAI const& other );

        virtual void OnInit() override;
        virtual void OnExit() override;

        virtual void OnUpdate(float dt) override;
        virtual void OnFixedUpdate() override;


};

