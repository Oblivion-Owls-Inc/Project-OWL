/// @file ExampleSystem.cpp
/// @author Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief Example System meant to be copy-pasted when creating new Systems
/// @version 0.1
/// @date 2023-09-05
/// 
/// @copyright Copyright (c) 2023

#include "ExampleSystem.h"

//-----------------------------------------------------------------------------
// virtual override methods
//-----------------------------------------------------------------------------

    
//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------

    /// @brief map of the ExampleSystem read methods
    ReadMethodMap< ExampleSystem > const ExampleSystem::s_ReadMethods = {};

    /// @brief  gets this System's read methods
    /// @return this System's read methods
    ReadMethodMap< System > const& ExampleSystem::GetReadMethods() const
    {
        return (ReadMethodMap< System > const&)s_ReadMethods;
    }

//-----------------------------------------------------------------------------
// singleton stuff
//-----------------------------------------------------------------------------

    /// @brief Constructs the ExampleSystem
    ExampleSystem::ExampleSystem() {}

    /// @brief The singleton instance of ExampleSystem
    ExampleSystem * ExampleSystem::s_Instance = nullptr;

    /// @brief gets the instance of ExampleSystem
    /// @return the instance of the ExampleSystem
    ExampleSystem * ExampleSystem::GetInstance()
    {
        if ( s_Instance == nullptr )
        {
            s_Instance = new ExampleSystem();
        }
        return s_Instance;
    }

//-----------------------------------------------------------------------------
