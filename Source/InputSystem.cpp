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

bool InputSystem::GetKeyDown(int glfw_key)
{
    return keyStates[glfw_key][0];
}

bool InputSystem::GetKeyUp(int glfw_key)
{
    return !GetKeyDown(glfw_key);
}

bool InputSystem::GetKeyTriggered(int glfw_key)
{
    return keyStates[glfw_key][1];
}

bool InputSystem::GetKeyReleased(int glfw_key)
{
    return keyStates[glfw_key][2];
}



bool InputSystem::GetMouseDown(int glfw_mouse_button)
{
    return keyStates[glfw_mouse_button][0];
}

bool InputSystem::GetMouseUp(int glfw_mouse_button)
{
    return !GetMouseDown(glfw_mouse_button);
}

bool InputSystem::GetMouseTriggered(int glfw_mouse_button)
{
    return keyStates[glfw_mouse_button][1];
}

bool InputSystem::GetMouseReleased(int glfw_mouse_button)
{
    return keyStates[glfw_mouse_button][2];
}



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

// MAY INCLUDE A DUPLICATE OF THE ABOVE FUNCTION WITH CONSTRUCTOR ARGUMENTS HERE
