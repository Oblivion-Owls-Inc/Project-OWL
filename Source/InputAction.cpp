/// @file       InputAction.h
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      action class, used to track an action with dynamic input
/// @version    0.1
/// @date       2024-03-11
/// 
/// @copyright  Copyright (c) 2024 Digipen Institute of Technology


#include "pch.h"

#include "InputAction.h"

#include "InputSystem.h"


//-------------------------------------------------------------------------
// public: constructor
//-------------------------------------------------------------------------


    /// @brief  contructor
    /// @param  name    the name of the action
    Action::Action( std::string const& name ) :
        m_Name( name )
    {}


    /// @brief  move-constructor
    /// @param  other   the Action to move into this one
    Action::Action( Action&& other ) noexcept :
        m_Inputs( std::move( other.m_Inputs ) ),
        m_Name( std::move( other.m_Name ) )
    {}

    
//-------------------------------------------------------------------------
// public: methods
//-------------------------------------------------------------------------


    /// @brief  adds an input to the action
    /// @param  glfwId  the GLFW id of the input to add
    void Action::AddInput( InputType inputType, int glfwId )
    {
        m_Inputs[ (int)inputType ].push_back( glfwId );
    }

    /// @brief  removes an input from the action
    /// @param  glfwId  the GLFW id of the input to add
    void Action::RemoveInput( InputType inputType, int glfwId )
    {
        std::vector< int >& inputVector = m_Inputs[ (int)inputType ];
        auto it = std::find( inputVector.begin(), inputVector.end(), glfwId );
        if ( it == inputVector.end() )
        {
            return;
        }

        inputVector.erase( it );
    }


    /// @brief  gets the name of this action
    /// @return the name of the action
    std::string const& Action::GetName() const
    {
        return m_Name;
    }

    /// @brief  sets the name of the action
    /// @param  new name of action
    void Action::SetName( std::string const& name )
    {
        m_Name = name;
    }


    /// @brief  gets if this action is down
    /// @return action down status
    bool Action::GetDown() const
    {
        // keyboard buttons
        for ( int key : m_Inputs[ (int)InputType::KeyboardButton ] )
        {
            if ( Input()->GetKeyDown( key ) )
            {
                return true;
            }
        }

        // mouse buttons
        for ( int mouseButton : m_Inputs[ (int)InputType::MouseButton ] )
        {
            if ( Input()->GetMouseDown( mouseButton ) )
            {
                return true;
            }
        }

        // skip remaining of no controller connected
        if ( glfwJoystickIsGamepad( 0 ) == GLFW_FALSE )
        {
            return false;
        }

        // controller buttons
        for ( int controllerButton : m_Inputs[ (int)InputType::ControllerButton ] )
        {
            if ( Input()->GetGamepadButtonDown( controllerButton ) )
            {
                return true;
            }
        }

        // controller axes
        for ( int controllerAxis : m_Inputs[ (int)InputType::ControllerAxisAsButton ] )
        {
            float axisValue = Input()->GetGamepadAxisState( 0, controllerAxis );

            // special case to handle triggers
            if ( controllerAxis == GLFW_GAMEPAD_AXIS_LEFT_TRIGGER || controllerAxis == GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER )
            {
                if ( axisValue > -0.5f )
                {
                    return true;
                }
            }
            else if ( axisValue >  0.5f || axisValue < -0.5f )
            {
                return true;
            }
        }
        

        return false;
    }

    /// @brief  gets if this action is triggered
    /// @return action triggered status
    bool Action::GetTriggered() const
    {
        bool triggered = false;

        // keyboard buttons
        for ( int key : m_Inputs[ (int)InputType::KeyboardButton ] )
        {
            if ( Input()->GetKeyDown( key ) )
            {
                if ( Input()->GetKeyTriggered( key ) )
                {
                    triggered = true;
                }
                else
                {
                    return false;
                }
            }
        }

        // mouse buttons
        for ( int mouseButton : m_Inputs[ (int)InputType::MouseButton ] )
        {
            if ( Input()->GetMouseDown( mouseButton ) )
            {
                if ( Input()->GetMouseTriggered( mouseButton ) )
                {
                    triggered = true;
                }
                else
                {
                    return false;
                }
            }
        }

        // skip remaining of no controller connected
        if ( glfwJoystickIsGamepad( 0 ) == GLFW_FALSE )
        {
            return triggered;
        }


        // controller buttons
        for ( int controllerButton : m_Inputs[ (int)InputType::ControllerButton ] )
        {
            if ( Input()->GetGamepadButtonDown( controllerButton ) )
            {
                if ( Input()->GetGamepadButtonTriggered( controllerButton ) )
                {
                    triggered = true;
                }
                else
                {
                    return false;
                }
            }
        }

        return triggered;
    }

    /// @brief  gets if this action is released
    /// @return action released status
    bool Action::GetReleased() const
    {
        bool released = false;

        // keyboard buttons
        for ( int key : m_Inputs[ (int)InputType::KeyboardButton ] )
        {
            if ( Input()->GetKeyDown( key ) )
            {
                return false;
            }

            if ( Input()->GetKeyReleased( key ) )
            {
                released = true;
            }
        }

        // mouse buttons
        for ( int mouseButton : m_Inputs[ (int)InputType::MouseButton ] )
        {
            if ( Input()->GetMouseDown( mouseButton ) )
            {
                return false;
            }

            if ( Input()->GetMouseReleased( mouseButton ) )
            {
                released = true;
            }
        }

        // skip remaining of no controller connected
        if ( glfwJoystickIsGamepad( 0 ) == GLFW_FALSE )
        {
            return released;
        }


        // controller buttons
        for ( int controllerButton : m_Inputs[ (int)InputType::ControllerButton ] )
        {
            if ( Input()->GetGamepadButtonDown( controllerButton ) )
            {
                return false;
            }

            if ( Input()->GetGamepadButtonReleased( controllerButton ) )
            {
                released = true;
            }
        }

        return released;
    }

    /// @brief  gets this action as an axis
    /// @return axis status clamped to -1 and 1
    float Action::GetAxis() const
    {
        float result = 0.0f;

        // keyboard buttons
        for ( int key : m_Inputs[ (int)InputType::KeyboardButton ] )
        {
            if ( Input()->GetKeyDown( key ) )
            {
                result += 1.0f;
            }
        }
        for ( int key : m_Inputs[ (int)InputType::KeyboardButtonNegative ] )
        {
            if ( Input()->GetKeyDown( key ) )
            {
                result -= 1.0f;
            }
        }

        // mouse buttons
        for ( int mouseButton : m_Inputs[ (int)InputType::MouseButton ] )
        {
            if ( Input()->GetMouseDown( mouseButton ) )
            {
                result += 1.0f;
            }
        }
        for ( int mouseButton : m_Inputs[ (int)InputType::MouseButtonNegative ] )
        {
            if ( Input()->GetMouseDown( mouseButton ) )
            {
                result -= 1.0f;
            }
        }

        // skip remaining of no controller connected
        if ( glfwJoystickIsGamepad( 0 ) == GLFW_FALSE )
        {
            return std::clamp( result, -1.0f, 1.0f );
        }

        // controller buttons
        for ( int controllerButton : m_Inputs[ (int)InputType::ControllerButton ] )
        {
            if ( Input()->GetGamepadButtonDown( controllerButton ) )
            {
                result += 1.0f;
            }
        }
        for ( int controllerButton : m_Inputs[ (int)InputType::ControllerButtonNegative ] )
        {
            if ( Input()->GetGamepadButtonDown( controllerButton ) )
            {
                result -= 1.0f;
            }
        }

        // controller axes
        for ( int controllerAxis : m_Inputs[ (int)InputType::ControllerAxis ] )
        {
            float axisAddition = Input()->GetGamepadAxisState( 0, controllerAxis );
            if ( controllerAxis == GLFW_GAMEPAD_AXIS_LEFT_TRIGGER || controllerAxis == GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER )
            {
                if ( axisAddition > 0.0f )
                {
                    result += axisAddition;
                }
            }
            else if (axisAddition > 0.5f || axisAddition < -0.5f )
            {
                if ( controllerAxis == GLFW_GAMEPAD_AXIS_LEFT_Y || controllerAxis == GLFW_GAMEPAD_AXIS_RIGHT_Y )
                {
                    axisAddition *= -1.0f;
                }
                result += axisAddition;
            }
        }

        return std::clamp( result, -1.0f, 1.0f );
    }


    /// @brief  clears all data from this Action
    void Action::Clear()
    {
        for ( std::vector< int >& vector : m_Inputs )
        {
            vector.clear();
        }

        m_Name.clear();
    }


    /// @brief  gets the array of inputs of the specified type
    /// @param  inputType   the type of input to get the inputs of
    std::vector< int > const& Action::GetInputVector( InputType inputType ) const
    {
        return m_Inputs[ (int)inputType ];
    }

    
