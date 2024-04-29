///*****************************************************************/
/// @file SteamSystem.cpp
/// @author Jax Clayton (jax.clayton@digipen.edu)
/// 
/// @brief SteamSystem is a System that manages the Steam API
/// 
/// @copyright  Copyright (c) 2024 Digipen Institute of Technology
///*****************************************************************/

#include "pch.h" // precompiled header has to be included first
#include "SteamSystem.h"

//-----------------------------------------------------------------------------
// public: methods
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// public: virtual override methods
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// private: members
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// private: methods
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// public: inspection
//-----------------------------------------------------------------------------

/// @brief Gets Called by the Debug system to display debug information
void SteamSystem::DebugWindow()
{}


//-----------------------------------------------------------------------------
// public: reading / writing
//-----------------------------------------------------------------------------


    /// @brief  gets this System's read methods
    /// @return this System's read methods
ReadMethodMap< ISerializable > const& SteamSystem::GetReadMethods() const
{
    static ReadMethodMap< SteamSystem > const readMethods = {};
    return (ReadMethodMap< ISerializable > const&)readMethods;
}


/// @brief  writes this SteamSystem to JSON
/// @return the JSON data of this SteamSystem
nlohmann::ordered_json SteamSystem::Write() const
{
    nlohmann::ordered_json json;
    return json;
}


//-----------------------------------------------------------------------------
// public: singleton stuff
//-----------------------------------------------------------------------------


    /// @brief  gets the instance of SteamSystem
    /// @return the instance of the SteamSystem
SteamSystem* SteamSystem::GetInstance()
{
    static std::unique_ptr< SteamSystem > s_Instance = nullptr;

    if (s_Instance == nullptr)
    {
        s_Instance.reset(new SteamSystem());
    }
    return s_Instance.get();
}


//-----------------------------------------------------------------------------
// private: singleton stuff
//-----------------------------------------------------------------------------


    /// @brief  Constructs the SteamSystem
SteamSystem::SteamSystem() :
    System("SteamSystem")
{}

//-----------------------------------------------------------------------------
