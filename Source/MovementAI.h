///*******************************************************************
/// @file   MovementAI.h
/// @brief  This is a temporary file for the movement AI
/// 
/// @author Jax Clayton
/// @date   September 2023
///*********************************************************************/

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
        MovementAI();
        ~MovementAI();

    public:
        Component* Clone() const override;
        void OnCollisionEvent() override;
        void OnUpdate(float dt) override;
        void OnFixedUpdate() override;
    private:

        void MovementAIUpdateRotation(float dt);
        void MovementAIUpdateVelocity(float dt);
        void MovementAIUpdateWeapon(float dt);
        void MovementAISpawnBullet();
        void MovementAISpiral(float dt);

    private:
        const float MovementAcceleration = 150.0f;
        const float MovementSpeedMax = 500.0f;
        const float MovementTurnRateMax = (float)M_PI / 1.5f;
        const float MovementWeaponCooldownTime = 0.25f;
        const float MovementWeaponBulletSpeed = 750.0f;
        const float MovementDeathDuration = 3.0f;
        const float FrameTime = 1.0f / 60.0f;
};

