/// @file InputInspector.cpp
/// @author Tyler Birdsall (tyler.birdsall@digipen.edu)
/// @brief Input system addition, holds Action and Debug info
/// @version 0.1
/// @date 2024-18-02
/// 
/// @copyright Copyright (c) 2024

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
        size_t vectorSize = m_Actions.size();
        for (int i = 0; i < vectorSize; ++i)
        {
            // display name of input
            if (ImGui::TreeNode(m_Actions[i].GetName().c_str()))
            {
                // set the name of the action
                snprintf(title, sizeof(title), "Set Name");
                if (ImGui::TreeNode(title))
                {
                    static char nameBuffer[128] = ""; // Buffer to hold the input
                    ImGui::InputText("New Name", nameBuffer, IM_ARRAYSIZE(nameBuffer));

                    if (ImGui::Button("Rename"))
                    {
                        std::string nameBuf = nameBuffer;
                        m_Actions[i].SetName(nameBuf);
                    }

                    ImGui::TreePop();
                }

                // key inputs for the action
                snprintf(title, sizeof(title), "Key Inputs");
                if (ImGui::TreeNode(title))
                {
                    std::vector<int> vector = m_Actions[i].GetActionVector(0);
                    size_t size = vector.size();
                    ImGui::Text("Inputs:");
                    for (int j = 0; j < size; ++j)
                    {
                        if (j % 5 != 0 || j == 0)
                        {
                            ImGui::SameLine();
                        }
                        ImGui::Text(GetDebugKeyName(vector[j]));
                    }
                    if (ImGui::Button("Add Key"))
                    {
                        m_ChangingAction = 1;
                        m_WhichAction = m_Actions[i].GetName();
                    }
                    if (m_ChangingAction == 1)
                    {
                        ImGui::Text("Press key to add");
                        m_Actions[i].ManualKeyCallback();
                    }
                    if (ImGui::Button("Remove Key"))
                    {
                        m_ChangingAction = 2;
                        m_WhichAction = m_Actions[i].GetName();
                    }
                    if (m_ChangingAction == 2)
                    {
                        ImGui::Text("Press key to remove");
                        m_Actions[i].ManualKeyCallback();
                    }
                    ImGui::TreePop();
                }

                // mouse inputs for the action
                snprintf(title, sizeof(title), "Mouse Inputs");
                if (ImGui::TreeNode(title))
                {
                    std::vector<int> vector = m_Actions[i].GetActionVector(1);
                    size_t size = vector.size();
                    ImGui::Text("Inputs:");
                    for (int j = 0; j < size; ++j)
                    {
                        if (j % 5 != 0 || j == 0)
                        {
                            ImGui::SameLine();
                        }
                        char mouseButton[8] = { 0 };
                        snprintf(mouseButton, sizeof(mouseButton), "M%d", vector[j]);
                        ImGui::Text(mouseButton);
                    }
                    if (ImGui::Button("Add Mouse"))
                    {
                        m_ChangingAction = 3;
                        m_WhichAction = m_Actions[i].GetName();
                    }
                    if (m_ChangingAction == 3)
                    {
                        ImGui::Text("Press mouse button to add");
                        for (int j = 0; j < GLFW_MOUSE_BUTTON_LAST; ++j)
                        {
                            if (GetMouseDown(j))
                            {
                                m_Actions[i].AddMouseInput(j);
                                m_ChangingAction = 0;
                            }
                        }
                    }
                    if (ImGui::Button("Remove Mouse"))
                    {
                        m_ChangingAction = 4;
                        m_WhichAction = m_Actions[i].GetName();
                    }
                    if (m_ChangingAction == 4)
                    {
                        ImGui::Text("Press mouse button to remove");
                        for (int j = 0; j < GLFW_MOUSE_BUTTON_LAST; ++j)
                        {
                            if (GetMouseDown(j))
                            {
                                m_Actions[i].RemoveMouseInput(j);
                                m_ChangingAction = 0;
                            }
                        }
                    }
                    ImGui::TreePop();
                }

                // controller inputs for the action
                snprintf(title, sizeof(title), "Controller Inputs");
                if (ImGui::TreeNode(title))
                {
                    std::vector<int> vector = m_Actions[i].GetActionVector(2);
                    size_t size = vector.size();
                    ImGui::Text("Inputs:");
                    for (int j = 0; j < size; ++j)
                    {
                        if (j % 5 != 0 || j == 0)
                        {
                            ImGui::SameLine();
                        }
                        switch (vector[j])
                        {
                        case GLFW_GAMEPAD_BUTTON_A:
                            ImGui::Text("A");
                            break;
                        case GLFW_GAMEPAD_BUTTON_B:
                            ImGui::Text("B");
                            break;
                        case GLFW_GAMEPAD_BUTTON_X:
                            ImGui::Text("X");
                            break;
                        case GLFW_GAMEPAD_BUTTON_Y:
                            ImGui::Text("Y");
                            break;
                        case GLFW_GAMEPAD_BUTTON_LEFT_BUMPER:
                            ImGui::Text("LB");
                            break;
                        case GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER:
                            ImGui::Text("RB");
                            break;
                        case GLFW_GAMEPAD_BUTTON_BACK:
                            ImGui::Text("Back");
                            break;
                        case GLFW_GAMEPAD_BUTTON_START:
                            ImGui::Text("START");
                            break;
                        case GLFW_GAMEPAD_BUTTON_GUIDE:
                            ImGui::Text("GUIDE");
                            break;
                        case GLFW_GAMEPAD_BUTTON_LEFT_THUMB:
                            ImGui::Text("LTHUMB");
                            break;
                        case GLFW_GAMEPAD_BUTTON_RIGHT_THUMB:
                            ImGui::Text("RTHUMB");
                            break;
                        case GLFW_GAMEPAD_BUTTON_DPAD_UP:
                            ImGui::Text("DUP");
                            break;
                        case GLFW_GAMEPAD_BUTTON_DPAD_DOWN:
                            ImGui::Text("DDOWN");
                            break;
                        case GLFW_GAMEPAD_BUTTON_DPAD_LEFT:
                            ImGui::Text("DLEFT");
                            break;
                        case GLFW_GAMEPAD_BUTTON_DPAD_RIGHT:
                            ImGui::Text("DRIGHT");
                            break;
                        default:
                            ImGui::Text("Error");
                            break;
                        }
                    }
                    if (ImGui::Button("Add Controller"))
                    {
                        m_ChangingAction = 5;
                        m_WhichAction = m_Actions[i].GetName();
                    }
                    if (m_ChangingAction == 5)
                    {
                        ImGui::Text("Press controller button to add");
                        for (int j = 0; j <= GLFW_GAMEPAD_BUTTON_LAST; ++j)
                        {
                            if (GetGamepadButtonDown(j))
                            {
                                m_Actions[i].AddControllerInput(j);
                                m_ChangingAction = 0;
                            }
                        }
                    }
                    if (ImGui::Button("Remove Controller"))
                    {
                        m_ChangingAction = 6;
                        m_WhichAction = m_Actions[i].GetName();
                    }
                    if (m_ChangingAction == 6)
                    {
                        ImGui::Text("Press controller button to remove");
                        for (int j = 0; j <= GLFW_GAMEPAD_BUTTON_LAST; ++j)
                        {
                            if (GetGamepadButtonDown(j))
                            {
                                m_Actions[i].RemoveControllerInput(j);
                                m_ChangingAction = 0;
                            }
                        }
                    }
                    ImGui::TreePop();
                }

                // negative axis key inputs
                snprintf(title, sizeof(title), "Key Negative Axis");
                if (ImGui::TreeNode(title))
                {
                    std::vector<int> vector = m_Actions[i].GetActionVector(3);
                    size_t size = vector.size();
                    ImGui::Text("Inputs:");
                    for (int j = 0; j < size; ++j)
                    {
                        if (j % 5 != 0 || j == 0)
                        {
                            ImGui::SameLine();
                        }
                        ImGui::Text(GetDebugKeyName(vector[j]));
                    }
                    if (ImGui::Button("Add Key"))
                    {
                        m_ChangingAction = 7;
                        m_WhichAction = m_Actions[i].GetName();
                    }
                    if (m_ChangingAction == 7)
                    {
                        ImGui::Text("Press key to add");
                        m_Actions[i].ManualKeyCallback();
                    }
                    if (ImGui::Button("Remove Key"))
                    {
                        m_ChangingAction = 8;
                        m_WhichAction = m_Actions[i].GetName();
                    }
                    if (m_ChangingAction == 8)
                    {
                        ImGui::Text("Press key to remove");
                        m_Actions[i].ManualKeyCallback();
                    }

                    ImGui::TreePop();
                }

                // negative axis mouse inputs
                snprintf(title, sizeof(title), "Mouse Negative Axis");
                if (ImGui::TreeNode(title))
                {
                    std::vector<int> vector = m_Actions[i].GetActionVector(4);
                    size_t size = vector.size();
                    ImGui::Text("Inputs:");
                    for (int j = 0; j < size; ++j)
                    {
                        if (j % 5 != 0 || j == 0)
                        {
                            ImGui::SameLine();
                        }
                        char mouseButton[8] = { 0 };
                        snprintf(mouseButton, sizeof(mouseButton), "M%d", vector[j]);
                        ImGui::Text(mouseButton);
                    }
                    if (ImGui::Button("Add Mouse"))
                    {
                        m_ChangingAction = 9;
                        m_WhichAction = m_Actions[i].GetName();
                    }
                    if (m_ChangingAction == 9)
                    {
                        ImGui::Text("Press mouse button to add");
                        for (int j = 0; j < GLFW_MOUSE_BUTTON_LAST; ++j)
                        {
                            if (GetMouseDown(j))
                            {
                                m_Actions[i].AddMouseAxisNegative(j);
                                m_ChangingAction = 0;
                            }
                        }
                    }
                    if (ImGui::Button("Remove Mouse"))
                    {
                        m_ChangingAction = 10;
                        m_WhichAction = m_Actions[i].GetName();
                    }
                    if (m_ChangingAction == 10)
                    {
                        ImGui::Text("Press mouse button to remove");
                        for (int j = 0; j < GLFW_MOUSE_BUTTON_LAST; ++j)
                        {
                            if (GetMouseDown(j))
                            {
                                m_Actions[i].RemoveMouseAxisNegative(j);
                                m_ChangingAction = 0;
                            }
                        }
                    }

                    ImGui::TreePop();
                }

                // negative controller axis inputs
                snprintf(title, sizeof(title), "Controller Negative Axis");
                if (ImGui::TreeNode(title))
                {
                    std::vector<int> vector = m_Actions[i].GetActionVector(5);
                    size_t size = vector.size();
                    ImGui::Text("Inputs:");
                    for (int j = 0; j < size; ++j)
                    {
                        if (j % 5 != 0 || j == 0)
                        {
                            ImGui::SameLine();
                        }
                        switch (vector[j])
                        {
                        case GLFW_GAMEPAD_BUTTON_A:
                            ImGui::Text("A");
                            break;
                        case GLFW_GAMEPAD_BUTTON_B:
                            ImGui::Text("B");
                            break;
                        case GLFW_GAMEPAD_BUTTON_X:
                            ImGui::Text("X");
                            break;
                        case GLFW_GAMEPAD_BUTTON_Y:
                            ImGui::Text("Y");
                            break;
                        case GLFW_GAMEPAD_BUTTON_LEFT_BUMPER:
                            ImGui::Text("LB");
                            break;
                        case GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER:
                            ImGui::Text("RB");
                            break;
                        case GLFW_GAMEPAD_BUTTON_BACK:
                            ImGui::Text("Back");
                            break;
                        case GLFW_GAMEPAD_BUTTON_START:
                            ImGui::Text("START");
                            break;
                        case GLFW_GAMEPAD_BUTTON_GUIDE:
                            ImGui::Text("GUIDE");
                            break;
                        case GLFW_GAMEPAD_BUTTON_LEFT_THUMB:
                            ImGui::Text("LTHUMB");
                            break;
                        case GLFW_GAMEPAD_BUTTON_RIGHT_THUMB:
                            ImGui::Text("RTHUMB");
                            break;
                        case GLFW_GAMEPAD_BUTTON_DPAD_UP:
                            ImGui::Text("DUP");
                            break;
                        case GLFW_GAMEPAD_BUTTON_DPAD_DOWN:
                            ImGui::Text("DDOWN");
                            break;
                        case GLFW_GAMEPAD_BUTTON_DPAD_LEFT:
                            ImGui::Text("DLEFT");
                            break;
                        case GLFW_GAMEPAD_BUTTON_DPAD_RIGHT:
                            ImGui::Text("DRIGHT");
                            break;
                        default:
                            ImGui::Text("Error");
                            break;
                        }
                    }
                    if (ImGui::Button("Add Controller"))
                    {
                        m_ChangingAction = 11;
                        m_WhichAction = m_Actions[i].GetName();
                    }
                    if (m_ChangingAction == 11)
                    {
                        ImGui::Text("Press controller button to add");
                        for (int j = 0; j <= GLFW_GAMEPAD_BUTTON_LAST; ++j)
                        {
                            if (GetGamepadButtonDown(j))
                            {
                                m_Actions[i].AddControllerAxisNegative(j);
                                m_ChangingAction = 0;
                            }
                        }
                    }
                    if (ImGui::Button("Remove Controller"))
                    {
                        m_ChangingAction = 12;
                        m_WhichAction = m_Actions[i].GetName();
                    }
                    if (m_ChangingAction == 12)
                    {
                        ImGui::Text("Press controller button to remove");
                        for (int j = 0; j <= GLFW_GAMEPAD_BUTTON_LAST; ++j)
                        {
                            if (GetGamepadButtonDown(j))
                            {
                                m_Actions[i].RemoveControllerAxisNegative(j);
                                m_ChangingAction = 0;
                            }
                        }
                    }

                    ImGui::TreePop();
                }

                // axis as inputs
                snprintf(title, sizeof(title), "Axis As Input");
                if (ImGui::TreeNode(title))
                {
                    std::vector<int> vector = m_Actions[i].GetActionVector(6);
                    size_t size = vector.size();
                    ImGui::Text("Inputs:");
                    for (int j = 0; j < size; ++j)
                    {
                        switch (vector[j])
                        {
                        case GLFW_GAMEPAD_AXIS_LEFT_X:
                            ImGui::Text("LX");
                            break;
                        case GLFW_GAMEPAD_AXIS_LEFT_Y:
                            ImGui::Text("LY");
                            break;
                        case GLFW_GAMEPAD_AXIS_RIGHT_X:
                            ImGui::Text("RX");
                            break;
                        case GLFW_GAMEPAD_AXIS_RIGHT_Y:
                            ImGui::Text("RY");
                            break;
                        case GLFW_GAMEPAD_AXIS_LEFT_TRIGGER:
                            ImGui::Text("LT");
                            break;
                        case GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER:
                            ImGui::Text("RT");
                            break;
                        default:
                            ImGui::Text("Error");
                            break;
                        }
                    }
                    if (ImGui::Button("Add Axis As Input"))
                    {
                        m_ChangingAction = 13;
                        m_WhichAction = m_Actions[i].GetName();
                    }
                    if (m_ChangingAction == 13)
                    {
                        ImGui::Text("Press axis to add");
                        for (int j = 0; j < 4; ++j)
                        {
                            if (GetGamepadAxisState(0, j) > 0.5 ||
                                GetGamepadAxisState(0, j) < -0.5)
                            {
                                m_Actions[i].AddAxisAsInput(j);
                                m_ChangingAction = 0;
                            }
                        }
                        for (int j = 4; j < 6; ++j)
                        {
                            if (GetGamepadAxisState(0, j) > 0.0)
                            {
                                m_Actions[i].AddAxisAsInput(j);
                                m_ChangingAction = 0;
                            }
                        }
                    }
                    if (ImGui::Button("Remove Axis As Input"))
                    {
                        m_ChangingAction = 14;
                        m_WhichAction = m_Actions[i].GetName();
                    }
                    if (m_ChangingAction == 14)
                    {
                        ImGui::Text("Press axis to remove");
                        for (int j = 0; j < 4; ++j)
                        {
                            if (GetGamepadAxisState(0, j) > 0.5 ||
                                GetGamepadAxisState(0, j) < -0.5)
                            {
                                m_Actions[i].RemoveAxisAsInput(j);
                                m_ChangingAction = 0;
                            }
                        }
                        for (int j = 4; j < 6; ++j)
                        {
                            if (GetGamepadAxisState(0, j) > 0.0)
                            {
                                m_Actions[i].RemoveAxisAsInput(j);
                                m_ChangingAction = 0;
                            }
                        }
                    }

                    ImGui::TreePop();
                }

                // axis inputs
                snprintf(title, sizeof(title), "Axis");
                if (ImGui::TreeNode(title))
                {
                    std::vector<int> vector = m_Actions[i].GetActionVector(7);
                    size_t size = vector.size();
                    ImGui::Text("Inputs:");
                    for (int j = 0; j < size; ++j)
                    {
                        ImGui::SameLine();
                        switch (vector[j])
                        {
                        case GLFW_GAMEPAD_AXIS_LEFT_X:
                            ImGui::Text("LX");
                            break;
                        case GLFW_GAMEPAD_AXIS_LEFT_Y:
                            ImGui::Text("LY");
                            break;
                        case GLFW_GAMEPAD_AXIS_RIGHT_X:
                            ImGui::Text("RX");
                            break;
                        case GLFW_GAMEPAD_AXIS_RIGHT_Y:
                            ImGui::Text("RY");
                            break;
                        case GLFW_GAMEPAD_AXIS_LEFT_TRIGGER:
                            ImGui::Text("LT");
                            break;
                        case GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER:
                            ImGui::Text("RT");
                            break;
                        default:
                            ImGui::Text("Error");
                            break;
                        }
                    }
                    if (ImGui::Button("Add Axis"))
                    {
                        m_ChangingAction = 15;
                        m_WhichAction = m_Actions[i].GetName();
                    }
                    if (m_ChangingAction == 15)
                    {
                        ImGui::Text("Press axis to add");
                        for (int j = 0; j < 4; ++j)
                        {
                            if (GetGamepadAxisState(0, j) > 0.5 ||
                                GetGamepadAxisState(0, j) < -0.5)
                            {
                                m_Actions[i].AddAxis(j);
                                m_ChangingAction = 0;
                            }
                        }
                        for (int j = 4; j < 6; ++j)
                        {
                            if (GetGamepadAxisState(0, j) > 0.0)
                            {
                                m_Actions[i].AddAxis(j);
                                m_ChangingAction = 0;
                            }
                        }
                    }
                    if (ImGui::Button("Remove Axis"))
                    {
                        m_ChangingAction = 16;
                        m_WhichAction = m_Actions[i].GetName();
                    }
                    if (m_ChangingAction == 16)
                    {
                        ImGui::Text("Press axis to remove");
                        for (int j = 0; j < 4; ++j)
                        {
                            if (GetGamepadAxisState(0, j) > 0.5 ||
                                GetGamepadAxisState(0, j) < -0.5)
                            {
                                m_Actions[i].RemoveAxis(j);
                                m_ChangingAction = 0;
                            }
                        }
                        for (int j = 4; j < 6; ++j)
                        {
                            if (GetGamepadAxisState(0, j) > 0.0)
                            {
                                m_Actions[i].RemoveAxis(j);
                                m_ChangingAction = 0;
                            }
                        }
                    }

                    ImGui::TreePop();
                }

                ImGui::TreePop();
            }

        }
    }
    ImGui::End();
}

