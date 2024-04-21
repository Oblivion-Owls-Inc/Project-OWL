///*****************************************************************/
/// @file PauseSystem.cpp
/// @author Jax Clayton (jax.clayton@digipen.edu)
/// @brief A system that pauses the game when the pause button is pressed.
/// 
/// @copyright  Copyright (c) 2024 Digipen Institute of Technology
///*****************************************************************/


#include "pch.h" // precompiled header has to be included first
#include "PauseSystem.h"
#include "DebugSystem.h"
#include "InputSystem.h"
#include "AssetLibrarySystem.h"
#include "Entity.h"
#include "EntitySystem.h"
#include "Engine.h"

///-----------------------------------------------------------------------------
/// Public Methods
///-----------------------------------------------------------------------------


///-----------------------------------------------------------------------------
/// Private Virtual override methods 
///-----------------------------------------------------------------------------

void PauseSystem::SetRunning(bool running)
{
    if (m_Running != running)
    {
        m_Running = running;
        togglePause();
    }
}

/// @brief  Gets called whenever a scene is exited
void PauseSystem::OnSceneExit()
{
    if (!m_Running && Debug().IsEditorRunning())
    {
        m_Running = true;
	    togglePause();
    }
}

/// @brief Gets Called by the Debug system to display debug information
void PauseSystem::DebugWindow()
{
	ImGui::Begin("Pause System");
	ImGui::Checkbox("Running", &m_Running);
	ImGui::End();
}

///-----------------------------------------------------------------------------
/// Private Methods
///-----------------------------------------------------------------------------

/// @brief  Pause the systems in the game
void PauseSystem::togglePause()
{
    for (System* system : GameEngine()->GetSystems())
    {

        if (m_EditorSystemNames.contains(system->GetName()) == false)
        {
            system->SetEnabled(m_Running); 
        }
    }
}


///-----------------------------------------------------------------------------
/// Private Reading
///-----------------------------------------------------------------------------

/// @brief map of the PauseSystem read methods
ReadMethodMap< PauseSystem > const PauseSystem::s_ReadMethods = {
  };

/// @brief  writes the PauseSystem config to json
/// @return the written json data
nlohmann::ordered_json PauseSystem::Write() const
{
    return nlohmann::ordered_json::object();
}


///-----------------------------------------------------------------------------
/// singleton stuff
///-----------------------------------------------------------------------------


/// @brief The constructor of PauseSystem
PauseSystem::PauseSystem() :
System("PauseSystem")
{
}

/// @brief Get the singleton instance of PauseSystem
/// @return The singleton instance of PauseSystem
PauseSystem* PauseSystem::GetInstance()
{
    static std::unique_ptr< PauseSystem > s_Instance = nullptr;
    if (s_Instance == nullptr)
    {
        s_Instance.reset( new PauseSystem() );
    }
    return s_Instance.get();
}
