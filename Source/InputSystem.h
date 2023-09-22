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

    /// @brief Gets called once when this System is added to the Engine
    virtual void OnInit() override {}


    /// @brief Gets called once every simulation frame. Use this function for anything that affects the simulation.
    virtual void OnFixedUpdate() override;


    /// @brief Gets called once every graphics frame. Do not use this function for anything that affects the simulation.
    /// @param dt the elapsed time in seconds since the previous frame
    virtual void OnUpdate( float dt ) override {}


    /// @brief Gets called once before the Engine closes
    virtual void OnExit() override {}


    /// @brief Gets called whenever a new Scene is loaded
    virtual void OnSceneLoad() override {}


    /// @brief Gets called whenever a scene is initialized
    virtual void OnSceneInit() override {}


    /// @brief Gets called whenever a scene is exited
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
    static InputSystem * GetInstance();

    // MAY INCLUDE A DUPLICATE OF THE ABOVE FUNCTION WITH CONSTRUCTOR ARGUMENTS HERE
    
    bool GetKeyDown(int glfw_key);
    bool GetKeyUp(int glfw_key);
    bool GetKeyTriggered(int glfw_key);
    bool GetKeyReleased(int glfw_key);
    
    bool GetMouseDown(int glfw_mouse_button);
    bool GetMouseUp(int glfw_mouse_button);
    bool GetMouseTriggered(int glfw_mouse_button);
    bool GetMouseReleased(int glfw_mouse_button);
    
    glm::vec2 GetMousePos();

    // Prevent copying
    InputSystem(InputSystem& other) = delete;
    void operator=(const InputSystem&) = delete;
};

__inline InputSystem* Input() { return InputSystem::GetInstance(); }