//-------------------------------------------------------------------------
// public: inspection
//-------------------------------------------------------------------------


    /// @brief  inspects this Action
    /// @return whether this Action was changed
    bool Action::Inspect()
    {
        if ( ImGui::TreeNode( m_Name.c_str() ) == false )
        {
            return false;
        }

        bool changed = ImGui::InputText( "name", &m_Name );


        // struct used to inspect each input type in a loop
        struct InputTypeInfo
        {
            char const* label;
            char const* (InputSystem::*getInputName)( int glfwId ) const;
            int (InputSystem::*getCurrentInput)() const;
        };

        // labels and function pointers to use for each input type
        InputTypeInfo constexpr inputTypeInfos[ (int)InputType::_Count ] = {
            { "Keyboard Buttons"           , &InputSystem::GetKeyboardButtonName  , &InputSystem::GetCurrentKeyboardButton   },
            { "Mouse Buttons"              , &InputSystem::GetMouseButtonName     , &InputSystem::GetCurrentMouseButton      },
            { "Controller Buttons"         , &InputSystem::GetControllerButtonName, &InputSystem::GetCurrentControllerButton },
            { "Keyboard Buttons Negative"  , &InputSystem::GetKeyboardButtonName  , &InputSystem::GetCurrentKeyboardButton   },
            { "Mouse Buttons Negative"     , &InputSystem::GetMouseButtonName     , &InputSystem::GetCurrentMouseButton      },
            { "Controller Buttons Negative", &InputSystem::GetControllerButtonName, &InputSystem::GetCurrentControllerButton },
            { "Controller Axes As Buttons" , &InputSystem::GetControllerAxisName  , &InputSystem::GetCurrentControllerAxis   },
            { "Controller Axes"            , &InputSystem::GetControllerAxisName  , &InputSystem::GetCurrentControllerAxis   }
        };

        for ( int inputType = (int)InputType::_First; inputType < (int)InputType::_Count; ++inputType )
        {

            // skip TreeNodes that are closed
            if ( ImGui::TreeNode( inputTypeInfos[ inputType ].label ) == false )
            {
                continue;
            }

            // the vector of inputs to modify this loop
            std::vector< int >& inputVector = m_Inputs[ inputType ];


            // display the currently configured inputs
            ImGui::Text( "Inputs:" );
            for ( int inputId : inputVector )
            {
                ImGui::Text( (Input()->*inputTypeInfos[ inputType ].getInputName)( inputId ) );
            }


            // variables used to wait for an input across frames
            static Action* waitingOnAction = nullptr; // the action we're waiting for an input to modify
            static bool removing = false;             // whether we're wating to remove instead of add an action
            static InputType waitingForInputType = InputType::KeyboardButton; // the type of input we're waiting for

            
            if ( ImGui::Button( "Add Input" ) )
            {
                waitingOnAction = this;
                removing = false;
                waitingForInputType = (InputType)inputType;
            }
            if ( waitingOnAction == this && removing == false && waitingForInputType == (InputType)inputType )
            {
                ImGui::Text( "Press Input to Add" );

                int inputId = (Input()->*inputTypeInfos[ inputType ].getCurrentInput)();
                if ( inputId != -1 )
                {
                    changed = true;
                    waitingOnAction = nullptr;
                    AddInput( (InputType)inputType, inputId );
                }
            }

            if ( ImGui::Button( "Remove Input" ) )
            {
                waitingOnAction = this;
                removing = true;
                waitingForInputType = (InputType)inputType;
            }
            if ( waitingOnAction == this && removing == true && waitingForInputType == (InputType)inputType )
            {
                ImGui::Text( "Press Input to Remove" );

                int inputId = (Input()->*inputTypeInfos[ inputType ].getCurrentInput)();
                if ( inputId != -1 )
                {
                    changed = true;
                    waitingOnAction = nullptr;
                    RemoveInput( (InputType)inputType, inputId );
                }
            }


            ImGui::TreePop();
        }

        ImGui::TreePop();

        return changed;
    }

    
