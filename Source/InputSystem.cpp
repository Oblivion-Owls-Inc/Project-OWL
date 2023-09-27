/// @file InputSystem.cpp
/// @author Tyler Birdsall (tyler.birdsall@digipen.edu
/// @brief Input system, handles key and mouse inputs
/// @version 0.1
/// @date 2023-09-12
/// 
/// @copyright Copyright (c) 2023

#include "InputSystem.h"
#include "glfw3.h"
#include "PlatformSystem.h"
#include <map>


/// @brief Constructs the InputSystem
InputSystem::InputSystem() {}

/// @brief fixed update for input, must be called
void InputSystem::OnFixedUpdate()
{
    for (auto& key : keyStates) {
        bool old = key.second[0];
        key.second[0] = glfwGetKey(PlatformSystem::GetInstance()->GetWindowHandle(), 
            key.first);

        if (key.second[0] == true && old == false)
        {
            key.second[1] = true;
        }
        else
        {
            key.second[1] = false;
        }
        if (key.second[0] == false && old == true)
        {
            key.second[2] = true;
        }
        else
        {
            key.second[2] = false;
        }
    }

    for (auto& key : mouseStates) {
        bool old = key.second[0];
        key.second[0] = glfwGetMouseButton(PlatformSystem::GetInstance()->GetWindowHandle(),
            key.first);

        if (key.second[0] == true && old == false)
        {
            key.second[1] = true;
        }
        else
        {
            key.second[1] = false;
        }
        if (key.second[0] == false && old == true)
        {
            key.second[2] = true;
        }
        else
        {
            key.second[2] = false;
        }
    }
}

/// @brief checks if a given key is down
/// @param glfw key to check
/// @return returns if key is down
bool InputSystem::GetKeyDown(int glfw_key)
{
    return keyStates[glfw_key][0];
}

/// @brief checks if a given key is up
/// @param glfw key to check
/// @return returns if key is up
bool InputSystem::GetKeyUp(int glfw_key)
{
    return !GetKeyDown(glfw_key);
}

/// @brief checks if a given key is triggered
/// @param glfw key to check
/// @return returns if key is triggered
bool InputSystem::GetKeyTriggered(int glfw_key)
{
    return keyStates[glfw_key][1];
}

/// @brief checks if a given key is released
/// @param glfw key to check
/// @return returns if key is released
bool InputSystem::GetKeyReleased(int glfw_key)
{
    return keyStates[glfw_key][2];
}


/// @brief checks if a given mouse button is down
/// @param glfw mouse button to check
/// @return returns if mouse button is down
bool InputSystem::GetMouseDown(int glfw_mouse_button)
{
    return keyStates[glfw_mouse_button][0];
}

/// @brief checks if a given mouse button is up
/// @param glfw mouse button to check
/// @return returns if mouse button is up
bool InputSystem::GetMouseUp(int glfw_mouse_button)
{
    return !GetMouseDown(glfw_mouse_button);
}

/// @brief checks if a given mouse button is triggered
/// @param glfw mouse button to check
/// @return returns if mouse button is triggered
bool InputSystem::GetMouseTriggered(int glfw_mouse_button)
{
    return keyStates[glfw_mouse_button][1];
}

/// @brief checks if a given mouse button is released
/// @param glfw mouse button to check
/// @return returns if mouse button is released
bool InputSystem::GetMouseReleased(int glfw_mouse_button)
{
    return keyStates[glfw_mouse_button][2];
}


/// @brief gets mouse pos
/// @return returns the current mouse pos as a vec2
glm::vec2 InputSystem::GetMousePos()
{
    glm::vec2 vector = { 0 , 0 };
    double x = 0;
    double y = 0;
    glfwGetCursorPos(PlatformSystem::GetInstance()->GetWindowHandle(), &x, &y);
    vector[0] = (float)x;
    vector[1] = (float)y;
    return vector;
}


/// @brief The singleton instance of InputSystem
InputSystem * InputSystem::instance = nullptr;

/// @brief gets the instance of InputSystem
/// @return the instance of the InputSystem
InputSystem * InputSystem::GetInstance()
{
    if ( instance == nullptr )
    {
        instance = new InputSystem();
    }
    return instance;
}

//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------

    /// @brief map of the InputSystem read methods
    ReadMethodMap< InputSystem > const InputSystem::s_ReadMethods = {};

    /// @brief  gets this System's read methods
    /// @return this System's read methods
    ReadMethodMap< System > const& InputSystem::GetReadMethods() const
    {
        return (ReadMethodMap< System > const&)s_ReadMethods;
    }
