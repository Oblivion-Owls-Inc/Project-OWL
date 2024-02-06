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

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

class CircleCollider;

class CheatSystem :public System
{
//--------------------------------------------------------------------------------
private: // virtual override methods
//--------------------------------------------------------------------------------

    /// @brief  Gets called once every graphics frame. Do not use this function for anything that affects the simulation.
    /// @param  dt  the elapsed time in seconds since the previous frame
    virtual void OnUpdate(float dt) override;

    /// @brief  Gets called once before the Engine closes
    virtual void OnExit() override;

//--------------------------------------------------------------------------------
public: // virtual override methods
//--------------------------------------------------------------------------------

    /// @brief The cheat/console window.
    virtual void DebugWindow() override;

//--------------------------------------------------------------------------------
private: // methods
//--------------------------------------------------------------------------------

    /// @brief Opens the console.
    /// @return Whether or not the key to open the console was pressed
    void OpenCheatMenu();

    /// @brief Run the cheats.
    void RunCheats();

    /// @brief Turns off player collisions.
    void noClip();

    /// @brief  toggles the inifinite resources cheat
    /// @return the current state of whether there are infinite resources
    bool toggleInfinteResources();

//--------------------------------------------------------------------------------
private: // members
//--------------------------------------------------------------------------------

    // Whether or not the console is open.
    bool m_CheatMenuIsOpen;
    // The different cheats
    bool m_ResourceSwitch;
    bool m_BaseGodMode;
    bool m_NoClip;
    bool m_Pause;
    // The player's collider.
    CircleCollider* m_CircleCollider;

//--------------------------------------------------------------------------------
private: // singleton 
//--------------------------------------------------------------------------------

    /// @brief  Constructs the CheatSystem
    CheatSystem();

    /// @brief  The singleton instance of CheatSystem
    static CheatSystem* s_Instance;

//--------------------------------------------------------------------------------
public: // singleton 
//--------------------------------------------------------------------------------

    /// @brief  gets the instance of AudioSystem
    /// @return the instance of the AudioSystem
    static CheatSystem* GetInstance();

    // Prevent copying
    CheatSystem(CheatSystem& other) = delete;
    void operator=(const CheatSystem&) = delete;
};

__inline CheatSystem* Cheats() {
    return CheatSystem::GetInstance();
}

