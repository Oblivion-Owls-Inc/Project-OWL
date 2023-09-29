/// @file AssetLibrarySystem.cpp
/// @author Tyler Birdsall (tyler.birdsall@digipen.edu)
/// @brief AssetLibrarySystem
/// @version 0.1
/// @date 2023-09-26
/// 
/// @copyright Copyright (c) 2023

#include "AssetLibrarySystem.h"

//-----------------------------------------------------------------------------
// virtual override methods
//-----------------------------------------------------------------------------

/// @brief  Gets called whenever a new Scene is loaded
/// @brief	Resets all to null, empty library on each load
template<class T>
void AssetLibrarySystem<T>::OnSceneLoad()
{
	for (int i = 0; i < 100; i++)
	{
		m_AssetList[i] = NULL;
	}
}

/// @brief  Gets called whenever a scene is initialized
/// @brief	Should read in all library assets and prep them
template<class T>
void AssetLibrarySystem<T>::OnSceneInit()
{
	/// 
	/// read in from a json file what should be built
	/// 
}

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
T* AssetLibrarySystem<T>::GetAsset(std::string const& name)
{
	T* check = LibraryFind(name);
	if (check != NULL)
	{
		return check;
	}
	//
	// read in a asset here, getting here means the asset didnt yet exist
	// or was possibly manually flushed and has to be rebuilt
	//
	return nullptr;
}

/// @brief  Flushes everything in the library
/// @brief  Automatically called on scene exit
template<class T>
void AssetLibrarySystem<T>::LibraryFlush()
{
	for (int i = 0; i < 100; i++)
	{
		if (m_AssetList[i] != NULL)
		{
			//delete m_AssetList[i];
			// add if we need to deallocate I dont think we do
			m_AssetList[i] = NULL;
		}
	}
}

//-----------------------------------------------------------------------------
// private: private functions
//-----------------------------------------------------------------------------

/// @brief  Adds an asset to the library
/// @param	asset - the asset to add
template<class T>
void AssetLibrarySystem<T>::LibraryAdd(T* asset)
{
	for (int i = 0; i < 100; i++)
	{
		if (m_AssetList[i] == NULL)
		{
			m_AssetList[i] = asset;
			break;
		}
	}
}

/// @brief  Finds and returns an asset if it exists
/// @return the found asset or nullptr is none is found
template<class T>
T* AssetLibrarySystem<T>::LibraryFind( std::string const& name)
{
	for (int i = 0; i < 100; i++)
	{
		if (m_AssetList[i] != NULL)
		{
			if ( name ==  m_AssetList[i]->GetName() )
			{
				return m_AssetList[i];
			}
		}
	}
	return nullptr;
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