//-------------------------------------------------------------------------
// private: inspection
//-------------------------------------------------------------------------


    


//-------------------------------------------------------------------------
// private: reading
//-------------------------------------------------------------------------


    /// @brief  read the key inputs for an action
    /// @param  data    the JSON data to read from
    void Action::readName( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_Name, data );
    }


    /// @brief  read the key inputs for an action
    /// @param  data    the JSON data to read from
    void Action::readKeyboardButtons( nlohmann::ordered_json const& data )
    {
        Stream::Read< int >( &m_Inputs[ (int)InputType::KeyboardButton ], data );
    }

    /// @brief  read the mouse inputs for an action
    /// @param  data    the JSON data to read from
    void Action::readMouseButtons( nlohmann::ordered_json const& data )
    {
        Stream::Read< int >( &m_Inputs[ (int)InputType::MouseButton ], data );
    }

    /// @brief  read the controller inputs for an action
    /// @param  data    the JSON data to read from
    void Action::readControllerButtons( nlohmann::ordered_json const& data )
    {
        Stream::Read< int >( &m_Inputs[ (int)InputType::ControllerButton ], data );
    }

    /// @brief  read the key axis inputs for an action
    /// @param  data    the JSON data to read from
    void Action::readKeyboardButtonsNegative( nlohmann::ordered_json const& data )
    {
        Stream::Read< int >( &m_Inputs[ (int)InputType::KeyboardButtonNegative ], data );
    }

    /// @brief  read the mouse axis inputs for an action
    /// @param  data    the JSON data to read from
    void Action::readMouseButtonsNegative( nlohmann::ordered_json const& data )
    {
        Stream::Read< int >( &m_Inputs[ (int)InputType::MouseButtonNegative ], data );
    }

    /// @brief  read the controller axis inputs for an action
    /// @param  data    the JSON data to read from
    void Action::readControllerButtonsNegative( nlohmann::ordered_json const& data )
    {
        Stream::Read< int >( &m_Inputs[ (int)InputType::ControllerButtonNegative ], data );
    }

    /// @brief  read the gamepad axis as input for an action
    /// @param  data    the JSON data to read from
    void Action::readControllerAxisAsButton( nlohmann::ordered_json const& data )
    {
        Stream::Read< int >( &m_Inputs[ (int)InputType::ControllerAxisAsButton ], data );
    }

    /// @brief  read the gamepad axis inputs for an action
    /// @param  data    the JSON data to read from
    void Action::readControllerAxis( nlohmann::ordered_json const& data )
    {
        Stream::Read< int >( &m_Inputs[ (int)InputType::ControllerAxis ], data );
    }


