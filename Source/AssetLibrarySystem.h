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

// base class so that LoadAssets can be called without knowing AssetType
class BaseAssetLibrarySystem : public System
{
public:
    virtual void LoadAssets( Stream data ) = 0;
protected:
    BaseAssetLibrarySystem() = default;
public:
    // Prevent copying
    BaseAssetLibrarySystem( BaseAssetLibrarySystem& other ) = delete;
    void operator=( BaseAssetLibrarySystem const& ) = delete;
};


/// @brief Asset System meant to be copy-pasted when creating new Systems
template < class AssetType >
class AssetLibrarySystem : public BaseAssetLibrarySystem
{

//-----------------------------------------------------------------------------
private: // virtual override methods
//-----------------------------------------------------------------------------

    /// @brief  Gets called whenever a scene is exited
    virtual void OnSceneExit() override;

    /// @brief  loads all assets of this AssetLibrary's type from JSON
    /// @param  data    the json data to load from
    virtual void LoadAssets( Stream data ) override;

//-----------------------------------------------------------------------------
public: // public functions
//-----------------------------------------------------------------------------

    /// @brief  Finds and returns an asset, builds if doesnt yet exist
    /// @return the constructed or found asset
    AssetType const* GetAsset( std::string const& name ) const;

    /// @brief  Adds an asset to the AssetLibrary
    /// @param  name    the name of the asset to add
    /// @param  asset   the asset to add
    void AddAsset( std::string const& name, AssetType* asset );

//-----------------------------------------------------------------------------
private: // private variables
//-----------------------------------------------------------------------------

    std::map< std::string, AssetType* > m_Assets;

//-----------------------------------------------------------------------------
private: // private functions
//-----------------------------------------------------------------------------

    /// @brief  Flushes everything in the library
    /// @brief  Automatically called on scene exit
    void FlushAssets();

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
    AssetLibrarySystem( AssetLibrarySystem& other ) = delete;
    void operator=( AssetLibrarySystem const& ) = delete;

};

/// @brief  shorthand function for getting an AssetLibrary instance
/// @tparam AssetType   the type of asset to get the AssetLibrary of
/// @return the AssetLibrary instance of that type of asset
template< class AssetType >
__inline AssetLibrarySystem< AssetType >* AssetLibrary()
{
    return AssetLibrarySystem< AssetType >::GetInstance();
}

#ifndef ASSETLIBRARYSYSTEM_C
#include "AssetLibrarySystem.cpp"
#endif
