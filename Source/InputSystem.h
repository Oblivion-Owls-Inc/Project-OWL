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
private:

    /// @brief unused input init
    virtual void OnInit() override {}


    /// @brief fixed update for input, must be called for input to function
    virtual void OnFixedUpdate() override;


    /// @brief unused update for input
    /// @param dt the elapsed time in seconds since the previous frame
    virtual void OnUpdate( float dt ) override {}


    /// @brief unused exit for input
    virtual void OnExit() override {}


    /// @brief unused for input
    virtual void OnSceneLoad() override {}


    /// @brief unused for input
    virtual void OnSceneInit() override {}


    /// @brief unused for input
    virtual void OnSceneExit() override {}


    /// @brief Loads the config data of this System
    /// @param configData the JSON object with all of the configData for this System
    virtual void Load( rapidjson::Value const& configData ) override {}


    /// @brief Constructs the InputSystem
    InputSystem();


     /// @brief The singleton instance of InputSystem
    static InputSystem * instance;
protected:
    
    map<int, bool[3]> keyStates;
    map<int, bool[3]> mouseStates;

public:

    /// @brief gets the instance of InputSystem
    /// @return the instance of the InputSystem
    static InputSystem * getInstance();

    /// @brief checks if a given key is down
    /// @param glfw key to check
    /// @return returns if key is down
    bool getKeyDown(int glfw_key);

    /// @brief checks if a given key is up
    /// @param glfw key to check
    /// @return returns if key is up
    bool getKeyUp(int glfw_key);

    /// @brief checks if a given key is triggered
    /// @param glfw key to check
    /// @return returns if key is triggered
    bool getKeyTriggered(int glfw_key);

    /// @brief checks if a given key is released
    /// @param glfw key to check
    /// @return returns if key is released
    bool getKeyReleased(int glfw_key);
    
    /// @brief checks if a given mouse button is down
    /// @param glfw mouse button to check
    /// @return returns if mouse button is down
    bool getMouseDown(int glfw_mouse_button);

    /// @brief checks if a given mouse button is up
    /// @param glfw mouse button to check
    /// @return returns if mouse button is up
    bool getMouseUp(int glfw_mouse_button);

    /// @brief checks if a given mouse button is triggered
    /// @param glfw mouse button to check
    /// @return returns if mouse button is triggered
    bool getMouseTriggered(int glfw_mouse_button);

    /// @brief checks if a given mouse button is released
    /// @param glfw mouse button to check
    /// @return returns if mouse button is released
    bool getMouseReleased(int glfw_mouse_button);
    
    /// @brief gets mouse pos
    /// @return returns the current mouse pos as a vec2
    glm::vec2 getMousePos();

    // Prevent copying
    InputSystem(InputSystem& other) = delete;
    void operator=(const InputSystem&) = delete;
};

/// @brief shortens input get instance to simply input
/// @return returns the input system instance
__inline InputSystem* Input() { return InputSystem::getInstance(); }