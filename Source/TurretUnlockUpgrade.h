/// @file       TurretUnlockUpgrade.h
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      collectable upgrade which unlocks a turret
/// @version    0.1
/// @date       April 2024
/// 
/// @copyright  Copyright (c) 2024 Digipen Institute of Technology

#pragma once

#include "Component.h"

#include "EntityReference.h"
#include "ComponentReference.h"
class ConstructionBehavior;
class Interactable;


/// @brief  collectable upgrade which unlocks a turret
class TurretUnlockUpgrade : public Component
{
//-----------------------------------------------------------------------------
public: // constructor / Destructor
//-----------------------------------------------------------------------------


    /// @brief  default constructor
    TurretUnlockUpgrade();


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


    /// @brief  the index of the building to unlock
    int m_UnlockBuildingIndex = 1;


    /// @brief  the ConstructionBehavior Component to unlock the building in
    ComponentReference< ConstructionBehavior > m_ConstructionBehavior;

    /// @brief  the Entity the ConstructionBehavior is attached to
    EntityReference m_ConstructionEntity = EntityReference( { &m_ConstructionBehavior } );


    /// @brief  the Interactable Component attached to this Entity
    ComponentReference< Interactable > m_Interactable;


//-----------------------------------------------------------------------------
private: // methods
//-----------------------------------------------------------------------------


    /// @brief  unlocks this upgrade's building
    void unlockBuilding();


//-----------------------------------------------------------------------------
public: // inspection
//-----------------------------------------------------------------------------


    /// @brief  shows the inspector for TurretUnlockUpgrade
    virtual void Inspector() override;


//-----------------------------------------------------------------------------
private: // reading
//-----------------------------------------------------------------------------


    /// @brief  reads the index of the building to unlock
    void readUnlockBuildingIndex( nlohmann::ordered_json const& data );

    /// @brief  reads the Entity the ConstructionBehavior is attached to
    void readConstructionEntity( nlohmann::ordered_json const& data );


//-----------------------------------------------------------------------------
public: // reading / writing
//-----------------------------------------------------------------------------


    /// @brief  gets the map of read methods for this TurretUnlockUpgrade
    /// @return the map of read methods for this TurretUnlockUpgrade
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override;


    /// @brief  writes this TurretUnlockUpgrade to JSON
    /// @return the JSON data of this TurretUnlockUpgrade
    virtual nlohmann::ordered_json Write() const override;

    
//-----------------------------------------------------------------------------
public: // copying
//-----------------------------------------------------------------------------


    /// @brief  clones this TurretUnlockUpgrade
    /// @return the newly created clone of this TurretUnlockUpgrade
    virtual TurretUnlockUpgrade* Clone() const override;


//-----------------------------------------------------------------------------
private: // copying
//-----------------------------------------------------------------------------


    /// @brief  copy-constructor for the TurretUnlockUpgrade
    /// @param  other   the other TurretUnlockUpgrade to copy
    TurretUnlockUpgrade( TurretUnlockUpgrade const& other );


    // disable assignment operator
    void operator =( TurretUnlockUpgrade const& ) = delete;


//-----------------------------------------------------------------------------
};
