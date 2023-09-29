/// @file AssetLibrarySystem.h
/// @author Tyler Birdsall (tyler.birdsall@digipen.edu)
/// @brief Asset System
/// @version 0.1
/// @date 2023-09-26
/// 
/// @copyright Copyright (c) 2023 Digipen Institute of Technology

#pragma once

#include "System.h"

/// @brief Asset System meant to be copy-pasted when creating new Systems
template <class T>
class AssetLibrarySystem : public System
{

//-----------------------------------------------------------------------------
private: // virtual override methods
//-----------------------------------------------------------------------------

    /// @brief  Gets called once when this System is added to the Engine
    virtual void OnInit() override {};

    /// @brief  Gets called once every simulation frame. Use this function for anything that affects the simulation.
    virtual void OnFixedUpdate() override {};

    /// @brief  Gets called once every graphics frame. Do not use this function for anything that affects the simulation.
    /// @param  dt  the elapsed time in seconds since the previous frame
    virtual void OnUpdate(float dt) override {};

    /// @brief  Gets called once before the Engine closes
    virtual void OnExit() override {};

    /// @brief  Gets called whenever a new Scene is loaded
    virtual void OnSceneLoad() override;

    /// @brief  Gets called whenever a scene is initialized
    virtual void OnSceneInit() override;

    /// @brief  Gets called whenever a scene is exited
    virtual void OnSceneExit() override;

//-----------------------------------------------------------------------------
public: // public functions
//-----------------------------------------------------------------------------

    /// @brief  Finds and returns an asset, builds if doesnt yet exist
    /// @return the constructed or found asset
    T* LibraryGet();

    /// @brief  Flushes everything in the library
    /// @brief  Automatically called on scene exit
    void LibraryFlush();

//-----------------------------------------------------------------------------
private: // private variables
//-----------------------------------------------------------------------------

    // 100 is arbitrary, tell me if you need more/it to dynamically grow
    T* m_AssetList[100];

//-----------------------------------------------------------------------------
private: // private functions
//-----------------------------------------------------------------------------

    /// @brief  Adds an asset to the library
    void LibraryAdd(T* asset);

    /// @brief  Finds and returns an asset if it exists
    /// @return the found asset or nullptr is none is found
    T* LibraryFind(const char* name);

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

