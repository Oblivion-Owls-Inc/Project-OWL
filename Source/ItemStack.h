/// @file       ItemStack.h
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      a stack of multiple items of the same type
/// @version    0.1
/// @date       2024-01-17
/// 
/// @copyright  Copyright (c) 2024 Digipen Institute of Technology

#pragma once

#include "ISerializable.h"

/// @brief  a stack of multiple items of the same type
struct ItemStack : public ISerializable
{
//-----------------------------------------------------------------------------
public: // members
//-----------------------------------------------------------------------------


    /// @brief  the id of the items in the stack
    int M_ItemId;

    /// @brief  how many items are in the stack
    int M_Count;


//-----------------------------------------------------------------------------
public: // constructor
//-----------------------------------------------------------------------------


    /// @brief  constructor
    ItemStack( int id = 0, int count = 1 );


//-----------------------------------------------------------------------------
public: // inspection
//-----------------------------------------------------------------------------


    /// @brief  inspets this ItemStack
    bool Inspect();


//-----------------------------------------------------------------------------
private: // read methods
//-----------------------------------------------------------------------------


    /// @brief  the id of the items in the stack
    /// @param  data    the json data to read from
    void readItemId( nlohmann::ordered_json const& data );

    /// @brief  how many items are in the stack
    /// @param  data    the json data to read from
    void readCount( nlohmann::ordered_json const& data );


//-----------------------------------------------------------------------------
public: // reading / writing
//-----------------------------------------------------------------------------


    /// @brief  gets the map of read methods for this ItemStack
    /// @return the map of read methods for this ItemStack
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override;


    /// @brief  Write all ItemStack data to JSON.
    /// @return The JSON containing the ItemStack data.
    virtual nlohmann::ordered_json Write() const override;


//-----------------------------------------------------------------------------
};
