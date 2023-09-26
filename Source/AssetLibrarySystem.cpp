/// @file AssetLibrarySystem.cpp
/// @author Tyler Birdsall (tyler.birdsall@digipen.edu)
/// @brief AssetLibrary System 
/// @version 0.1
/// @date 2023-09-26
/// 
/// @copyright Copyright (c) 2023

#include "AssetLibrarySystem.h"

//-----------------------------------------------------------------------------
// virtual override methods
//-----------------------------------------------------------------------------

    

//-----------------------------------------------------------------------------
// singleton stuff
//-----------------------------------------------------------------------------

    /// @brief Constructs the AssetLibrarySystem
    AssetLibrarySystem::AssetLibrarySystem() {}

    /// @brief The singleton instance of AssetLibrarySystem
    AssetLibrarySystem * AssetLibrarySystem::instance = nullptr;

    /// @brief gets the instance of AssetLibrarySystem
    /// @return the instance of the AssetLibrarySystem
    AssetLibrarySystem * AssetLibrarySystem::getInstance()
    {
        if ( instance == nullptr )
        {
            instance = new AssetLibrarySystem();
        }
        return instance;
    }

//-----------------------------------------------------------------------------
