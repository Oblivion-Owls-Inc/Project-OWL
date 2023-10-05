/// @file AssetLibrarySystem.cpp
/// @author Tyler Birdsall (tyler.birdsall@digipen.edu)
/// @brief AssetLibrarySystem
/// @version 0.1
/// @date 2023-09-26
/// 
/// @copyright Copyright (c) 2023

#define ASSETLIBRARYSYSTEM_C

#ifndef ASSETLIBRARYSYSTEM_H
#include "AssetLibrarySystem.h"
#endif


//-----------------------------------------------------------------------------
// virtual override methods
//-----------------------------------------------------------------------------


    /// @brief  Gets called whenever a scene is exited
    template< class AssetType >
    void AssetLibrarySystem< AssetType >::OnSceneExit()
    {
	    FlushAssets();
    }


//-----------------------------------------------------------------------------
// public: public functions
//-----------------------------------------------------------------------------


    /// @brief  Finds and returns an asset, builds if doesnt yet exist
    /// @return the constructed or found asset
    template< class AssetType >
    AssetType const* AssetLibrarySystem< AssetType >::GetAsset( std::string const& name ) const
    {
        auto itr = m_Assets.find(name);
        if (itr != m_Assets.end())
        {
            return itr->second;
        }
        else
        {
            std::ostringstream errorMessage;
            errorMessage <<
                "Error: Asset of type \"" << typeid( AssetType ).name() <<
                "\" with name \"" << name <<
                "\" could not be found";
            throw std::runtime_error( errorMessage.str() );
        }
    }


    /// @brief  Adds an asset to the AssetLibrary
    /// @param  name    the name of the asset to add
    /// @param  asset   the asset to add
    template< class AssetType >
    void AssetLibrarySystem< AssetType >::AddAsset( std::string const& name, AssetType* asset )
    {
        assert( m_Assets.find( name ) == m_Assets.end() );
        m_Assets.insert( { name, asset } );
    }


    /// @brief  loads all assets of this AssetLibrary's type from JSON
    /// @param  data    the json data to load from
    template< class AssetType >
    void AssetLibrarySystem< AssetType >::LoadAssets( Stream data )
    {
        for ( auto& assetData : data.GetObject() )
        {
            // create and read the asset
            AssetType* asset = new AssetType();
            Stream( assetData.value ).Read( asset );

            // insert the asset into the map
            AddAsset( assetData.name.GetString(), asset );
        }
    }


//-----------------------------------------------------------------------------
// private: private functions
//-----------------------------------------------------------------------------


    /// @brief  Flushes everything in the library
    /// @brief  Automatically called on scene exit
    template<class AssetType>
    void AssetLibrarySystem<AssetType>::FlushAssets()
    {
        for ( auto& key : m_Assets )
        {
            delete key.second;
        }
        m_Assets.clear();
    }


//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------


    /// @brief map of the AssetLibrarySystem read methods
    template < class AssetType >
    ReadMethodMap< AssetLibrarySystem< AssetType > > const AssetLibrarySystem<AssetType>::s_ReadMethods = {};


    /// @brief  gets this System's read methods
    /// @return this System's read methods
    template < class AssetType >
    ReadMethodMap< System > const& AssetLibrarySystem< AssetType >::GetReadMethods() const
    {
        return ( ReadMethodMap< System > const& )s_ReadMethods;
    }


//-----------------------------------------------------------------------------
// singleton stuff
//-----------------------------------------------------------------------------


    /// @brief constructs the AssetLibrarySystem
    template< class AssetType >
    AssetLibrarySystem< AssetType >::AssetLibrarySystem() :
        BaseAssetLibrarySystem( std::string( "AssetLibrary<" ) + typeid( AssetType ).name() + ">" )
    {}

    /// @brief the singleton instance of AssetLibrarySystem
    template< class AssetType >
    AssetLibrarySystem< AssetType >* AssetLibrarySystem< AssetType >::s_Instance = nullptr;


    /// @brief gets the instance of AssetLibrarySystem
    /// @return the instance of the AssetLibrarySystem
    template< class AssetType >
    AssetLibrarySystem< AssetType >* AssetLibrarySystem< AssetType >::GetInstance()
    {
        if ( s_Instance == nullptr )
        {
            s_Instance = new AssetLibrarySystem();
        }
        return s_Instance;
    }


//-----------------------------------------------------------------------------
