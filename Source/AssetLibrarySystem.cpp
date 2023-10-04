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
/// @brief	Flushes and resets library
template<class T>
void AssetLibrarySystem<T>::OnSceneExit()
{
	LibraryFlush();
}

//-----------------------------------------------------------------------------
// public: public functions
//-----------------------------------------------------------------------------

/// @brief  Finds and returns an asset, builds if doesnt yet exist
/// @return the constructed or found asset
template<class T>
T const* AssetLibrarySystem<T>::GetAsset(std::string const& name) const
{
    auto itr = m_Assets.find(name);
    if (itr != m_Assets.end())
    {
        return itr->second;
    }
    else
    {
        return nullptr;
    }
}

template<class T>
void AssetLibrarySystem<T>::LoadAssets(Stream data)
{
    for (auto& assetData : data.GetObject())
    {
        T* asset = new T();
        Stream(assetData.value).Read(asset);
        m_Assets.insert(assetData.name.GetString(), asset);
    }
}





//-----------------------------------------------------------------------------
// private: private functions
//-----------------------------------------------------------------------------

/// @brief  Flushes everything in the library
/// @brief  Automatically called on scene exit
template<class T>
void AssetLibrarySystem<T>::LibraryFlush()
{
    for (auto& key : m_Assets)
    {
        delete key.second;
    }
    m_Assets.clear();
}

//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------

/// @brief map of the AssetLibrarySystem read methods
template < class T >
ReadMethodMap< AssetLibrarySystem< T > > const AssetLibrarySystem<T>::s_ReadMethods = {};



/// @brief  gets this System's read methods
/// @return this System's read methods
template < class T >
ReadMethodMap< System > const& AssetLibrarySystem< T >::GetReadMethods() const
{
    return (ReadMethodMap< System > const&)s_ReadMethods;
}

//-----------------------------------------------------------------------------
// singleton stuff
//-----------------------------------------------------------------------------

/// @brief Constructs the AssetLibrarySystem
template<class T>
AssetLibrarySystem< T >::AssetLibrarySystem() {}

/// @brief The singleton instance of AssetLibrarySystem
template<class T>
AssetLibrarySystem< T >* AssetLibrarySystem< T >::s_Instance = nullptr;

/// @brief gets the instance of AssetLibrarySystem
/// @return the instance of the AssetLibrarySystem
template<class T>
AssetLibrarySystem< T >* AssetLibrarySystem< T>::GetInstance()
{
    if (s_Instance == nullptr)
    {
        s_Instance = new AssetLibrarySystem();
    }
    return s_Instance;
}

//-----------------------------------------------------------------------------
