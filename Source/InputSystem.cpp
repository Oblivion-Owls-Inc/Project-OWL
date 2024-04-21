/// \file InputSystem.cpp
/// \author Tyler Birdsall (tyler.birdsall@digipen.edu)
/// \brief Input system, m_Handles key and mouse inputs
/// \version 0.1
/// \date 2023-09-12
/// 
/// \copyright Copyright (c) 2024 Digipen Institute of Technology


#include "pch.h" // precompiled header has to be included first
#include "InputSystem.h"
#include "PlatformSystem.h"
#include "CameraSystem.h"
#include "Engine.h"
#include "DebugSystem.h"

#include "ActionReference.h"


/// @brief  updates map realted to fixed or standard update
void InputSystem::mapUpdate()
{
    Engine::UpdateMode mode = GameEngine()->GetCurrentUpdate();
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

    for ( int key = 0; key < m_KeyStatesHold->size(); ++key )
    {
        auto& state = (*m_KeyStatesHold)[ key ];
        bool old = state[ 0 ];
        state[ 0 ] = glfwGetKey( m_Handle, key );

        if ( state[ 0 ] )
        {
            m_ControllerIsMostRecentInput = false;
        }

        if (state[0] == true && old == false)
        {
            state[1] = true;
        }
        else
        {
            state[1] = false;
        }
        if (state[0] == false && old == true)
        {
            state[2] = true;
        }
        else
        {
            state[2] = false;
        }
    }

    for ( int key = 0; key < m_MouseStatesHold->size(); ++key )
    {
        auto& state = (*m_MouseStatesHold)[ key ];
        bool old = state[0];
        state[0] = glfwGetMouseButton(m_Handle, key);

        if ( state[ 0 ] )
        {
            m_ControllerIsMostRecentInput = false;
        }

        if (state[0] == true && old == false)
        {
            state[1] = true;
        }
        else
        {
            state[1] = false;
        }
        if (state[0] == false && old == true)
        {
            state[2] = true;
        }
        else
        {
            state[2] = false;
        }
    }

    GLFWgamepadstate controllerState;
    glfwGetGamepadState(GLFW_JOYSTICK_1, &controllerState);

    for ( int key = 0; key < m_ControllerStatesHold->size(); ++key )
    {
        auto& state = (*m_ControllerStatesHold)[ key ];

        state[1] = (state[0] == false && controllerState.buttons[key] == GLFW_PRESS);
       
        state[2] = (state[0] == true && controllerState.buttons[key] == GLFW_RELEASE);

        state[0] = controllerState.buttons[key] == GLFW_PRESS;

        if ( state[ 0 ] )
        {
            m_ControllerIsMostRecentInput = true;
        }
    }

    for (int i = 0; i < m_Amount; i++)
    {
        for (auto& key : windows[i])
        {
            bool old = key.second[0];
            key.second[0] = glfwGetKey(m_AltHandles[i], key.first);

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
    m_Handle = PlatformSystem::GetInstance()->GetWindowHandle();

    glfwSetScrollCallback( m_Handle, onMouseScrollCallback );
}

/// @brief  exit system
void InputSystem::OnExit()
{

}

/// @brief fixed update for input, must be called
void InputSystem::OnFixedUpdate()
{
    mapUpdate();

    m_FixedDeltaScroll = 0.0f;
}

/// @brief update system
void InputSystem::OnUpdate(float dt)
{
    mapUpdate();

    m_DeltaScroll = 0.0f;
    glfwPollEvents();
}

int InputSystem::InitAlternateWindow(GLFWwindow* m_Handle)
{
    m_AltHandles.push_back(m_Handle);
    std::map<int, bool[3]> newMap;
    windows.push_back(newMap);
    m_Amount++;
    return m_Amount;
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
    Engine::UpdateMode mode = GameEngine()->GetCurrentUpdate();
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
    Engine::UpdateMode mode = GameEngine()->GetCurrentUpdate();
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
    Engine::UpdateMode mode = GameEngine()->GetCurrentUpdate();
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
    Engine::UpdateMode mode = GameEngine()->GetCurrentUpdate();
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
    Engine::UpdateMode mode = GameEngine()->GetCurrentUpdate();
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
    Engine::UpdateMode mode = GameEngine()->GetCurrentUpdate();
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
    Engine::UpdateMode mode = GameEngine()->GetCurrentUpdate();
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
    Engine::UpdateMode mode = GameEngine()->GetCurrentUpdate();
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
    Engine::UpdateMode mode = GameEngine()->GetCurrentUpdate();
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


/// @brief  gets the mouse pos in screen space
/// @return the current mouse pos in screen space
glm::vec2 InputSystem::GetMousePosScreen() const
{
    glm::vec< 2, double > mousePosRaw;
    glfwGetCursorPos( Platform()->GetWindowHandle(), &mousePosRaw.x, &mousePosRaw.y );

    glm::vec2 mousePosScreen = mousePosRaw; // convert from double to float
    return mousePosScreen - Platform()->GetGameWindowPos();
}


/// @brief gets mouse pos in UI space
/// @return returns the current mouse pos as a vec2
glm::vec2 InputSystem::GetMousePosUI()
{
    return Cameras()->GetMat_ScreenToUi() * glm::vec4( GetMousePosScreen(), 0.0f, 1.0f );
}

/// @brief gets mouse pos in World space
/// @return returns the current mouse pos as a vec2
glm::vec2 InputSystem::GetMousePosWorld()
{
    return Cameras()->GetMat_ScreenToWorld() * glm::vec4( GetMousePosScreen(), 0.0f, 1.0f );
}


    /// @brief  gets how much the mouse has scrolled since last frame
    /// @return how much the mouse has scrolled since last frame
    float InputSystem::GetMouseDeltaScroll()
    {
        if ( GameEngine()->GetCurrentUpdate() == Engine::UpdateMode::fixedUpdate )
        {
            return m_FixedDeltaScroll;
        }
        else
        {
            return m_DeltaScroll;
        }
    }


//-----------------------------------------------------------------------------
// private: methods
//-----------------------------------------------------------------------------


    // callback called whenever the mouse scrolls
    void InputSystem::onMouseScrollCallback( GLFWwindow*, double, double scrollY )
    {
        Input()->m_DeltaScroll += (float)scrollY;
        Input()->m_FixedDeltaScroll += (float)scrollY;
    }

    /// @brief  gets an action by its name
    /// @param  name name of the action
    /// @retun  pointer to the action
    Action* InputSystem::GetActionByName(std::string const& name)
    {
        for (int i = 0; i < m_Actions.size(); ++i)
        {
            if (m_Actions[i].GetName() == name)
            {
                return &(m_Actions[i]);
            }
        }
        return nullptr;
    }


    /// @brief  gets the vector of Actions in the InputSystem
    /// @return the vector of Actions
    std::vector< Action > const& InputSystem::GetActions() const
    {
        return m_Actions;
    }


    /// @brief  gets the currently down keyboard button
    /// @return the GLFW id of the currently down keyboard button (-1 if no button down)
    int InputSystem::GetCurrentKeyboardButton() const
    {
        for ( int i = 0; i <= GLFW_KEY_LAST; ++i )
        {
            if ( glfwGetKey( Input()->GetHandle(), i ) == GLFW_PRESS )
            {
                return i;
            }
        }

        return -1;
    }

    /// @brief  gets the currently down mouse button
    /// @return the GLFW id of the currently down mouse button (-1 if no button down)
    int InputSystem::GetCurrentMouseButton() const
    {
        for ( int i = 0; i <= GLFW_MOUSE_BUTTON_LAST; ++i )
        {
            if ( glfwGetMouseButton( Input()->GetHandle(), i ) == GLFW_PRESS )
            {
                return i;
            }
        }

        return -1;
    }

    /// @brief  gets the currently down controller button
    /// @return the GLFW id of the currently down controller button (-1 if no button down)
    int InputSystem::GetCurrentControllerButton() const
    {
        if ( glfwJoystickIsGamepad( 0 ) == GLFW_FALSE )
        {
            return -1;
        }

        GLFWgamepadstate gamepadState;
        glfwGetGamepadState( 0, &gamepadState );

        for ( int i = 0; i <= GLFW_GAMEPAD_BUTTON_LAST; ++i )
        {
            if ( gamepadState.buttons[ i ] == GLFW_PRESS )
            {
                return i;
            }
        }

        return -1;
    }

    /// @brief  gets the currently down controller axis
    /// @return the GLFW id of the currently down controller axis (-1 if no axis active)
    int InputSystem::GetCurrentControllerAxis() const
    {
        if ( glfwJoystickIsGamepad( 0 ) == GLFW_FALSE )
        {
            return -1;
        }

        GLFWgamepadstate gamepadState;
        glfwGetGamepadState( 0, &gamepadState );

        for ( int i = 0; i <= GLFW_GAMEPAD_AXIS_LAST; ++i )
        {
            if ( i == GLFW_GAMEPAD_AXIS_LEFT_TRIGGER || i == GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER )
            {
                if ( gamepadState.axes[ i ] > -0.5f )
                {
                    return i;
                }
            }
            else
            {
                if ( gamepadState.axes[ i ] > 0.5f || gamepadState.axes[ i ] < -0.5f )
                {
                    return i;
                }
            }
        }

        return -1;
    }


    /// @brief  was the most recent input from a controller
    /// @return whether the most recent input was from a controller
    bool InputSystem::IsControllerMostRecentInput() const
    {
        return m_ControllerIsMostRecentInput && glfwJoystickIsGamepad( GLFW_JOYSTICK_1 );
    }

    /// @brief  gets whether there is a playstation controller plugged in
    /// @return whether there is a playstation controller plugged in
    bool InputSystem::ControllerIsPlaystation() const
    {
        if ( glfwJoystickIsGamepad( GLFW_JOYSTICK_1 ) == GLFW_FALSE )
        {
            return false;
        }

        std::string name = glfwGetJoystickName( GLFW_JOYSTICK_1 );
        return name == "PS4 Controller" || name == "PS5 Controller";
    }


    /// @brief  adds an ActionReference to the InputSystem
    /// @param  actionReference the ActionReference to add
    void InputSystem::AddActionReference( ActionReference* actionReference )
    {
        m_ActionReferences.push_back( actionReference );
    }

    /// @brief  removes an ActionReference from the InputSystem
    /// @param  actionReference the ActionReference to remove
    void InputSystem::RemoveActionReference( ActionReference* actionReference )
    {
        auto it = std::find( m_ActionReferences.begin(), m_ActionReferences.end(), actionReference );
        if ( it == m_ActionReferences.end() )
        {
            char const* actionReferenceName = actionReference == nullptr ? "nullptr" : actionReference->GetOwnerName().c_str();
            Debug() << "Error: could not find ActionReference owned by \"" << actionReferenceName << "\" to remove" << std::endl;
            return;
        }

        m_ActionReferences.erase( it );
    }


//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------


    /// @brief  read actions for input
    void InputSystem::readActions( nlohmann::ordered_json const& data )
    {
        Stream::Read< Action >( &m_Actions, data );
    }


/// @brief  map of the SceneSystem read methods
ReadMethodMap< InputSystem > const InputSystem::s_ReadMethods = {
    { "Actions", &readActions }
};

/// @brief  writes this System to json
nlohmann::ordered_json InputSystem::Write() const
{
    nlohmann::ordered_json data;

    nlohmann::ordered_json& writeActions = data["Actions"];
    for (int i = 0; i < m_Actions.size(); i++)
    {
        writeActions.push_back(m_Actions[i].Write());
    }

    return data;
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
        m_Handle(nullptr),
        m_Amount(0)
    {
        // Updates the mapping of gamepad controllers.
        glfwUpdateGamepadMappings(Stream::ReadFromTXTFile("Data/Controller Mappings/gamecontrollerdb.txt").c_str());
    }

    /// @brief gets the instance of InputSystem
    /// @return the instance of the InputSystem
    InputSystem * InputSystem::GetInstance()
    {
        static std::unique_ptr< InputSystem > s_Instance = nullptr;

        if (s_Instance == nullptr )
        {
            s_Instance.reset( new InputSystem());
        }
        return s_Instance.get();
    }

//-----------------------------------------------------------------------------
