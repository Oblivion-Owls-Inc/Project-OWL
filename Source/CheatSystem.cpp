///-----------------------------------------------------------------------------//
/// @file   CheatSystem.cpp
/// @brief  CheatSystem Class definition.
/// 
/// @author Aidan Straker (aidan.straker@digipen.edu)
/// @date   October 2023
///
/// @copyright Copyright (c) 2024 Digipen Institute of Technology
///-----------------------------------------------------------------------------//


#include "pch.h" // precompiled header has to be included first
#include "CheatSystem.h"

#include "InputSystem.h"  // GetInstance, GetKeyTriggered
#include "EntitySystem.h" // Entities, Destroy, GetComponent, GetEntities

       // GetHealth
#include "Pool.h"         // SetCurrent
#include "ConstructionBehavior.h"
#include "SceneSystem.h"  // GetInstance
#include "CircleCollider.h"

#include "MiningLaser.h"
#include "PlayerController.h"
#include "BehaviorSystem.h"
#include "HomeBase.h"
#include "LightingSystem.h"


//--------------------------------------------------------------------------------
// private: virtual overrides
//--------------------------------------------------------------------------------

    /// @brief Gets called once every graphics frame. Do not use this function for anything that affects the simulation.
    /// @param dt The elapsed time in seconds since the previous frame.
    void CheatSystem::OnUpdate(float dt)
    {
        if (Input()->GetKeyTriggered(GLFW_KEY_F1))
        {
            m_CheatMenuIsOpen = !m_CheatMenuIsOpen;
        }

        if (m_CheatMenuIsOpen)
        {
            CheatMenu();
        }

        RunCheats();
    }

    /// @brief Gets called once before the engine closes.
    void CheatSystem::OnExit(){}

//--------------------------------------------------------------------------------
// public: virtual overrides
//--------------------------------------------------------------------------------

    /// @brief The cheat menu/console.
    void CheatSystem::DebugWindow()
    {
        bool open = GetDebugEnabled();
        if(ImGui::Begin("CheatSystem", &open ))
        {
            ImGui::InputText("Lose Scene", &m_LoseSceneName);
            ImGui::InputText("Restart Scene", &m_RestartSceneName);
            ImGui::InputText("Win Scene", &m_WinSceneName);
        }

        ImGui::End();
        SetDebugEnable(open);
    }