//-----------------------------------------------------------------------------
// public class implimentation
//-----------------------------------------------------------------------------

/// @brief  contructor
/// @param  name of the action
/// @param  description of the action (viewable in editor)
InputSystem::Action::Action(std::string name) :
    m_Name(name)
{
}

void InputSystem::Action::removeByInput(std::vector<int>* vector, int input)
{
    auto it = std::find(vector->begin(), vector->end(), input);
    if (it == vector->end())
    {
        Debug() << "WARNING: Action could not remove a given input";
        return;
    }
    vector->erase(it);
}

/// @brief  removes all inputs for this action and empties name
void InputSystem::Action::Flush()
{
    m_Keys.clear();
    m_Mouse.clear();
    m_Controller.clear();
    m_KeyAxis.clear();
    m_MouseAxis.clear();
    m_ControllerAxis.clear();
    m_GamepadAxisAsInput.clear();
    m_GamepadAxis.clear();
    m_Name = "";
}

/// @brief  retrieves a private vector for inspector
/// @param  vector number
std::vector<int> InputSystem::Action::GetActionVector(int vector) const
{
    switch (vector)
    {
    case 0:
        return m_Keys;
    case 1:
        return m_Mouse;
    case 2:
        return m_Controller;
    case 3:
        return m_KeyAxis;
    case 4:
        return m_MouseAxis;
    case 5:
        return m_ControllerAxis;
    case 6:
        return m_GamepadAxisAsInput;
    case 7:
        return m_GamepadAxis;
    default:
        return m_Keys;
    }
}

