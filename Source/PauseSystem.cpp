///*****************************************************************/
/// @file PauseSystem.cpp
/// @author Jax Clayton (jax.clayton@digipen.edu)
/// @brief A system that pauses the game when the pause button is pressed.
/// 
/// @copyright  Copyright (c) 2023 Digipen Institute of Technology
///*****************************************************************/

#include "PauseSystem.h"
#include "DebugSystem.h"
#include "InputSystem.h"
#include "Engine.h"




///-----------------------------------------------------------------------------
/// Private Virtual override methods
///-----------------------------------------------------------------------------

/// @brief Update the PauseSystem
/// @param dt The time elapsed since the last update
void PauseSystem::OnUpdate(float dt)
{
    if (Input()->GetKeyTriggered(GLFW_KEY_TAB))
    {
        m_Running = !m_Running;
    }

	pauseGame();
}

/// @brief  Gets called whenever a scene is exited
void PauseSystem::OnSceneExit()
{
#ifdef NDEBUG
    if (m_Running)
    {
	   m_Running = true;
	   pauseGame();
    }
#endif // NDEBUG
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
void PauseSystem::pauseGame()
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
    return nlohmann::ordered_json();
}


///-----------------------------------------------------------------------------
/// singleton stuff
///-----------------------------------------------------------------------------


/// @brief The constructor of PauseSystem
PauseSystem::PauseSystem() :
System("PauseSystem")
{
}

/// @brief The singleton s_Instance of PauseSystem
PauseSystem* PauseSystem::s_Instance = nullptr;

/// @brief Get the singleton instance of PauseSystem
/// @return The singleton instance of PauseSystem
PauseSystem* PauseSystem::GetInstance()
{
    if (s_Instance == nullptr)
    {
        s_Instance = new PauseSystem();
    }
    return s_Instance;
}
