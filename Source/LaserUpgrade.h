/// @file       LaserUpgrade.h
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      collectable upgrade to the player's MiningLaser
/// @version    0.1
/// @date       April 2024
/// 
/// @copyright  Copyright (c) 2024 Digipen Institute of Technology

#pragma once

#include "Component.h"

#include "EntityReference.h"
#include "ComponentReference.h"
class MiningLaser;
class Interactable;


/// @brief  collectable upgrade to the player's MiningLaser
class LaserUpgrade : public Component
{
//-----------------------------------------------------------------------------
public: // constructor / Destructor
//-----------------------------------------------------------------------------


    /// @brief  default constructor
    LaserUpgrade();


//-----------------------------------------------------------------------------
public: // methods
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
public: // accessors
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
public: // virtual override methods
//-----------------------------------------------------------------------------


    /// @brief  called once when entering the scene
    virtual void OnInit() override;

    /// @brief  called once when exiting the scene
    virtual void OnExit() override;


//-----------------------------------------------------------------------------
private: // members
//-----------------------------------------------------------------------------


    /// @brief  how much the mining speed is multiplied by
    float m_SpeedMultiplier = 2.0f;

    /// @brief  how much the damage of the mining laser is multiplied by
    float m_DamageMultiplier = 2.0f;

    /// @brief  how much the lasers range is increased by
    float m_RangeIncrease = 1.0f;

    /// @brief  how much the max toughness the laser can mine is increased by
    float m_MaxToughnessIncrease = 1.0f;


    /// @brief  the MiningLaser to upgrade
    ComponentReference< MiningLaser > m_MiningLaser;

    /// @brief  the Entity the MiningLaser is attached to
    EntityReference m_MiningLaserEntity = EntityReference( { &m_MiningLaser } );


    /// @brief  the Interactable Component attached to this Entity
    ComponentReference< Interactable > m_Interactable;


//-----------------------------------------------------------------------------
private: // methods
//-----------------------------------------------------------------------------


    /// @brief  upgrades the laser
    void upgradeLaser();


//-----------------------------------------------------------------------------
public: // inspection
//-----------------------------------------------------------------------------


    /// @brief  shows the inspector for LaserUpgrade
    virtual void Inspector() override;


//-----------------------------------------------------------------------------
private: // reading
//-----------------------------------------------------------------------------


    /// @brief  reads how much the mining speed is multiplied by
    /// @param  data    the JSON data to read from
    void readSpeedMultiplier( nlohmann::ordered_json const& data );

    /// @brief  reads how much the damage of the mining laser is multiplied by
    /// @param  data    the JSON data to read from
    void readDamageMultiplier( nlohmann::ordered_json const& data );

    /// @brief  reads how much the lasers range is increased by
    /// @param  data    the JSON data to read from
    void readRangeIncrease( nlohmann::ordered_json const& data );

    /// @brief  reads how much the max toughness the laser can mine is increased by
    /// @param  data    the JSON data to read from
    void readMaxToughnessIncrease( nlohmann::ordered_json const& data );

    /// @brief  reads the Entity the MiningLaser is attached to
    /// @param  data    the JSON data to read from
    void readMiningLaserEntity( nlohmann::ordered_json const& data );


//-----------------------------------------------------------------------------
public: // reading / writing
//-----------------------------------------------------------------------------


    /// @brief  gets the map of read methods for this LaserUpgrade
    /// @return the map of read methods for this LaserUpgrade
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override;


    /// @brief  writes this LaserUpgrade to JSON
    /// @return the JSON data of this LaserUpgrade
    virtual nlohmann::ordered_json Write() const override;

    
//-----------------------------------------------------------------------------
public: // copying
//-----------------------------------------------------------------------------


    /// @brief  clones this LaserUpgrade
    /// @return the newly created clone of this LaserUpgrade
    virtual LaserUpgrade* Clone() const override;


//-----------------------------------------------------------------------------
private: // copying
//-----------------------------------------------------------------------------


    /// @brief  copy-constructor for the LaserUpgrade
    /// @param  other   the other LaserUpgrade to copy
    LaserUpgrade( LaserUpgrade const& other );


    // disable assignment operator
    void operator =( LaserUpgrade const& ) = delete;


//-----------------------------------------------------------------------------
};
