/// @file       ExampleSystem.cpp
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      DESCRIPTION HERE
/// @version    0.1
/// @date       DATE HERE
/// 
/// @copyright  Copyright (c) 2024 Digipen Instutute of Technology


#include "pch.h" // precompiled header has to be included first
#include "ExampleSystem.h"

//-----------------------------------------------------------------------------
// public: methods
//-----------------------------------------------------------------------------

    
//-----------------------------------------------------------------------------
// public: virtual override methods
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// private: members
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// private: methods
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// public: inspection
//-----------------------------------------------------------------------------


    /// @brief Gets Called by the Debug system to display debug information
    void ExampleSystem::DebugWindow()
    {}


//-----------------------------------------------------------------------------
// public: reading / writing
//-----------------------------------------------------------------------------


    /// @brief  gets this System's read methods
    /// @return this System's read methods
    ReadMethodMap< ISerializable > const& ExampleSystem::GetReadMethods() const
    {
        static ReadMethodMap< ExampleSystem > const readMethods = {};
        return (ReadMethodMap< ISerializable > const&)readMethods;
    }


    /// @brief  writes this ExampleSystem to JSON
    /// @return the JSON data of this ExampleSystem
    nlohmann::ordered_json ExampleSystem::Write() const
    {
        nlohmann::ordered_json json;
        return json;
    }


//-----------------------------------------------------------------------------
// public: singleton stuff
//-----------------------------------------------------------------------------


    /// @brief  gets the instance of ExampleSystem
    /// @return the instance of the ExampleSystem
    ExampleSystem* ExampleSystem::GetInstance()
    {
        static std::unique_ptr< ExampleSystem > s_Instance = nullptr;

        if (s_Instance == nullptr )
        {
            s_Instance.reset(new ExampleSystem());
        }
        return s_Instance.get();
    }


//-----------------------------------------------------------------------------
// private: singleton stuff
//-----------------------------------------------------------------------------


    /// @brief  Constructs the ExampleSystem
    ExampleSystem::ExampleSystem() :
        System( "ExampleSystem" )
    {}


//-----------------------------------------------------------------------------
