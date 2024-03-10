/// @file       ControlPromptSystem.cpp
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      System that displays ControlPrompts from Actions
/// @version    0.1
/// @date       2024-03-09
/// 
/// @copyright  Copyright (c) 2024 Digipen Instutute of Technology

#pragma once

#include "pch.h"

#include "ControlPromptSystem.h"

#include "DebugSystem.h"

//-----------------------------------------------------------------------------
// public: methods
//-----------------------------------------------------------------------------

    
    /// @brief  gets the frame index of the specified glfw input
    /// @param  inputType   the type of input to get the prompt frame index of
    /// @param  glfwId      the GLFW id of the input
    /// @return the frame index of the input
    int ControlPromptSystem::GetPromptFrameIndex( InputType inputType, int glfwId ) const
    {
        // get the appropriate texture mappings array
        std::vector< std::array< int, 2 > > const* const textureMappingArrays[] = {
            &m_KeyboardMappings,
            &m_MouseMappings,
            &m_GamepadButtonsXboxMappings,
            &m_GamepadButtonsPlaystationMappings,
            &m_GamepadAxesMappings
        };
        std::vector< std::array< int, 2 > > const* textureMappingArray = textureMappingArrays[ static_cast< int >( inputType ) ];

        // binary search to find the prompt
        auto it = std::lower_bound(
            textureMappingArray->begin(), textureMappingArray->end(), glfwId,
            []( std::array< int, 2 > const& mapping, int const& id ) -> bool
            {
                return mapping[ 0 ] < id;
            }
        );

        // verify a prompt was found
        if ( it == textureMappingArray->end() || (*it)[ 0 ] != glfwId )
        {
            return -1;
        }

        return (*it)[ 1 ];
    }


//-----------------------------------------------------------------------------
// public: accessors
//-----------------------------------------------------------------------------


    /// @brief  gets the keyboard prompts Texture
    /// @return the keyboard prompts texture
    Texture const* ControlPromptSystem::GetKeyboardPromptTexture() const
    {
        return &m_KeyboardPromptsTexture;
    }

    /// @brief  gets the mouse prompts Texture
    /// @return the mouse prompts texture
    Texture const* ControlPromptSystem::GetMousePromptTexture() const
    {
        return &m_MousePromptsTexture;
    }

    /// @brief  gets the gamepad prompts Texture
    /// @return the gamepad prompts texture
    Texture const* ControlPromptSystem::GetGamepadPromptTexture() const
    {
        return &m_GamepadPromptsTexture;
    }


    /// @brief  gets the keyboard mappings (GLFW id, texture index)
    /// @return the keyboard mappings (GLFW id, texture index)
    std::vector< std::array< int, 2 > > const& ControlPromptSystem::getKeyboardMappings() const
    {
        return m_KeyboardMappings;
    }

    /// @brief  gets the mouse button mappings (GLFW id, texture index)
    /// @return the mouse button mappings (GLFW id, texture index)
    std::vector< std::array< int, 2 > > const& ControlPromptSystem::getMouseMappings() const
    {
        return m_MouseMappings;
    }

    /// @brief  gets the gamepad button Xbox mappings (GLFW id, texture index)
    /// @return the gamepad button Xbox mappings (GLFW id, texture index)
    std::vector< std::array< int, 2 > > const& ControlPromptSystem::getGamepadButtonsXboxMappings() const
    {
        return m_GamepadButtonsXboxMappings;
    }

    /// @brief  gets the gamepad button Playstation mappings (GLFW id, texture index)
    /// @return the gamepad button Playstation mappings (GLFW id, texture index)
    std::vector< std::array< int, 2 > > const& ControlPromptSystem::getGamepadButtonsPlaystationMappings() const
    {
        return m_GamepadButtonsPlaystationMappings;
    }

    /// @brief  gets the gamepad axes mappings (GLFW id, texture index)
    /// @return the gamepad axes mappings (GLFW id, texture index)
    std::vector< std::array< int, 2 > > const& ControlPromptSystem::getGamepadAxesMappings() const
    {
        return m_GamepadAxesMappings;
    }


//-----------------------------------------------------------------------------
// public: virtual override methods
//-----------------------------------------------------------------------------


    /// @brief  Gets called once when this System is added to the Engine
    void ControlPromptSystem::OnInit()
    {
        loadTextureMappings();
    }

    
