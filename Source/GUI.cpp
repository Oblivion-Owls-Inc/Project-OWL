///*****************************************************************
/// @file GUI.cpp
/// @author Jax Clayton (jax.clayton@digipen.edu)
/// @brief Defines the DebugSystem class for debugging functionality.
/// @version 0.1
/// @date 2023-09-07
/// 
/// @copyright Copyright (c) 2023
///*****************************************************************/
#include "DebugSystem.h"
#include "GUI.h"
#include <cmath>

/**
 * @brief Construct a new GUI::GUI object   
 * 
 */
GUI::GUI() : active(true), windowTitle("GUI")
{
}

/**
 * @brief Construct a new GUI::GUI object
 * 
 * @param windowTitle 
 * @param initialVisibility 
 */
GUI::GUI(const char* windowTitle, bool initialVisibility) : active(initialVisibility), windowTitle(windowTitle) 
{
}

GUI::~GUI()
{
}

void GUI::Render() 
{
   
}

/**
 * @brief Check if the GUI element is visible.
 * 
 * @return true 
 * @return false 
 */
bool GUI::IsVisible() const
{
    return active;
}

/**
 * @brief Set the GUI element as inactive.
 * 
 */

void GUI::SetInactive()
{
    active = false;
}

/**
 * @brief Set the GUI element as active.
 * 
 */
void GUI::SetActive()
{
    active = true;
}

/**
 * @brief Get the window title.
 * 
 * @return const char* 
 */
const char* GUI::GetWindowTitle() const
{
    return windowTitle;
}

/**
 * @class DebugMenu
 * @brief Debugging menu class derived from GUI.
 */
DebugMenu::DebugMenu() : active(true), color{ 0.0f, 0.0f, 0.0f, 1.0f }, GUI("Debug Menu", true)   
{
}

DebugMenu::~DebugMenu()
{
}

/**
 * @brief Set the GUI element as inactive.
 * 
 */
void DebugMenu::SetActive()
{
    active = true;
}

/**
 * @brief Render the GUI element.
 * 
 */
void DebugMenu::Render()
{
    if (active)
    {
        // Rendering code goes here
        ImGui::Begin(GetWindowTitle(), &this->active, ImGuiWindowFlags_MenuBar);
        if (ImGui::BeginMenuBar()) 
        {
            if (ImGui::BeginMenu("File")) 
            {
                if (ImGui::MenuItem("Open..", "Ctrl+O")) {  }
                if (ImGui::MenuItem("Save", "Ctrl+S")) {  }
                if (ImGui::MenuItem("Close", "Ctrl+W")) { this->active = false; }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Show"))
            {
                if (ImGui::MenuItem("FPS", "Shift+O")) 
                {
					DebugSystem::GetInstance()->ToggleFPS(); 
                }
                if (ImGui::MenuItem("Dev Console", "Ctrl+Shift+S")) {}
                if (ImGui::MenuItem("TBD", "Ctrl+Shift+W")) {}
                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }

        // Edit a color stored as 4 floats
        ImGui::ColorEdit4("Color", color);

        float deltaTime = ImGui::GetIO().DeltaTime;

        // Define the number of samples
        const int numSamples = 100;

        // Generate and plot the samples
        float samples[numSamples];
        for (int n = 0; n < numSamples; n++) {
            samples[n] = 1.0f / deltaTime;
        }

        ImGui::PlotLines("FPS", samples, numSamples);
        // Display contents in a scrolling region
        ImGui::TextColored(ImVec4(1, 1, 0, 1), "Console View");
        ImGui::BeginChild("Scrolling");
        ImGui::TextUnformatted(DebugSystem::GetInstance()->logBuffer.begin(), DebugSystem::GetInstance()->logBuffer.end()); // Display the text buffer
        ImGui::EndChild();

        ImGui::End();

    }

}
