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
// singleton stuff
//-----------------------------------------------------------------------------

    /// @brief Constructs the ExampleSystem
    ExampleSystem::ExampleSystem() {}

    /// @brief The singleton instance of ExampleSystem
    ExampleSystem * ExampleSystem::instance = nullptr;

    /// @brief gets the instance of ExampleSystem
    /// @return the instance of the ExampleSystem
    ExampleSystem * ExampleSystem::GetInstance()
    {
        if ( instance == nullptr )
        {
            instance = new ExampleSystem();
        }
        return instance;
    }

//-----------------------------------------------------------------------------
