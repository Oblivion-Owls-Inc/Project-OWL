/// @file       ResourcesUiManager.h
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      Component that manages the UI for displaying what resources a player currently has
/// @version    0.1
/// @date       2024-02-21
/// 
/// @copyright  Copyright (c) 2024 Digipen Institute of Technology

#pragma once

#include "Component.h"

#include "ItemStack.h"

#include "AssetReference.h"
#include "EntityReference.h"
#include "ComponentReference.h"
class Inventory;
class UiElement;
class Sprite;
class ResourceCounterUi;



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

    
    /// @brief  whether to hide the box sprite when empty
    bool m_HideWhenEmpty = false;


    /// @brief  the amount of space between the top and bottom of the box and the resource counters
    float m_Padding = true;

    /// @brief  the amount of space between each resource counter
    float m_Spacing = true;


    /// @brief  prefab to spawn of a counter of a single resource type
    AssetReference< Entity > m_ResourceCounterPrefab;


    /// @brief  the Inventory component to display the contents of
    ComponentReference< Inventory > m_Inventory;

    /// @brief  reference to the Entity that has the Inventory to display
    EntityReference m_InventoryEntity = EntityReference( { &m_Inventory } );


    /// @brief  the UiElement of the parent box containing the resource counters
    ComponentReference< UiElement > m_UiElement;

    /// @brief  the Sprite Component of the box containing the resource counters
    ComponentReference< Sprite > m_Sprite;


    /// @brief  The Resource Counters that are children of this UiElement
    std::vector< ResourceCounterUi* > m_ResourceCounters;


//-----------------------------------------------------------------------------
private: // methods
//-----------------------------------------------------------------------------


    /// @brief  sets up the item counters using the contents of the Inventory
    void setupItemCounters();

    /// @brief  clears all Item Counters from this Component
    void clearItemCounters();


    /// @brief  callback which gets called after an ItemStack is added to the Inventory
    /// @param  itemStack   the ItemStack that was added
    void onItemStackCallback( ItemStack const& itemStack );


    /// @brief  updates the resource counter of the item of the itemstack with the count of the itemstack
    /// @param  itemStack   the itemStack representing the resource counter to change, and what to change it to
    void updateResourceCounter( ItemStack const& itemStack );

    /// @brief  adds and initializes a ResourceCounter
    /// @param  itemStack   the ItemStack to initialize the ResourceCounter with
    void addResourceCounter( ItemStack const& itemStack );

    /// @brief  removes a resource counter from this ResourceUiManager
    /// @param  resourceCounter iterator of the resourceCounter to remove
    void removeResourceCounter( std::vector< ResourceCounterUi* >::iterator const& resourceCounter );


    /// @brief  updates the Transforms off all UiElements in the resources ui
    void updateTransforms();


//-----------------------------------------------------------------------------
public: // inspection
//-----------------------------------------------------------------------------


    /// @brief  shows the inspector for ResourcesUiManager
    virtual void Inspector() override;


//-----------------------------------------------------------------------------
private: // reading
//-----------------------------------------------------------------------------


    /// @brief  reads whether to hide the box sprite when empty
    /// @param  data    the JSON data to read from
    void readHideWhenEmpty( nlohmann::ordered_json const& data );


    /// @brief  reads the amount of space between the top and bottom of the box and the resource counters
    /// @param  data    the JSON data to read from
    void readPadding( nlohmann::ordered_json const& data );

    /// @brief  reads the amount of space between the top and bottom of the box and the resource counters
    /// @param  data    the JSON data to read from
    void readSpacing( nlohmann::ordered_json const& data );


    /// @brief  reads the reference to the Entity that has the Inventory to display
    /// @param  data    the JSON data to read from
    void readInventoryEntity( nlohmann::ordered_json const& data );

    /// @brief  reads the prefab to spawn of a counter of a single resource type
    /// @param  data    the JSON data to read from
    void readResourceCounterPrefab( nlohmann::ordered_json const& data );


//-----------------------------------------------------------------------------
public: // reading / writing
//-----------------------------------------------------------------------------


    /// @brief  gets the map of read methods for this ResourcesUiManager
    /// @return the map of read methods for this ResourcesUiManager
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override;


    /// @brief  writes this ResourcesUiManager to JSON
    /// @return the JSON data of this ResourcesUiManager
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
