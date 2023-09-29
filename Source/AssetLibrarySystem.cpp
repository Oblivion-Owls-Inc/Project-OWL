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

template<class T>
void AssetLibrarySystem<T>::OnSceneLoad()
{
	for (int i = 0; i < 100; i++)
	{
		m_AssetList[i] = NULL;
	}
}

template<class T>
void AssetLibrarySystem<T>::OnSceneInit()
{
	/// 
	/// read in from a json file what should be built
	/// 
}

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
T* AssetLibrarySystem<T>::LibraryGet()
{
	return nullptr;
}

/// @brief  Flushes everything in the library
/// @brief  Automatically called on scene exit
template<class T>
void AssetLibrarySystem<T>::LibraryFlush()
{
}

//-----------------------------------------------------------------------------
// private: private functions
//-----------------------------------------------------------------------------

/// @brief  Adds an asset to the library
/// @param	asset - the asset to add
template<class T>
void AssetLibrarySystem<T>::LibraryAdd(T* asset)
{

}

/// @brief  Finds and returns an asset if it exists
/// @return the found asset or nullptr is none is found
template<class T>
T* AssetLibrarySystem<T>::LibraryFind(const char* name)
{
	for (int i = 0; i < 100; i++)
	{
		if (m_AssetList[i] != NULL)
		{
			if (strcmp(name, m_AssetList[i]->name))
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

/* commented out for now because it broke when I was templatizing lol, have fun

/// @brief map of the AssetLibrarySystem read methods
ReadMethodMap< AssetLibrarySystem const AssetLibrarySystem::s_ReadMethods = {};



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
}*/

//-----------------------------------------------------------------------------