//--------------------------------------------------------------------------------
// public: cheat functions
//--------------------------------------------------------------------------------

    /// @brief The actual cheat menu
    void CheatSystem::CheatMenu()
    {
        // The ImGui window.
        if (ImGui::Begin("Cheat Menu", &m_CheatMenuIsOpen, ImGuiWindowFlags_AlwaysAutoResize))
        {
            // The size of the ImGui window.
            ImGui::SetNextWindowSize(ImVec2(500, 100), ImGuiCond_FirstUseEver);

            // The infinite resources button.
            if (ImGui::Button(m_ToggleInfiniteResource ? "Turn Off InfResources" : "Turn On InfResources"))
            {
                m_ToggleInfiniteResource = ToggleInfinteResources();
            }
            ImGui::SameLine();
            ImGui::Text("Infinite Resources");

            // The infinite base health button
            if (ImGui::Button(m_ToggleBaseGodMode ? "Turn Off InfBase Health" : "Turn On InfBase Health"))
            {
                InfiniteBaseHealth();
            }
            ImGui::SameLine();
            ImGui::Text("Infinite Base Health");

            // The infinite player health button
            if (ImGui::Button(m_TogglePlayerInfiniteHealth ? "Turn Off InfPlayerHealth" : "Turn On InfPlayerHealth"))
            {
                InfinitePlayerHealth();
            }
            ImGui::SameLine();
            ImGui::Text("Infinite Player Health");

            // The one shot one kill button
            if (ImGui::Button(m_ToggleOneShotOneKill ? "Turn Off One Shot One Kill" : "Turn On One Shot One Kill"))
            {
                OneShotOneKill();
            }
            ImGui::SameLine();
            ImGui::Text("One Shot One Kill");

            // The Infinite Laser Range Button
            if (ImGui::Button(m_ToggleMaxLaserRange ? "Turn off Infinite Laser Range" : "Turn on Infinite Laser Range"))
            {
                InfiniteLaserRange();
            }
            ImGui::SameLine();
            ImGui::Text("Infinite Laser Range");

            // The ability to mine almost anything button
            if (ImGui::Button(m_ToggleMaxLaserTougness ? "Turn off Infinite Mining Toughness" : "Turn on Infinite Mining Toughness"))
            {
                InfiniteLaserToughness();
            }
            ImGui::SameLine();
            ImGui::Text("Mine almost any block");

            // Mine at the speed of light.
            if (ImGui::Button(m_ToggleMaxLaserMiningSpeed ? "Turn off Infinite Laser Mining Speed" : "Turn on Infinite Laser Mining Spedd"))
            {
                InfiniteLaserMiningSpeed();
            }
            ImGui::SameLine();
            ImGui::Text("Laser go brr");

            // The kill all enemies button.
            if (ImGui::Button(m_ToggleKillAllEnemies ? "Turn Off Kill all enemies" : "Turn On Kill All Enemies"))
            {
                KillAllEnemies();
            }
            ImGui::SameLine();
            ImGui::Text("Kill All Enemies");

            // The No Clip Button
            if (ImGui::Button(m_ToggleNoClip ? "Turn Off No Clip" : "Turn On No Clip"))
            {
                NoClip();
            }
            ImGui::SameLine();
            ImGui::Text("Disable Player Collisions");
            
            if (ImGui::Button(m_UnlockAllTurrets ? "Reset All Turrets States" : "Unlock All Turrets"))
            {
                UnlockAllTurrets();
            }
            ImGui::SameLine();
            ImGui::Text(" Unlocks all Turrets");

            // The instant win button
            if (ImGui::Button("Instant Win"))
            {
                InstantWin();
            }
            ImGui::SameLine();
            ImGui::Text("Instantly wins the game");

            // The instant lose button
            if (ImGui::Button("Instant Lose"))
            {
                InstantLose();
            }
            ImGui::SameLine();
            ImGui::Text("Instantly loses the game");

            // Resets the game scene.
            if (ImGui::Button("Reset Game"))
            {
                ResetGame();
            }
            ImGui::SameLine();
            ImGui::Text("Resets the game");

            if (ImGui::Button(m_ToggleLight ? "Turn off lighting" : "Turn on Lighting"))
            {
                ToggleLighting();
            }
            ImGui::SameLine();
            ImGui::Text("Turns lighting on or off");
        }

        ImGui::End();
    }

    /// @brief  The code for the cheats.
    void CheatSystem::RunCheats()
    {
        // While the cheat is active constantly sets base health to 9999
        if(m_ToggleBaseGodMode)
        {
           for(HomeBase* base : Components<HomeBase>()->GetComponents())
           {
               if (base != nullptr)
               {
                   Health* health = base->GetHealth();
                   if (health != nullptr)
                   {
                       health->SetHealth(9999);
                   }
               }
           }
        }

        // While the cheat is active constantly sets player health to 9999
        if (m_TogglePlayerInfiniteHealth)
        {
            for (PlayerController* player : Behaviors< PlayerController >()->GetComponents())
            {
                Health* health = player->GetHealth();
                if (health != nullptr)
                {
                    health->SetHealth(9999);
                }
            }
        }

        // While the cheat is active constantly kill enemies.
        if (m_ToggleKillAllEnemies)
        {
            for (auto i : Entities()->GetEntities())
            {
                if (i->GetName() == "Enemy")
                {
                    i->Destroy();
                }
            }
        }
    }

    /// @brief Infinite Player Health Cheat
    void CheatSystem::InfinitePlayerHealth()
    {
        for (PlayerController* player : Behaviors<PlayerController>()->GetComponents())
        {
            Health* health = player->GetHealth();
            if (health == nullptr)
            {
                Debug() << "Infinite Player Health: Health Component is NULL" << std::endl;
                continue;
            }

            // Save previous health value and turn on cheat
            if (m_TogglePlayerInfiniteHealth == false)
            {
                m_PreviousPlayerHealth = health->GetHealth()->GetCurrent();
                m_TogglePlayerInfiniteHealth = true;
            }
            else
            {
                health->SetHealth(m_PreviousPlayerHealth);
                m_TogglePlayerInfiniteHealth = false;
            }
        }
    }

    /// @brief Infinite Base Health Cheat
    void CheatSystem::InfiniteBaseHealth()
    {
        for (HomeBase* base : Components<HomeBase>()->GetComponents())
        {
            Health* health = base->GetHealth();
            if (health == nullptr)
            {
                Debug() << "Infinite Base Health: Health Component is NULL" << std::endl;
                continue;
            }

            if (m_ToggleBaseGodMode == false)
            {
                m_PreviousBaseHealth = health->GetHealth()->GetCurrent();
                m_ToggleBaseGodMode = true;
            }
            else
            {
                health->SetHealth(m_PreviousBaseHealth);
                m_ToggleBaseGodMode = false;
            }
        }
    }

    /// @brief Sets the laser to do massive damage.
    void CheatSystem::OneShotOneKill()
    {
        for (PlayerController* player : Behaviors<PlayerController>()->GetComponents())
        {
            MiningLaser* laser = player->GetMiningLaser();
            if (laser == nullptr)
            {
                Debug() << "One Shot One Kill: Mining Laser Component is NULL" << std::endl;
                continue;
            }


            if (m_ToggleOneShotOneKill == false)
            {
                m_PreviousLaserDamage = laser->GetDamageRate();
                laser->SetDamageRate(9999.0f);
                m_ToggleOneShotOneKill = true;
            }
            else
            {
                laser->SetDamageRate(m_PreviousLaserDamage);
                m_ToggleOneShotOneKill = false;
            }
        }
    }

    /// @brief Toggles Infinite Laser Range
    void CheatSystem::InfiniteLaserRange()
    {
        for (PlayerController* player : Behaviors<PlayerController>()->GetComponents())
        {
            MiningLaser* laser = player->GetMiningLaser();
            if (laser == nullptr)
            {
                Debug() << "Infinite Laser Range: Mining Laser Component is NULL" << std::endl;
                continue;
            }


            if (m_ToggleMaxLaserRange == false)
            {
                m_PreviousLaserRange = laser->GetRange();
                laser->SetRange(999.0f);
                m_ToggleMaxLaserRange = true;
            }
            else
            {
                laser->SetRange(m_PreviousLaserRange);
                m_ToggleMaxLaserRange = false;
            }
        }
    }

    /// @brief Allows the mining laser to destroy most blocks
    void CheatSystem::InfiniteLaserToughness()
    {
        for (PlayerController* player : Behaviors<PlayerController>()->GetComponents())
        {
            MiningLaser* laser = player->GetMiningLaser();
            if (laser == nullptr)
            {
                Debug() << "Infinite Laser Toughness: Mining Laser Component is NULL" << std::endl;
                continue;
            }


            if (m_ToggleMaxLaserTougness == false)
            {
                m_PreviousLaserMaxToughness = laser->GetMaxToughness();
                laser->SetMaxToughness(9999.0f);
                m_ToggleMaxLaserTougness = true;
            }
            else
            {
                laser->SetMaxToughness(m_PreviousLaserMaxToughness);
                m_ToggleMaxLaserTougness = false;
            }
        }
    }

    void CheatSystem::InfiniteLaserMiningSpeed()
    {
        for (PlayerController* player : Behaviors<PlayerController>()->GetComponents())
        {
            MiningLaser* laser = player->GetMiningLaser();
            if (laser == nullptr)
            {
                Debug() << "Infinite Laser Mining Speed: Mining Laser Component is NULL" << std::endl;
                continue;
            }


            if (m_ToggleMaxLaserMiningSpeed == false)
            {
                m_PreviousLaserMiningSpeed = laser->GetMiningSpeed();
                laser->SetMiningSpeed(9999.0f);
                m_ToggleMaxLaserMiningSpeed = true;
            }
            else
            {
                laser->SetMiningSpeed(m_PreviousLaserMiningSpeed);
                m_ToggleMaxLaserMiningSpeed = false;
            }
        }
    }

    /// @brief Kills all enemies.
    void CheatSystem::KillAllEnemies()
    {
        if (m_ToggleKillAllEnemies == false)
        {
            for (auto i : Entities()->GetEntities())
            {
                if (i->GetName() == "Enemy")
                {
                    i->Destroy();
                }
            }
            m_ToggleKillAllEnemies = true;
        }
    }

    /// @brief Turns off player collisions
    void CheatSystem::NoClip()
    {
        // Get the player's circle collider.
        Entity* player = Entities()->GetEntity("Player");
        if(player)
        {
            m_PlayerCircleCollider = player->GetComponent<CircleCollider>();
            if (m_PlayerCircleCollider)
            {
                static int flag;
                static int ID;

                if (m_ToggleNoClip == false)
                {
                    flag = m_PlayerCircleCollider->GetCollisionLayerFlags();
                    ID = m_PlayerCircleCollider->GetCollisionLayer();
                    m_PlayerCircleCollider->SetCollisionLayerFlags(0);
                    m_PlayerCircleCollider->SetCollisionLayer(INT_MAX);
                    m_ToggleNoClip = true;
                }
                else
                {
                    m_PlayerCircleCollider->SetCollisionLayerFlags(flag);
                    m_PlayerCircleCollider->SetCollisionLayer(ID);
                    m_ToggleNoClip = false;
                }
            }
        }
    }

    /// @brief  toggles the inifinite resources cheat
    /// @return the current state of whether there are infinite resources
    bool CheatSystem::ToggleInfinteResources()
    {
        Entity* constructionEntity = Entities()->GetEntity( "ConstructionManager" );
        if ( constructionEntity == nullptr )
        {
            return false;
        }

        ConstructionBehavior* constructionBehavior = constructionEntity->GetComponent< ConstructionBehavior >();
        if ( constructionBehavior == nullptr )
        {
            return false;
        }

        bool cheatIsOn = !constructionBehavior->GetIgnoreCosts();
        constructionBehavior->SetIgnoreCosts( cheatIsOn );

        m_ToggleInfiniteResource = cheatIsOn;
        return cheatIsOn;
    }

    /// @brief Unlocks all turrets
    void CheatSystem::UnlockAllTurrets()
    {
        Entity* constructionEntity = Entities()->GetEntity("ConstructionManager");
        if (constructionEntity == nullptr)
        {
            return;
        }

        ConstructionBehavior* constructionBehavior = constructionEntity->GetComponent< ConstructionBehavior >();
        if (constructionBehavior == nullptr)
        {
            return;
        }

        if (m_UnlockAllTurrets == false)
        {
            for (int i = 0; i < 6; i++)
            {
                m_BuildingStates.emplace(i, constructionBehavior->BuildingIsUnlocked(i));
                constructionBehavior->SetBuildingUnlocked(i, true);
            }
            m_UnlockAllTurrets = true;
        }
        else
        {
            for (auto iter = m_BuildingStates.begin(); iter != m_BuildingStates.end(); ++iter)
            {
                constructionBehavior->SetBuildingUnlocked(iter->first, iter->second);
            }
            m_UnlockAllTurrets = false;
        }
    }

    /// @brief Enables or disables lighting
    void CheatSystem::ToggleLighting()
    {
        m_ToggleLight = Lights()->GetLightingEnabled();

        if (m_ToggleLight == false)
        {
            Lights()->SetLightingEnabled(true);
            m_ToggleLight = true;
        }
        else
        {
            Lights()->SetLightingEnabled(false);
            m_ToggleLight = false;
        }
    }

    /// @brief Instantly wins the game
    void CheatSystem::InstantWin() { Scenes()->SetNextScene(m_WinSceneName); }

    /// @brief Instantly loses the game
    void CheatSystem::InstantLose() { Scenes()->SetNextScene(m_LoseSceneName); }

    /// @brief Restarts the game.
    void CheatSystem::ResetGame() { Scenes()->SetNextScene(m_RestartSceneName); }

