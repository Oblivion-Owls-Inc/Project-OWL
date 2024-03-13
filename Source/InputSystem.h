/// @file InputSystem.h
/// @author Tyler Birdsall (tyler.birdsall@digipen.edu
/// @brief Input system, m_Handles key and mouse inputs
/// @version 0.1
/// @date 2023-09-12
/// 
/// @copyright Copyright (c) 2023

#pragma once

#include "pch.h" // precompiled header has to be included first
#include "System.h"

#include "InputAction.h"
class ActionReference;

/// @brief Example System meant to be copy-pasted when creating new Systems
class InputSystem : public System
{
//-----------------------------------------------------------------------------
private: // virtual override methods
//-----------------------------------------------------------------------------

    /// @brief  Gets called when system initializes
    virtual void OnInit() override;

    /// @brief  Gets called when system exits
    virtual void OnExit() override;

    /// @brief fixed update for input, must be called for input to function
    virtual void OnFixedUpdate() override;

    /// @brief update for input, must be called for input to function
    virtual void OnUpdate(float dt) override;

//-----------------------------------------------------------------------------
public: // inspection
//-----------------------------------------------------------------------------


    /// @brief Gets Called by the Debug system to display debug information
    virtual void DebugWindow() override;


    /// @brief  gets the name of a GLFW keyboard button
    /// @param  glfwId  the ID of the keyboard button to get the name of
    /// @return the name of the keyboard button
    char const* GetKeyboardButtonName( int glfwId ) const;

    /// @brief  gets the name of a GLFW mouse button
    /// @param  glfwId  the ID of the mouse button to get the name of
    /// @return the name of the mouse button
    char const* GetMouseButtonName( int glfwId ) const;

    /// @brief  gets the name of a GLFW controller button
    /// @param  glfwId  the ID of the controller button to get the name of
    /// @return the name of the controller button
    char const* GetControllerButtonName( int glfwId ) const;

    /// @brief  gets the name of a GLFW controller axis
    /// @param  glfwId  the ID of the controller axis to get the name of
    /// @return the name of the controller axis
    char const* GetControllerAxisName( int glfwId ) const;


//-----------------------------------------------------------------------------
private: // private variables
//-----------------------------------------------------------------------------
    
    // checks to indicate an action is being changed
    /// @brief  int definition of which change state is occuring
    int m_ChangingAction = 0;
    /// @brief  name of action changing
    std::string m_WhichAction = "";

    // bool array 0 down 1 triggered 2 released
    /// @brief  window pointer
    GLFWwindow* m_Handle;

    // debug window chech
    /// @brief  is input open
    bool m_InputIsOpen = false;

    // pointers to maps for MapUpdate
    /// @brief  pointer for key states
    std::array< bool[ 3 ], GLFW_KEY_LAST + 1 >* m_KeyStatesHold;
    /// @brief  pointer for mouse states
    std::array< bool[ 3 ], GLFW_MOUSE_BUTTON_LAST + 1 >* m_MouseStatesHold;
    /// @brief  pointer for controller states
    std::array< bool[ 3 ], GLFW_GAMEPAD_BUTTON_LAST + 1 >* m_ControllerStatesHold;

    // maps for various states (keys, mouse, controller etc)
    /// @brief  key states map
    std::array< bool[ 3 ], GLFW_KEY_LAST + 1 > m_KeyStates;
    /// @brief  fixed update key states map
    std::array< bool[ 3 ], GLFW_KEY_LAST + 1 > m_FixedKeyStates;
    /// @brief  controller states map
    std::array< bool[ 3 ], GLFW_GAMEPAD_BUTTON_LAST + 1 > m_ControllerStates;
    /// @brief  fixed update controller states map
    std::array< bool[ 3 ], GLFW_GAMEPAD_BUTTON_LAST + 1 > m_FixedControllerStates;
    /// @brief  mouse states map
    std::array< bool[ 3 ], GLFW_MOUSE_BUTTON_LAST + 1 > m_MouseStates;
    /// @brief  fixed update mouse states map
    std::array< bool[ 3 ], GLFW_MOUSE_BUTTON_LAST + 1 > m_FixedMouseStates;

