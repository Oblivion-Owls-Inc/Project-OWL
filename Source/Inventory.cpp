/// @file       Inventory.cpp
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      Inventory component that stores items
/// @version    0.1
/// @date       2023-10-20
/// 
/// @copyright  Copyright (c) 2023 Digipen Institute of Technology

#include "pch.h" // precompiled header has to be included first
#include "Inventory.h"

#include "Inspection.h"

//-----------------------------------------------------------------------------
// public: types
//-----------------------------------------------------------------------------

    
//-----------------------------------------------------------------------------
// private: types
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// public: accessors
//-----------------------------------------------------------------------------


    /// @brief  gets the items in this inventory
    /// @return the items in this inventory
    std::vector< ItemStack > const& Inventory::GetItems() const
    {
        return m_Items;
    }


//-----------------------------------------------------------------------------
// public:  methods
//-----------------------------------------------------------------------------


    /// @brief  adds an itemStack to this Inventory
    /// @param  newStack    the itemStack to add to the inventory
    void Inventory::AddItemStack( ItemStack const& newStack )
    {
        for ( ItemStack& oldStack : m_Items )
        {
            if ( oldStack.M_ItemId != newStack.M_ItemId )
            {
                continue;
            }

            oldStack.M_Count += newStack.M_Count;

            for ( auto& [ ownerId, callback ] : m_OnRemoveItemStackCallbacks )
            {
                callback( newStack );
            }

            return;
        }

        m_Items.push_back( newStack );

        for ( auto& [ ownerId, callback ] : m_OnRemoveItemStackCallbacks )
        {
            callback( newStack );
        }
    }

    /// @brief  adds a collection of itemStack to this Inventory
    /// @param  itemStacks  the itemStacks to add to the inventory
    void Inventory::AddItemStacks( std::vector< ItemStack > const& itemStacks )
    {
        for ( ItemStack const& stack : itemStacks )
        {
            AddItemStack( stack );
        }
    }


    /// @brief  removes the specified items from the Inventory
    /// @param  removeStack the type and number of items to remove
    void Inventory::RemoveItemStack( ItemStack const& removeStack )
    {
        for ( int i = 0; i < m_Items.size(); ++i )
        {
            if ( m_Items[ i ].M_ItemId != removeStack.M_ItemId )
            {
                continue;
            }

            m_Items[ i ].M_Count -= removeStack.M_Count;

            if ( m_Items[ i ].M_Count <= 0 )
            {
                m_Items.erase( m_Items.begin() + i );
            }

            for ( auto& [ ownerId, callback ] : m_OnRemoveItemStackCallbacks )
            {
                callback( removeStack );
            }

            return;
        }
    }

    /// @brief  removes a collection of itemStack to this Inventory
    /// @param  itemStacks  the itemStacks to remove to the inventory
    void Inventory::RemoveItemStacks( std::vector< ItemStack > const& itemStacks )
    {
        for ( ItemStack const& stack : itemStacks )
        {
            RemoveItemStack( stack );
        }
    }


    /// @brief  checks if the Inventory contains the specified items
    /// @param  checkStack  the itemStack to check if the Inventory contains
    /// @return whether the inventory contains the items
    bool Inventory::ContainsItemStack( ItemStack const& checkStack ) const
    {
        for ( ItemStack const& stack : m_Items )
        {
            if ( stack.M_ItemId == checkStack.M_ItemId )
            {
                return stack.M_Count >= checkStack.M_Count;
            }
        }

        return false;
    }

    /// @brief  checks if the Inventory contains the specified item stacks
    /// @param  itemStacks  the itemStacks to check if the Inventory contains
    /// @return whether the inventory contains the item stacks
    bool Inventory::ContainsItemStacks( std::vector< ItemStack > const& itemStacks ) const
    {
        for ( ItemStack const& stack : itemStacks )
        {
            if ( ContainsItemStack( stack ) == false )
            {
                return false;
            }
        }

        return true;
    }


    /// @brief  gets the number of items of the specified type from this Inventory
    /// @param  itemId  the ID of the type of item to get the count of
    /// @return the amount of that type of item in the Inventory
    int Inventory::GetItemCount( int itemId ) const
    {
        auto it = std::find_if(
            m_Items.begin(), m_Items.end(),
            [ itemId ]( ItemStack const& itemStack ) -> bool
            {
                return itemStack.M_ItemId == itemId;
            }
        );
        if ( it == m_Items.end() )
        {
            return 0;
        }

        return it->M_Count;
    }


    /// @brief  clears all items from this Inventory
    void Inventory::Clear()
    {
        for ( ItemStack& itemStack : m_Items )
        {
            ItemStack oldItemStack = itemStack;
            itemStack.M_Count = 0;
            for ( auto& [ ownerId, callback ] : m_OnRemoveItemStackCallbacks )
            {
                callback( oldItemStack );
            }
        }

        m_Items.clear();
    }


    /// @brief  adds a callback to this Inventory that gets called whenever an ItemStack is added to the Inventory
    /// @param  ownerId     the ID of the owner of the callback
    /// @param  callback    the callback to call
    void Inventory::AddOnAddItemStackCallback( unsigned ownerId, std::function< void( ItemStack const& itemStack ) > callback )
    {
        m_OnAddItemStackCallbacks.emplace( ownerId, callback );
    }

    /// @brief  removes a callback to this Inventory that gets called whenever an ItemStack is added to the Inventory
    /// @param  ownerId     the ID of the owner of the callback
    void Inventory::RemoveOnAddItemStackCallback( unsigned ownerId )
    {
        auto it = m_OnAddItemStackCallbacks.find( ownerId );
        if ( it == m_OnAddItemStackCallbacks.end() )
        {
            Debug() << "Error: could not find OnAddItemStack Callback to remove (" << GetName() << ")" << std::endl;
            return;
        }

        m_OnAddItemStackCallbacks.erase( it );
    }


    /// @brief  adds a callback to this Inventory that gets called whenever an ItemStack is removed from the Inventory
    /// @param  ownerId     the ID of the owner of the callback
    /// @param  callback    the callback to call
    void Inventory::AddOnRemoveItemStackCallback( unsigned ownerId, std::function< void( ItemStack const& itemStack ) > callback )
    {
        m_OnRemoveItemStackCallbacks.emplace( ownerId, callback );
    }

    /// @brief  removes a callback to this Inventory that gets called whenever an ItemStack is removed from the Inventory
    /// @param  ownerId     the ID of the owner of the callback
    void Inventory::RemoveOnRemoveItemStackCallback( unsigned ownerId )
    {
        auto it = m_OnRemoveItemStackCallbacks.find( ownerId );
        if ( it == m_OnRemoveItemStackCallbacks.end() )
        {
            Debug() << "Error: could not find OnAddItemStack Callback to remove (" << GetName() << ")" << std::endl;
            return;
        }

        m_OnRemoveItemStackCallbacks.erase( it );
    }


