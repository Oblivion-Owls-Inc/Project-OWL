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

    /// @brief  Gets called when system exits
    virtual void OnExit() override;

    /// @brief fixed update for input, must be called for input to function
    virtual void OnFixedUpdate() override;

    /// @brief update for input, must be called for input to function
    virtual void OnUpdate(float dt) override;

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
public: // inspection
//-----------------------------------------------------------------------------


    /// @brief Gets Called by the Debug system to display debug information
    virtual void DebugWindow() override;

    /// @brief Constructs the InputSystem
    InputSystem();


    /// @brief The singleton instance of InputSystem
    static InputSystem * instance;

//-----------------------------------------------------------------------------
public: // public variables
//-----------------------------------------------------------------------------

    // checks to indicate an action is being changed
    int M_changeingAction = 0;
    std::string M_whichAction = "";

private:
    class Action; // forward reference

protected:
    
    // bool array 0 down 1 triggered 2 released
    GLFWwindow* handle;

    // debug window chech
    bool m_InputIsOpen = false;

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

    // number of actions attached to input
    int m_numActions = 0;

    

    // map of actions
    vector<Action> m_Actions;

//-----------------------------------------------------------------------------
private: // private methods
//-----------------------------------------------------------------------------

    /// @brief  updates map realted to fixed or standard update
    void mapUpdate();

    /// @brief  convert matrix to vec2
    /// @param  transformation  the transformation to apply to the screen-space mouse position
    glm::vec2 getMousePosAfterTransformation( glm::mat4 const& transformation ) const;

