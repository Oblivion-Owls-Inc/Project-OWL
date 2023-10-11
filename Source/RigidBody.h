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
//-----------------------------------------------------------------------------
public: // constructor / destructors
//-----------------------------------------------------------------------------

    /// @brief Default constructor for the RigidBody class.
    RigidBody();

//-----------------------------------------------------------------------------
public: // virtual override methods
//-----------------------------------------------------------------------------

    /// @brief Default constructor for the RigidBody class.
    virtual void OnInit();

    /// @brief  called when this Component's Entity is removed from the Scene
    /// @note   NOT CALLED WHEN THE SCENE IS EXITED - that should be handled by this Component's System
    virtual void OnExit();

    /// @brief Update method called per frame.
    /// @param dt The time elapsed since the last frame.
    virtual void OnUpdate(float dt) override;

    /// @brief Fixed update method called at a fixed time step.
    virtual void OnFixedUpdate() override;

    /// @brief  Called whenever a Collider on this Behavior's Entity collides
    /// @param  other           the entity that was collided with
    /// @param  collisionData   additional data about the collision
    virtual void OnCollision( Entity* other, CollisionData const& collisionData ) override;

    /// @brief Used by the Debug System to display information about this Component
    virtual void Inspector() override;

//-----------------------------------------------------------------------------
public: // accessors
//-----------------------------------------------------------------------------


    /// @brief  Get the acceleration vector of the rigidBody.
    /// @return the acceleration vector.
    __inline glm::vec3 const& GetAcceleration() const { return m_Acceleration; }

    /// @brief  Set the acceleration vector of the rigidBody.
    /// @param  acceleration    the new acceleration vector.
    __inline void SetAcceleration( glm::vec3 const& acceleration ) { m_Acceleration = acceleration; }


    /// @brief  Get the velocity vector of the rigidBody.
    /// @return the velocity vector.
    __inline glm::vec3 const& GetVelocity() const { return m_Velocity; }

    /// @brief  Set the velocity vector of the rigidBody.
    /// @param  velocity    the new velocity vector.
    __inline void SetVelocity( glm::vec3 const& velocity ) { m_Velocity = velocity; }


    /// @brief Get the rotational velocity of the rigidBody.
    /// @return The rotational velocity.
    __inline float GetRotationalVelocity() { return m_RotationalVelocity; }

    /// @brief Set the rotational velocity of the rigidBody.
    /// @param rotationalVelocity The new rotational velocity.
    __inline void SetRotationalVelocity( float rotationalVelocity ) { m_RotationalVelocity = rotationalVelocity; }


    /// @brief Get the mass of the rigidBody.
    /// @return The mass of the rigidBody
    __inline float GetMass() { return m_Mass; }

    /// @brief Set the mass of the rigidBody.
    /// @param mass The new mass.
    __inline void SetMass( float mass ) { m_Mass = mass; }


    /// @brief Get the restitution of the rigidBody.
    /// @return The restitution.
    __inline float GetRestitution() { return m_Restitution; }

    /// @brief Set the restitution of the rigidBody.
    /// @param restitution The new restitution.
    __inline void SetRestitution( float restitution ) { m_Restitution = restitution; }


    /// @brief Get the friction of the rigidBody.
    /// @return The friction.
    __inline float GetFriction() { return m_Friction; }

    /// @brief Set the friction of the rigidBody.
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


//-----------------------------------------------------------------------------
private: // reading
//-----------------------------------------------------------------------------


    /// @brief reads the velocity from json
    /// @param data the json data
    void readVelocity(Stream data);

    /// @brief reads the acceleration from json
    /// @param data the json data
    void readAcceleration(Stream data);

    /// @brief reads the rotationalVelocity from json
    /// @param data the json data
    void readRotationalVelocity(Stream data);


    /// @brief reads the mass from json
    /// @param data the json data
    void readMass(Stream data);

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
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override
    {
        return (ReadMethodMap< ISerializable > const&)s_ReadMethods;
    }

//-----------------------------------------------------------------------------
private: // member variables
//-----------------------------------------------------------------------------

    /// @brief The velocity vector of the rigidBody.
    glm::vec3 m_Velocity;

    /// @brief The acceleration vector of the rigidBody.
    glm::vec3 m_Acceleration;

    /// @brief The rotational velocity of the rigidBody.
    float m_RotationalVelocity;


    /// @brief the mass of this RigidBody
    float m_Mass;

    /// @brief how bouncy this RigidBody is
    float m_Restitution;

    /// @brief how much friction this RigidBody has
    float m_Friction;


    /// @brief flag of whether a collision between two rigidBodies has already been resolved;
    bool m_CollisionResolved;


//-----------------------------------------------------------------------------
private: // copying
//-----------------------------------------------------------------------------


    /// @brief  clones this RigidBody
    /// @return the newly created clone of this RigidBody
    virtual Component* Clone() const override;

    /// @brief  copy-constructor for the RigidBody
    /// @param  other   the other RigidBody to copy
    RigidBody( const RigidBody& other );


//-----------------------------------------------------------------------------
public: // copying
//-----------------------------------------------------------------------------

    // diable = operator
    void operator =( const RigidBody& ) = delete;

//-----------------------------------------------------------------------------
};