/// @brief  adds a key to the action
/// @param  key to add
void InputSystem::Action::AddKeyInput(int glfw_key)
{
    m_Keys.push_back(glfw_key);
}

/// @brief  removes a key from the action
/// @param  key to remove
void InputSystem::Action::RemoveKeyInput(int glfw_key)
{
    removeByInput(&m_Keys, glfw_key);
}

/// @brief  adds a mouse input to the action
/// @param  mouse input to add
void InputSystem::Action::AddMouseInput(int glfw_mouse_button)
{
    m_Mouse.push_back(glfw_mouse_button);
}

/// @brief  removes a mouse input from the action
/// @param  mouse input to remove
void InputSystem::Action::RemoveMouseInput(int glfw_mouse_button)
{
    removeByInput(&m_Mouse, glfw_mouse_button);
}

/// @brief  adds a controller to the action
/// @param  controller input to add
void InputSystem::Action::AddControllerInput(int glfw_button)
{
    m_Controller.push_back(glfw_button);
}

/// @brief  removes a controller input from the action
/// @param  controller input to remove
void InputSystem::Action::RemoveControllerInput(int glfw_button)
{
    removeByInput(&m_Controller, glfw_button);
}

/// @brief  adds an axis as a button (triggers)
/// @param  axis to add as input
void InputSystem::Action::AddAxisAsInput(int glfw_axis_id)
{
    m_GamepadAxisAsInput.push_back(glfw_axis_id);
}

