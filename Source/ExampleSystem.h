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
public: // methods
//-----------------------------------------------------------------------------

    
//-----------------------------------------------------------------------------
public: // virtual override methods
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
private: // members
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
private: // methods
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
public: // inspection
//-----------------------------------------------------------------------------


    /// @brief Gets Called by the Debug system to display debug information
    virtual void DebugWindow() override;


//-----------------------------------------------------------------------------
public: // reading / writing
//-----------------------------------------------------------------------------


    /// @brief  gets this System's read methods
    /// @return this System's read methods
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override;


    /// @brief  writes this ExampleSystem to JSON
    /// @return the JSON data of this ExampleSystem
    virtual nlohmann::ordered_json Write() const override;


//-----------------------------------------------------------------------------
public: // singleton stuff
//-----------------------------------------------------------------------------


    /// @brief  gets the instance of ExampleSystem
    /// @return the instance of the ExampleSystem
    static ExampleSystem* GetInstance();


//-----------------------------------------------------------------------------
private: // singleton stuff
//-----------------------------------------------------------------------------


    /// @brief  Constructs the ExampleSystem
    ExampleSystem();

    // Prevent copying
    ExampleSystem( ExampleSystem const& ) = delete;
    void operator =( ExampleSystem const& ) = delete;


//-----------------------------------------------------------------------------
};

