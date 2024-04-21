///-----------------------------------------------------------------------------//
/// @file   CheatSystem.h
/// @brief  CheatSystem Class Declaration
/// 
/// @author Aidan Straker (aidan.straker@digipen.edu)
/// @date   October 2023
///
/// @copyright Copyright (c) 2024 Digipen Institute of Technology
///-----------------------------------------------------------------------------//

#pragma once
#include "System.h"
#include "DebugSystem.h"
#include <string>
#include <map>


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
public: // methods
//--------------------------------------------------------------------------------

    /// @brief The actual cheat menu
    void CheatMenu();

    /// @brief Run the cheats.
    void RunCheats();

    /// @brief Infinite Player Health.
    void InfinitePlayerHealth();

    void InfiniteBaseHealth();

    /// @brief Kills enemy with one hit from the laser.
    void OneShotOneKill();

    /// @brief The maximum range of the laser
    void InfiniteLaserRange();

    /// @brief The maximum toughness of the laser
    void InfiniteLaserToughness();

    /// @brief The maximum mining speed of the laser.
    void InfiniteLaserMiningSpeed();

    /// @brief Kills All Enemies.
    void KillAllEnemies();

    /// @brief Turns off player collisions.
    void NoClip();

    /// @brief  Toggles the infinite resources cheat
    /// @return The current state of whether there are infinite resources
    bool ToggleInfinteResources();

    /// @brief Unlocks all turrets
    void UnlockAllTurrets();

    /// @brief Turns the lighting on/off
    void ToggleLighting();

    /// @brief Instantly wins the game
    void InstantWin();

    /// @brief Instantly loses the game
    void InstantLose();

    /// @brief Resets the speicified scene
    void ResetGame();

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
    /// @brief Toggles killing all enemies.
    bool m_ToggleKillAllEnemies;
    /// @brief Toggles the maximum laser range.
    bool m_ToggleMaxLaserRange;
    /// @brief Toggles the maximum laser toughness.
    bool m_ToggleMaxLaserTougness;
    /// @brief Toggles the maximum mining speed.
    bool m_ToggleMaxLaserMiningSpeed;
    // Unlocks all turrets
    bool m_UnlockAllTurrets;
    // Toggle lighting on and off.
    bool m_ToggleLight;
    /// @brief Store the previous value of the player's health
    int m_PreviousPlayerHealth;
    /// @brief Store the previous value of the base's health.
    int m_PreviousBaseHealth;
    /// @brief Store the previous value of the laser's damage.
    float m_PreviousLaserDamage;
    /// @brief Store the previous value of the laser's range.
    float m_PreviousLaserRange;
    /// @brief Store the previous value of the laser's toughness.
    float m_PreviousLaserMaxToughness;
    /// @brief Store the previous mining laser speed.
    float m_PreviousLaserMiningSpeed;
    /// @brief The name of the lose scene.
    std::string m_LoseSceneName;
    /// @brief The name of the scene to reset too.
    std::string m_RestartSceneName;
    /// @brief The name of the win scene.
    std::string m_WinSceneName;
    /// @brief A map of building indexes and their states
    std::map<int, bool> m_BuildingStates;

    /// @brief The player's circle collider.
    CircleCollider* m_PlayerCircleCollider;


//-----------------------------------------------------------------------------
private: // reading
//-----------------------------------------------------------------------------

    /// @brief Read in the name of the lose scene from JSON.
    /// @param data The JSON file to read from.
    void readLoseSceneName(nlohmann::ordered_json const& data);

    /// @brief Read in the name of the restart scene from JSON.
    /// @param data The JSON file to read from.
    void readRestartSceneName(nlohmann::ordered_json const& data);

    /// @brief Read in the name of the win scene from JSON.
    /// @param data The JSON file to read from.
    void readWinSceneName(nlohmann::ordered_json const& data);


//-----------------------------------------------------------------------------
public: // reading / writing
//-----------------------------------------------------------------------------

    /// @brief  Gets this System's read methods
    /// @return This System's read methods
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override;


    /// @brief  Writes this CheatSystem to JSON
    /// @return The JSON data of this CheatSystem
    virtual nlohmann::ordered_json Write() const override;

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