//-----------------------------------------------------------------------------
public: // public class
//-----------------------------------------------------------------------------

    /// @brief  action class, used to track an action with dynamic input
    class Action : public ISerializable
    {
    private:
        /// @brief  key inputs
        std::vector<int> m_keys; // 0
        /// @brief  mouse inputs
        std::vector<int> m_mouse; // 1
        /// @brief  controller inputs
        std::vector<int> m_controller; // 2
        /// @brief  opposing axis for key inputs
        std::vector<int> m_keyAxis; // 3
        /// @brief  opposing axis for mouse inputs
        std::vector<int> m_mouseAxis; // 4
        /// @brief  opposing axis for controller inputs
        std::vector<int> m_controllerAxis; // 5
        /// @brief  gamepad axis as input, sticks/triggers
        std::vector<int> m_gamepadAxisAsInput; // 6
        /// @brief  gamepad axis, sticks/triggers
        std::vector<int> m_gamepadAxis; // 7
        /// @brief  action name
        std::string m_name;
        /// @brief  editor description of action
        std::string m_description;

    private:

        /// @brief  remove an input from action by input id
        /// @param  vector to remove from
        /// @param  input id to remove
        void removeByInput(std::vector<int>* vector, int input);
        
        /// @brief the map of read methods for this Component
        static ReadMethodMap< Action > const s_ReadMethods;

        /// @brief read the key inputs for an action
        void readName(nlohmann::ordered_json const& json);

        /// @brief read the key inputs for an action
        void readDescription(nlohmann::ordered_json const& json);

        /// @brief read the key inputs for an action
        void readKeys(nlohmann::ordered_json const& json);

        /// @brief read the mouse inputs for an action
        void readMouse(nlohmann::ordered_json const& json);

        /// @brief read the controller inputs for an action
        void readController(nlohmann::ordered_json const& json);

        /// @brief read the key axis inputs for an action
        void readKeyAxis(nlohmann::ordered_json const& json);

        /// @brief read the mouse axis inputs for an action
        void readMouseAxis(nlohmann::ordered_json const& json);

        /// @brief read the controller axis inputs for an action
        void readControllerAxis(nlohmann::ordered_json const& json);

        /// @brief read the gamepad axis as input for an action
        void readGamepadAxisAsInput(nlohmann::ordered_json const& json);

        /// @brief read the gamepad axis inputs for an action
        void readGamepadAxis(nlohmann::ordered_json const& json);

    public:

        /// @brief read method map for an Action
        virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override
        {
            return (ReadMethodMap< ISerializable > const&)s_ReadMethods;
        }

        /// @brief	write an Action to json
        virtual nlohmann::ordered_json Write() const override;

    public:

        /// @brief  contructor
        /// @param  name of the action
        /// @param  description of the action (viewable in editor)
        Action(std::string name = "", std::string description = "");
        
        /// @brief  removes all inputs for this action 
        ///         and empties name/description
        void Flush();

        /// @brief  retrieves a private vector for inspector
        /// @param  vector number
        std::vector<int> GetActionVector(int vector) const;

        /// @brief  adds a key to the action
        /// @param  key to add
        void AddKeyInput(int glfw_key);

        /// @brief  removes a key from the action
        /// @param  key to remove
        void RemoveKeyInput(int glfw_key);
        
        /// @brief  adds a mouse input to the action
        /// @param  mouse input to add
        void AddMouseInput(int glfw_mouse_button);

        /// @brief  removes a mouse input from the action
        /// @param  mouse input to remove
        void RemoveMouseInput(int glfw_mouse_button);

        /// @brief  adds a controller to the action
        /// @param  controller input to add
        void AddControllerInput(int glfw_button);

        /// @brief  removes a controller input from the action
        /// @param  controller input to remove
        void RemoveControllerInput(int glfw_button);

        /// @brief  adds an axis as a button (triggers)
        /// @param  axis to add as input
        void AddAxisAsInput(int glfw_axis_id);

        /// @brief  removes an axis as a button (triggers)
        /// @param  axis to remove from input
        void RemoveAxisAsInput(int glfw_axis_id);

        /// @brief  adds a key input axis
        /// @param  glfw_key positive axis key
        void AddKeyAxisPositive(int glfw_key);

        /// @brief  adds a key input axis
        /// @param  glfw_key_negative negative axis key
        void AddKeyAxisNegative(int glfw_key_negative);

        /// @brief  removes a key input axis
        /// @param  glfw_key positive axis key
        void RemoveKeyAxisPositive(int glfw_key);

        /// @brief  removes a key input axis
        /// @param  glfw_key_negative negative axis key
        void RemoveKeyAxisNegative(int glfw_key_negative);

        /// @brief  adds a mouse input axis
        /// @param  glfw_mouse positive axis mouse
        void AddMouseAxisPositive(int glfw_mouse);

        /// @brief  adds a mouse input axis
        /// @param  glfw_mouse_negative negative axis mouse
        void AddMouseAxisNegative(int glfw_mouse_negative);

        /// @brief  removes a mouse input axis
        /// @param  glfw_mouse positive axis mouse
        void RemoveMouseAxisPositive(int glfw_mouse);

        /// @brief  removes a mouse input axis
        /// @param  glfw_mouse_negative negative axis mouse
        void RemoveMouseAxisNegative(int glfw_mouse_negative);

        /// @brief  adds a controller input axis
        /// @param  glfw_controller positive axis controller
        void AddControllerAxisPositive(int glfw_controller);

        /// @brief  adds a controller input axis
        /// @param  glfw_controller_negative negative axis controller
        void AddControllerAxisNegative(int glfw_controller_negative);

        /// @brief  removes a controller input axis
        /// @param  glfw_controller positive axis controller
        /// @param  glfw_controller_negative negative axis controller
        void RemoveControllerAxisPositive(int glfw_controller);

        /// @brief  removes a controller input axis
        /// @param  glfw_controller positive axis controller
        /// @param  glfw_controller_negative negative axis controller
        void RemoveControllerAxisNegative(int glfw_controller_negative);

        /// @brief  adds a gamepad axis (stick/trigger)
        /// @param  axis id to add
        void AddAxis(int glfw_axis_id);
        
        /// @brief  removes a gamepad axis (stick/trigger)
        /// @param  axis id to add
        void RemoveAxis(int glfw_axis_id);

        /// @brief  sets the name of the action
        /// @param  new name of action
        void SetName(std::string& name);

        /// @brief  gets the name of this action
        /// @return the name of the action
        std::string GetName() const;

        /// @brief  sets the description of the action
        /// @param  new description of action
        void SetDescription(std::string& discription);

        /// @brief  gets the description of this action
        /// @return the description of the action
        std::string GetDescription() const;

        /// @brief  gets if this action is down
        /// @return action down status
        bool GetDown() const;

        /// @brief  gets if this action is triggered
        /// @return action triggered status
        bool GetTriggered() const;
        
        /// @brief  gets if this action is released
        /// @return action released status
        bool GetReleased() const;

        /// @brief  gets this action as an axis
        /// @return axis status clamped to -1 and 1
        float GetAxis() const;
    };

//-----------------------------------------------------------------------------
public: // accessors
//-----------------------------------------------------------------------------

    /// @brief gets the instance of InputSystem
    /// @return the instance of the InputSystem
    static InputSystem * GetInstance();

    GLFWwindow* GetHandle() { return handle; }

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

    /// @brief  gets an action by its name
    /// @param  name name of the action
    /// @retun  pointer to the action
    Action* GetActionByName(std::string& name);

//-----------------------------------------------------------------------------
private: // reading
//-----------------------------------------------------------------------------


    /// @brief  map of the SceneSystem read methods
    static ReadMethodMap< InputSystem > const s_ReadMethods;

    /// @brief  gets this System's read methods
    /// @return this System's read methods
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override
    {
        return (ReadMethodMap< ISerializable > const&)s_ReadMethods;
    }

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