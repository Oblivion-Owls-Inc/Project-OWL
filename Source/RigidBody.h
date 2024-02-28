///*****************************************************************/
/// @file	 RigidBody.h
/// @author  Jax Clayton (jax.clayton@digipen.edu)
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
#include "Entity.h"
#include "Behavior.h"

#include "ComponentReference.h"
class Transform;
class Collider;
class StaticBody;

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
public: // methods
//-----------------------------------------------------------------------------

    /// @brief  applies an acceleration to this RigidBody this frame
    /// @param  acceleration    the acceleration to apply
    void ApplyAcceleration( glm::vec2 const& acceleration );

    /// @brief  adds to the Velocity of this Rigidbody
    /// @param  velocity    the velocity to apply
    void ApplyVelocity( glm::vec2 const& velocity );
    
    /// @brief  applies a force to this RigidBody this frame
    /// @param  force   the force to apply
    void ApplyForce( glm::vec2 const& force );

    /// @brief  applies an impulse to this RigidBody
    /// @param  impulse the impulse to apply
    void ApplyImpulse( glm::vec2 const& impulse );

//-----------------------------------------------------------------------------
public: // accessors
//-----------------------------------------------------------------------------

    /// @brief  Get the acceleration vector of the rigidBody.
    /// @return the acceleration vector.
    glm::vec2 const& GetAcceleration() const { return m_Acceleration; }

    /// @brief  Set the acceleration vector of the rigidBody.
    /// @param  acceleration    the new acceleration vector.
    void SetAcceleration( glm::vec2 const& acceleration ) { m_Acceleration = acceleration; }


    /// @brief  Get the velocity vector of the rigidBody.
    /// @return the velocity vector.
    glm::vec2 const& GetVelocity() const { return m_Velocity; }

    /// @brief  Set the velocity vector of the rigidBody.
    /// @param  velocity    the new velocity vector.
    void SetVelocity( glm::vec2 const& velocity ) { m_Velocity = velocity; }


    /// @brief Get the rotational velocity of the rigidBody.
    /// @return The rotational velocity.
    float GetRotationalVelocity() const { return m_RotationalVelocity; }

    /// @brief Set the rotational velocity of the rigidBody.
    /// @param rotationalVelocity The new rotational velocity.
    void SetRotationalVelocity( float rotationalVelocity ) { m_RotationalVelocity = rotationalVelocity; }


    /// @brief Get the mass of the rigidBody.
    /// @return The mass of the rigidBody
    float GetMass() const { return m_Mass; }

    /// @brief Set the mass of the rigidBody.
    /// @param mass The new mass.
    void SetMass( float mass ) { m_Mass = mass; }


    /// @brief Get the restitution of the rigidBody.
    /// @return The restitution.
    float GetRestitution() const { return m_Restitution; }

    /// @brief Set the restitution of the rigidBody.
    /// @param restitution The new restitution.
    void SetRestitution( float restitution ) { m_Restitution = restitution; }


    /// @brief Get the friction of the rigidBody.
    /// @return The friction.
    float GetFriction() const { return m_Friction; }

    /// @brief Set the friction of the rigidBody.
    /// @param friction The new friction.
    void SetFriction( float friction ) { m_Friction = friction; }


    /// @brief Get the drag of the rigidBody.
    /// @return The drag.
    float GetDrag() const { return m_Drag; }

    /// @brief Set the drag of the rigidBody.
    /// @param drag The new drag.
    void SetDrag( float drag ) { m_Drag = drag; }


    /// @brief  gets whether the collision between two RigidBodies has already been resolved;
    /// @return whether the collision between two RigidBodies has already been resolved;
    /// @note   SHOULD ONLY BE CALLED BY RigidBody::OnCollision();
    bool GetCollisionResolved() const { return m_CollisionResolved; }

    /// @brief  sets whether the collision between two RigidBodies has already been resolved;
    /// @param  collisionResolved   whether the collision between two RigidBodies has already been resolved;
    /// @note   SHOULD ONLY BE CALLED BY RigidBody::OnCollision();
    void SetCollisionResolved( bool collisionResolved ) { m_CollisionResolved = collisionResolved; }

