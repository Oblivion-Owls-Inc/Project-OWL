///-----------------------------------------------------------------------------//
/// @file   CheatSystem.h
/// @brief  CheatSystem Class Declaration
/// 
/// @author Aidan Straker (aidan.straker)
/// @date   October 2023
///
/// @copyright (c) 2023 DigiPen (USA) Corporation.
///-----------------------------------------------------------------------------//

#pragma once
#include "System.h"
#include "DebugSystem.h"

class CheatSystem :public System
{
//-----------------------------------------------------------------------------
private: // virtual override methods
//-----------------------------------------------------------------------------

    /// @brief  Gets called once when this System is added to the Engine
    virtual void OnInit() override;

    /// @brief  Gets called once every graphics frame. Do not use this function for anything that affects the simulation.
    /// @param  dt  the elapsed time in seconds since the previous frame
    virtual void OnUpdate(float dt) override;

    /// @brief  Gets called once before the Engine closes
    virtual void OnExit() override;

//-----------------------------------------------------------------------------
private: // singleton stuff
//-----------------------------------------------------------------------------

    /// @brief  Constructs the CheatSystem
    CheatSystem();

    /// @brief  The singleton instance of CheatSystem
    static CheatSystem* s_Instance;

//-----------------------------------------------------------------------------
public: // singleton stuff
//-----------------------------------------------------------------------------

    /// @brief  gets the instance of AudioSystem
    /// @return the instance of the AudioSystem
    static CheatSystem* GetInstance();

    // Prevent copying
    CheatSystem(CheatSystem& other) = delete;
    void operator=(const CheatSystem&) = delete;
};


