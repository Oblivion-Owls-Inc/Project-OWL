///*****************************************************************/
/// @file	 RigidBody.h
/// @Author  Jax Clayton (jax.clayton@digipen.edu)
/// @date	 9/15/2021
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
    /// @return A new RigidBody component that is a copy of this s_Instance.
    Component* Clone() const override;

    /// @brief Update method called per frame.
    /// @param dt The time elapsed since the last frame.
    virtual void OnUpdate(float dt) override;

    /// @brief Fixed update method called at a fixed time step.
    virtual void OnFixedUpdate() override;

    /// @brief  Called whenever a Collider on this Behavior's Entity collides
    /// @param  other           the entity that was collided with
    /// @param  collisionData   additional data about the collision
    virtual void OnCollision( Entity* other, CollisionData const& collisionData ) override;

//-----------------------------------------------------------------------------
public: // accessors
//-----------------------------------------------------------------------------

    /// @brief  Get the acceleration vector of the rigid body.
    /// @return the acceleration vector.
    __inline glm::vec3 const& GetAcceleration() const { return m_Acceleration; }
    /// @brief  Set the acceleration vector of the rigid body.
    /// @param  acceleration    the new acceleration vector.
    __inline void SetAcceleration( vec3 const& acceleration ) { m_Acceleration = acceleration; }

    /// @brief  Get the velocity vector of the rigid body.
    /// @return the velocity vector.
    __inline glm::vec3 const& GetVelocity() const { return m_Velocity; }
    /// @brief  Set the velocity vector of the rigid body.
    /// @param  velocity    the new velocity vector.
    __inline void SetVelocity( vec3 const& velocity ) { m_Velocity = velocity; }

    /// @brief  Get the old translation vector of the rigid body.
    /// @return the old translation vector.
    __inline glm::vec3 const& GetOldTranslation() const { return m_OldTranslation; }

    /// @brief Get the rotational velocity of the rigid body.
    /// @return The rotational velocity.
    __inline float GetRotationalVelocity() { return m_RotationalVelocity; }
    /// @brief Set the rotational velocity of the rigid body.
    /// @param rotationalVelocity The new rotational velocity.
    __inline void SetRotationalVelocity( float rotationalVelocity ) { m_RotationalVelocity = rotationalVelocity; }

    /// @brief Get the inverse mass of the rigid body.
    /// @return The inverse mass.
    __inline float GetInverseMass() { return m_InverseMass; }
    /// @brief Set the inverse mass of the rigid body.
    /// @param inverseMass The new inverse mass.
    __inline void SetInverseMass( float inverseMass ) { m_InverseMass = inverseMass; }

    /// @brief Get the restitution of the rigid body.
    /// @return The restitution.
    __inline float GetRestitution() { return m_Restitution; }
    /// @brief Set the restitution of the rigid body.
    /// @param restitution The new restitution.
    __inline void SetRestitution( float restitution ) { m_Restitution = restitution; }

    /// @brief Get the friction of the rigid body.
    /// @return The friction.
    __inline float GetFriction() { return m_Friction; }
    /// @brief Set the friction of the rigid body.
    /// @param friction The new friction.
    __inline void SetFriction( float friction ) { m_Friction = friction; }

    /// @brief  gets whether the collision between two RigidBodies has already been resolved;
    /// @return whether the collision between two RigidBodies has already been resolved;
    /// @note   SHOULD ONLY BE CALLED BY RigidBody::OnCollision();
    __inline bool GetCollisionResolved() const { return m_CollisionResolved; }
    /// @brief  sets whether the collision between two RigidBodies has already been resolved;
    /// @param  collisionResolved   whether the collision between two RigidBodies has already been resolved;
    /// @note   SHOULD ONLY BE CALLED BY RigidBody::OnCollision();
    __inline void SetCollisionResolved( bool collisionResolved ) { m_CollisionResolved = collisionResolved; }

private: // reading

    /// @brief reads the velocity from json
    /// @param data the json data
    void readVelocity(Stream data);

    /// @brief reads the acceleration from json
    /// @param data the json data
    void readAcceleration(Stream data);

    /// @brief reads the rotationalVelocity from json
    /// @param data the json data
    void readRotationalVelocity(Stream data);

    /// @brief reads the inverseMass from json
    /// @param data the json data
    void readInverseMass(Stream data);

    /// @brief reads the restitution from json
    /// @param data the json data
    void readRestitution(Stream data);

    /// @brief reads the friction from json
    /// @param data the json data
    void readFriction(Stream data);

    /// @brief the map of read methods for RigidBodys
    static ReadMethodMap< RigidBody > s_ReadMethods;

    /// @brief gets the map of read methods for this Component
    /// @return the map of read methods for this Component
    virtual ReadMethodMap< Component > const& GetReadMethods() const override;

private:
    /// @brief The velocity vector of the rigid body.
    vec3 m_Velocity;

    /// @brief The acceleration vector of the rigid body.
    vec3 m_Acceleration;

    /// @brief The old translation vector of the rigid body.
    vec3 m_OldTranslation;

    /// @brief The rotational velocity of the rigid body.
    float m_RotationalVelocity;

    /// @brief the inverse of the mass of this RigidBody
    float m_InverseMass;

    /// @brief how bouncy this RigidBody is
    float m_Restitution;

    /// @brief how much friction this RigidBody has
    float m_Friction;

    /// @brief flag of whether a collision between two rigidBodies has already been resolved;
    bool m_CollisionResolved;
};