    // m_Handles for alternate windows
    /// @brief  alternate window m_Handles
    std::vector<GLFWwindow*> m_AltHandles;
    /// @brief  map for additional windows
    std::vector<std::map<int, bool[3]>> windows;
    /// @brief  m_Amount of additional windows
    int m_Amount = 0;

    /// @brief  how much the mouse has scrolled this graphicsframe
    float m_DeltaScroll = 0.0f;
    /// @brief  how much the mouse has scrolled this simulation frame
    float m_FixedDeltaScroll = 0.0f;


    /// @brief  whether a controller is plugged in and was used more recently than mouse+keyboard
    bool m_ControllerIsMostRecentInput = false;


    /// @brief  map of actions
    std::vector<Action> m_Actions = {};


    /// @brief  the ActionReferences currently attached to the InputSystem
    std::vector< ActionReference* > m_ActionReferences = {};


//-----------------------------------------------------------------------------
private: // private methods
//-----------------------------------------------------------------------------

    /// @brief  updates map realted to fixed or standard update
    void mapUpdate();

    // callback called whenever the mouse scrolls
    static void onMouseScrollCallback(GLFWwindow* window, double scrollX, double scrollY);

//-----------------------------------------------------------------------------
public: // accessors
//-----------------------------------------------------------------------------

    /// @brief gets the instance of InputSystem
    /// @return the instance of the InputSystem
    static InputSystem * GetInstance();

    GLFWwindow* GetHandle() { return m_Handle; }

    /// @brief  sets a new window m_Handle
    /// @param  m_Handle of new window
    /// @return int, pass this back to check window
    int InitAlternateWindow(GLFWwindow* m_Handle);

    /// @brief checks if a given key is down
    /// @param glfw key to check
    /// @return returns if key is down
    bool GetKeyDown(int glfw_key, int altWindow = 0);

    /// @brief Checks if a given game pad button is down
    /// @param glfw_key button to check
    /// @return returns if the button is down.
    bool GetGamepadButtonDown(int glfw_button);

    /// @brief checks if a given key is up
    /// @param glfw key to check
    /// @return returns if key is up
    bool GetKeyUp(int glfw_key, int altWindow = 0);

    /// @brief Checks if a given game pad button is up
    /// @param glfw_key button to check
    /// @return returns if the button is up.
    bool GetGamepadButtonUp(int glfw_button);

    /// @brief checks if a given key is triggered
    /// @param glfw key to check
    /// @return returns if key is triggered
    bool GetKeyTriggered(int glfw_key, int altWindow = 0);

    /// @brief Checks if a given game pad button is triggered.
    /// @param glfw_key button to check
    /// @return returns if the button is triggered.
    bool GetGamepadButtonTriggered(int glfw_button);

    /// @brief checks if a given key is released
    /// @param glfw key to check
    /// @return returns if key is released
    bool GetKeyReleased(int glfw_key, int altWindow = 0);

    /// @brief Checks if a given game pad button is released.
    /// @param glfw_button button to check
    /// @return returns if the button is released.
    bool GetGamepadButtonReleased(int glfw_button);

    /// @brief Checks the state of a passed in input key
    /// @param JID   - The ID of the controller to grab.
    /// @param input - The button or axis to check.
    /// @return Returns the state of an axis (float).
    float GetGamepadAxisState(int gamepad_id, int axis_id);
    
    /// @brief checks if a given mouse button is down
    /// @param glfw mouse button to check
    /// @return returns if mouse button is down
    bool GetMouseDown(int glfw_mouse_button);

    /// @brief checks if a given mouse button is up
    /// @param glfw mouse button to check
    /// @return returns if mouse button is up
    bool GetMouseUp(int glfw_mouse_button);