//--------------------------------------------------------------------------------
// private: read
//--------------------------------------------------------------------------------

    /// @brief Read in the name of the lose scene from JSON
    /// @param data The JSON to read from.
    void CheatSystem::readLoseSceneName(nlohmann::ordered_json const& data)
    {
        Stream::Read(m_LoseSceneName, data);
    }

    /// @brief Read in the name of the restart scene from JSON
    /// @param data The JSON to read from.
    void CheatSystem::readRestartSceneName(nlohmann::ordered_json const& data)
    {
        Stream::Read(m_RestartSceneName, data);
    }

    /// @brief Read in the name of the lose scene from JSON
    /// @param data The JSON to read from.
    void CheatSystem::readWinSceneName(nlohmann::ordered_json const& data)
    {
        Stream::Read(m_WinSceneName, data);
    }

//--------------------------------------------------------------------------------
// public: reading/writing
//--------------------------------------------------------------------------------

    /// @brief  Gets this system's read methods.
    /// @return This system's read methods
    ReadMethodMap<ISerializable> const& CheatSystem::GetReadMethods() const
    {
        static ReadMethodMap<CheatSystem> const readMethods = {
            { "LoseSceneName"    , &CheatSystem::readLoseSceneName    },
            { "RestartSceneName" , &CheatSystem::readRestartSceneName },
            { "WinSceneName"     , &CheatSystem::readWinSceneName     }
        };

        return (ReadMethodMap< ISerializable > const&)readMethods;
    }

    /// @brief  Writes this CheatSystem to JSON
    /// @return The JSON data of this CheatSystem
    nlohmann::ordered_json CheatSystem::Write() const
    {
        nlohmann::ordered_json json;

        json["LoseSceneName"] = Stream::Write(m_LoseSceneName);
        json["RestartSceneName"] = Stream::Write(m_RestartSceneName);
        json["WinSceneName"] = Stream::Write(m_WinSceneName);

        return json;
    }

