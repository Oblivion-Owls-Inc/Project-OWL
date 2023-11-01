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
#include "InputSystem.h" // GetInstance
#include "EntitySystem.h"
#include "AssetLibrarySystem.h"
#include "BaseBehavior.h"
#include "Pool.h"
#include "ConstructionBehavior.h"

///-------------------------------------------------------------------------------
/// Static Variables
///-------------------------------------------------------------------------------

// Get an instance of the input system.
static InputSystem* input = InputSystem::GetInstance();

//--------------------------------------------------------------------------------
// public: accessors
//--------------------------------------------------------------------------------


//--------------------------------------------------------------------------------
// private: virtual overrides
//--------------------------------------------------------------------------------

/// @brief Gets called when this system is added to the entity.
void CheatSystem::OnInit(){}

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
        if (ImGui::Button(m_ResourceSwitch ? "Turn Off InfResources" : "Turn On InfResources"))
        {
            m_ResourceSwitch = !m_ResourceSwitch;
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

        if (ImGui::Button(m_KillAllEnemies ? "All enemies killed" : "Kill all enemies"))
        {
            m_KillAllEnemies = !m_KillAllEnemies;
        }
        ImGui::SameLine();
        ImGui::Text("Kill All Enemies");
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
    if (input->GetKeyTriggered(GLFW_KEY_F1))
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
       Entity* base = Entities()->GetEntity("Base");
       if(base)
       {
           BaseBehavior* baseBehavior = base->GetComponent<BaseBehavior>();
           Pool<int>* health = baseBehavior->GetHealth();
           health->SetCurrent(9999);
       }
    }

    // The infinite resources cheat.
    if (m_ResourceSwitch)
    {
        Entity* resource = Entities()->GetEntity("ConstructionManager");
        ConstructionBehavior* construction = resource->GetComponent<ConstructionBehavior>();
        construction->SetCurrentResources(1000);
    }

    // Kill all enemies cheat.
    if(m_KillAllEnemies)
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

//--------------------------------------------------------------------------------
// private: Singleton
//--------------------------------------------------------------------------------

/// @brief Default constructor.
CheatSystem::CheatSystem():
    System("CheatSystem"),
    m_CheatMenuIsOpen(false),
    m_BaseGodMode(false),
    m_ResourceSwitch(false),
    m_KillAllEnemies(false)
{}

/// @brief The singleton instance of CheatSystem.
CheatSystem* CheatSystem::s_Instance = nullptr;

//--------------------------------------------------------------------------------
// public: Singleton
//--------------------------------------------------------------------------------

/// @brief  Gets the instance of the CheatSystem.
/// @return The instance of the CheatSystem.
CheatSystem* CheatSystem::GetInstance()
{
    if (s_Instance == nullptr)
    {
        s_Instance = new CheatSystem();
    }
    return s_Instance;
}