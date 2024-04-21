/// @file       ItemStack.cpp
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      a stack of multiple items of the same type
/// @version    0.1
/// @date       2024-01-17
/// 
/// @copyright  Copyright (c) 2024 Digipen Institute of Technology

#include "pch.h" // precompiled header has to be included first
#include "ItemStack.h"
#include "Inspection.h"

//-----------------------------------------------------------------------------
// public: constructor
//-----------------------------------------------------------------------------


    /// @brief  constructor
    ItemStack::ItemStack( int id, int count ) :
        M_ItemId( id ),
        M_Count( count )
    {}


//-----------------------------------------------------------------------------
// public: inspection
//-----------------------------------------------------------------------------


    /// @brief  inspets this ItemStack
    /// @return whether the ItemStack was changed
    bool ItemStack::Inspect()
    {
        bool changed = false;

        changed = ImGui::DragInt( "item id", &M_ItemId, 0.05f, 0, INT_MAX ) || changed;
        changed = ImGui::DragInt( "item count", &M_Count, 0.05f, 0, INT_MAX ) || changed;

        return changed;
    }
    

//-----------------------------------------------------------------------------
// private: read methods
//-----------------------------------------------------------------------------


    /// @brief  the id of the items in the stack
    /// @param  data    the json data to read from
    void ItemStack::readItemId( nlohmann::ordered_json const& data )
    {
        Stream::Read( M_ItemId, data );
    }

    /// @brief  how many items are in the stack
    /// @param  data    the json data to read from
    void ItemStack::readCount( nlohmann::ordered_json const& data )
    {
        Stream::Read( M_Count, data );
    }


//-----------------------------------------------------------------------------
// public: reading / writing
//-----------------------------------------------------------------------------


    /// @brief  gets the map of read methods for this ItemStack
    /// @return the map of read methods for this ItemStack
    ReadMethodMap< ISerializable > const& ItemStack::GetReadMethods() const
    {
        static ReadMethodMap< ItemStack > const readMethods = {
            { "ItemId", &ItemStack::readItemId },
            { "Count" , &ItemStack::readCount  }
        };

        return (ReadMethodMap< ISerializable > const&)readMethods;
    }


    /// @brief  Write all ItemStack data to JSON.
    /// @return The JSON containing the ItemStack data.
    nlohmann::ordered_json ItemStack::Write() const
    {
        nlohmann::ordered_json json;

        json[ "ItemId" ] = Stream::Write( M_ItemId );
        json[ "Count"  ] = Stream::Write( M_Count  );

        return json;
    }


//-----------------------------------------------------------------------------
