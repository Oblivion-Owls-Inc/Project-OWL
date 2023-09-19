/*******************************************************************
 * \file   MovementAI.h
 * \brief  This is a temporary file for the movement AI
 * 
 * \author Jax Clayton
 * \date   September 2023
 *********************************************************************/
#pragma once
#include "Behavior.h"

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#define _USE_MATH_DEFINES
#include <math.h>

//------------------------------------------------------------------------------
// Class:
//------------------------------------------------------------------------------
class MovementAI :
    public Behavior
{
    public:
        MovementAI();
        ~MovementAI();

    public:
        Component* Clone() const override;
        void CollisionEvent(Entity* other) override;
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

