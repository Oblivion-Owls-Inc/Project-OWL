///*****************************************************************/
/// @file	 BulletBehavior.h
/// @author  Jax Clayton (jax.clayton@digipen.edu)
/// @date	 9/15/2021
/// @brief   Bullet behavior class 
/// @details Game Behavior that handles bullet movement and collision
///*****************************************************************/

#pragma once
#include "Behavior.h"
#include "Pool.h"

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

    /// @brief Set the target the bullet is aiming at
    /// @param target - the target to aim at
    void SetTarget(RayCastHit target);

    /// @brief Set the damage the bullet will do
    /// @param damage - the damage the bullet will do
    __inline void SetBulletDamage(float damage) { m_BulletDamage = damage; }

    /// @brief Sets the speed of the bullet
    /// @param speed - the speed of the bullet
    __inline void SetBulletSpeed(float speed) { m_BulletSpeed = speed; }


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
    virtual void OnCollision(Collider* other, CollisionData const& collisionData);

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
    RayCastHit m_Target;

    float m_BulletDamage;

    float m_BulletSpeed;

    Pool<float> m_BulletLifeTime;

//-----------------------------------------------------------------------------
private: // reading
//-----------------------------------------------------------------------------

    ///
    void readPool(nlohmann::ordered_json const& data);


    /// @brief the map of read methods for this Component
    static ReadMethodMap< BulletBehavior > const s_ReadMethods;

    /// @brief gets the map of read methods for this Component
    /// @return the map of read methods for this Component
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override
    {
        return (ReadMethodMap< ISerializable > const&)s_ReadMethods;
    }

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

