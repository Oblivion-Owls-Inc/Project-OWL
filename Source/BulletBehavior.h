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

class BulletBehavior : public Behavior
{
//-----------------------------------------------------------------------------
public: // constructor / destructors
//-----------------------------------------------------------------------------

    /// @brief  constructor
    BulletBehavior();

//-----------------------------------------------------------------------------
public: // accessors
//-----------------------------------------------------------------------------

    /// @brief Set the damage the bullet will do
    /// @param damage - the damage the bullet will do
    void SetDamage( int damage) { m_Damage = damage; }

//-----------------------------------------------------------------------------
public: // virtual override methods
//-----------------------------------------------------------------------------

    /// @brief Default constructor for the BulletBehavior class.
    virtual void OnInit() override;

    /// @brief  called when this Component's Entity is removed from the Scene
    /// @note   NOT CALLED WHEN THE SCENE IS EXITED - that should be handled by this Component's System
    virtual void OnExit() override;

    /// @brief Fixed update method called at a fixed time step.
    virtual void OnFixedUpdate() override;

    /// @brief Used by the Debug System to display information about this Component
    virtual void Inspector() override;

//-----------------------------------------------------------------------------
private: // methods
//-----------------------------------------------------------------------------

    /// @brief  called whenever this Entity's Collider enters a collision
    /// @param  other   the collider that was collided with
    void onCollisionEnter( Collider* other );

//-----------------------------------------------------------------------------
private: // member variables
//-----------------------------------------------------------------------------

    /// @brief  how much damage this Bullet will do
    int m_Damage = 1;

    /// @brief  the lifetime of this Bullet
    Pool<float> m_LifeTime;

//-----------------------------------------------------------------------------
private: // reading
//-----------------------------------------------------------------------------

    /// @brief  reads how long this Bullet will last
    /// @param  data    the json data to read from
    void readLifetime( nlohmann::ordered_json const& data );

    /// @brief  reads this Bullet's damage
    /// @param  data    the json data to read from
    void readDamage( nlohmann::ordered_json const& data );

    /// @brief the map of read methods for this Component
    static ReadMethodMap< BulletBehavior > const s_ReadMethods;


//-----------------------------------------------------------------------------
public: // reading / writing
//-----------------------------------------------------------------------------

    /// @brief gets the map of read methods for this Component
    /// @return the map of read methods for this Component
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override
    {
        return (ReadMethodMap< ISerializable > const&)s_ReadMethods;
    }

    /// @brief  writes this BulletBehavior to json
    /// @return the written json data
    virtual nlohmann::ordered_json Write() const;

//-----------------------------------------------------------------------------
private: // copying
//-----------------------------------------------------------------------------

    /// @brief  copy-constructor for the RigidBody
    /// @param  other   the other RigidBody to copy
    BulletBehavior( const BulletBehavior& other );

//-----------------------------------------------------------------------------
public: // copying
//-----------------------------------------------------------------------------

    /// @brief  clones this RigidBody
    /// @return the newly created clone of this RigidBody
    virtual BulletBehavior* Clone() const override
    {
        return new BulletBehavior( *this );
    }

    // diable = operator
    void operator =( const BulletBehavior& ) = delete;

//-----------------------------------------------------------------------------
};

