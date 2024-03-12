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
        Keyboard = 0             , /// @brief Keyboard buttons
        Mouse                    , /// @brief Mouse buttons
        GamepadButtonsXbox       , /// @brief Xbox gamepad buttons
        GamepadButtonsPlaystation, /// @brief Playstation gamepad buttons
        GamepadAxes              , /// @brief gamepad axes
    };


//-----------------------------------------------------------------------------
public: // methods
//-----------------------------------------------------------------------------


    /// @brief  gets the frame index of the specified glfw input
    /// @param  inputType   the type of input to get the prompt frame index of
    /// @param  glfwId      the GLFW id of the input
    /// @return the frame index of the input
    int GetPromptFrameIndex( InputType inputType, int glfwId ) const;


    /// @brief  gets the texture of the specified InputType
    /// @param  inputType   the input type to get the texture for
    /// @return the prompts texture for the specified input type
    Texture const* GetPromptTexture( InputType inputType ) const;

    
//-----------------------------------------------------------------------------
public: // accessors
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
public: // virtual override methods
//-----------------------------------------------------------------------------


    /// @brief  Gets called once when this System is added to the Engine
    virtual void OnInit() override;

    /// @brief  Gets called once when the Engine closes
    virtual void OnExit() override;

    
//-----------------------------------------------------------------------------
private: // struct MappingInfo
//-----------------------------------------------------------------------------

    
    /// @brief  struct that helps serialize prompt mapping and texture data
    struct MappingInfo : public ISerializable
    {
    //-------------------------------------------------------------------------
    public: // members
    //-------------------------------------------------------------------------


        /// @brief  Texture to display keyboard prompts with
        Texture M_KeyboardPromptsTexture;

        /// @brief  Texture to display mouse prompts with
        Texture M_MousePromptsTexture;

        /// @brief  Texture to display gamepad prompts with
        Texture M_GamepadPromptsTexture;


        /// @brief  keyboard mappings (GLFW id, texture index)
        std::vector< std::array< int, 2 > > M_KeyboardMappings = {};

        /// @brief  mouse button mappings (GLFW id, texture index)
        std::vector< std::array< int, 2 > > M_MouseMappings = {};

        /// @brief  gamepad button Xbox mappings (GLFW id, texture index)
        std::vector< std::array< int, 2 > > M_GamepadButtonsXboxMappings = {};

        /// @brief  gamepad button Playstation mappings (GLFW id, texture index)
        std::vector< std::array< int, 2 > > M_GamepadButtonsPlaystationMappings = {};

        /// @brief  gamepad axes mappings (GLFW id, texture index)
        std::vector< std::array< int, 2 > > M_GamepadAxesMappings = {};

        
    //-------------------------------------------------------------------------
    private: // reading / writing
    //-------------------------------------------------------------------------


        /// @brief  loads the prompt texture mappings from JSON
        /// @param  mappings    the mappings array to load into
        /// @param  data        the JSON data to read from
        static void loadMappings( std::vector< std::array< int, 2 > >& mappings, nlohmann::ordered_json const& data );

        /// @brief  loads the prompt texture mappings from JSON
        /// @param  mappings    the mappings array to save
        /// @return the JSON data of the mappings array
        static nlohmann::ordered_json saveMappings( std::vector< std::array< int, 2 > > const& mappings );


        /// @brief  reads Texture to display keyboard prompts with
        /// @param  data    the JSON data to read from
        void readKeyboardPromptsTexture( nlohmann::ordered_json const& data );

        /// @brief  reads Texture to display mouse prompts with
        /// @param  data    the JSON data to read from
        void readMousePromptsTexture( nlohmann::ordered_json const& data );

        /// @brief  reads Texture to display gamepad prompts with
        /// @param  data    the JSON data to read from
        void readGamepadPromptsTexture( nlohmann::ordered_json const& data );


        /// @brief  reads keyboard mappings (GLFW id, texture index)
        /// @param  data    the JSON data to read from
        void readKeyboardMappings( nlohmann::ordered_json const& data );

        /// @brief  reads mouse button mappings (GLFW id, texture index)
        /// @param  data    the JSON data to read from
        void readMouseMappings( nlohmann::ordered_json const& data );

        /// @brief  reads gamepad button Xbox mappings (GLFW id, texture index)
        /// @param  data    the JSON data to read from
        void readGamepadButtonsXboxMappings( nlohmann::ordered_json const& data );

        /// @brief  reads gamepad button Playstation mappings (GLFW id, texture index)
        /// @param  data    the JSON data to read from
        void readGamepadButtonsPlaystationMappings( nlohmann::ordered_json const& data );

        /// @brief  reads gamepad axes mappings (GLFW id, texture index)
        /// @param  data    the JSON data to read from
        void readGamepadAxesMappings( nlohmann::ordered_json const& data );


    //-------------------------------------------------------------------------
    public: // reading / writing
    //-------------------------------------------------------------------------


        /// @brief  gets this System's read methods
        /// @return this System's read methods
        virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override;


        /// @brief  writes this ControlPromptSystem to JSON
        /// @return the JSON data of this ControlPromptSystem
        virtual nlohmann::ordered_json Write() const override;


    //-------------------------------------------------------------------------
    };



//-----------------------------------------------------------------------------
private: // members
//-----------------------------------------------------------------------------


    /// @brief  filepath to the prompt mappings JSON file
    std::string m_PromptMappingFilepath = "Data/ButtonPromptMappings/ButtonPrompts.json";


    /// @brief  struct that holds the texture and mapping info for button prompts
    MappingInfo m_MappingInfo;


//-----------------------------------------------------------------------------
private: // methods
//-----------------------------------------------------------------------------



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

/// @brief  shorthand method for ControlPromptSystem::GetInstance()
/// @return the ControlPromptSystem instance
ControlPromptSystem* ControlPrompts();
