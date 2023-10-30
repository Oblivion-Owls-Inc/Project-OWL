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
void CheatSystem::OnUpdate(float dt){}

/// @brief Gets called once before the engine closes.
void CheatSystem::OnExit(){}

//--------------------------------------------------------------------------------
// public: virtual overrides
//--------------------------------------------------------------------------------

/// @brief The cheat menu/console.
void CheatSystem::DebugWindow()
{
    ImGui::Begin("Cheat Menu");
    ImGui::End();
}

//--------------------------------------------------------------------------------
// private: methods
//--------------------------------------------------------------------------------

/// @brief 
/// @return 
bool CheatSystem::OpenCheatMenu()
{
    return m_CheatMenuIsOpen = input->GetKeyTriggered(GLFW_KEY_APOSTROPHE);
}

//--------------------------------------------------------------------------------
// private: Singleton
//--------------------------------------------------------------------------------

/// @brief Default constructor.
CheatSystem::CheatSystem():
    System("CheatSystem"),
    m_CheatMenuIsOpen(false)
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