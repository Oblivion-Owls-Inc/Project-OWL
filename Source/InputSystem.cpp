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

bool InputSystem::getKeyDown(int glfw_key)
{
    return keyStates[glfw_key][0];
}

bool InputSystem::getKeyUp(int glfw_key)
{
    return !getKeyDown(glfw_key);
}

bool InputSystem::getKeyTriggered(int glfw_key)
{
    return keyStates[glfw_key][1];
}

bool InputSystem::getKeyReleased(int glfw_key)
{
    return keyStates[glfw_key][2];
}



bool InputSystem::getMouseDown(int glfw_mouse_button)
{
    return keyStates[glfw_mouse_button][0];
}

bool InputSystem::getMouseUp(int glfw_mouse_button)
{
    return !getMouseDown(glfw_mouse_button);
}

bool InputSystem::getMouseTriggered(int glfw_mouse_button)
{
    return keyStates[glfw_mouse_button][1];
}

bool InputSystem::getMouseReleased(int glfw_mouse_button)
{
    return keyStates[glfw_mouse_button][2];
}



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


/// @brief the Read Methods used in this System
std::map< std::string, ReadMethod< InputSystem > > const InputSystem::ReadMethods = {};

/// @brief Gets the read methods of this System
/// @return the map of read methods of this System
std::map< std::string, ReadMethod< System > > const& InputSystem::GetReadMethods()
{
    return (std::map< std::string, ReadMethod< System > > const&)ReadMethods;
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

// MAY INCLUDE A DUPLICATE OF THE ABOVE FUNCTION WITH CONSTRUCTOR ARGUMENTS HERE
