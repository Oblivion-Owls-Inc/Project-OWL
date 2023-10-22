///*****************************************************************/
/// @file	 BulletBehavior.h
/// @author  Jax Clayton (jax.clayton@digipen.edu)
/// @date	 9/15/2021
/// @brief   Bullet behavior class 
/// @details Game Behavior that handles bullet movement and collision
///*****************************************************************/

#pragma once
#include "Behavior.h"

class StaticBody;

class BulletBehavior :
    public Behavior
{
//-----------------------------------------------------------------------------
public: // constructor / destructors
//-----------------------------------------------------------------------------
    BulletBehavior();

//-----------------------------------------------------------------------------
public: // methods
//-----------------------------------------------------------------------------

    void SetTarget(Entity* target);

//-----------------------------------------------------------------------------
public: // accessors
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
public: // virtual override methods
//-----------------------------------------------------------------------------

    /// @brief Default constructor for the BulletBehavior class.
    virtual void OnInit() override;

    /// @brief  called when this Component's Entity is removed from the Scene
    /// @note   NOT CALLED WHEN THE SCENE IS EXITED - that should be handled by this Component's System
    virtual void OnExit() override;

    /// @brief  Called whenever a Collider on this Behavior's Entity collides
    /// @param  other           the entity that was collided with
    /// @param  collisionData   additional data about the collision
    virtual void OnCollision(Entity* other, CollisionData const& collisionData);

    /// @brief  resolve collision between this RigidBody and a StaticBody
    /// @param  other           the StaticBody to collide with
    /// @param  collisionData   additional data about the collision
    void CollideWithStatic( StaticBody const* other, CollisionData const& collisionData );

    /// @brief Update method called per frame.
    /// @param dt The time elapsed since the last frame.
    virtual void OnUpdate(float dt) override;

    /// @brief Fixed update method called at a fixed time step.
    virtual void OnFixedUpdate() override;

    /// @brief Used by the Debug System to display information about this Component
    virtual void Inspector() override;
//-----------------------------------------------------------------------------
private: // methods
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
private: // member variables
//-----------------------------------------------------------------------------
  
//-----------------------------------------------------------------------------
private: // reading
//-----------------------------------------------------------------------------
    
//-----------------------------------------------------------------------------
public: // writing
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
private: // copying
//-----------------------------------------------------------------------------


    /// @brief  clones this RigidBody
    /// @return the newly created clone of this RigidBody
    virtual Component* Clone() const override;

    /// @brief  copy-constructor for the RigidBody
    /// @param  other   the other RigidBody to copy
    BulletBehavior( const BulletBehavior& other );


//-----------------------------------------------------------------------------
public: // copying
//-----------------------------------------------------------------------------

    // diable = operator
    void operator =( const BulletBehavior& ) = delete;

};

