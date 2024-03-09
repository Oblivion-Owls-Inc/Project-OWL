///*****************************************************************/
/// @file	    BulletAoe.h
/// @author     Tyler Birdsall (tyler.birdsall@digipen.edu)
/// @date	    3//2024
/// @brief      Component that destroys itself and deals aoe damage on contact
/// 
/// @copyright  Copyright (c) 2024 Digipen Institute of Technology
///*****************************************************************/

#pragma once

#include "Component.h"
#include "Bullet.h"

#include "ComponentReference.h"
#include "AssetReference.h"
class Collider;


/// @brief   Component that destroys itself and deals aoe damage on contact
class BulletAoe : public Bullet
{
//-----------------------------------------------------------------------------
public: // constructor / destructors
//-----------------------------------------------------------------------------

    /// @brief  constructor
    BulletAoe();


//-----------------------------------------------------------------------------
private: /// Members
//-----------------------------------------------------------------------------

    /// @brief The aoe pulse prefab to spawn
    AssetReference< Entity > m_AoePulsePrefab;

//-----------------------------------------------------------------------------
private: // methods
//-----------------------------------------------------------------------------


    /// @brief  called whenever this Entity's Collider enters a collision
    /// @param  other   the collider that was collided with
    void onCollisionEnter( Collider* other ) override;

//-----------------------------------------------------------------------------
public: // inspection
//-----------------------------------------------------------------------------


    /// @brief Used by the Debug System to display information about this Component
    virtual void Inspector() override;

//-----------------------------------------------------------------------------
private: // reading
//-----------------------------------------------------------------------------

    /// @brief Reads the name of the AoePulse prefab to grab from AssetLib
    /// @param data - the json data to read from
    void readAoePulsePrefab(nlohmann::ordered_json const& data);

    /// @brief  reads this BulletAoe's damage
    /// @param  data    the json data to read from
    void readDamage( nlohmann::ordered_json const& data );


//-----------------------------------------------------------------------------
public: // reading / writing
//-----------------------------------------------------------------------------


    /// @brief gets the map of read methods for this Component
    /// @return the map of read methods for this Component
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override;

    /// @brief  writes this BulletAoe to json
    /// @return the written json data
    virtual nlohmann::ordered_json Write() const override;

    
//-----------------------------------------------------------------------------
public: // copying
//-----------------------------------------------------------------------------


    /// @brief  clones this RigidBody
    /// @return the newly created clone of this RigidBody
    virtual BulletAoe* Clone() const override;


//-----------------------------------------------------------------------------
private: // copying
//-----------------------------------------------------------------------------


    /// @brief  copy-constructor for the RigidBody
    /// @param  other   the other RigidBody to copy
    BulletAoe( const BulletAoe& other );


    // disable assignment operator
    void operator =( const BulletAoe& ) = delete;


//-----------------------------------------------------------------------------
};

