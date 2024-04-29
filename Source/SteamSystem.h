///*****************************************************************/
/// @file SteamSystem.h
/// @author Jax Clayton (jax.clayton@digipen.edu)
/// 
/// @brief SteamSystem is a System that manages the Steam API
/// 
/// @copyright  Copyright (c) 2024 Digipen Institute of Technology
///*****************************************************************/


#pragma once
#include "System.h"
#include "SteamAchievements.h"


/// @brief SteamSystem is a System that manages the Steam API
class SteamSystem : public System
{
//-----------------------------------------------------------------------------
public: // methods
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
public: // virtual override methods

    //-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
private: // members
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
private: // methods
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
public: // inspection
//-----------------------------------------------------------------------------


    /// @brief Gets Called by the Debug system to display debug information
    virtual void DebugWindow() override;


//-----------------------------------------------------------------------------
public: // reading / writing
//-----------------------------------------------------------------------------


    /// @brief  gets this System's read methods
    /// @return this System's read methods
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override;


    /// @brief  writes this SteamSystem to JSON
    /// @return the JSON data of this SteamSystem
    virtual nlohmann::ordered_json Write() const override;


//-----------------------------------------------------------------------------
public: // singleton stuff
//-----------------------------------------------------------------------------


    /// @brief  gets the instance of SteamSystem
    /// @return the instance of the SteamSystem
    static SteamSystem* GetInstance();


//-----------------------------------------------------------------------------
private: // singleton stuff
//-----------------------------------------------------------------------------


    /// @brief  Constructs the SteamSystem
    SteamSystem();

    // Prevent copying
    SteamSystem( SteamSystem const& ) = delete;
    void operator =( SteamSystem const& ) = delete;


//-----------------------------------------------------------------------------
};

__inline SteamSystem* SteamAPI() { return SteamSystem::GetInstance(); }