/// @file       ItemStack.h
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      a stack of multiple items of the same type
/// @version    0.1
/// @date       2024-01-17
/// 
/// @copyright  Copyright (c) 2023 Digipen Institute of Technology

#pragma once

/// @brief  a stack of multiple items of the same type
struct ItemStack
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
    ItemStack( int id, int count = 1 ) :
        M_ItemId( id ),
        M_Count( count )
    {}


//-----------------------------------------------------------------------------
};