/// @brief  removes an axis as a button (triggers)
/// @param  axis to remove from input
void InputSystem::Action::RemoveAxisAsInput(int glfw_axis_id)
{
    removeByInput(&m_GamepadAxisAsInput, glfw_axis_id);
}

/// @brief  adds a key input axis
/// @param  glfw_key positive axis key
void InputSystem::Action::AddKeyAxisPositive(int glfw_key)
{
    AddKeyInput(glfw_key);
}

/// @brief  adds a key input axis
/// @param  glfw_key_negative negative axis key
void InputSystem::Action::AddKeyAxisNegative(int glfw_key_negative)
{
    m_KeyAxis.push_back(glfw_key_negative);
}

/// @brief  removes a key input axis
/// @param  glfw_key positive axis key
void InputSystem::Action::RemoveKeyAxisPositive(int glfw_key)
{
    RemoveKeyInput(glfw_key);
}

/// @brief  removes a key input axis
/// @param  glfw_key_negative negative axis key
void InputSystem::Action::RemoveKeyAxisNegative(int glfw_key_negative)
{
    removeByInput(&m_KeyAxis, glfw_key_negative);
}

/// @brief  adds a mouse input axis
/// @param  glfw_mouse positive axis mouse
void InputSystem::Action::AddMouseAxisPositive(int glfw_mouse)
{
    m_Mouse.push_back(glfw_mouse);
}

