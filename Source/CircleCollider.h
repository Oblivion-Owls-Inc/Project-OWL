///*************************************************************************
/// @file CircleCollider.h
///*************************************************************************
#pragma once

#include "Collider.h"
#include "Stream.h"

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

        void SetRadius(float m_Radius);

        float GetRadius() const;

        virtual void OnFixedUpdate() override;


    private:
        float m_Radius;  // Radius of the circle collider
        bool doesDamage = false;

//-----------------------------------------------------------------------------
private: // reading
//-----------------------------------------------------------------------------

    /// @brief        Read in the radius for the circle collider component.
    /// @param stream The JSON to read from.
    void ReadRadius(Stream stream);

    /// @brief map of the read methods for this Component
    static ReadMethodMap< CircleCollider > s_ReadMethods;

    /// @brief gets the map of read methods for this Component
    /// @return the map of read methods for this Component
    virtual ReadMethodMap< Component > const& GetReadMethods() const override;

//-----------------------------------------------------------------------------

};
