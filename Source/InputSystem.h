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


//-----------------------------------------------------------------------------
private: // reading
//-----------------------------------------------------------------------------

    /// @brief map of the InputSystem read methods
    static ReadMethodMap< InputSystem > const s_ReadMethods;

    /// @brief  gets this System's read methods
    /// @return this System's read methods
    virtual ReadMethodMap< System > const& GetReadMethods() const override;

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