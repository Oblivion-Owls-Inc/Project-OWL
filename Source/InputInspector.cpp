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
    default:
        break;
    }
    glfwSetKeyCallback(Input()->GetHandle(), nullptr);
    Input()->M_changeingAction = 0;
}

/// @brief Gets Called by the Debug system to display debug information
void InputSystem::DebugWindow()
{
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

        int vectorSize = m_Actions.size();
        for (int i = 0; i < vectorSize; ++i)
        {
            if (ImGui::TreeNode(m_Actions[i].GetName().c_str()))
            {
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

                    ImGui::TreePop();
                }

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

                    ImGui::TreePop();
                }

                snprintf(title, sizeof(title), "Mouse Negative Axis");
                if (ImGui::TreeNode(title))
                {
                    std::vector<int> vector = m_Actions[i].GetActionVector(4);
                    int size = vector.size();
                    ImGui::Text("Inputs:");
                    for (int j = 0; j < size; ++j)
                    {
                        ImGui::SameLine();
                        ImGui::Text(glfwGetKeyName(vector[j], 0));
                    }

                    ImGui::TreePop();
                }

                snprintf(title, sizeof(title), "Controller Negative Axis");
                if (ImGui::TreeNode(title))
                {
                    std::vector<int> vector = m_Actions[i].GetActionVector(5);
                    int size = vector.size();
                    ImGui::Text("Inputs:");
                    for (int j = 0; j < size; ++j)
                    {
                        ImGui::SameLine();
                        ImGui::Text(glfwGetKeyName(vector[j], 0));
                    }

                    ImGui::TreePop();
                }

                snprintf(title, sizeof(title), "Axis As Input");
                if (ImGui::TreeNode(title))
                {
                    std::vector<int> vector = m_Actions[i].GetActionVector(6);
                    int size = vector.size();
                    ImGui::Text("Inputs:");
                    for (int j = 0; j < size; ++j)
                    {
                        ImGui::SameLine();
                        ImGui::Text(glfwGetKeyName(vector[j], 0));
                    }

                    ImGui::TreePop();
                }

                snprintf(title, sizeof(title), "Axis");
                if (ImGui::TreeNode(title))
                {
                    std::vector<int> vector = m_Actions[i].GetActionVector(7);
                    int size = vector.size();
                    ImGui::Text("Inputs:");
                    for (int j = 0; j < size; ++j)
                    {
                        ImGui::SameLine();
                        ImGui::Text(glfwGetKeyName(vector[j], 0));
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
// private class implimentation
//-----------------------------------------------------------------------------

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

std::vector<int> InputSystem::Action::GetActionVector(int vector)
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

void InputSystem::Action::AddKeyInput(int glfw_key)
{
    m_keys.push_back(glfw_key);
}

void InputSystem::Action::RemoveKeyInput(int glfw_key)
{
    removeByInput(&m_keys, glfw_key);
}

void InputSystem::Action::AddMouseInput(int glfw_mouse_button)
{
    m_mouse.push_back(glfw_mouse_button);
}

void InputSystem::Action::RemoveMouseInput(int glfw_mouse_button)
{
    removeByInput(&m_mouse, glfw_mouse_button);
}

void InputSystem::Action::AddControllerInput(int glfw_button)
{
    m_controller.push_back(glfw_button);
}

void InputSystem::Action::RemoveControllerInput(int glfw_button)
{
    removeByInput(&m_controller, glfw_button);
}

void InputSystem::Action::AddAxisAsInput(int glfw_axis_id)
{
    m_gamepadAxisAsInput.push_back(glfw_axis_id);
}

void InputSystem::Action::RemoveAxisAsInput(int glfw_axis_id)
{
    removeByInput(&m_gamepadAxisAsInput, glfw_axis_id);
}

void InputSystem::Action::AddKeyAxisPositive(int glfw_key)
{
    AddKeyInput(glfw_key);
}

void InputSystem::Action::AddKeyAxisNegative(int glfw_key_negative)
{
    m_keyAxis.push_back(glfw_key_negative);
}

void InputSystem::Action::RemoveKeyAxisPositive(int glfw_key)
{
    RemoveKeyInput(glfw_key);
}

void InputSystem::Action::RemoveKeyAxisNegative(int glfw_key_negative)
{
    removeByInput(&m_keyAxis, glfw_key_negative);
}

void InputSystem::Action::AddMouseAxisPositive(int glfw_mouse)
{
    m_mouse.push_back(glfw_mouse);
}

void InputSystem::Action::AddMouseAxisNegative(int glfw_mouse_negative)
{
    m_mouseAxis.push_back(glfw_mouse_negative);
}

void InputSystem::Action::RemoveMouseAxisPositive(int glfw_mouse)
{
    RemoveMouseInput(glfw_mouse);
}

void InputSystem::Action::RemoveMouseAxisNegative(int glfw_mouse_negative)
{
    removeByInput(&m_mouseAxis, glfw_mouse_negative);
}

void InputSystem::Action::AddControllerAxisPositive(int glfw_controller)
{
    AddControllerInput(glfw_controller);
}

void InputSystem::Action::AddControllerAxisNegative(int glfw_controller_negative)
{
    m_controllerAxis.push_back(glfw_controller_negative);
}

void InputSystem::Action::RemoveControllerAxisPositive(int glfw_controller)
{
    RemoveControllerInput(glfw_controller);
}

void InputSystem::Action::RemoveControllerAxisNegative(int glfw_controller_negative)
{
    removeByInput(&m_controllerAxis, glfw_controller_negative);
}

void InputSystem::Action::AddAxis(int glfw_axis_id)
{
    m_gamepadAxis.push_back(glfw_axis_id);
}

void InputSystem::Action::RemoveAxis(int glfw_axis_id)
{
    removeByInput(&m_gamepadAxis, glfw_axis_id);
}

void InputSystem::Action::SetName(std::string name)
{
    m_name = name;
}

std::string InputSystem::Action::GetName()
{
    return m_name;
}

void InputSystem::Action::SetDescription(std::string description)
{
    m_description = description;
}

std::string InputSystem::Action::GetDescription()
{
    return m_description;
}

bool InputSystem::Action::GetDown()
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
        if (Input()->GetGamepadAxisState(0, m_gamepadAxisAsInput[i]) > 0.1 ||
            Input()->GetGamepadAxisState(0, m_gamepadAxisAsInput[i]) < -0.1)
        {
            return true;
        }
    }

    return false;
}

bool InputSystem::Action::GetTriggered()
{
    return false;
}

bool InputSystem::Action::GetReleased()
{
    return false;
}

float InputSystem::Action::GetAxis()
{
    return 0.0f;
}