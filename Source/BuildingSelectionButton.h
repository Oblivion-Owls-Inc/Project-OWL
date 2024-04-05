/// @file       BuildingSelectionButton.h
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      button that selects which building the ConstructionManager should place
/// @version    0.1
/// @date       DATE HERE
/// 
/// @copyright  Copyright (c) 2024 Digipen Institute of Technology

#pragma once

#include "Behavior.h"

#include "ComponentReference.h"
class UiButton;
class Sprite;

#include "EntityReference.h"
class ConstructionBehavior;

#include "AssetReference.h"
#include "Texture.h"


/// @brief  button that selects which building the ConstructionManager should place
class BuildingSelectionButton : public Behavior
{
//-----------------------------------------------------------------------------
public: // constructor / Destructor
//-----------------------------------------------------------------------------


    /// @brief  default constructor
    BuildingSelectionButton();


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


    /// @brief  called every simulation frame
    virtual void OnFixedUpdate() override;


//-----------------------------------------------------------------------------
private: // members
//-----------------------------------------------------------------------------


    /// @brief  the building index that this button selects
    int m_BuildingIndex = 0;


    /// @brief  texture to display when this button is not selected
    AssetReference< Texture > m_UnselectedTexture;

    /// @brief  texture to display when this button is selected
    AssetReference< Texture > m_SelectedTexture;

    /// @brief  texture to display when this button is locked
    AssetReference< Texture > m_LockedTexture;


    /// @brief  the ConstructionManager Behavior
    ComponentReference< ConstructionBehavior > m_ConstructionBehavior;

    /// @brief  the ConstructionManager Entity
    EntityReference m_ConstructionEntity = EntityReference( { &m_ConstructionBehavior } );


    /// @brief  the UiButton attached to this Entity
    ComponentReference< UiButton > m_UiButton;

    /// @brief  the Sprite attached to this Entity
    ComponentReference< Sprite > m_Sprite;


//-----------------------------------------------------------------------------
private: // methods
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
public: // inspection
//-----------------------------------------------------------------------------


    /// @brief  shows the inspector for BuildingSelectionButton
    virtual void Inspector() override;


//-----------------------------------------------------------------------------
private: // reading
//-----------------------------------------------------------------------------


    /// @brief  reads the building index that this button selects
    /// @param  data    the JSON data to read from
    void readBuildingIndex( nlohmann::ordered_json const& data );


    /// @brief  reads the texture to display when this button is not selected
    /// @param  data    the JSON data to read from
     void readUnselectedTexture( nlohmann::ordered_json const& data );

    /// @brief  reads the texture to display when this button is selected
    /// @param  data    the JSON data to read from
     void readSelectedTexture( nlohmann::ordered_json const& data );

     /// @brief  reads the texture to display when this button is locked
     /// @param  data    the JSON data to read from
     void readLockedTexture( nlohmann::ordered_json const& data );


    /// @brief  reads the ConstructionManager Entity
    /// @param  data    the JSON data to read from
    void readConstructionEntity( nlohmann::ordered_json const& data );


//-----------------------------------------------------------------------------
public: // reading / writing
//-----------------------------------------------------------------------------


    /// @brief  gets the map of read methods for this BuildingSelectionButton
    /// @return the map of read methods for this BuildingSelectionButton
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override;


    /// @brief  writes this BuildingSelectionButton to JSON
    /// @return the JSON data of this BuildingSelectionButton
    virtual nlohmann::ordered_json Write() const override;

    
//-----------------------------------------------------------------------------
public: // copying
//-----------------------------------------------------------------------------


    /// @brief  clones this BuildingSelectionButton
    /// @return the newly created clone of this BuildingSelectionButton
    virtual BuildingSelectionButton* Clone() const override;


//-----------------------------------------------------------------------------
private: // copying
//-----------------------------------------------------------------------------


    /// @brief  copy-constructor for the BuildingSelectionButton
    /// @param  other   the other BuildingSelectionButton to copy
    BuildingSelectionButton( BuildingSelectionButton const& other );


    // disable assignment operator
    void operator =( BuildingSelectionButton const& ) = delete;


//-----------------------------------------------------------------------------
};