//-----------------------------------------------------------------------------
public: // virtual override methods
//-----------------------------------------------------------------------------

    /// @brief Default constructor for the RigidBody class.
    virtual void OnInit() override;

    /// @brief  called when this Component's Entity is removed from the Scene
    /// @note   NOT CALLED WHEN THE SCENE IS EXITED - that should be handled by this Component's System
    virtual void OnExit() override;

    /// @brief Update method called per frame.
    /// @param dt The time elapsed since the last frame.
    virtual void OnUpdate( float dt ) override;

    /// @brief Fixed update method called at a fixed time step.
    virtual void OnFixedUpdate() override;

    /// @brief Used by the Debug System to display information about this Component
    virtual void Inspector() override;

//-----------------------------------------------------------------------------
private: // methods
//-----------------------------------------------------------------------------

    /// @brief  Called whenever a Collider on this Behavior's Entity collides
    /// @param  other           the collider that was collided with
    /// @param  collisionData   additional data about the collision
    void OnCollision( Collider* other, CollisionData const& collisionData );

    /// @brief  resolve collision between this RigidBody and a StaticBody
    /// @param  other           the StaticBody to collide with
    /// @param  collisionData   additional data about the collision
    void CollideWithStatic( StaticBody const* other, CollisionData const& collisionData );

//-----------------------------------------------------------------------------
private: // member variables
//-----------------------------------------------------------------------------

    /// @brief The velocity vector of the rigidBody.
    glm::vec2 m_Velocity = { 0.0f, 0.0f };

    /// @brief The acceleration vector of the rigidBody.
    glm::vec2 m_Acceleration = { 0.0f, 0.0f };

    /// @brief The rotational velocity of the rigidBody.
    float m_RotationalVelocity = 0.0f;


    /// @brief the mass of this RigidBody
    float m_Mass = 1.0f;

    /// @brief how bouncy this RigidBody is
    float m_Restitution = 1.0f;

    /// @brief how much friction this RigidBody has
    float m_Friction = 0.0f;

    /// @brief  how much drag / air resistance this RigidBody has
    float m_Drag = 0.0f;


    /// @brief flag of whether a collision between two rigidBodies has already been resolved;
    bool m_CollisionResolved = false;


    /// @brief  the transform associated with this RigidBody
    ComponentReference< Transform > m_Transform;

    /// @brief  the Collider associated with this RigidBody
    ComponentReference< Collider, false > m_Collider;


//-----------------------------------------------------------------------------
private: // reading
//-----------------------------------------------------------------------------


    /// @brief  reads the velocity from json
    /// @param  data    the json data
    void readVelocity( nlohmann::ordered_json const& data );

    /// @brief  reads the acceleration from json
    /// @param  data    the json data
    void readAcceleration( nlohmann::ordered_json const& data );

    /// @brief  reads the rotationalVelocity from json
    /// @param  data    the json data
    void readRotationalVelocity( nlohmann::ordered_json const& data );


    /// @brief  reads the mass from json
    /// @param  data    the json data
    void readMass( nlohmann::ordered_json const& data );

    /// @brief  reads the restitution from json
    /// @param  data    the json data
    void readRestitution( nlohmann::ordered_json const& data );

    /// @brief  reads the friction from json
    /// @param  data    the json data
    void readFriction( nlohmann::ordered_json const& data );

    /// @brief  reads te drag from json
    /// @param  data    the json data
    void readDrag( nlohmann::ordered_json const& data );


//-----------------------------------------------------------------------------
public: // reading writing
//-----------------------------------------------------------------------------


    /// @brief gets the map of read methods for this Component
    /// @return the map of read methods for this Component
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override;


    /// @brief  Write all RigidBody component data to a JSON file.
    /// @return The JSON file containing the RigidBody component data.
    virtual nlohmann::ordered_json Write() const override;

    
//-----------------------------------------------------------------------------
public: // copying
//-----------------------------------------------------------------------------


    /// @brief  clones this RigidBody
    /// @return the newly created clone of this RigidBody
    virtual RigidBody* Clone() const override;


//-----------------------------------------------------------------------------
private: // copying
//-----------------------------------------------------------------------------


    /// @brief  copy-constructor for the RigidBody
    /// @param  other   the other RigidBody to copy
    RigidBody( RigidBody const& other );


    // diable = operator
    void operator =( RigidBody const& ) = delete;


//-----------------------------------------------------------------------------
};