/// @brief  adds a mouse input axis
/// @param  glfw_mouse_negative negative axis mouse
void InputSystem::Action::AddMouseAxisNegative(int glfw_mouse_negative)
{
    m_MouseAxis.push_back(glfw_mouse_negative);
}

/// @brief  removes a mouse input axis
/// @param  glfw_mouse positive axis mouse
void InputSystem::Action::RemoveMouseAxisPositive(int glfw_mouse)
{
    RemoveMouseInput(glfw_mouse);
}

/// @brief  removes a mouse input axis
/// @param  glfw_mouse_negative negative axis mouse
void InputSystem::Action::RemoveMouseAxisNegative(int glfw_mouse_negative)
{
    removeByInput(&m_MouseAxis, glfw_mouse_negative);
}

/// @brief  adds a controller input axis
/// @param  glfw_controller positive axis controller
void InputSystem::Action::AddControllerAxisPositive(int glfw_controller)
{
    AddControllerInput(glfw_controller);
}

/// @brief  adds a controller input axis
/// @param  glfw_controller_negative negative axis controller
void InputSystem::Action::AddControllerAxisNegative(int glfw_controller_negative)
{
    m_ControllerAxis.push_back(glfw_controller_negative);
}

/// @brief  removes a controller input axis
/// @param  glfw_controller positive axis controller
/// @param  glfw_controller_negative negative axis controller
void InputSystem::Action::RemoveControllerAxisPositive(int glfw_controller)
{
    RemoveControllerInput(glfw_controller);
}