//--------------------------------------------------------------------------------
// private: Singleton
//--------------------------------------------------------------------------------

    /// @brief Default constructor.
    CheatSystem::CheatSystem() :
        System("CheatSystem"),
        m_CheatMenuIsOpen(false),
        m_ToggleBaseGodMode(false),
        m_ToggleInfiniteResource(false),
        m_ToggleNoClip(false),
        m_ToggleKillAllEnemies(false),
        m_PlayerCircleCollider(nullptr),
        m_ToggleOneShotOneKill(false),
        m_TogglePlayerInfiniteHealth(false),
        m_ToggleMaxLaserRange(false),
        m_ToggleMaxLaserTougness(false),
        m_ToggleMaxLaserMiningSpeed(false),
        m_UnlockAllTurrets(false),
        m_ToggleLight(Lights()->GetLightingEnabled()),
        m_PreviousBaseHealth(0),
        m_PreviousPlayerHealth(0),
        m_PreviousLaserDamage(0.0f),
        m_PreviousLaserRange(0.0f),
        m_PreviousLaserMaxToughness(0.0f),
        m_PreviousLaserMiningSpeed(0.0f)
    {}


//--------------------------------------------------------------------------------
// public: Singleton
//--------------------------------------------------------------------------------

    /// @brief  Gets the instance of the CheatSystem.
    /// @return The instance of the CheatSystem.
    CheatSystem* CheatSystem::GetInstance()
    {
        static std::unique_ptr<CheatSystem> s_Instance = nullptr;

        if (s_Instance == nullptr)
        {
            s_Instance.reset(new CheatSystem());
        }
        return s_Instance.get();
    }