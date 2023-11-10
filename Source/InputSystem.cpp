/// @file InputSystem.cpp
/// @author Tyler Birdsall (tyler.birdsall@digipen.edu
/// @brief Input system, handles key and mouse inputs
/// @version 0.1
/// @date 2023-09-12
/// 
/// @copyright Copyright (c) 2023

#include "InputSystem.h"
#include "PlatformSystem.h"
#include <map>
#include "CameraSystem.h"
#include "Engine.h"
#include "glfw3.h"

/// @brief  updates map realted to fixed or standard update
void InputSystem::mapUpdate()
{
    Engine::UpdateMode mode = Engine::GetInstance()->GetCurrentUpdate();
    if (mode == Engine::UpdateMode::fixedUpdate)
    {
        m_KeyStatesHold = &m_FixedKeyStates;
        m_MouseStatesHold = &m_FixedMouseStates;
    }
    if (mode == Engine::UpdateMode::update)
    {
        m_KeyStatesHold = &m_KeyStates;
        m_MouseStatesHold = &m_MouseStates;
    }

    for (auto& key : *m_KeyStatesHold)
    {
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

    for (auto& key : *m_MouseStatesHold)
    {
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

/// @brief fixed update for input, must be called
void InputSystem::OnFixedUpdate()
{
    mapUpdate();
}

void InputSystem::OnUpdate(float dt)
{
    mapUpdate();
}



/// @brief checks if a given key is down
/// @param glfw key to check
/// @return returns if key is down
bool InputSystem::GetKeyDown(int glfw_key)
{
    Engine::UpdateMode mode = Engine::GetInstance()->GetCurrentUpdate();
    if (mode == Engine::UpdateMode::fixedUpdate)
    {
        return m_FixedKeyStates[glfw_key][0];
    }
    if (mode == Engine::UpdateMode::update)
    {
        return m_KeyStates[glfw_key][0];
    }
    return NULL;
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
    Engine::UpdateMode mode = Engine::GetInstance()->GetCurrentUpdate();
    if (mode == Engine::UpdateMode::fixedUpdate)
    {
        return m_FixedKeyStates[glfw_key][1];
    }
    if (mode == Engine::UpdateMode::update)
    {
        return m_KeyStates[glfw_key][1];
    }
    return NULL;
}

/// @brief checks if a given key is released
/// @param glfw key to check
/// @return returns if key is released
bool InputSystem::GetKeyReleased(int glfw_key)
{
    Engine::UpdateMode mode = Engine::GetInstance()->GetCurrentUpdate();
    if (mode == Engine::UpdateMode::fixedUpdate)
    {
        return m_FixedKeyStates[glfw_key][2];
    }
    if (mode == Engine::UpdateMode::update)
    {
        return m_KeyStates[glfw_key][2];
    }
    return NULL;
}


/// @brief checks if a given mouse button is down
/// @param glfw mouse button to check
/// @return returns if mouse button is down
bool InputSystem::GetMouseDown(int glfw_mouse_button)
{
    Engine::UpdateMode mode = Engine::GetInstance()->GetCurrentUpdate();
    if (mode == Engine::UpdateMode::fixedUpdate)
    {
        return m_FixedMouseStates[glfw_mouse_button][0];
    }
    if (mode == Engine::UpdateMode::update)
    {
        return m_MouseStates[glfw_mouse_button][0];
    }
    return NULL;
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
    Engine::UpdateMode mode = Engine::GetInstance()->GetCurrentUpdate();
    if (mode == Engine::UpdateMode::fixedUpdate)
    {
        return m_FixedMouseStates[glfw_mouse_button][1];
    }
    if (mode == Engine::UpdateMode::update)
    {
        return m_MouseStates[glfw_mouse_button][1];
    }
    return NULL;
}

/// @brief checks if a given mouse button is released
/// @param glfw mouse button to check
/// @return returns if mouse button is released
bool InputSystem::GetMouseReleased(int glfw_mouse_button)
{
    Engine::UpdateMode mode = Engine::GetInstance()->GetCurrentUpdate();
    if (mode == Engine::UpdateMode::fixedUpdate)
    {
        return m_FixedMouseStates[glfw_mouse_button][2];
    }
    if (mode == Engine::UpdateMode::update)
    {
        return m_MouseStates[glfw_mouse_button][2];
    }
    return NULL;
}

static glm::vec2 convert(glm::mat4 matrix)
{
    glm::vec2 vec = { 0 , 0 };
    glm::vec4 vector = { 0 , 0 , 0, 1 };
    double x = 0;
    double y = 0;
    glfwGetCursorPos(PlatformSystem::GetInstance()->GetWindowHandle(), &x, &y);
    vector[0] = (float)x;
    vector[1] = (float)y;
    vector = matrix * vector;
    vec[0] = vector[0];
    vec[1] = vector[1];
    return vec;
}
/// @brief gets mouse pos in UI space
/// @return returns the current mouse pos as a vec2
glm::vec2 InputSystem::GetMousePosUI()
{
    glm::mat4 matrix = Cameras()->GetMat_ScreenToUI();
    
    return convert(matrix);
}

/// @brief gets mouse pos in World space
/// @return returns the current mouse pos as a vec2
glm::vec2 InputSystem::GetMousePosWorld()
{
    glm::mat4 matrix = Cameras()->GetMat_ScreenToWorld();
    
    return convert(matrix);
}

//-----------------------------------------------------------------------------
// singleton stuff
//-----------------------------------------------------------------------------

    /// @brief Constructs the InputSystem
    InputSystem::InputSystem() :
        System( "InputSystem" ),
        m_KeyStatesHold(&m_KeyStates),
        m_MouseStatesHold(&m_MouseStates)
    {}

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