/// @brief  removes a controller input axis
/// @param  glfw_controller positive axis controller
/// @param  glfw_controller_negative negative axis controller
void InputSystem::Action::RemoveControllerAxisNegative(int glfw_controller_negative)
{
    removeByInput(&m_ControllerAxis, glfw_controller_negative);
}

/// @brief  adds a gamepad axis (stick/trigger)
/// @param  axis id to add
void InputSystem::Action::AddAxis(int glfw_axis_id)
{
    m_GamepadAxis.push_back(glfw_axis_id);
}

/// @brief  removes a gamepad axis (stick/trigger)
/// @param  axis id to add
void InputSystem::Action::RemoveAxis(int glfw_axis_id)
{
    removeByInput(&m_GamepadAxis, glfw_axis_id);
}

/// @brief  sets the name of the action
/// @param  new name of action
void InputSystem::Action::SetName(std::string const& name)
{
    m_Name = name;
}

/// @brief  gets the name of this action
/// @return the name of the action
std::string const& InputSystem::Action::GetName() const
{
    static const std::string defaultString = "NO NAME";
    return m_Name != "" ? m_Name : defaultString;
}

/// @brief  gets if this action is down
/// @return action down status
bool InputSystem::Action::GetDown() const
{
    int size = static_cast<int>(m_Keys.size());
    for (int i = 0; i < size; ++i)
    {
        if (Input()->GetKeyDown(m_Keys[i]))
        {
            return true;
        }
    }

    size = static_cast<int>(m_Mouse.size());
    for (int i = 0; i < size; ++i)
    {
        if (Input()->GetMouseDown(m_Mouse[i]))
        {
            return true;
        }
    }

    size = static_cast<int>(m_Controller.size());
    for (int i = 0; i < size; ++i)
    {
        if (Input()->GetGamepadButtonDown(m_Controller[i]))
        {
            return true;
        }
    }
    size = static_cast<int>(m_GamepadAxisAsInput.size());
    for (int i = 0; i < size; ++i)
    {
        if (m_GamepadAxisAsInput[i] == 4 || m_GamepadAxisAsInput[i] == 5)
        {
            if ( glfwJoystickPresent( 0 ) == GLFW_TRUE && Input()->GetGamepadAxisState(0, m_GamepadAxisAsInput[i]) > -0.5)
            {
                return true;
            }
        }
        else if (Input()->GetGamepadAxisState(0, m_GamepadAxisAsInput[i]) > 0.5 ||
                 Input()->GetGamepadAxisState(0, m_GamepadAxisAsInput[i]) < -0.5)
        {
            return true;
        }
    }

    return false;
}

