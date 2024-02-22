/// @file InputInspector.cpp
/// @author Tyler Birdsall (tyler.birdsall@digipen.edu)
/// @brief Input system addition, holds Action and Debug info
/// @version 0.1
/// @date 2024-18-02
/// 
/// @copyright Copyright (c) 2024

#include "InputSystem.h"
#include "PlatformSystem.h"
#include <map>
#include "CameraSystem.h"
#include "Engine.h"
#include "DebugSystem.h"

//-----------------------------------------------------------------------------
// public: inspection
//-----------------------------------------------------------------------------

/// @brief callback for recording an action
/// @param window,   window to watch key inputs for
/// @param key,      key captured
/// @param scancode, scancode for the callback (unused)
/// @param action,   action from callback (unused)
/// @param mods,     modifiers such as sticky keys (unused)
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    int whichAction = Input()->M_changeingAction;
    switch (whichAction)
    {
    case 1:
        Input()->GetActionByName(Input()->M_whichAction)->AddKeyInput(key);
        break;
    case 2:
        Input()->GetActionByName(Input()->M_whichAction)->RemoveKeyInput(key);
        break;
    case 7:
        Input()->GetActionByName(Input()->M_whichAction)->AddKeyAxisNegative(key);
        break;
    case 8:
        Input()->GetActionByName(Input()->M_whichAction)->RemoveKeyAxisNegative(key);
        break;
    default:
        break;
    }
    glfwSetKeyCallback(Input()->GetHandle(), nullptr);
    Input()->M_changeingAction = 0;
}

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

            static char descriptionBuffer[256] = ""; // Buffer to hold the input
            ImGui::InputText("Description", descriptionBuffer, IM_ARRAYSIZE(descriptionBuffer));

            if (ImGui::Button("Create"))
            {
                m_Actions.push_back(Action(nameBuffer, descriptionBuffer));
            }
            ImGui::TreePop();
        }

        // for each vector
        int vectorSize = m_Actions.size();
        for (int i = 0; i < vectorSize; ++i)
        {
            // display name of input
            if (ImGui::TreeNode(m_Actions[i].GetName() != "" ? m_Actions[i].GetName().c_str() : "NO NAME"))
            {
                // set the name of the action
                snprintf(title, sizeof(title), "Set Name");
                if (ImGui::TreeNode(title))
                {
                    static char nameBuffer[128] = ""; // Buffer to hold the input
                    ImGui::InputText("New Name", nameBuffer, IM_ARRAYSIZE(nameBuffer));

                    if (ImGui::Button("Rename"))
                    {
                        m_Actions[i].SetName(nameBuffer);
                    }

                    ImGui::TreePop();
                }

                // set the description of the action
                snprintf(title, sizeof(title), "Description");
                if (ImGui::TreeNode(title))
                {
                    char buffer[512] = { 0 };
                    char* offset = buffer;
                    strncpy_s(buffer, m_Actions[i].GetDescription().c_str(), 512);
                    int size = m_Actions[i].GetDescription().size();
                    while (size > 0)
                    {
                        snprintf(title, sizeof(title), offset);
                        ImGui::Text(title);
                        size -= 32;
                        offset += 32;
                    }

                    static char descriptionBuffer[512] = ""; // Buffer to hold the input
                    ImGui::InputText("New Description", descriptionBuffer, IM_ARRAYSIZE(descriptionBuffer));

                    if (ImGui::Button("Change"))
                    {
                        m_Actions[i].SetDescription(descriptionBuffer);
                    }

                    ImGui::TreePop();
                }

                // key inputs for the action
                snprintf(title, sizeof(title), "Key Inputs");
                if (ImGui::TreeNode(title))
                {
                    std::vector<int> vector = m_Actions[i].GetActionVector(0);
                    int size = vector.size();
                    ImGui::Text("Inputs:");
                    for (int j = 0; j < size; ++j)
                    {
                        ImGui::SameLine();
                        ImGui::Text(glfwGetKeyName(vector[j], 0));
                    }
                    if (ImGui::Button("Add Key"))
                    {
                        glfwSetKeyCallback(handle, &key_callback);
                        M_changeingAction = 1;
                        M_whichAction = m_Actions[i].GetName();
                    }
                    if (M_changeingAction == 1)
                    {
                        ImGui::Text("Press key to add");
                    }
                    if (ImGui::Button("Remove Key"))
                    {
                        glfwSetKeyCallback(handle, &key_callback);
                        M_changeingAction = 2;
                        M_whichAction = m_Actions[i].GetName();
                    }
                    if (M_changeingAction == 2)
                    {
                        ImGui::Text("Press key to remove");
                    }
                    ImGui::TreePop();
                }

                // mouse inputs for the action
                snprintf(title, sizeof(title), "Mouse Inputs");
                if (ImGui::TreeNode(title))
                {
                    std::vector<int> vector = m_Actions[i].GetActionVector(1);
                    int size = vector.size();
                    ImGui::Text("Inputs:");
                    for (int j = 0; j < size; ++j)
                    {
                        ImGui::SameLine();
                        char mouseButton[8] = { 0 };
                        snprintf(mouseButton, sizeof(mouseButton), "M%d", vector[j]);
                        ImGui::Text(mouseButton);
                    }
                    if (ImGui::Button("Add Mouse"))
                    {
                        M_changeingAction = 3;
                        M_whichAction = m_Actions[i].GetName();
                    }
                    if (M_changeingAction == 3)
                    {
                        ImGui::Text("Press mouse button to add");
                        for (int j = 0; j < GLFW_MOUSE_BUTTON_LAST; ++j)
                        {
                            if (GetMouseDown(j))
                            {
                                m_Actions[i].AddMouseInput(j);
                                M_changeingAction = 0;
                            }
                        }
                    }
                    if (ImGui::Button("Remove Mouse"))
                    {
                        M_changeingAction = 4;
                        M_whichAction = m_Actions[i].GetName();
                    }
                    if (M_changeingAction == 4)
                    {
                        ImGui::Text("Press mouse button to remove");
                        for (int j = 0; j < GLFW_MOUSE_BUTTON_LAST; ++j)
                        {
                            if (GetMouseDown(j))
                            {
                                m_Actions[i].RemoveMouseInput(j);
                                M_changeingAction = 0;
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
                    int size = vector.size();
                    ImGui::Text("Inputs:");
                    for (int j = 0; j < size; ++j)
                    {
                        ImGui::SameLine();
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
                        M_changeingAction = 5;
                        M_whichAction = m_Actions[i].GetName();
                    }
                    if (M_changeingAction == 5)
                    {
                        ImGui::Text("Press controller button to add");
                        for (int j = 0; j <= GLFW_GAMEPAD_BUTTON_LAST; ++j)
                        {
                            if (GetGamepadButtonDown(j))
                            {
                                m_Actions[i].AddControllerInput(j);
                                M_changeingAction = 0;
                            }
                        }
                    }
                    if (ImGui::Button("Remove Controller"))
                    {
                        M_changeingAction = 6;
                        M_whichAction = m_Actions[i].GetName();
                    }
                    if (M_changeingAction == 6)
                    {
                        ImGui::Text("Press controller button to remove");
                        for (int j = 0; j <= GLFW_GAMEPAD_BUTTON_LAST; ++j)
                        {
                            if (GetGamepadButtonDown(j))
                            {
                                m_Actions[i].RemoveControllerInput(j);
                                M_changeingAction = 0;
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
                    int size = vector.size();
                    ImGui::Text("Inputs:");
                    for (int j = 0; j < size; ++j)
                    {
                        ImGui::SameLine();
                        ImGui::Text(glfwGetKeyName(vector[j], 0));
                    }
                    if (ImGui::Button("Add Key"))
                    {
                        glfwSetKeyCallback(handle, &key_callback);
                        M_changeingAction = 7;
                        M_whichAction = m_Actions[i].GetName();
                    }
                    if (M_changeingAction == 7)
                    {
                        ImGui::Text("Press key to add");
                    }
                    if (ImGui::Button("Remove Key"))
                    {
                        glfwSetKeyCallback(handle, &key_callback);
                        M_changeingAction = 8;
                        M_whichAction = m_Actions[i].GetName();
                    }
                    if (M_changeingAction == 8)
                    {
                        ImGui::Text("Press key to remove");
                    }

                    ImGui::TreePop();
                }

                // negative axis mouse inputs
                snprintf(title, sizeof(title), "Mouse Negative Axis");
                if (ImGui::TreeNode(title))
                {
                    std::vector<int> vector = m_Actions[i].GetActionVector(4);
                    int size = vector.size();
                    ImGui::Text("Inputs:");
                    for (int j = 0; j < size; ++j)
                    {
                        ImGui::SameLine();
                        char mouseButton[8] = { 0 };
                        snprintf(mouseButton, sizeof(mouseButton), "M%d", vector[j]);
                        ImGui::Text(mouseButton);
                    }
                    if (ImGui::Button("Add Mouse"))
                    {
                        M_changeingAction = 9;
                        M_whichAction = m_Actions[i].GetName();
                    }
                    if (M_changeingAction == 9)
                    {
                        ImGui::Text("Press mouse button to add");
                        for (int j = 0; j < GLFW_MOUSE_BUTTON_LAST; ++j)
                        {
                            if (GetMouseDown(j))
                            {
                                m_Actions[i].AddMouseAxisNegative(j);
                                M_changeingAction = 0;
                            }
                        }
                    }
                    if (ImGui::Button("Remove Mouse"))
                    {
                        M_changeingAction = 10;
                        M_whichAction = m_Actions[i].GetName();
                    }
                    if (M_changeingAction == 10)
                    {
                        ImGui::Text("Press mouse button to remove");
                        for (int j = 0; j < GLFW_MOUSE_BUTTON_LAST; ++j)
                        {
                            if (GetMouseDown(j))
                            {
                                m_Actions[i].RemoveMouseAxisNegative(j);
                                M_changeingAction = 0;
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
                    int size = vector.size();
                    ImGui::Text("Inputs:");
                    for (int j = 0; j < size; ++j)
                    {
                        ImGui::SameLine();
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
                        M_changeingAction = 11;
                        M_whichAction = m_Actions[i].GetName();
                    }
                    if (M_changeingAction == 11)
                    {
                        ImGui::Text("Press controller button to add");
                        for (int j = 0; j <= GLFW_GAMEPAD_BUTTON_LAST; ++j)
                        {
                            if (GetGamepadButtonDown(j))
                            {
                                m_Actions[i].AddControllerAxisNegative(j);
                                M_changeingAction = 0;
                            }
                        }
                    }
                    if (ImGui::Button("Remove Controller"))
                    {
                        M_changeingAction = 12;
                        M_whichAction = m_Actions[i].GetName();
                    }
                    if (M_changeingAction == 12)
                    {
                        ImGui::Text("Press controller button to remove");
                        for (int j = 0; j <= GLFW_GAMEPAD_BUTTON_LAST; ++j)
                        {
                            if (GetGamepadButtonDown(j))
                            {
                                m_Actions[i].RemoveControllerAxisNegative(j);
                                M_changeingAction = 0;
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
                    int size = vector.size();
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
                    if (ImGui::Button("Add Axis As Input"))
                    {
                        M_changeingAction = 13;
                        M_whichAction = m_Actions[i].GetName();
                    }
                    if (M_changeingAction == 13)
                    {
                        ImGui::Text("Press axis to add");
                        for (int j = 0; j < 4; ++j)
                        {
                            if (GetGamepadAxisState(0, j) > 0.5 ||
                                GetGamepadAxisState(0, j) < -0.5)
                            {
                                m_Actions[i].AddAxisAsInput(j);
                                M_changeingAction = 0;
                            }
                        }
                        for (int j = 4; j < 6; ++j)
                        {
                            if (GetGamepadAxisState(0, j) > 0.0)
                            {
                                m_Actions[i].AddAxisAsInput(j);
                                M_changeingAction = 0;
                            }
                        }
                    }
                    if (ImGui::Button("Remove Axis As Input"))
                    {
                        M_changeingAction = 14;
                        M_whichAction = m_Actions[i].GetName();
                    }
                    if (M_changeingAction == 14)
                    {
                        ImGui::Text("Press axis to remove");
                        for (int j = 0; j < 4; ++j)
                        {
                            if (GetGamepadAxisState(0, j) > 0.5 ||
                                GetGamepadAxisState(0, j) < -0.5)
                            {
                                m_Actions[i].RemoveAxisAsInput(j);
                                M_changeingAction = 0;
                            }
                        }
                        for (int j = 4; j < 6; ++j)
                        {
                            if (GetGamepadAxisState(0, j) > 0.0)
                            {
                                m_Actions[i].RemoveAxisAsInput(j);
                                M_changeingAction = 0;
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
                    int size = vector.size();
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
                        M_changeingAction = 15;
                        M_whichAction = m_Actions[i].GetName();
                    }
                    if (M_changeingAction == 15)
                    {
                        ImGui::Text("Press axis to add");
                        for (int j = 0; j < 4; ++j)
                        {
                            if (GetGamepadAxisState(0, j) > 0.5 ||
                                GetGamepadAxisState(0, j) < -0.5)
                            {
                                m_Actions[i].AddAxis(j);
                                M_changeingAction = 0;
                            }
                        }
                        for (int j = 4; j < 6; ++j)
                        {
                            if (GetGamepadAxisState(0, j) > 0.0)
                            {
                                m_Actions[i].AddAxis(j);
                                M_changeingAction = 0;
                            }
                        }
                    }
                    if (ImGui::Button("Remove Axis"))
                    {
                        M_changeingAction = 16;
                        M_whichAction = m_Actions[i].GetName();
                    }
                    if (M_changeingAction == 16)
                    {
                        ImGui::Text("Press axis to remove");
                        for (int j = 0; j < 4; ++j)
                        {
                            if (GetGamepadAxisState(0, j) > 0.5 ||
                                GetGamepadAxisState(0, j) < -0.5)
                            {
                                m_Actions[i].RemoveAxis(j);
                                M_changeingAction = 0;
                            }
                        }
                        for (int j = 4; j < 6; ++j)
                        {
                            if (GetGamepadAxisState(0, j) > 0.0)
                            {
                                m_Actions[i].RemoveAxis(j);
                                M_changeingAction = 0;
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
InputSystem::Action::Action(std::string name, std::string description) :
    m_name(name), m_description(description)
{
    Input()->m_numActions++;
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

/// @brief  removes all inputs for this action 
///         and empties name/description
void InputSystem::Action::Flush()
{
    m_keys.clear();
    m_mouse.clear();
    m_controller.clear();
    m_keyAxis.clear();
    m_mouseAxis.clear();
    m_controllerAxis.clear();
    m_gamepadAxisAsInput.clear();
    m_gamepadAxis.clear();
    m_name = "";
    m_description = "";
    Input()->m_numActions = 0;
}

/// @brief  retrieves a private vector for inspector
/// @param  vector number
std::vector<int> InputSystem::Action::GetActionVector(int vector) const
{
    switch (vector)
    {
    case 0:
        return m_keys;
    case 1:
        return m_mouse;
    case 2:
        return m_controller;
    case 3:
        return m_keyAxis;
    case 4:
        return m_mouseAxis;
    case 5:
        return m_controllerAxis;
    case 6:
        return m_gamepadAxisAsInput;
    case 7:
        return m_gamepadAxis;
    default:
        return m_keys;
    }
}

/// @brief  adds a key to the action
/// @param  key to add
void InputSystem::Action::AddKeyInput(int glfw_key)
{
    m_keys.push_back(glfw_key);
}

/// @brief  removes a key from the action
/// @param  key to remove
void InputSystem::Action::RemoveKeyInput(int glfw_key)
{
    removeByInput(&m_keys, glfw_key);
}

/// @brief  adds a mouse input to the action
/// @param  mouse input to add
void InputSystem::Action::AddMouseInput(int glfw_mouse_button)
{
    m_mouse.push_back(glfw_mouse_button);
}

/// @brief  removes a mouse input from the action
/// @param  mouse input to remove
void InputSystem::Action::RemoveMouseInput(int glfw_mouse_button)
{
    removeByInput(&m_mouse, glfw_mouse_button);
}

/// @brief  adds a controller to the action
/// @param  controller input to add
void InputSystem::Action::AddControllerInput(int glfw_button)
{
    m_controller.push_back(glfw_button);
}

/// @brief  removes a controller input from the action
/// @param  controller input to remove
void InputSystem::Action::RemoveControllerInput(int glfw_button)
{
    removeByInput(&m_controller, glfw_button);
}

/// @brief  adds an axis as a button (triggers)
/// @param  axis to add as input
void InputSystem::Action::AddAxisAsInput(int glfw_axis_id)
{
    m_gamepadAxisAsInput.push_back(glfw_axis_id);
}

/// @brief  removes an axis as a button (triggers)
/// @param  axis to remove from input
void InputSystem::Action::RemoveAxisAsInput(int glfw_axis_id)
{
    removeByInput(&m_gamepadAxisAsInput, glfw_axis_id);
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
    m_keyAxis.push_back(glfw_key_negative);
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
    removeByInput(&m_keyAxis, glfw_key_negative);
}

/// @brief  adds a mouse input axis
/// @param  glfw_mouse positive axis mouse
void InputSystem::Action::AddMouseAxisPositive(int glfw_mouse)
{
    m_mouse.push_back(glfw_mouse);
}

/// @brief  adds a mouse input axis
/// @param  glfw_mouse_negative negative axis mouse
void InputSystem::Action::AddMouseAxisNegative(int glfw_mouse_negative)
{
    m_mouseAxis.push_back(glfw_mouse_negative);
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
    removeByInput(&m_mouseAxis, glfw_mouse_negative);
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
    m_controllerAxis.push_back(glfw_controller_negative);
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
    removeByInput(&m_controllerAxis, glfw_controller_negative);
}

/// @brief  adds a gamepad axis (stick/trigger)
/// @param  axis id to add
void InputSystem::Action::AddAxis(int glfw_axis_id)
{
    m_gamepadAxis.push_back(glfw_axis_id);
}

/// @brief  removes a gamepad axis (stick/trigger)
/// @param  axis id to add
void InputSystem::Action::RemoveAxis(int glfw_axis_id)
{
    removeByInput(&m_gamepadAxis, glfw_axis_id);
}

/// @brief  sets the name of the action
/// @param  new name of action
void InputSystem::Action::SetName(std::string name)
{
    m_name = name;
}

/// @brief  gets the name of this action
/// @return the name of the action
std::string InputSystem::Action::GetName() const
{
    return m_name;
}

/// @brief  sets the description of the action
/// @param  new description of action
void InputSystem::Action::SetDescription(std::string description)
{
    m_description = description;
}

/// @brief  gets the description of this action
/// @return the description of the action
std::string InputSystem::Action::GetDescription() const
{
    return m_description;
}

/// @brief  gets if this action is down
/// @return action down status
bool InputSystem::Action::GetDown() const
{
    int size = (int)m_keys.size();
    for (int i = 0; i < size; ++i)
    {
        if (Input()->GetKeyDown(m_keys[i]))
        {
            return true;
        }
    }

    size = (int)m_mouse.size();
    for (int i = 0; i < size; ++i)
    {
        if (Input()->GetMouseDown(m_mouse[i]))
        {
            return true;
        }
    }

    size = (int)m_controller.size();
    for (int i = 0; i < size; ++i)
    {
        if (Input()->GetGamepadButtonDown(m_controller[i]))
        {
            return true;
        }
    }

    size = (int)m_gamepadAxisAsInput.size();
    for (int i = 0; i < size; ++i)
    {
        if (Input()->GetGamepadAxisState(0, m_gamepadAxisAsInput[i]) > 0.5 ||
            Input()->GetGamepadAxisState(0, m_gamepadAxisAsInput[i]) < -0.5)
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

    int size = (int)m_keys.size();
    for (int i = 0; i < size; ++i)
    {
        if (Input()->GetKeyDown(m_keys[i]))
        {
            if (Input()->GetKeyTriggered(m_keys[i]))
            {
                triggered = true;
            }
            else
            {
                return false;
            }
        }
    }

    size = (int)m_mouse.size();
    for (int i = 0; i < size; ++i)
    {
        if (Input()->GetMouseDown(m_mouse[i]))
        {
            if (Input()->GetMouseTriggered(m_mouse[i]))
            {
                triggered = true;
            }
            else
            {
                return false;
            }
        }
    }

    size = (int)m_controller.size();
    for (int i = 0; i < size; ++i)
    {
        if (Input()->GetGamepadButtonDown(m_controller[i]))
        {
            if (Input()->GetGamepadButtonTriggered(m_controller[i]))
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

    int size = (int)m_keys.size();
    for (int i = 0; i < size; ++i)
    {
        if (Input()->GetKeyDown(m_keys[i]))
        {
            return false;
        }
        if (Input()->GetKeyReleased(m_keys[i]))
        {
            released = true;
        }
    }

    size = (int)m_mouse.size();
    for (int i = 0; i < size; ++i)
    {
        if (Input()->GetMouseDown(m_mouse[i]))
        {
            return false;
        }
        if (Input()->GetMouseReleased(m_mouse[i]))
        {
            released = true;
        }
    }

    size = (int)m_controller.size();
    for (int i = 0; i < size; ++i)
    {
        if (Input()->GetGamepadButtonDown(m_controller[i]))
        {
            return false;
        }
        if (Input()->GetGamepadButtonReleased(m_controller[i]))
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
    int size = (int)m_keys.size();
    for (int i = 0; i < size; ++i)
    {
        if (Input()->GetKeyDown(m_keys[i]))
        {
            result += 1;
        }
        if (Input()->GetKeyDown(m_keyAxis[i]))
        {
            result -= 1;
        }
    }

    size = (int)m_mouse.size();
    for (int i = 0; i < size; ++i)
    {
        if (Input()->GetMouseDown(m_mouse[i]))
        {
            result += 1;
        }
        if (Input()->GetMouseDown(m_mouseAxis[i]))
        {
            result -= 1;
        }
    }

    size = (int)m_controller.size();
    for (int i = 0; i < size; ++i)
    {
        if (Input()->GetGamepadButtonDown(m_controller[i]))
        {
            result += 1;
        }
        if (Input()->GetGamepadButtonDown(m_controllerAxis[i]))
        {
            result -= 1;
        }
    }

    size = (int)m_gamepadAxisAsInput.size();
    for (int i = 0; i < size; ++i)
    {
        result += Input()->GetGamepadAxisState(0, m_gamepadAxisAsInput[i]);
        result += Input()->GetGamepadAxisState(0, m_gamepadAxisAsInput[i]);
    }

    result = min(1.0f, result);
    result = max(-1.0f, result);
    return result;
}