//-----------------------------------------------------------------------------
// private: methods
//-----------------------------------------------------------------------------


    /// @brief  loads the prompt texture mappings from JSON
    void ControlPromptSystem::loadTextureMappings()
    {
        // parse the JSON file and ensure it's valid
        nlohmann::ordered_json mappingData = Stream::ParseFromFile( m_PromptMappingFilepath );
        if ( mappingData.is_object() == false )
        {
            Debug() << "Warning: ControlPromptMappings JSON is not an object" << std::endl;
            return;
        }

        // loop through each array of texture mappings
        std::pair< std::vector< std::array< int, 2 > >*, std::string const > const mappingArrays[] = {
            { &m_KeyboardMappings                 , "Keyboard"                  },
            { &m_MouseMappings                    , "Mouse"                     },
            { &m_GamepadButtonsXboxMappings       , "GamepadButtonsXbox"        },
            { &m_GamepadButtonsPlaystationMappings, "GamepadButtonsPlaystation" },
            { &m_GamepadAxesMappings              , "GamepadAxes"               }
        };
        for ( auto& [ mappingArray, mappingName ] : mappingArrays )
        {
            // verify that the JSON contains the array
            if ( mappingData.contains( mappingName ) == false )
            {
                Debug() << "Warning: ControlPromptMappings JSON does not contain \"" << mappingName << "\"" << std::endl;
                return;
            }
            nlohmann::ordered_json const& mappingArrayData = mappingData.at( mappingName );

            // loop through each mapping in the array
            for ( nlohmann::ordered_json const& mappingData : mappingArrayData )
            {
                // ensure the mapping follows the correct format
                if (
                    mappingData.is_array() == false || mappingData.size() != 2 ||
                    mappingData[ 0 ].is_number_integer() == false || mappingData[ 1 ].is_number_unsigned() == false
                    )
                {
                    Debug() << "Warning: ControlPromptMappings JSON \"" << mappingName << "\" has the incorrect format" << std::endl;
                    return;
                }

                // save the mapping
                mappingArray->emplace_back();

                mappingArray->back()[ 0 ] = mappingData[ 0 ].get< int >();
                mappingArray->back()[ 1 ] = mappingData[ 1 ].get< int >();
            }
        }
    }


//-----------------------------------------------------------------------------
// public: inspection
//-----------------------------------------------------------------------------


    /// @brief Gets Called by the Debug system to display debug information
    void ControlPromptSystem::DebugWindow()
    {
        bool showWindow = GetDebugEnabled();

        if ( ImGui::Begin( "Render System", &showWindow ) )
        {
            // inspect textures
            std::pair< Texture*, char const* > const promptsTextures[] = {
                { &m_KeyboardPromptsTexture, "Keyboard Prompts Texture" },
                { &m_MousePromptsTexture   , "Mouse Prompts Texture"    },
                { &m_GamepadPromptsTexture , "Gamepad Prompts Texture"  },
            };
            for ( auto& [ texture, label ] : promptsTextures )
            {
                if ( ImGui::TreeNode( label ) )
                {
                    texture->Inspect();
                    ImGui::TreePop();
                }
            }

            ImGui::NewLine();

            // inspect mappings source
            ImGui::InputText( "texture mappings filename", &m_PromptMappingFilepath );
            if ( ImGui::Button( "Reload Texture Mappings" ) )
            {
                loadTextureMappings();
            }

        }
        ImGui::End();

        SetDebugEnable( showWindow );
    }

    
//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------

    
    /// @brief  reads the filepath to the prompt mappings JSON file
    /// @param  data    the JSON data to read from
    void ControlPromptSystem::readPromptMappingFilepath( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_PromptMappingFilepath, data );
    }


//-----------------------------------------------------------------------------
// public: reading / writing
//-----------------------------------------------------------------------------


    /// @brief  gets this System's read methods
    /// @return this System's read methods
    ReadMethodMap< ISerializable > const& ControlPromptSystem::GetReadMethods() const
    {
        static ReadMethodMap< ControlPromptSystem > const readMethods = {
            { "PromptMappingFilepath", &ControlPromptSystem::readPromptMappingFilepath }
        };

        return (ReadMethodMap< ISerializable > const&)readMethods;
    }


    /// @brief  writes this ControlPromptSystem to JSON
    /// @return the JSON data of this ControlPromptSystem
    nlohmann::ordered_json ControlPromptSystem::Write() const
    {
        nlohmann::ordered_json json = nlohmann::ordered_json::object();

        json[ "PromptMappingFilepath" ] = Stream::Write( m_PromptMappingFilepath );

        return json;
    }


//-----------------------------------------------------------------------------
// public: singleton stuff
//-----------------------------------------------------------------------------


    /// @brief  gets the instance of ControlPromptSystem
    /// @return the instance of the ControlPromptSystem
    ControlPromptSystem* ControlPromptSystem::GetInstance()
    {
        static ControlPromptSystem* instance = nullptr;
        if ( instance == nullptr )
        {
            instance = new ControlPromptSystem();
        }

        return instance;
    }


//-----------------------------------------------------------------------------
// private: singleton stuff
//-----------------------------------------------------------------------------


    /// @brief  Constructs the ControlPromptSystem
    ControlPromptSystem::ControlPromptSystem() :
        System( "ControlPromptSystem" )
    {}


//-----------------------------------------------------------------------------