/// @brief  gets if this action is triggered
/// @return action triggered status
bool InputSystem::Action::GetTriggered() const
{
    bool triggered = false;

    int size = static_cast<int>(m_Keys.size());
    for (int i = 0; i < size; ++i)
    {
        if (Input()->GetKeyDown(m_Keys[i]))
        {
            if (Input()->GetKeyTriggered(m_Keys[i]))
            {
                triggered = true;
            }
            else
            {
                return false;
            }
        }
    }

    size = static_cast<int>(m_Mouse.size());
    for (int i = 0; i < size; ++i)
    {
        if (Input()->GetMouseDown(m_Mouse[i]))
        {
            if (Input()->GetMouseTriggered(m_Mouse[i]))
            {
                triggered = true;
            }
            else
            {
                return false;
            }
        }
    }

    size = static_cast<int>(m_Controller.size());
    for (int i = 0; i < size; ++i)
    {
        if (Input()->GetGamepadButtonDown(m_Controller[i]))
        {
            if (Input()->GetGamepadButtonTriggered(m_Controller[i]))
            {
                triggered = true;
            }
            else
            {
                return false;
            }
        }
    }

    return triggered;
}

/// @brief  gets if this action is released
/// @return action released status
bool InputSystem::Action::GetReleased() const
{
    bool released = false;

    int size = static_cast<int>(m_Keys.size());
    for (int i = 0; i < size; ++i)
    {
        if (Input()->GetKeyDown(m_Keys[i]))
        {
            return false;
        }
        if (Input()->GetKeyReleased(m_Keys[i]))
        {
            released = true;
        }
    }

    size = static_cast<int>(m_Mouse.size());
    for (int i = 0; i < size; ++i)
    {
        if (Input()->GetMouseDown(m_Mouse[i]))
        {
            return false;
        }
        if (Input()->GetMouseReleased(m_Mouse[i]))
        {
            released = true;
        }
    }

    size = static_cast<int>(m_Controller.size());
    for (int i = 0; i < size; ++i)
    {
        if (Input()->GetGamepadButtonDown(m_Controller[i]))
        {
            return false;
        }
        if (Input()->GetGamepadButtonReleased(m_Controller[i]))
        {
            released = true;
        }
    }

    return released;
}

