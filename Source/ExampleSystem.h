/// @file       ExampleSystem.h
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      Example System meant to be copy-pasted when creating new Systems
/// @version    0.1
/// @date       2023-09-05
/// 
/// @copyright  Copyright (c) 2023

#pragma once

#include "System.h"

/// @brief  Example System meant to be copy-pasted when creating new Systems
class ExampleSystem : public System
{

//-----------------------------------------------------------------------------
private: // virtual override methods
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
private: // reading
//-----------------------------------------------------------------------------

    /// @brief  map of the ExampleSystem read methods
    static ReadMethodMap< ExampleSystem > const s_ReadMethods;

    /// @brief  gets this System's read methods
    /// @return this System's read methods
    virtual ReadMethodMap< System > const& GetReadMethods() const override;

//-----------------------------------------------------------------------------
private: // singleton stuff
//-----------------------------------------------------------------------------

    /// @brief  Constructs the ExampleSystem
    ExampleSystem();

    /// @brief  The singleton instance of ExampleSystem
    static ExampleSystem * s_Instance;

//-----------------------------------------------------------------------------
public: // singleton stuff
//-----------------------------------------------------------------------------

    /// @brief  gets the instance of ExampleSystem
    /// @return the instance of the ExampleSystem
    static ExampleSystem * GetInstance();

    // Prevent copying
    ExampleSystem( ExampleSystem& other ) = delete;
    void operator=( const ExampleSystem& ) = delete;

};

