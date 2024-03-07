///-----------------------------------------------------------------------------//
/// @file   CheatSystem.cpp
/// @brief  CheatSystem Class definition.
/// 
/// @author Aidan Straker (aidan.straker)
/// @date   October 2023
///
/// @copyright (c) 2023 DigiPen (USA) Corporation.
///-----------------------------------------------------------------------------//


#include "pch.h" // precompiled header has to be included first
#include "CheatSystem.h"
#include "basics.h"
#include "InputSystem.h"  // GetInstance, GetKeyTriggered
#include "EntitySystem.h" // Entities, Destroy, GetComponent, GetEntities
#include "AssetLibrarySystem.h"
#include "Health.h" // GetHealth
#include "Pool.h"         // SetCurrent
#include "ConstructionBehavior.h"
#include "SceneSystem.h"  // GetInstance
#include "CircleCollider.h"
#include "Entity.h"

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
            if ( ImGui::Button( m_ResourceSwitch ? "Turn Off InfResources" : "Turn On InfResources") )
            {
                m_ResourceSwitch = toggleInfinteResources();
            }
            ImGui::SameLine();
            ImGui::Text("Infinite Resources");

            // The infinite base health button
            if (ImGui::Button(m_BaseGodMode ? "Turn Off InfBase" : "Turn On InfBase"))
            {
                m_BaseGodMode = !m_BaseGodMode;
            }
            ImGui::SameLine();
            ImGui::Text("Infinite Base Health");

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
            if (ImGui::Button(m_NoClip ? "Turn Off No Clip" : "Turn On No Clip"))
            {
                m_NoClip = !m_NoClip;
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

            // Pauses the game
            if (ImGui::Button(m_Pause ? "Unpause the game" : "Pause the game"))
            {
                m_Pause = !m_Pause;
                Debug().SetNonEditorSystemsEnabled(!m_Pause);
            }
            ImGui::SameLine();
            ImGui::Text("Pauses the game");
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
        if(m_BaseGodMode)
        {
           Entity* base = Entities()->GetEntity( "Base" );
           if ( base )
           {
               Health* health = base->GetComponent< Health >();
               health->GetHealth()->SetCurrent( 9999 );
           }
        }
    }

    /// @brief Turns off player collisions
    void CheatSystem::noClip()
    {
        // Get the player's circle collider.
        Entity* player = Entities()->GetEntity("Player");
        if(player)
        {
            m_CircleCollider = player->GetComponent<CircleCollider>();
            if (m_CircleCollider)
            {
                static int flag;
                static int ID;

                if (m_NoClip)
                {
                    flag = m_CircleCollider->GetCollisionLayerFlags();
                    ID = m_CircleCollider->GetCollisionLayer();
                    m_CircleCollider->SetCollisionLayerFlags(0);
                    m_CircleCollider->SetCollisionLayer(INT_MAX);
                }
                else
                {
                    m_CircleCollider->SetCollisionLayerFlags(flag);
                    m_CircleCollider->SetCollisionLayer(ID);
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
    CheatSystem::CheatSystem():
        System("CheatSystem"),
        m_CheatMenuIsOpen(false),
        m_BaseGodMode(false),
        m_ResourceSwitch(false),
        m_NoClip(false),
        m_Pause(false),
        m_CircleCollider(nullptr)
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