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

    GLFWgamepadstate state;
    glfwGetGamepadState(GLFW_JOYSTICK_1, &state);

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
        key.second[1] = (key.second[0] == false && state.buttons[key.first] == GLFW_PRESS);
       
        key.second[2] = (key.second[0] == true && state.buttons[key.first] == GLFW_RELEASE);

        key.second[0] = state.buttons[key.first] == GLFW_PRESS;
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

//-----------------------------------------------------------------------------
// accessors
//-----------------------------------------------------------------------------

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

/// @brief Checks the state of a passed in input key
/// @param JID   - The ID of the controller to grab.
/// @param input - The button or axis to check.
/// @return Returns the state of an axis (float).
float InputSystem::GetGamepadAxisState(int gamepad_id, int axis_id)
{
    // Check if the joy stick is present
    if (glfwJoystickPresent(gamepad_id) == GLFW_TRUE)
    {
        GLFWgamepadstate state;
        glfwGetGamepadState(gamepad_id, &state);
        return state.axes[axis_id];
    }

    return 0.0f;
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


/// @brief  convert matrix to vec2
/// @param  transformation  the transformation to apply to the screen-space mouse position
glm::vec2 InputSystem::getMousePosAfterTransformation( glm::mat4 const& transformation ) const
{
    glm::vec< 2, double > mousePosRaw;
    glfwGetCursorPos( Platform()->GetWindowHandle(), &mousePosRaw.x, &mousePosRaw.y );

    glm::vec2 mousePosScreen = mousePosRaw; // convert from double to float
    mousePosScreen -= Platform()->GetGameWindowPos();

    return transformation * glm::vec4( mousePosScreen, 0.0f, 1.0f );
}

/// @brief gets mouse pos in UI space
/// @return returns the current mouse pos as a vec2
glm::vec2 InputSystem::GetMousePosUI()
{
    glm::mat4 matrix = Cameras()->GetMat_ScreenToUI();
    
    return getMousePosAfterTransformation( matrix );
}

/// @brief gets mouse pos in World space
/// @return returns the current mouse pos as a vec2
glm::vec2 InputSystem::GetMousePosWorld()
{
    glm::mat4 matrix = Cameras()->GetMat_ScreenToWorld();
    
    return getMousePosAfterTransformation( matrix );
}

/// @brief  gets an action by its name
/// @param  name name of the action
/// @retun  pointer to the action
InputSystem::Action* InputSystem::GetActionByName(std::string name)
{
    return nullptr;
}

//-----------------------------------------------------------------------------
// private class implimentation
//-----------------------------------------------------------------------------

InputSystem::Action::Action(std::string name, std::string description) :
m_name(name), m_description(description)
{
}

void InputSystem::Action::Flush()
{
}

void InputSystem::Action::AddKeyInput(int glfw_key)
{
}

void InputSystem::Action::RemoveKeyInput(int glfw_key)
{
}

void InputSystem::Action::AddMouseInput(int glfw_mouse_button)
{
}

void InputSystem::Action::RemoveMouseInput(int glfw_mouse_button)
{
}

void InputSystem::Action::AddControllerInput(int glfw_button)
{
}

void InputSystem::Action::RemoveControllerInput(int glfw_button)
{
}

void InputSystem::Action::AddAxisAsInput(int glfw_axis_id)
{
}

void InputSystem::Action::RemoveAxisAsInput(int glfw_axis_id)
{
}

void InputSystem::Action::AddKeyAxisPositive(int glfw_key)
{
}

void InputSystem::Action::AddKeyAxisNegative(int glfw_key_negative)
{
}

void InputSystem::Action::RemoveKeyAxisPositive(int glfw_key)
{
}

void InputSystem::Action::RemoveKeyAxisNegative(int glfw_key_negative)
{
}

void InputSystem::Action::AddMouseAxisPositive(int glfw_mouse)
{
}

void InputSystem::Action::AddMouseAxisNegative(int glfw_mouse_negative)
{
}

void InputSystem::Action::RemoveMouseAxisPositive(int glfw_mouse)
{
}

void InputSystem::Action::RemoveMouseAxisNegative(int glfw_mouse_negative)
{
}

void InputSystem::Action::AddControllerAxisPositive(int glfw_controller)
{
}

void InputSystem::Action::AddControllerAxisNegative(int glfw_controller_negative)
{
}

void InputSystem::Action::RemoveControllerAxisPositive(int glfw_controller)
{
}



void InputSystem::Action::RemoveControllerAxisNegative(int glfw_controller_negative)
{
}

void InputSystem::Action::AddAxis(int glfw_axis_id)
{
}

void InputSystem::Action::RemoveAxis(int glfw_axis_id)
{
}

void InputSystem::Action::SetName(std::string name)
{
}

std::string InputSystem::Action::GetName()
{
    return std::string();
}

bool InputSystem::Action::GetDown()
{
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
    {
        // Updates the mapping of gamepad controllers.
        glfwUpdateGamepadMappings(Stream::ReadFromTXTFile("Data/Controller Mappings/gamecontrollerdb.txt").c_str());
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
