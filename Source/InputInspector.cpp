/// \file InputInspector.cpp
/// \author Tyler Birdsall (tyler.birdsall@digipen.edu)
/// \brief Input system addition, holds Action and Debug info
/// \version 0.1
/// \date 2024-18-02
/// 
/// \copyright (c) 2024 DigiPen (USA) Corporation.

#include "pch.h" // precompiled header has to be included first
#include "InputSystem.h"
#include "PlatformSystem.h"
#include "CameraSystem.h"
#include "Engine.h"
#include "DebugSystem.h"

#include "ActionReference.h"

//-----------------------------------------------------------------------------
// public: inspection
//-----------------------------------------------------------------------------

/// @brief Gets Called by the Debug system to display debug information
void InputSystem::DebugWindow()
{
    // begin the input window
    if (ImGui::Begin("Input System", &m_InputIsOpen, ImGuiWindowFlags_AlwaysAutoResize))
    {
        char title[33];
        snprintf(title, sizeof(title), "Create Action");
        if (ImGui::TreeNode(title))
        {
            static char nameBuffer[128] = ""; // Buffer to hold the input
            ImGui::InputText("Action Name", nameBuffer, IM_ARRAYSIZE(nameBuffer));

            if (ImGui::Button("Create"))
            {
                size_t previousCapacity = m_Actions.capacity();
                m_Actions.push_back(Action(nameBuffer));

                // detect when the vector reallocates
                if ( previousCapacity != m_Actions.capacity() )
                {
                    // make all actionReferences find their actions again
                    for ( ActionReference* reference : m_ActionReferences )
                    {
                        *reference = GetActionByName( reference->GetName() );
                    }
                }
            }
            ImGui::TreePop();
        }

        // for each vector
        for ( Action& action : m_Actions )
        {
            
            action.Inspect();

        }
    }
    ImGui::End();
}
