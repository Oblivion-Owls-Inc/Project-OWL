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


    /// @brief the Read Methods used in this System
    static std::map< std::string, ReadMethod< InputSystem > > const ReadMethods;

    /// @brief Gets the read methods of this System
    /// @return the map of read methods of this System
    virtual std::map< std::string, ReadMethod< System > > const& GetReadMethods() override;


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

    // MAY INCLUDE A DUPLICATE OF THE ABOVE FUNCTION WITH CONSTRUCTOR ARGUMENTS HERE
    
    bool getKeyDown(int glfw_key);
    bool getKeyUp(int glfw_key);
    bool getKeyTriggered(int glfw_key);
    bool getKeyReleased(int glfw_key);
    
    bool getMouseDown(int glfw_mouse_button);
    bool getMouseUp(int glfw_mouse_button);
    bool getMouseTriggered(int glfw_mouse_button);
    bool getMouseReleased(int glfw_mouse_button);
    
    glm::vec2 getMousePos();

    // Prevent copying
    InputSystem(InputSystem& other) = delete;
    void operator=(const InputSystem&) = delete;
};

__inline InputSystem* Input() { return InputSystem::getInstance(); }