//-----------------------------------------------------------------------------
// private: virtual override methods
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// private: helper methods
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// private: inspection
//-----------------------------------------------------------------------------

    
    /// @brief  displays this Inventory in the Inspector
    void Inventory::Inspector()
    {
        // TODO: make this work with callbacks
        Inspection::InspectArray< ItemStack >(
            "items",
            &m_Items,
            []( ItemStack* itemStack ) -> bool
            {
                return itemStack->Inspect();
            }
        );
    }


//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------


    /// @brief  reads the ItemStack that this Inventory holds
    /// @param  data    the json data to read from
    void Inventory::readItems( nlohmann::ordered_json const& data )
    {
        m_Items.clear();
        for ( auto const& itemData : data )
        {
            ItemStack stack;
            Stream::Read( stack, itemData );
            m_Items.push_back( stack );
        }
    }


//-----------------------------------------------------------------------------
// public: reading / writing
//-----------------------------------------------------------------------------


    /// @brief  gets the map of read methods for this Component
    /// @return the map of read methods for this Component
    ReadMethodMap< ISerializable > const& Inventory::GetReadMethods() const
    {
        static ReadMethodMap< Inventory > const readMethods = {
            { "Items", &Inventory::readItems }
        };

        return (ReadMethodMap< ISerializable > const&)readMethods;
    }


    /// @brief  Write all Inventory data to a JSON file.
    /// @return The JSON file containing the Inventory data.
    nlohmann::ordered_json Inventory::Write() const
    {
        nlohmann::ordered_json json;

        nlohmann::ordered_json& itemData = json[ "Items" ];
        for ( ItemStack const& stack : m_Items )
        {
            itemData.push_back( Stream::Write( stack ) );
        }

        return json;
    }


//-----------------------------------------------------------------------------
// public: constructor / Destructor
//-----------------------------------------------------------------------------


    /// @brief  constructor
    Inventory::Inventory() :
        Component( typeid( Inventory ) )
    {}


//-----------------------------------------------------------------------------
// public: copying
//-----------------------------------------------------------------------------


    /// @brief  clones this Inventory
    /// @return the newly created clone of this Inventory
    Inventory* Inventory::Clone() const
    {
        return new Inventory( *this );
    }


//-----------------------------------------------------------------------------
// private: copying
//-----------------------------------------------------------------------------


    /// @brief  copy-constructor for the Inventory
    /// @param  other   the other Inventory to copy
    Inventory::Inventory( const Inventory& other ) :
        Component( other ),
        m_Items( other.m_Items )
    {}

//-----------------------------------------------------------------------------