    /// @brief checks if a given mouse button is triggered
    /// @param glfw mouse button to check
    /// @return returns if mouse button is triggered
    bool GetMouseTriggered(int glfw_mouse_button);

    /// @brief checks if a given mouse button is released
    /// @param glfw mouse button to check
    /// @return returns if mouse button is released
    bool GetMouseReleased(int glfw_mouse_button);

    /// @brief  gets the mouse pos in screen space
    /// @return the current mouse pos in screen space
    glm::vec2 GetMousePosScreen() const;

    /// @brief gets mouse pos in UI space
    /// @return returns the current mouse pos as a vec2
    glm::vec2 GetMousePosUI();

    /// @brief gets mouse pos in world space
    /// @return returns the current mouse pos as a vec2
    glm::vec2 GetMousePosWorld();

    /// @brief  gets how much the mouse has scrolled since last frame
    /// @return how much the mouse has scrolled since last frame
    float GetMouseDeltaScroll();

    /// @brief  gets an action by its name
    /// @param  name name of the action
    /// @return  pointer to the action
    Action* GetActionByName(std::string const& name);

    /// @brief  gets the vector of Actions in the InputSystem
    /// @return the vector of Actions
    std::vector< Action > const& GetActions() const;


    /// @brief  gets the currently down keyboard button
    /// @return the GLFW id of the currently down keyboard button (-1 if no button down)
    int GetCurrentKeyboardButton() const;

    /// @brief  gets the currently down mouse button
    /// @return the GLFW id of the currently down mouse button (-1 if no button down)
    int GetCurrentMouseButton() const;

    /// @brief  gets the currently down controller button
    /// @return the GLFW id of the currently down controller button (-1 if no button down)
    int GetCurrentControllerButton() const;

    /// @brief  gets the currently down controller axis
    /// @return the GLFW id of the currently down controller axis (-1 if no axis active)
    int GetCurrentControllerAxis() const;


    /// @brief  gets whether the most recent input was from a controller and a controller is plugged in
    /// @return whether the most recent input was from a controller
    bool IsControllerMostRecentInput() const;

    /// @brief  gets whether there is a playstation controller plugged in
    /// @return whether there is a playstation controller plugged in
    bool ControllerIsPlaystation() const;


    /// @brief  adds an ActionReference to the InputSystem
    /// @param  actionReference the ActionReference to add
    void AddActionReference( ActionReference* actionReference );

    /// @brief  removes an ActionReference from the InputSystem
    /// @param  actionReference the ActionReference to remove
    void RemoveActionReference( ActionReference* actionReference );


//-----------------------------------------------------------------------------
private: // singleton stuff
//-----------------------------------------------------------------------------

    // Prevent copying
    InputSystem(InputSystem& other) = delete;
    void operator=(const InputSystem&) = delete;

    /// @brief Constructs the InputSystem
    InputSystem();

    /// @brief The singleton instance of InputSystem
    static InputSystem* instance;

//-----------------------------------------------------------------------------
public: // reading/writing
//-----------------------------------------------------------------------------

    /// @brief  map of the SceneSystem read methods
    static ReadMethodMap< InputSystem > const s_ReadMethods;

    /// @brief  gets this System's read methods
    /// @return this System's read methods
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override
    {
        return (ReadMethodMap< ISerializable > const&)s_ReadMethods;
    }

//-----------------------------------------------------------------------------
private: // reading
//-----------------------------------------------------------------------------

    /// @brief  read actions for input
    void readActions(nlohmann::ordered_json const& data);

//-----------------------------------------------------------------------------
public: // writing
//-----------------------------------------------------------------------------

    /// @brief  writes this System to json
    virtual nlohmann::ordered_json Write() const override;
};

/// @brief shortens input get instance to simply input
/// @return returns the input system instance
__inline InputSystem* Input() { return InputSystem::GetInstance(); }