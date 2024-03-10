/// @file       ControlPromptSystem.h
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      System that displays ControlPrompts from Actions
/// @version    0.1
/// @date       DATE HERE
/// 
/// @copyright  Copyright (c) 2024 Digipen Instutute of Technology

#pragma once

#include "pch.h"

#include "System.h"

#include "Texture.h"

/// @brief  System that displays ControlPrompts from Actions
class ControlPromptSystem : public System
{
//-----------------------------------------------------------------------------
public: // types
//-----------------------------------------------------------------------------


    /// @brief  type of input to display a prompt for
    enum class InputType
    {
        Keyboard                  = 0, /// @brief Keyboard buttons
        Mouse                     = 1, /// @brief Mouse buttons
        GamepadButtonsXbox        = 2, /// @brief Xbox gamepad buttons
        GamepadButtonsPlaystation = 3, /// @brief Playstation gamepad buttons
        GamepadAxes               = 4  /// @brief gamepad axes
    };


//-----------------------------------------------------------------------------
public: // methods
//-----------------------------------------------------------------------------


    /// @brief  gets the frame index of the specified glfw input
    /// @param  inputType   the type of input to get the prompt frame index of
    /// @param  glfwId      the GLFW id of the input
    /// @return the frame index of the input
    int GetPromptFrameIndex( InputType inputType, int glfwId ) const;

    
//-----------------------------------------------------------------------------
public: // accessors
//-----------------------------------------------------------------------------


    /// @brief  gets the keyboard prompts Texture
    /// @return the keyboard prompts texture
    Texture const* GetKeyboardPromptTexture() const;

    /// @brief  gets the mouse prompts Texture
    /// @return the mouse prompts texture
    Texture const* GetMousePromptTexture() const;

    /// @brief  gets the gamepad prompts Texture
    /// @return the gamepad prompts texture
    Texture const* GetGamepadPromptTexture() const;


//-----------------------------------------------------------------------------
public: // virtual override methods
//-----------------------------------------------------------------------------


    /// @brief  Gets called once when this System is added to the Engine
    virtual void OnInit() override;


//-----------------------------------------------------------------------------
private: // members
//-----------------------------------------------------------------------------


    /// @brief  Texture to display keyboard prompts with
    Texture m_KeyboardPromptsTexture;

    /// @brief  Texture to display mouse prompts with
    Texture m_MousePromptsTexture;

    /// @brief  Texture to display gamepad prompts with
    Texture m_GamepadPromptsTexture;


    /// @brief  filepath to the prompt mappings JSON file
    std::string m_PromptMappingFilepath = "Data/ButtonPromptMappings/ButtonPrompts.json";


    /// @brief  keyboard mappings (GLFW id, texture index)
    std::vector< std::array< int, 2 > > m_KeyboardMappings = {};

    /// @brief  mouse button mappings (GLFW id, texture index)
    std::vector< std::array< int, 2 > > m_MouseMappings = {};

    /// @brief  gamepad button Xbox mappings (GLFW id, texture index)
    std::vector< std::array< int, 2 > > m_GamepadButtonsXboxMappings = {};

    /// @brief  gamepad button Playstation mappings (GLFW id, texture index)
    std::vector< std::array< int, 2 > > m_GamepadButtonsPlaystationMappings = {};

    /// @brief  gamepad axes mappings (GLFW id, texture index)
    std::vector< std::array< int, 2 > > m_GamepadAxesMappings = {};


//-----------------------------------------------------------------------------
private: // methods
//-----------------------------------------------------------------------------


    /// @brief  loads the prompt texture mappings from JSON
    void loadTextureMappings();


//-----------------------------------------------------------------------------
public: // inspection
//-----------------------------------------------------------------------------


    /// @brief Gets Called by the Debug system to display debug information
    virtual void DebugWindow() override;

    
//-----------------------------------------------------------------------------
private: // reading
//-----------------------------------------------------------------------------

    
    /// @brief  reads the filepath to the prompt mappings JSON file
    /// @param  data    the JSON data to read from
    void readPromptMappingFilepath( nlohmann::ordered_json const& data );


//-----------------------------------------------------------------------------
public: // reading / writing
//-----------------------------------------------------------------------------


    /// @brief  gets this System's read methods
    /// @return this System's read methods
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override;


    /// @brief  writes this ControlPromptSystem to JSON
    /// @return the JSON data of this ControlPromptSystem
    virtual nlohmann::ordered_json Write() const override;


//-----------------------------------------------------------------------------
public: // singleton stuff
//-----------------------------------------------------------------------------


    /// @brief  gets the instance of ControlPromptSystem
    /// @return the instance of the ControlPromptSystem
    static ControlPromptSystem* GetInstance();


//-----------------------------------------------------------------------------
private: // singleton stuff
//-----------------------------------------------------------------------------


    /// @brief  Constructs the ControlPromptSystem
    ControlPromptSystem();

    // Prevent copying
    ControlPromptSystem( ControlPromptSystem const& ) = delete;
    void operator =( ControlPromptSystem const& ) = delete;


//-----------------------------------------------------------------------------
};