/// @file       InputAction.h
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      action class, used to track an action with dynamic input
/// @version    0.1
/// @date       2024-03-11
/// 
/// @copyright  Copyright (c) 2024 Digipen Institute of Technology


#pragma once

#include "pch.h"

#include "ISerializable.h"


/// @brief  action class, used to track an action with dynamic input
class Action : public ISerializable
{
//-------------------------------------------------------------------------
public: // types
//-------------------------------------------------------------------------

    
    /// @brief  enum of types of inputs for Actions
    enum class InputType
    {
        KeyboardButton = 0      , /// @brief  keyboard key used as positive input
        MouseButton             , /// @brief  mouse button used as positive input
        ControllerButton        , /// @brief  controller button used as positive input
        KeyboardButtonNegative  , /// @brief  keyboard key used as negative input
        MouseButtonNegative     , /// @brief  mouse button used as negative input
        ControllerButtonNegative, /// @brief  controller button used as negative input
        ControllerAxisAsButton  , /// @brief  controller axis used as button
        ControllerAxis          , /// @brief  controller axis used as axis

        _Count                  , /// @brief  the number of different input types
        _First = 0              , /// @brief  the first input type (for looping)
    };


//-------------------------------------------------------------------------
public: // constructor
//-------------------------------------------------------------------------


    /// @brief  contructor
    /// @param  name    the name of the action
    Action( std::string const& name = "" );


    /// @brief  move-constructor
    /// @param  other   the Action to move into this one
    Action( Action&& other );

    
//-------------------------------------------------------------------------
public: // methods
//-------------------------------------------------------------------------


    /// @brief  adds an input to the action
    /// @param  glfwId  the GLFW id of the input to add
    void AddInput( InputType inputType, int glfwId );

    /// @brief  removes an input from the action
    /// @param  glfwId  the GLFW id of the input to add
    void RemoveInput( InputType inputType, int glfwId );


    /// @brief  gets the name of this action
    /// @return the name of the action
    std::string const& GetName() const;

    /// @brief  sets the name of the action
    /// @param  new name of action
    void SetName( std::string const& name );


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


    /// @brief  clears all data from this Action
    void Clear();


    /// @brief  gets the array of inputs of the specified type
    /// @param  inputType   the type of input to get the inputs of
    std::vector< int > const& GetInputVector( InputType inputType ) const;


//-------------------------------------------------------------------------
private: // member variables
//-------------------------------------------------------------------------


    /// @brief  the vectors of each different input type
    std::array< std::vector< int >, (int)InputType::_Count > m_Inputs;


    /// @brief  the name of this action
    std::string m_Name = "";

    
//-------------------------------------------------------------------------
public: // inspection
//-------------------------------------------------------------------------


    /// @brief  inspects this Action
    /// @return whether this Action was changed
    bool Inspect();

    
//-------------------------------------------------------------------------
private: // inspection
//-------------------------------------------------------------------------


    


//-------------------------------------------------------------------------
private: // reading
//-------------------------------------------------------------------------


    /// @brief  read the key inputs for an action
    /// @param  data    the JSON data to read from
    void readName( nlohmann::ordered_json const& data );


    /// @brief  read the key inputs for an action
    /// @param  data    the JSON data to read from
    void readKeyboardButtons( nlohmann::ordered_json const& data );

    /// @brief  read the mouse inputs for an action
    /// @param  data    the JSON data to read from
    void readMouseButtons( nlohmann::ordered_json const& data );

    /// @brief  read the controller inputs for an action
    /// @param  data    the JSON data to read from
    void readControllerButtons( nlohmann::ordered_json const& data );

    /// @brief  read the key axis inputs for an action
    /// @param  data    the JSON data to read from
    void readKeyboardButtonsNegative( nlohmann::ordered_json const& data );

    /// @brief  read the mouse axis inputs for an action
    /// @param  data    the JSON data to read from
    void readMouseButtonsNegative( nlohmann::ordered_json const& data );

    /// @brief  read the controller axis inputs for an action
    /// @param  data    the JSON data to read from
    void readControllerButtonsNegative( nlohmann::ordered_json const& data );

    /// @brief  read the gamepad axis as input for an action
    /// @param  data    the JSON data to read from
    void readControllerAxisAsButton( nlohmann::ordered_json const& data );

    /// @brief  read the gamepad axis inputs for an action
    /// @param  data    the JSON data to read from
    void readControllerAxis( nlohmann::ordered_json const& data );


//-------------------------------------------------------------------------
public: // reading / writing
//-------------------------------------------------------------------------


    /// @brief read method map for an Action
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override;

    /// @brief	write an Action to json
    virtual nlohmann::ordered_json Write() const override;


//-------------------------------------------------------------------------
};