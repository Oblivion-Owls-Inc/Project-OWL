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


/// @brief Example System meant to be copy-pasted when creating new Systems
using namespace std;
class InputSystem : public System
{
//-----------------------------------------------------------------------------
private: // virtual override methods
//-----------------------------------------------------------------------------

    /// @brief fixed update for input, must be called for input to function
    virtual void OnFixedUpdate() override;

//-----------------------------------------------------------------------------

     /// @brief Constructs the InputSystem
    InputSystem();


     /// @brief The singleton instance of InputSystem
    static InputSystem * instance;
protected:
    
    map<int, bool[3]> m_KeyStates;
    map<int, bool[3]> m_MouseStates;

public:

    /// @brief gets the instance of InputSystem
    /// @return the instance of the InputSystem
    static InputSystem * GetInstance();

    /// @brief checks if a given key is down
    /// @param glfw key to check
    /// @return returns if key is down
    bool GetKeyDown(int glfw_key);

    /// @brief checks if a given key is up
    /// @param glfw key to check
    /// @return returns if key is up
    bool GetKeyUp(int glfw_key);

    /// @brief checks if a given key is triggered
    /// @param glfw key to check
    /// @return returns if key is triggered
    bool GetKeyTriggered(int glfw_key);

    /// @brief checks if a given key is released
    /// @param glfw key to check
    /// @return returns if key is released
    bool GetKeyReleased(int glfw_key);
    
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