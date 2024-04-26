///*****************************************************************/
/// @file	    BulletAoePulse.h
/// @author     Tyler Birdsall (tyler.birdsall@digipen.edu)
/// @date	    3//2024
/// @brief      Component that destroys itself and deals aoe damage on contact
/// 
/// @copyright  Copyright (c) 2024 Digipen Institute of Technology
///*****************************************************************/

#pragma once
#include "Bullet.h"


/// @brief   Component that destroys itself and deals aoe damage on contact
class BulletAoePulse : public Bullet
{
//-----------------------------------------------------------------------------
public: // constructor / destructors
//-----------------------------------------------------------------------------

    /// @brief  constructor
    BulletAoePulse();

//-----------------------------------------------------------------------------
public: // virtual override methods
//-----------------------------------------------------------------------------

    /// @brief Default constructor for the BulletAoePulse class.
    virtual void OnInit() override;

//-----------------------------------------------------------------------------
private: // member variables
//-----------------------------------------------------------------------------

    /// @brief  how much damage this Bullet will do
    int m_Damage = 1;

    /// @brief  how large the aoe pulse is
    float m_Radius = 1.0f;

//-----------------------------------------------------------------------------
public: // accessors
//-----------------------------------------------------------------------------

    /// @brief Set the damage the bullet will do
    /// @param damage - the damage the bullet will do
    void SetDamage(int damage);

    /// @brief get the damage the bullet will do
    int GetDamage();

    /// @brief Set the radius of the bullet pulse
    /// @param radius - the radius of the bullet pulse
    void SetRadius(float radius);

    /// @brief Get the radius of the bullet pulse
    float GetRadius();

//-----------------------------------------------------------------------------
public: // inspection
//-----------------------------------------------------------------------------


    /// @brief Used by the Debug System to display information about this Component
    virtual void Inspector() override;

//-----------------------------------------------------------------------------
private: // reading
//-----------------------------------------------------------------------------

    /// @brief  reads this BulletAoePulse's radius
    /// @param  data    the json data to read from
    void readRadius(nlohmann::ordered_json const& data);


//-----------------------------------------------------------------------------
public: // reading / writing
//-----------------------------------------------------------------------------


    /// @brief gets the map of read methods for this Component
    /// @return the map of read methods for this Component
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override;

    /// @brief  writes this BulletAoePulse to json
    /// @return the written json data
    virtual nlohmann::ordered_json Write() const override;

    
//-----------------------------------------------------------------------------
public: // copying
//-----------------------------------------------------------------------------


    /// @brief  clones this RigidBody
    /// @return the newly created clone of this RigidBody
    virtual BulletAoePulse* Clone() const override;


//-----------------------------------------------------------------------------
private: // copying
//-----------------------------------------------------------------------------


    /// @brief  copy-constructor for the RigidBody
    /// @param  other   the other RigidBody to copy
    BulletAoePulse( const BulletAoePulse& other );


    // disable assignment operator
    void operator =( const BulletAoePulse& ) = delete;


//-----------------------------------------------------------------------------
};

