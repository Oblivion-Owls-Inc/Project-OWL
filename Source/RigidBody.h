///*****************************************************************/
/// @file	 RigidBody.h
/// @Author  Jax Clayton (jax.clayton@digipen.edu)
/// @date	 9/5/2023
/// @brief   RigidBody class header
/// @details This class contains the RigidBody component
///*****************************************************************/

#pragma once
///*****************************************************************/
/// Includes
///*****************************************************************/
#include "Component.h"
#include "basics.h"
#include "Transform.h"
#include "Entity.h"
#include "Behavior.h"

///*****************************************************************/
/// RigidBody class
/// @brief This class contains the RigidBody component which is used
///  for physics on a game object
///*****************************************************************/

class RigidBody : public Behavior
{
public:
    /// @brief Default constructor for the RigidBody class.
    RigidBody();

    /// @brief Copy constructor for the RigidBody class.
    /// @param other The RigidBody object to copy.
    RigidBody(const RigidBody& other);

    /// @brief Destructor for the RigidBody class.
    ~RigidBody();

    /// @brief Create a copy of this RigidBody component.
    /// @return A new RigidBody component that is a copy of this instance.
    Component* Clone() const override;

    /// @brief Update method called per frame.
    /// @param dt The time elapsed since the last frame.
    virtual void OnUpdate(float dt) override;

    /// @brief Fixed update method called at a fixed time step.
    virtual void OnFixedUpdate() override {};

    /// @brief Get the acceleration vector of the rigid body.
    /// @return A pointer to the acceleration vector.
    vec3* getAcceleration();

    /// @brief Get the velocity vector of the rigid body.
    /// @return A pointer to the velocity vector.
    vec3* getVelocity();

    /// @brief Get the old translation vector of the rigid body.
    /// @return A pointer to the old translation vector.
    vec3* getOldTranslation();

    /// @brief Get the rotational velocity of the rigid body.
    /// @return The rotational velocity.
    float getRotationalVelocity();

    /// @brief Set the acceleration vector of the rigid body.
    /// @param Acceleration A pointer to the new acceleration vector.
    void setAcceleration(const vec3* Acceleration);

    /// @brief Set the velocity vector of the rigid body.
    /// @param Velocity A pointer to the new velocity vector.
    void setVelocity(const vec3* Velocity);

    /// @brief Set the old translation vector of the rigid body.
    /// @param OldTranslation A pointer to the old translation vector.
    void setOldTranslation(const vec3* OldTranslation);

    /// @brief Set the rotational velocity of the rigid body.
    /// @param rotational_velocity The new rotational velocity.
    void SetRotationalVelocity(float rotational_velocity);

    /// @brief Handle collision events with other entities.
    /// @param other The other Entity involved in the collision.
    virtual void OnCollisionEvent();

private: // reading

    /// @brief reads the velocity from json
    /// @param data the json data
    void ReadVelocity(Stream data);

    /// @brief reads the acceleration from json
    /// @param data the json data
    void ReadAcceleration(Stream data);

    /// @brief reads the rotationalVelocity from json
    /// @param data the json data
    void ReadRotationalVelocity(Stream data);

    /// @brief the map of read methods for RigidBodys
    static ReadMethodMap< RigidBody > readMethods;

    /// @brief gets the map of read methods for this Component
    /// @return the map of read methods for this Component
    virtual ReadMethodMap< Component > const& getReadMethods() override;

private:
    /// @brief The velocity vector of the rigid body.
    vec3 _velocity;

    /// @brief The acceleration vector of the rigid body.
    vec3 _acceleration;

    /// @brief The old translation vector of the rigid body.
    vec3 _oldTranslation;

    /// @brief The rotational velocity of the rigid body.
    float _rotationalVelocity;
};