//-------------------------------------------------------------------------
// public: reading / writing
//-------------------------------------------------------------------------


    /// @brief read method map for an Action
    ReadMethodMap< ISerializable > const& Action::GetReadMethods() const
    {
        static ReadMethodMap< Action > const readMethods = {
            { "Name"                     , &Action::readName                      },
            { "KeyboardButtons"          , &Action::readKeyboardButtons           },
            { "MouseButtons"             , &Action::readMouseButtons              },
            { "ControllerButtons"        , &Action::readControllerButtons         },
            { "KeyboardButtonsNegative"  , &Action::readKeyboardButtonsNegative   },
            { "MouseButtonsNegative"     , &Action::readMouseButtonsNegative      },
            { "ControllerButtonsNegative", &Action::readControllerButtonsNegative },
            { "ControllerAxisAsButton"   , &Action::readControllerAxisAsButton    },
            { "ControllerAxis"           , &Action::readControllerAxis            }
        };

        return (ReadMethodMap< ISerializable > const&)readMethods;
    }

    /// @brief	write an Action to json
    nlohmann::ordered_json Action::Write() const
    {
        nlohmann::ordered_json json = nlohmann::ordered_json::object();

        json[ "Name"                      ] = Stream::Write( m_Name );
        json[ "KeyboardButtons"           ] = Stream::WriteArray< int >( m_Inputs[ (int)InputType::KeyboardButton           ] );
        json[ "MouseButtons"              ] = Stream::WriteArray< int >( m_Inputs[ (int)InputType::MouseButton              ] );
        json[ "ControllerButtons"         ] = Stream::WriteArray< int >( m_Inputs[ (int)InputType::ControllerButton         ] );
        json[ "KeyboardButtonsNegative"   ] = Stream::WriteArray< int >( m_Inputs[ (int)InputType::KeyboardButtonNegative   ] );
        json[ "MouseButtonsNegative"      ] = Stream::WriteArray< int >( m_Inputs[ (int)InputType::MouseButtonNegative      ] );
        json[ "ControllerButtonsNegative" ] = Stream::WriteArray< int >( m_Inputs[ (int)InputType::ControllerButtonNegative ] );
        json[ "ControllerAxisAsButton"    ] = Stream::WriteArray< int >( m_Inputs[ (int)InputType::ControllerAxisAsButton   ] );
        json[ "ControllerAxis"            ] = Stream::WriteArray< int >( m_Inputs[ (int)InputType::ControllerAxis           ] );

        return json;
    }


//-------------------------------------------------------------------------