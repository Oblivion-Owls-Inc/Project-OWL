/// @file       ControlPrompt.cpp
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      Displays a control prompt for an ActionReference
/// @version    0.1
/// @date       2024-03-11
/// 
/// @copyright  Copyright (c) 2024 Digipen Institute of Technology


#include "pch.h"

#include "ControlPrompt.h"

#include "ComponentReference.t.h"
#include "Sprite.h"

#include "ControlPromptSystem.h"


//-----------------------------------------------------------------------------
// public: constructor / Destructor
//-----------------------------------------------------------------------------


    /// @brief  default constructor
    ControlPrompt::ControlPrompt() :
        Behavior( typeid( ControlPrompt ) )
    {}


//-----------------------------------------------------------------------------
// public: methods
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// public: accessors
//-----------------------------------------------------------------------------


    /// @brief  gets the Sprite Component attached
    /// @return the Sprite Component attached
    Sprite* ControlPrompt::GetSprite()
    {
        return m_Sprite;
    }


//-----------------------------------------------------------------------------
// public: virtual override methods
//-----------------------------------------------------------------------------


    /// @brief  called once when entering the scene
    void ControlPrompt::OnInit()
    {
        m_Action.SetOwnerName( GetName() );
        m_Action.Init();

        m_Sprite.Init( GetEntity() );

        ControlPrompts()->AddComponent( this );
    }

    /// @brief  called once when exiting the scene
    void ControlPrompt::OnExit()
    {
        m_Sprite.Exit();

        ControlPrompts()->RemoveComponent( this );
    }


    /// @brief  called every graphics frame
    /// @param  dt  the amount of time since the last frame
    void ControlPrompt::OnUpdate( float dt )
    {
        updateSprite();
    }


//-----------------------------------------------------------------------------
// private: methods
//-----------------------------------------------------------------------------


    /// @brief  converts from an Action::InputType to a ControlPrompt::InputType
    /// @param  inputType   the Action::InputType to convert
    /// @return the ControlPrompt::InputType cooresponding to the Action::InputType
    ControlPrompt::InputType ControlPrompt::convertInputType( Action::InputType inputType )
    {
        switch ( inputType )
        {
            case Action::InputType::KeyboardButton:
            case Action::InputType::KeyboardButtonNegative:
                return ControlPrompt::InputType::Keyboard;
            case Action::InputType::MouseButton:
            case Action::InputType::MouseButtonNegative:
                return ControlPrompt::InputType::Mouse;
            case Action::InputType::ControllerButton:
            case Action::InputType::ControllerButtonNegative:
                return (
                    Input()->ControllerIsPlaystation() ? 
                        ControlPrompt::InputType::GamepadButtonsPlaystation :
                        ControlPrompt::InputType::GamepadButtonsXbox
                );
            case Action::InputType::ControllerAxisAsButton:
            case Action::InputType::ControllerAxis:
                return ControlPrompt::InputType::GamepadAxes;
            default:
                return ControlPrompt::InputType::Keyboard;
        }
    }


    /// @brief  updates the attached Sprite to match the attached Action
    void ControlPrompt::updateSprite()
    {
        if ( m_Action == nullptr || m_Sprite == nullptr )
        {
            return;
        }

        // input types to use if displaying mouse+keyboard prompts
        Action::InputType constexpr keyboardInputTypes[] = {
            Action::InputType::MouseButton,
            Action::InputType::MouseButtonNegative,
            Action::InputType::KeyboardButton,
            Action::InputType::KeyboardButtonNegative
        };

        // input types to use if displaying controller prompts
        Action::InputType constexpr controllerInputTypes[] = {
            Action::InputType::ControllerButton,
            Action::InputType::ControllerButtonNegative,
            Action::InputType::ControllerAxisAsButton,
            Action::InputType::ControllerAxis
        };

        // NOTE: this range-based for loop only works because the two arrays are the same size and therefore the same type
        // if their sizes ever need to be different, an alternative solution must be found
        for ( Action::InputType inputType : Input()->IsControllerMostRecentInput() ? controllerInputTypes : keyboardInputTypes )
        {
            std::vector< int > const& inputs = m_Action->GetInputVector( inputType );

            if ( inputs.empty() )
            {
                continue;
            }

            ControlPrompt::InputType promptType = convertInputType( inputType );

            m_Sprite->SetTexture( ControlPrompts()->GetPromptTexture( promptType ) );
            m_Sprite->SetFrameIndex( ControlPrompts()->GetPromptFrameIndex( promptType, inputs[ 0 ] ) );

            return;
        }

        // if no input found, display no prompt
        m_Sprite->SetFrameIndex( -1 );
    }


//-----------------------------------------------------------------------------
// public: inspection
//-----------------------------------------------------------------------------


    /// @brief  shows the inspector for ControlPrompt
    void ControlPrompt::Inspector()
    {
        m_Action.Inspect( "Control Action" );
    }


//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------


    /// @brief  read the Action to display a prompt for
    /// @param  data    the JSON data to read from
    void ControlPrompt::readAction( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_Action, data );
    }


//-----------------------------------------------------------------------------
// public: reading / writing
//-----------------------------------------------------------------------------


    /// @brief  gets the map of read methods for this ControlPrompt
    /// @return the map of read methods for this ControlPrompt
    ReadMethodMap< ISerializable > const& ControlPrompt::GetReadMethods() const
    {
        static ReadMethodMap< ControlPrompt > const readMethods = {
            { "Action", &ControlPrompt::readAction }
        };

        return (ReadMethodMap< ISerializable > const&)readMethods;
    }


    /// @brief  writes this ControlPrompt to JSON
    /// @return the JSON data of this ControlPrompt
    nlohmann::ordered_json ControlPrompt::Write() const
    {
        nlohmann::ordered_json json = nlohmann::ordered_json::object();

        json[ "Action" ] = Stream::Write( m_Action );

        return json;
    }

    
//-----------------------------------------------------------------------------
// public: copying
//-----------------------------------------------------------------------------


    /// @brief  clones this ControlPrompt
    /// @return the newly created clone of this ControlPrompt
    ControlPrompt* ControlPrompt::Clone() const
    {
        return new ControlPrompt( *this );
    }


//-----------------------------------------------------------------------------
// private: copying
//-----------------------------------------------------------------------------


    /// @brief  copy-constructor for the ControlPrompt
    /// @param  other   the other ControlPrompt to copy
    ControlPrompt::ControlPrompt( ControlPrompt const& other ) :
        Behavior( other ),
        m_Action( other.m_Action )
    {}


//-----------------------------------------------------------------------------
