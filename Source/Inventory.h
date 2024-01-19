/// @file       Inventory.h
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      Inventory component that stores items
/// @version    0.1
/// @date       2023-10-20
/// 
/// @copyright  Copyright (c) 2023 Digipen Institute of Technology

#pragma once

#include "Component.h"

#include "ItemStack.h"
#include <vector>


/// @brief  Inventory component that stores items
class Inventory : public Component
{
//-----------------------------------------------------------------------------
public: // types
//-----------------------------------------------------------------------------

    
//-----------------------------------------------------------------------------
private: // types
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
public: // accessors
//-----------------------------------------------------------------------------


    /// @brief  gets the items in this inventory
    /// @return the items in this inventory
    std::vector< ItemStack > const& GetItems() const;


//-----------------------------------------------------------------------------
public: //  methods
//-----------------------------------------------------------------------------


    /// @brief  adds an itemStack to this Inventory
    /// @param  itemStack   the itemStack to add to the inventory
    void AddItemStack( ItemStack const& itemStack );

    /// @brief  adds a collection of itemStack to this Inventory
    /// @param  itemStacks  the itemStacks to add to the inventory
    void AddItemStacks( std::vector< ItemStack > const& itemStacks );


    /// @brief  removes the specified items from the Inventory
    /// @param  itemStack   the type and number of items to remove
    void RemoveItemStack( ItemStack const& itemStack );

    /// @brief  removes a collection of itemStack to this Inventory
    /// @param  itemStacks  the itemStacks to remove to the inventory
    void RemoveItemStacks( std::vector< ItemStack > const& itemStacks );


    /// @brief  checks if the Inventory contains the specified items
    /// @param  itemStack   the itemStack to check if the Inventory contains
    /// @return whether the inventory contains the items
    bool ContainsItemStack( ItemStack const& itemStack ) const;

    /// @brief  checks if the Inventory contains the specified item stacks
    /// @param  itemStacks  the itemStacks to check if the Inventory contains
    /// @return whether the inventory contains the item stacks
    bool ContainsItemStacks(  std::vector< ItemStack > const& itemStacks ) const;


//-----------------------------------------------------------------------------
private: // virtual override methods
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
private: // members
//-----------------------------------------------------------------------------


    /// @brief  the items in this inventory
    std::vector< ItemStack > m_Items;


//-----------------------------------------------------------------------------
private: // helper methods
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
private: // inspection
//-----------------------------------------------------------------------------

    
    /// @brief  displays this Inventory in the Inspector
    virtual void Inspector() override;


//-----------------------------------------------------------------------------
private: // reading
//-----------------------------------------------------------------------------


    /// @brief  reads the ItemStack that this Inventory holds
    /// @param  data    the json data to read from
    void readItems( nlohmann::ordered_json const& data );


//-----------------------------------------------------------------------------
public: // reading / writing
//-----------------------------------------------------------------------------


    /// @brief  gets the map of read methods for this Component
    /// @return the map of read methods for this Component
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override;


    /// @brief  Write all Inventory data to a JSON file.
    /// @return The JSON file containing the Inventory data.
    virtual nlohmann::ordered_json Write() const override;


//-----------------------------------------------------------------------------
public: // constructor / Destructor
//-----------------------------------------------------------------------------


    /// @brief  constructor
    Inventory();


//-----------------------------------------------------------------------------
public: // copying
//-----------------------------------------------------------------------------


    /// @brief  clones this Inventory
    /// @return the newly created clone of this Inventory
    virtual Inventory* Clone() const override;


//-----------------------------------------------------------------------------
private: // copying
//-----------------------------------------------------------------------------


    /// @brief  copy-constructor for the Inventory
    /// @param  other   the other Inventory to copy
    Inventory( const Inventory& other );

    // diable = operator
    void operator =( Inventory const& ) = delete;

//-----------------------------------------------------------------------------
};
