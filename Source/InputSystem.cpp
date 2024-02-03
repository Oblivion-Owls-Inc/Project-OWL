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


/// @brief  updates map realted to fixed or standard update
void InputSystem::mapUpdate()
{
    Engine::UpdateMode mode = Engine::GetInstance()->GetCurrentUpdate();
    if (mode == Engine::UpdateMode::fixedUpdate)
    {
        m_KeyStatesHold = &m_FixedKeyStates;
        m_MouseStatesHold = &m_FixedMouseStates;
        m_ControllerStatesHold = &m_FixedControllerStates;
    }
    if (mode == Engine::UpdateMode::update)
    {
        m_KeyStatesHold = &m_KeyStates;
        m_MouseStatesHold = &m_MouseStates;
        m_ControllerStatesHold = &m_ControllerStates;
    }

    int buttonCount = 0;
    const unsigned char* buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &buttonCount);

    for (auto& key : *m_KeyStatesHold)
    {
        bool old = key.second[0];
        key.second[0] = glfwGetKey(handle, key.first);

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
        key.second[0] = glfwGetMouseButton(handle, key.first);

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

    for (auto& key : *m_ControllerStatesHold)
    {
        key.second[1] = (key.second[0] == false && buttons[key.first] == GLFW_PRESS);
       
        key.second[2] = (key.second[0] == true && buttons[key.first] == GLFW_RELEASE);

        key.second[0] = buttons[key.first] == GLFW_PRESS;
    }

    for (int i = 0; i < amount; i++)
    {
        for (auto& key : windows[i])
        {
            bool old = key.second[0];
            key.second[0] = glfwGetKey(altHandles[i], key.first);

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
}

/// @brief  initialize system
void InputSystem::OnInit()
{
    handle = PlatformSystem::GetInstance()->GetWindowHandle();
}

/// @brief  exit system
void InputSystem::OnExit()
{
    
}

/// @brief fixed update for input, must be called
void InputSystem::OnFixedUpdate()
{
    mapUpdate();
}

/// @brief update system
void InputSystem::OnUpdate(float dt)
{
    mapUpdate();
}

int InputSystem::InitAlternateWindow(GLFWwindow* handle)
{
    altHandles.push_back(handle);
    map<int, bool[3]> newMap;
    windows.push_back(newMap);
    amount++;
    return amount;
}

/// @brief checks if a given key is down
/// @param glfw key to check
/// @return returns if key is down
bool InputSystem::GetKeyDown(int glfw_key, int altWindow)
{
    
    if (altWindow > 0)
    {
        return windows[altWindow - 1][glfw_key][0];
    }
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

/// @brief Checks if a given button is down.
/// @param glfw_button the button to check
/// @return returns if the button is down.
bool InputSystem::GetGamepadButtonDown(int glfw_button)
{
    Engine::UpdateMode mode = Engine::GetInstance()->GetCurrentUpdate();
    if (mode == Engine::UpdateMode::fixedUpdate)
    {
        return m_FixedControllerStates[glfw_button][0];
    }
    if (mode == Engine::UpdateMode::update)
    {
        return m_ControllerStates[glfw_button][0];
    }

    return NULL;
}

/// @brief checks if a given key is up
/// @param glfw key to check
/// @return returns if key is up
bool InputSystem::GetKeyUp(int glfw_key, int altWindow)
{
    return !GetKeyDown(glfw_key, altWindow);
}

bool InputSystem::GetGamepadButtonUp(int glfw_button)
{
    return !GetGamepadButtonDown(glfw_button);
}

/// @brief checks if a given key is triggered
/// @param glfw key to check
/// @return returns if key is triggered
bool InputSystem::GetKeyTriggered(int glfw_key, int altWindow)
{
    if (altWindow > 0)
    {
        return windows[altWindow - 1][glfw_key][1];
    }
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

/// @brief Check if a given button is triggered
/// @param glfw_button the button to check
/// @return returns if button is triggered.
bool InputSystem::GetGamepadButtonTriggered(int glfw_button)
{
    Engine::UpdateMode mode = Engine::GetInstance()->GetCurrentUpdate();
    if (mode == Engine::UpdateMode::fixedUpdate)
    {
        return m_FixedControllerStates[glfw_button][1];
    }
    if (mode == Engine::UpdateMode::update)
    {
        return m_ControllerStates[glfw_button][1];
    }
    return NULL;
}

/// @brief checks if a given key is released
/// @param glfw key to check
/// @return returns if key is released
bool InputSystem::GetKeyReleased(int glfw_key, int altWindow)
{
    if (altWindow > 0)
    {
        return windows[altWindow - 1][glfw_key][2];
    }
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

/// @brief Checks if a given button is released
/// @param glfw_button the button to check
/// @return returns if the button is released
bool InputSystem::GetGamepadButtonReleased(int glfw_button)
{
    Engine::UpdateMode mode = Engine::GetInstance()->GetCurrentUpdate();
    if (mode == Engine::UpdateMode::fixedUpdate)
    {
        return m_FixedControllerStates[glfw_button][2];
    }
    if (mode == Engine::UpdateMode::update)
    {
        return m_ControllerStates[glfw_button][2];
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

glm::vec2 InputSystem::convert(glm::mat4 matrix)
{
    glm::vec2 vec = { 0 , 0 };
    glm::vec4 vector = { 0 , 0 , 0, 1 };
    double x = 0;
    double y = 0;
    glfwGetCursorPos(handle, &x, &y);
    vector[0] = (float)x;
    vector[1] = (float)y;
    vector = matrix * vector;
    vec[0] = vector[0];
    vec[1] = vector[1];
    return vec;
}

/// @brief Get the position of the joystick
/// @param glfw_joystick the joystick to check
/// @return The position of the left joystick x axis.
float InputSystem::GetLeftThumbstickXAxis(int glfw_joystick)
{
    int axescount = 0;
    // Get the axes associated with the joystick
    const float* axes = glfwGetJoystickAxes(glfw_joystick, &axescount);
    if (!axes)
    {
        return 0.0f;
    }
    return axes[0];
}

/// @brief Get the position of the joystick y-axis
/// @param glfw_joystick the joystick to check
/// @return The position of the y-axis
float InputSystem::GetLeftThumbstickYAxis(int glfw_joystick)
{
    int axescount = 0;
    // Get the axes associated with the joystick
    const float* axes = glfwGetJoystickAxes(glfw_joystick, &axescount);
    if (!axes)
    {
        return 0.0f;
    }
    return axes[1];
}

/// @brief Get the position of the right thumbstick X axis
/// @param glw_joystick the jotsick position to get
/// @return The position of the right thumbstick X axis
float InputSystem::GetRightThumbstickXAxis(int glfw_joystick)
{
    int axescount = 0;
    // Get the axes associated with the joystick
    const float* axes = glfwGetJoystickAxes(glfw_joystick, &axescount);
    if (!axes)
    {
        return 0.0f;
    }
    return axes[2];
}

/// @brief Get the position of the right thumbstick Y axis
/// @param glw_joystick the jotsick position to get
/// @return The position of the right thumbstick Y axis
float InputSystem::GetRightThumbstickYAxis(int glfw_joystick)
{
    int axescount = 0;
    // Get the axes associated with the joystick
    const float* axes = glfwGetJoystickAxes(glfw_joystick, &axescount);
    if (!axes)
    {
        return 0.0f;
    }
    return axes[3];
}

/// @brief Get the position of the joystick's right trigger
/// @param glfw_joystick - the joystick
/// @return the position of the joystick's right trigger.
float InputSystem::GetRightTrigger(int glfw_joystick)
{
    int axescount = 0;
    // Get the axes associated with the joystick
    const float* axes = glfwGetJoystickAxes(glfw_joystick, &axescount);
    if (!axes)
    {
        return 0.0f;
    }
    return axes[5];
}

/// @brief Get the position of the joystick's left trigger
/// @param glfw_joystick - the joystick
/// @return the position of the joystick's left trigger.
float InputSystem::GetLeftTrigger(int glfw_joystick)
{
    int axescount = 0;
    // Get the axes associated with the joystick
    const float* axes = glfwGetJoystickAxes(glfw_joystick, &axescount);
    if (!axes)
    {
        return 0.0f;
    }
    return axes[4];
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
        m_MouseStatesHold(&m_MouseStates),
        m_ControllerStatesHold(&m_ControllerStates),
        handle(nullptr),
        amount(0)
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
