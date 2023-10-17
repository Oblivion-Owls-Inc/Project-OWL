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
    virtual void LoadAssets( nlohmann::ordered_json const& data ) = 0;
    virtual nlohmann::ordered_json SaveAssets() const = 0;
protected:
    BaseAssetLibrarySystem( std::string const& name ) :
        System( name )
    {}
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
    virtual void LoadAssets( nlohmann::ordered_json const& data ) override;

    /// @brief  saves all assets of this AssetLibrary's type to JSON
    /// @return the written json data
    virtual nlohmann::ordered_json SaveAssets() const override;

    virtual void DebugWindow() override;

//-----------------------------------------------------------------------------
public: // public functions
//-----------------------------------------------------------------------------

    /// @brief  Finds and returns an asset, builds if doesnt yet exist
    /// @return the constructed or found asset
    AssetType const* GetAsset( std::string const& name ) const;

    /// @brief Finds an returns the name of the specified asset.
    /// @param asset The asset to search for.
    /// @return The name of the asset.
    char const* GetAssetName( AssetType const* asset ) const;

    /// @brief  Adds an asset to the AssetLibrary
    /// @param  name    the name of the asset to add
    /// @param  asset   the asset to add
    void AddAsset( std::string const& name, AssetType* asset );

    /// @brief  Gets All the assets in the AssetLibrary
    std::map< std::string, AssetType* > const& GetAssets() const;

//-----------------------------------------------------------------------------
private: // private variables
//-----------------------------------------------------------------------------

    std::map< std::string, AssetType* > m_Assets;

    static bool s_ShowAssetLibraryList;

//-----------------------------------------------------------------------------
private: // private functions
//-----------------------------------------------------------------------------

    /// @brief  Flushes everything in the library
    /// @brief  Automatically called on scene exit
    void FlushAssets();

    ///
    void ListAssets();

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
