/// @file AssetLibrarySystem.h
/// @author Tyler Birdsall (tyler.birdsall@digipen.edu)
/// @brief Asset System
/// @version 0.1
/// @date 2023-09-05
/// 
/// @copyright Copyright (c) 2023 Digipen Institute of Technology

#pragma once

#include "System.h"

/// @brief Asset System meant to be copy-pasted when creating new Systems
class AssetLibrarySystem : public System
{

//-----------------------------------------------------------------------------
private: // virtual override methods
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
public: // public functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
private: // reading
//-----------------------------------------------------------------------------

    /// @brief map of the AssetLibrarySystem read methods
    static ReadMethodMap< AssetLibrarySystem > const s_ReadMethods;

    /// @brief  gets this System's read methods
    /// @return this System's read methods
    virtual ReadMethodMap< System > const& GetReadMethods() const override;

//-----------------------------------------------------------------------------
private: // singleton stuff
//-----------------------------------------------------------------------------

    /// @brief Constructs the AssetLibrarySystem
    AssetLibrarySystem();

    /// @brief The singleton instance of AssetLibrarySystem
    static AssetLibrarySystem* s_Instance;

//-----------------------------------------------------------------------------
public: // singleton stuff
//-----------------------------------------------------------------------------

    /// @brief gets the instance of AssetLibrarySystem
    /// @return the instance of the AssetLibrarySystem
    static AssetLibrarySystem* GetInstance();

    // Prevent copying
    AssetLibrarySystem(AssetLibrarySystem& other) = delete;
    void operator=(const AssetLibrarySystem&) = delete;

};

