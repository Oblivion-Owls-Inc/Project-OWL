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
        key.second[0] = glfwGetKey(PlatformSystem::getInstance()->GetWindowHandle(), 
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
        key.second[0] = glfwGetMouseButton(PlatformSystem::getInstance()->GetWindowHandle(),
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
bool InputSystem::getKeyDown(int glfw_key)
{
    return keyStates[glfw_key][0];
}

/// @brief checks if a given key is up
/// @param glfw key to check
/// @return returns if key is up
bool InputSystem::getKeyUp(int glfw_key)
{
    return !getKeyDown(glfw_key);
}

/// @brief checks if a given key is triggered
/// @param glfw key to check
/// @return returns if key is triggered
bool InputSystem::getKeyTriggered(int glfw_key)
{
    return keyStates[glfw_key][1];
}

/// @brief checks if a given key is released
/// @param glfw key to check
/// @return returns if key is released
bool InputSystem::getKeyReleased(int glfw_key)
{
    return keyStates[glfw_key][2];
}


/// @brief checks if a given mouse button is down
/// @param glfw mouse button to check
/// @return returns if mouse button is down
bool InputSystem::getMouseDown(int glfw_mouse_button)
{
    return keyStates[glfw_mouse_button][0];
}

/// @brief checks if a given mouse button is up
/// @param glfw mouse button to check
/// @return returns if mouse button is up
bool InputSystem::getMouseUp(int glfw_mouse_button)
{
    return !getMouseDown(glfw_mouse_button);
}

/// @brief checks if a given mouse button is triggered
/// @param glfw mouse button to check
/// @return returns if mouse button is triggered
bool InputSystem::getMouseTriggered(int glfw_mouse_button)
{
    return keyStates[glfw_mouse_button][1];
}

/// @brief checks if a given mouse button is released
/// @param glfw mouse button to check
/// @return returns if mouse button is released
bool InputSystem::getMouseReleased(int glfw_mouse_button)
{
    return keyStates[glfw_mouse_button][2];
}


/// @brief gets mouse pos
/// @return returns the current mouse pos as a vec2
glm::vec2 InputSystem::getMousePos()
{
    glm::vec2 vector = { 0 , 0 };
    double x = 0;
    double y = 0;
    glfwGetCursorPos(PlatformSystem::getInstance()->GetWindowHandle(), &x, &y);
    vector[0] = (float)x;
    vector[1] = (float)y;
    return vector;
}


/// @brief The singleton instance of InputSystem
InputSystem * InputSystem::instance = nullptr;

/// @brief gets the instance of InputSystem
/// @return the instance of the InputSystem
InputSystem * InputSystem::getInstance()
{
    if ( instance == nullptr )
    {
        instance = new InputSystem();
    }
    return instance;
}

