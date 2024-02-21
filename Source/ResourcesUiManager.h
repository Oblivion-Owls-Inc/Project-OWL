/// @file       ResourcesUiManager.h
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      Component that manages the UI for displaying what resources a player currently has
/// @version    0.1
/// @date       2024-02-21
/// 
/// @copyright  Copyright (c) 2024 Digipen Institute of Technology

#pragma once

#include "Component.h"

#include "ComponentReference.h"
class UiElement;
class Sprite;
class Text;

#include "EntityReference.h"
class Inventory;

#include "AssetReference.h"


/// @brief  Component that manages the UI for displaying what resources a player currently has
class ResourcesUiManager : public Component
{
//-----------------------------------------------------------------------------
public: // constructor / Destructor
//-----------------------------------------------------------------------------


    /// @brief  default constructor
    ResourcesUiManager();



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

    
    /// @brief  hides the box sprite when empty
    bool m_HideWhenEmpty;


    /// @brief  the amount of space between the top and bottom of the box and the resource counters
    float m_Padding;

    /// @brief  the amount of space between each resource counter
    float m_CounterOffsets;


    /// @brief  prefab to spawn of a counter of a single resource type
    AssetReference< Entity > m_ResourceCounterUiPrefab;


    /// @brief  the Inventory component to display the contents of
    ComponentReference< Inventory > m_Inventory;

    /// @brief  reference to the Entity that has the Inventory to display
    EntityReference m_InventoryEntity = EntityReference( { &m_Inventory } );


    /// @brief  the UiElement of the parent box containing the resource counters
    ComponentReference< UiElement > m_UiElement;

    /// @brief  the Sprite Component of the box containing the resource counters
    ComponentReference< Sprite > m_Sprite;


//-----------------------------------------------------------------------------
private: // methods
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
public: // inspection
//-----------------------------------------------------------------------------


    /// @brief  shows the inspector for ResourcesUiManager
    virtual void Inspector() override;


//-----------------------------------------------------------------------------
private: // reading
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
public: // reading / writing
//-----------------------------------------------------------------------------


    /// @brief  gets the map of read methods for this Component
    /// @return the map of read methods for this Component
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override;


    /// @brief  Writes all HomeBase data to a JSON file.
    /// @return The JSON file containing the data.
    virtual nlohmann::ordered_json Write() const override;

    
//-----------------------------------------------------------------------------
public: // copying
//-----------------------------------------------------------------------------


    /// @brief  clones this ResourcesUiManager
    /// @return the newly created clone of this ResourcesUiManager
    virtual ResourcesUiManager* Clone() const override;


//-----------------------------------------------------------------------------
private: // copying
//-----------------------------------------------------------------------------


    /// @brief  copy-constructor for the ResourcesUiManager
    /// @param  other   the other ResourcesUiManager to copy
    ResourcesUiManager( ResourcesUiManager const& other );


    // disable assignment operator
    void operator =( ResourcesUiManager const& ) = delete;


//-----------------------------------------------------------------------------
};
