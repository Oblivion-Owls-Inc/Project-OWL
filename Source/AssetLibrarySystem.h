/// @file AssetLibrarySystem.h
/// @author Tyler Birdsall (tyler.birdsall@digipen.edu)
/// @brief Asset System
/// @version 0.1
/// @date 2023-09-26
/// 
/// @copyright Copyright (c) 2023 Digipen Institute of Technology

#pragma once

#define ASSETLIBRARYSYSTEM_H

#include "System.h"
#include <map>
#include <string>

/// @brief Asset System meant to be copy-pasted when creating new Systems
template <class Asset>
class AssetLibrarySystem : public System
{

//-----------------------------------------------------------------------------
private: // virtual override methods
//-----------------------------------------------------------------------------

    /// @brief  Gets called whenever a scene is exited
    virtual void OnSceneExit() override;

//-----------------------------------------------------------------------------
public: // public functions
//-----------------------------------------------------------------------------

    /// @brief  Finds and returns an asset, builds if doesnt yet exist
    /// @return the constructed or found asset
    Asset const* GetAsset(std::string const& name) const;


    void LoadAssets(Stream data);

//-----------------------------------------------------------------------------
private: // private variables
//-----------------------------------------------------------------------------

    std::map<std::string, Asset*> m_Assets;

//-----------------------------------------------------------------------------
private: // private functions
//-----------------------------------------------------------------------------

    /// @brief  Flushes everything in the library
    /// @brief  Automatically called on scene exit
    void LibraryFlush();

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

#ifndef ASSETLIBRARYSYSTEM_C
#include "AssetLibrarySystem.cpp"
#endif
