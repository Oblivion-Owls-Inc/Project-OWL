/// @file AssetLibrarySystem.cpp
/// @author Tyler Birdsall (tyler.birdsall@digipen.edu)
/// @brief AssetLibrarySystem
/// @version 0.1
/// @date 2023-09-05
/// 
/// @copyright Copyright (c) 2023

#include "AssetLibrarySystem.h"

//-----------------------------------------------------------------------------
// virtual override methods
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------

    /// @brief map of the AssetLibrarySystem read methods
ReadMethodMap< AssetLibrarySystem > const AssetLibrarySystem::s_ReadMethods = {};

/// @brief  gets this System's read methods
/// @return this System's read methods
ReadMethodMap< System > const& AssetLibrarySystem::GetReadMethods() const
{
    return (ReadMethodMap< System > const&)s_ReadMethods;
}

//-----------------------------------------------------------------------------
// singleton stuff
//-----------------------------------------------------------------------------

    /// @brief Constructs the AssetLibrarySystem
AssetLibrarySystem::AssetLibrarySystem() {}

/// @brief The singleton instance of AssetLibrarySystem
AssetLibrarySystem* AssetLibrarySystem::s_Instance = nullptr;

/// @brief gets the instance of AssetLibrarySystem
/// @return the instance of the AssetLibrarySystem
AssetLibrarySystem* AssetLibrarySystem::GetInstance()
{
    if (s_Instance == nullptr)
    {
        s_Instance = new AssetLibrarySystem();
    }
    return s_Instance;
}

//-----------------------------------------------------------------------------
