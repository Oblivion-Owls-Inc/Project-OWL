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


//-----------------------------------------------------------------------------
// public: constructor / Destructor
//-----------------------------------------------------------------------------


    /// @brief  default constructor
    ControlPrompt::ControlPrompt() :
        Component( typeid( ControlPrompt ) )
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
        m_Sprite.SetOnConnectCallback(
            [ this ]() {
                updateSprite();
            }
        );

        m_Action.SetOwnerName( GetName() );
        m_Action.Init();

        m_Sprite.Init( GetEntity() );
    }

    /// @brief  called once when exiting the scene
    void ControlPrompt::OnExit()
    {
        m_Sprite.Exit();
    }


//-----------------------------------------------------------------------------
// private: methods
//-----------------------------------------------------------------------------


    /// @brief  converts from an Action::InputType to a ControlPromptSystem::InputType
    /// @param  inputType   the Action::InputType to convert
    /// @return the ControlPromptSystem::InputType cooresponding to the Action::InputType
    ControlPromptSystem::InputType ControlPrompt::convertInputType( Action::InputType inputType )
    {
        switch ( inputType )
        {
            case Action::InputType::KeyboardButton:
            case Action::InputType::KeyboardButtonNegative:
                return ControlPromptSystem::InputType::Keyboard;
            case Action::InputType::MouseButton:
            case Action::InputType::MouseButtonNegative:
                return ControlPromptSystem::InputType::Mouse;
            case Action::InputType::ControllerButton:
            case Action::InputType::ControllerButtonNegative:
                return ControlPromptSystem::InputType::GamepadButtonsXbox;
                // TODO: make sure using the correct controller type
                // return ControlPromptSystem::InputType::GamepadButtonsPlaystation;
            case Action::InputType::ControllerAxisAsButton:
            case Action::InputType::ControllerAxis:
                return ControlPromptSystem::InputType::GamepadAxes;
            default:
                return ControlPromptSystem::InputType::Keyboard;
        }
    }


    /// @brief  updates the attached Sprite to match the attached Action
    /// @brief  assumes m_Sprite is non-null, doesn't assume m_Action
    void ControlPrompt::updateSprite()
    {
        if ( m_Action == nullptr )
        {
            return;
        }

        // TODO: determine whether to display keyboard+mouse prompts, or controller prompts
        // TODO: if displaying controller prompts, determine which controller prompts to display

        for ( int inputType = (int)Action::InputType::_First; inputType < (int)Action::InputType::_Count; ++inputType )
        {
            std::vector< int > const& inputs = m_Action->GetInputVector( (Action::InputType)inputType );

            if ( inputs.empty() )
            {
                continue;
            }

            ControlPromptSystem::InputType promptType = convertInputType( (Action::InputType)inputType );

            m_Sprite->SetTexture( ControlPrompts()->GetPromptTexture( promptType ) );
            m_Sprite->SetFrameIndex( ControlPrompts()->GetPromptFrameIndex( promptType, inputs[ 0 ] ) );
        }
    }


//-----------------------------------------------------------------------------
// public: inspection
//-----------------------------------------------------------------------------


    /// @brief  shows the inspector for ControlPrompt
    void ControlPrompt::Inspector()
    {
        if ( m_Action.Inspect( "Control Action" ) )
        {
            if ( m_Sprite != nullptr )
            {
                updateSprite();
            }
        }
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
        Component( other ),
        m_Action( other.m_Action )
    {}


//-----------------------------------------------------------------------------
