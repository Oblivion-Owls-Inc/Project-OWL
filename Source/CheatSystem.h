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
class Health;

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

    /// @brief Infinite Player Health.
    void InfinitePlayerHealth();

    void InfiniteBaseHealth();

    /// @brief Kills enemy with one hit from the laser.
    void OneShotOneKill();

    /// @brief Turns off player collisions.
    void noClip();

    /// @brief  Toggles the infinite resources cheat
    /// @return The current state of whether there are infinite resources
    bool toggleInfinteResources();

//--------------------------------------------------------------------------------
private: // members
//--------------------------------------------------------------------------------

    /// @brief Is the cheat menu open
    bool m_CheatMenuIsOpen;
    /// @brief Toggles infinite resources
    bool m_ToggleInfiniteResource;
    /// @brief Toggles base infinite health
    bool m_ToggleBaseGodMode;
    /// @brief Toggles player infinite health
    bool m_TogglePlayerInfiniteHealth;
    /// @brief Mining Laser one shot one kill.
    bool m_ToggleOneShotOneKill;
    /// @brief Toggles player no clipping
    bool m_ToggleNoClip;
    /// @brief Store the previous value of the player's health
    int m_PreviousPlayerHealth;
    /// @brief Store the previous value of the base's health.
    int m_PreviousBaseHealth;
    /// @brief Store the previous value of the laser's damage.
    float m_PreviousLaserDamage;

    /// @brief Pauses the game (no longer necessary).
    bool m_Pause;

    /// @brief The player's circle collider.
    CircleCollider* m_PlayerCircleCollider;

//--------------------------------------------------------------------------------
private: // singleton 
//--------------------------------------------------------------------------------

    /// @brief  Constructs the CheatSystem
    CheatSystem();

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

