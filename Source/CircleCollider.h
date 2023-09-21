///*************************************************************************
/// @file CircleCollider.h
///*************************************************************************
#pragma once

#include "Collider.h"

 /**
  * @class CircleCollider
  * @brief A class representing a circular collider used in physics simulations.
  */
class CircleCollider : public Collider
{
    public:
        /**
         * @brief Constructor for CircleCollider.
         */
        CircleCollider();

        /**
         * @brief Copy constructor for CircleCollider.
         * @param other The CircleCollider object to copy.
         */
        CircleCollider(const CircleCollider& other);

        /**
         * @brief Clone method for CircleCollider.
         * @return A cloned CircleCollider object.
         */
        virtual Component* Clone() const override;

        /**
         * @brief Override the IsColliding method to handle circle-circle collisions.
         * @param other The collider to check for collision with.
         * @return True if a collision is detected, false otherwise.
         */
        virtual bool CheckIfColliding(const Collider* other) override;

        void SetRadius(float _radius);

        float GetRadius() const;

        virtual void OnFixedUpdate() override;

    private:
        float radius;  // Radius of the circle collider
        bool doesDamage = false;
};
