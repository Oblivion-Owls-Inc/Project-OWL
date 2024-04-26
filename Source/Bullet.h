///*****************************************************************/
/// @file	    Bullet.h
/// @author     Jax Clayton (jax.clayton@digipen.edu)
/// @date	    9/15/2021
/// @brief      Component that destroys itself and deals damage on contact
/// 
/// @copyright  Copyright (c) 2024 Digipen Institute of Technology
///*****************************************************************/

#pragma once


/// @brief   Component that destroys itself and deals damage on contact
class Bullet : public Component
{
//-----------------------------------------------------------------------------
public: // constructor / destructors
//-----------------------------------------------------------------------------


    /// @brief  constructor
    Bullet();

    /// @brief  derived constructor
    Bullet(std::type_index m_Type);


//-----------------------------------------------------------------------------
public: // accessors
//-----------------------------------------------------------------------------


    /// @brief Set the damage the bullet will do
    /// @param damage - the damage the bullet will do
    void SetDamage( int damage );

    /// @brief Get the damage the bullet will do
    int GetDamage() const;


//-----------------------------------------------------------------------------
public: // virtual override methods
//-----------------------------------------------------------------------------


    /// @brief Default constructor for the Bullet class.
    virtual void OnInit() override;

    /// @brief  called when this Component's Entity is removed from the Scene
    virtual void OnExit() override;


//-----------------------------------------------------------------------------
private: // member variables
//-----------------------------------------------------------------------------


    /// @brief  how much damage this Bullet will do
    int m_Damage = 1;


    /// @brief  the Collider attached to this Bullet
    ComponentReference< Collider > m_Collider;

    
//-----------------------------------------------------------------------------
private: // methods
//-----------------------------------------------------------------------------


    /// @brief  called whenever this Entity's Collider enters a collision
    /// @param  other   the collider that was collided with
    virtual void onCollisionEnter( Collider* other );


//-----------------------------------------------------------------------------
public: // inspection
//-----------------------------------------------------------------------------


    /// @brief Used by the Debug System to display information about this Component
    virtual void Inspector() override;

    
//-----------------------------------------------------------------------------
public: // copying
//-----------------------------------------------------------------------------


    /// @brief  clones this RigidBody
    /// @return the newly created clone of this RigidBody
    virtual Bullet* Clone() const override;


//-----------------------------------------------------------------------------
private: // copying
//-----------------------------------------------------------------------------


    /// @brief  copy-constructor for the RigidBody
    /// @param  other   the other RigidBody to copy
    Bullet( const Bullet& other );


    // disable assignment operator
    void operator =( const Bullet& ) = delete;


//-----------------------------------------------------------------------------
};

