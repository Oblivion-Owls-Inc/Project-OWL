/// @file InputSystem.h
/// @author Tyler Birdsall (tyler.birdsall@digipen.edu
/// @brief Input system, handles key and mouse inputs
/// @version 0.1
/// @date 2023-09-12
/// 
/// @copyright Copyright (c) 2023

#pragma once

#include <map>
#include "System.h"
#include "glm/glm.hpp"
#include "glfw3.h"


/// @brief Example System meant to be copy-pasted when creating new Systems
using namespace std;
class InputSystem : public System
{
//-----------------------------------------------------------------------------
private: // virtual override methods
//-----------------------------------------------------------------------------

    /// @brief  Gets called when system initializes
    virtual void OnInit() override;

    /// @brief  Gets called when system initializes
    virtual void OnExit() override;

    /// @brief fixed update for input, must be called for input to function
    virtual void OnFixedUpdate() override;

    /// @brief update for input, must be called for input to function
    virtual void OnUpdate(float dt) override;

//-----------------------------------------------------------------------------

    /// @brief Constructs the InputSystem
    InputSystem();


    /// @brief The singleton instance of InputSystem
    static InputSystem * instance;



protected:
    
    // bool array 0 down 1 triggered 2 released
    GLFWwindow* handle;

    // pointers to maps for MapUpdate
    map<int, bool[3]>* m_KeyStatesHold;
    map<int, bool[3]>* m_MouseStatesHold;
    map<int, bool[3]>* m_ControllerStatesHold;

    // maps for various states (keys, mouse, controller etc)
    map<int, bool[3]> m_KeyStates;
    map<int, bool[3]> m_FixedKeyStates;
    map<int, bool[3]> m_ControllerStates;
    map<int, bool[3]> m_FixedControllerStates;
    map<int, bool[3]> m_MouseStates;
    map<int, bool[3]> m_FixedMouseStates;

    // handles for alternate windows
    std::vector<GLFWwindow*> altHandles;
    std::vector<map<int, bool[3]>> windows;
    int amount = 0;

//-----------------------------------------------------------------------------
private: // private methods
//-----------------------------------------------------------------------------

    /// @brief  updates map realted to fixed or standard update
    void mapUpdate();

    /// @brief convert matrix to vec2
    glm::vec2 convert(glm::mat4 matrix);

    

//-----------------------------------------------------------------------------
public: // accessors
//-----------------------------------------------------------------------------

    /// @brief gets the instance of InputSystem
    /// @return the instance of the InputSystem
    static InputSystem * GetInstance();

    /// @brief  sets a new window handle
    /// @param  handle of new window
    /// @return int, pass this back to check window
    int InitAlternateWindow(GLFWwindow* handle);

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

    /// @brief gets mouse pos in UI space
    /// @return returns the current mouse pos as a vec2
    glm::vec2 GetMousePosUI();

    /// @brief gets mouse pos in world space
    /// @return returns the current mouse pos as a vec2
    glm::vec2 GetMousePosWorld();

    // Prevent copying
    InputSystem(InputSystem& other) = delete;
    void operator=(const InputSystem&) = delete;
};

/// @brief shortens input get instance to simply input
/// @return returns the input system instance
__inline InputSystem* Input() { return InputSystem::GetInstance(); }