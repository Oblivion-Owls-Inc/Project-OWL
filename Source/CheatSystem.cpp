///-----------------------------------------------------------------------------//
/// @file   CheatSystem.cpp
/// @brief  CheatSystem Class definition.
/// 
/// @author Aidan Straker (aidan.straker)
/// @date   October 2023
///
/// @copyright (c) 2023 DigiPen (USA) Corporation.
///-----------------------------------------------------------------------------//

#include "CheatSystem.h"
#include "basics.h"
#include "InputSystem.h"  // GetInstance, GetKeyTriggered
#include "EntitySystem.h" // Entities, Destroy, GetComponent, GetEntities
#include "AssetLibrarySystem.h"
#include "Health.h"       // GetHealth
#include "Pool.h"         // SetCurrent
#include "ConstructionBehavior.h"
#include "SceneSystem.h"  // GetInstance
#include "CircleCollider.h"
#include "Entity.h"
#include "MiningLaser.h"

//--------------------------------------------------------------------------------
// private: virtual overrides
//--------------------------------------------------------------------------------

    /// @brief Gets called once every graphics frame. Do not use this function for anything that affects the simulation.
    /// @param dt The elapsed time in seconds since the previous frame.
    void CheatSystem::OnUpdate(float dt)
    {
        OpenCheatMenu();
        if (m_CheatMenuIsOpen)
        {
            DebugWindow();
        }
    }

    /// @brief Gets called once before the engine closes.
    void CheatSystem::OnExit(){}

//--------------------------------------------------------------------------------
// public: virtual overrides
//--------------------------------------------------------------------------------

    /// @brief The cheat menu/console.
    void CheatSystem::DebugWindow()
    {
    
        // The ImGui window.
        if (ImGui::Begin("Cheat Menu", &m_CheatMenuIsOpen, ImGuiWindowFlags_AlwaysAutoResize))
        {
            // The size of the ImGui window.
            ImGui::SetNextWindowSize(ImVec2(500, 100), ImGuiCond_FirstUseEver);

            // The infinite resources button.
            if ( ImGui::Button( m_ToggleInfiniteResource ? "Turn Off InfResources" : "Turn On InfResources") )
            {
                m_ToggleInfiniteResource = toggleInfinteResources();
            }
            ImGui::SameLine();
            ImGui::Text("Infinite Resources");

            // The infinite base health button
            if (ImGui::Button(m_ToggleBaseGodMode ? "Turn Off InfBase Health" : "Turn On InfBase Health"))
            {
                m_ToggleBaseGodMode = !m_ToggleBaseGodMode;
            }
            ImGui::SameLine();
            ImGui::Text("Infinite Base Health");

            // The infinite player health button
            if (ImGui::Button(m_TogglePlayerInfiniteHealth ? "Turn Off InfPlayerHealth" : "Turn On InfPlayerHealth"))
            {
                m_TogglePlayerInfiniteHealth = !m_TogglePlayerInfiniteHealth;
            }
            ImGui::SameLine();
            ImGui::Text("Infinite Player Health");

            // The kill all enemies button.
            if (ImGui::Button("Kill all enemies"))
            {
                for (auto i : Entities()->GetEntities())
                {
                    if (i->GetName() == "Enemy")
                    {
                        i->Destroy();
                    }
                }
            }
            ImGui::SameLine();
            ImGui::Text("Kill All Enemies");

            // The No Clip Button
            if (ImGui::Button(m_ToggleNoClip ? "Turn Off No Clip" : "Turn On No Clip"))
            {
                m_ToggleNoClip = !m_ToggleNoClip;
                noClip();
            }
            ImGui::SameLine();
            ImGui::Text("Disable Player Collisions");

            // The instant win button
            if (ImGui::Button("Instant Win"))
            {
                Scenes()->SetNextScene("GameWin");
            }
            ImGui::SameLine();
            ImGui::Text("Instantly wins the game");

            // The instant lose button
            if (ImGui::Button("Instant Lose"))
            {
                Scenes()->SetNextScene("GameOver");
            }
            ImGui::SameLine();
            ImGui::Text("Instantly loses the game");

            // Resets the game scene.
            if (ImGui::Button("Reset Game"))
            {
                Scenes()->SetNextScene("Prototype");
            }
            ImGui::SameLine();
            ImGui::Text("Resets the game");

        }
    
        ImGui::End();

        RunCheats();
    }

//--------------------------------------------------------------------------------
// private: methods
//--------------------------------------------------------------------------------

    /// @brief  Are the left shift and tilde keys pressed?
    /// @return If they are open the cheat menu.
    void CheatSystem::OpenCheatMenu()
    {
        if (Input()->GetKeyTriggered(GLFW_KEY_F1) )
        {
            m_CheatMenuIsOpen = !m_CheatMenuIsOpen;
        }
    }

    /// @brief  The code for the cheats.
    void CheatSystem::RunCheats()
    {
        // The infinite base health cheat.
        if(m_ToggleBaseGodMode)
        {
           Entity* base = Entities()->GetEntity( "Base" );
           if ( base != nullptr )
           {
               Health* health = base->GetComponent< Health >();
               if (health != nullptr)
               {
                   health->SetHealth(9999);
               }
           }
        }

        if (m_TogglePlayerInfiniteHealth)
        {
            Entity* player = Entities()->GetEntity("Player");
            if (player != nullptr)
            {
                Health* health = player->GetComponent<Health>();
                if (health != nullptr)
                {
                    health->SetHealth(9999);
                }
            }
        }
    }

    /// @brief Infinite Player Health Cheat
    void CheatSystem::InfinitePlayerHealth()
    {
        Entity* player = Entities()->GetEntity("Player");
        if (player == nullptr)
        {
            Debug() << "Infinite Player Health: Player Entity does not exist" << std::endl;
            return;
        }
        Health* health = player->GetComponent<Health>();
        if (health == nullptr)
        {
            Debug() << "Infinite Player Health: Player health component is NULL" << std::endl;
            return;
        }


        if(m_TogglePlayerInfiniteHealth == false)
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

    /// @brief 
    void CheatSystem::OneShotOneKill()
    {
        Entity* player = Entities()->GetEntity("Player");
        if (player)
        {
            MiningLaser* laser = player->GetComponent<MiningLaser>();
            laser->SetDamageRate(9999.0f);
        }
    }

    



    /// @brief Turns off player collisions
    void CheatSystem::noClip()
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

                if (m_ToggleNoClip)
                {
                    flag = m_PlayerCircleCollider->GetCollisionLayerFlags();
                    ID = m_PlayerCircleCollider->GetCollisionLayer();
                    m_PlayerCircleCollider->SetCollisionLayerFlags(0);
                    m_PlayerCircleCollider->SetCollisionLayer(INT_MAX);
                }
                else
                {
                    m_PlayerCircleCollider->SetCollisionLayerFlags(flag);
                    m_PlayerCircleCollider->SetCollisionLayer(ID);
                }
            }
        }
    }


    /// @brief  toggles the inifinite resources cheat
    /// @return the current state of whether there are infinite resources
    bool CheatSystem::toggleInfinteResources()
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

        return cheatIsOn;
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
        m_Pause(false),
        m_PlayerCircleCollider(nullptr),
        m_ToggleOneShotOneKill(false),
        m_TogglePlayerInfiniteHealth(false),
        m_PreviousBaseHealth(0),
        m_PreviousPlayerHealth(0),
        m_PreviousLaserDamage(0.0f)
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