/// @brief  gets this action as an axis
/// @return axis status clamped to -1 and 1
float InputSystem::Action::GetAxis() const
{
    float result = 0.0f;
    int size = static_cast<int>(m_Keys.size());
    for (int i = 0; i < size; ++i)
    {
        if (Input()->GetKeyDown(m_Keys[i]))
        {
            result += 1;
        }
    }
    size = static_cast<int>(m_KeyAxis.size());
    for (int i = 0; i < size; ++i)
    {
        if (Input()->GetKeyDown(m_KeyAxis[i]))
        {
            result -= 1;
        }
    }

    size = static_cast<int>(m_Mouse.size());
    for (int i = 0; i < size; ++i)
    {
        if (Input()->GetMouseDown(m_Mouse[i]))
        {
            result += 1;
        }
    }
    size = static_cast<int>(m_MouseAxis.size());
    for (int i = 0; i < size; ++i)
    {
        if (Input()->GetMouseDown(m_MouseAxis[i]))
        {
            result -= 1;
        }
    }

    size = static_cast<int>(m_Controller.size());
    for (int i = 0; i < size; ++i)
    {
        if (Input()->GetGamepadButtonDown(m_Controller[i]))
        {
            result += 1;
        }
    }
    size = static_cast<int>(m_ControllerAxis.size());
    for (int i = 0; i < size; ++i)
    {
        if (Input()->GetGamepadButtonDown(m_ControllerAxis[i]))
        {
            result -= 1;
        }
    }

    size = static_cast<int>(m_GamepadAxis.size());
    for (int i = 0; i < size; ++i)
    {
        float axisAddition = Input()->GetGamepadAxisState(0, m_GamepadAxis[i]);
        if (m_GamepadAxis[i] == 4 || m_GamepadAxis[i] == 5)
        {
            if (axisAddition > 0.0)
            {
                result += axisAddition;
            }
        }
        else if (axisAddition > 0.5 || axisAddition < -0.5)
        {
            if (m_GamepadAxis[i] == 1)
            {
                axisAddition *= -1;
            }
            result += axisAddition;
        }
    }

    result = std::min(1.0f, result);
    result = std::max(-1.0f, result);
    return result;
}