




#include "PauseSystem.h"
#include "DebugSystem.h"
#include "InputSystem.h"
#include "Engine.h"





/// @brief Update the PauseSystem
/// @param dt The time elapsed since the last update
void PauseSystem::OnUpdate(float dt)
{
    if (Input()->GetKeyTriggered(GLFW_KEY_TAB))
    {
        m_Running = !m_Running;
    }

	PauseGame();
}

void PauseSystem::OnSceneExit()
{
    if (m_Running)
    {
	   m_Running = true;
	   PauseGame();
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
void PauseSystem::PauseGame()
{
    for (System* system : GameEngine()->GetSystems())
    {

        if (m_EditorSystemNames.contains(system->GetName()) == false)
        {
            system->SetEnabled(m_Running);
        }
    }
}

/// @brief map of the PauseSystem read methods
ReadMethodMap< PauseSystem > const PauseSystem::s_ReadMethods = {
  };

/// @brief  writes the PauseSystem config to json
/// @return the written json data
nlohmann::ordered_json PauseSystem::Write() const
{
    return nlohmann::ordered_json();
}


//-----------------------------------------------------------------------------
// singleton stuff
//-----------------------------------------------------------------------------


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
