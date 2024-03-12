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
            &m_MappingInfo.M_KeyboardMappings,
            &m_MappingInfo.M_MouseMappings,
            &m_MappingInfo.M_GamepadButtonsXboxMappings,
            &m_MappingInfo.M_GamepadButtonsPlaystationMappings,
            &m_MappingInfo.M_GamepadAxesMappings
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


    /// @brief  gets the texture of the specified InputType
    /// @param  inputType   the input type to get the texture for
    /// @return the prompts texture for the specified input type
    Texture const* ControlPromptSystem::GetPromptTexture( InputType inputType ) const
    {
        switch ( inputType )
        {
            case InputType::Keyboard:
                return &m_MappingInfo.M_KeyboardPromptsTexture;
            case InputType::Mouse:
                return &m_MappingInfo.M_MousePromptsTexture;
            case InputType::GamepadButtonsXbox:
            case InputType::GamepadButtonsPlaystation:
            case InputType::GamepadAxes:
                return &m_MappingInfo.M_GamepadPromptsTexture;
            default:
                return &m_MappingInfo.M_KeyboardPromptsTexture;
        }
    }


//-----------------------------------------------------------------------------
// public: accessors
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// public: virtual override methods
//-----------------------------------------------------------------------------


    /// @brief  Gets called once when this System is added to the Engine
    void ControlPromptSystem::OnInit()
    {
        if ( m_PromptMappingFilepath.empty() )
        {
            Debug() << "Error: ControlPromptSystem.m_PromptMappingFilepath is empty" << std::endl;
            return;
        }

        Stream::PushDebugLocation( m_PromptMappingFilepath + "::" );

        Stream::Read( m_MappingInfo, Stream::ParseFromFile( m_PromptMappingFilepath ) );

        Stream::PopDebugLocation();
    }

    /// @brief  Gets called once when the Engine closes
    void ControlPromptSystem::OnExit()
    {
        if ( m_PromptMappingFilepath.empty() )
        {
            Debug() << "Error: ControlPromptSystem.m_PromptMappingFilepath is empty" << std::endl;
            return;
        }

        Stream::WriteToFile( m_PromptMappingFilepath, Stream::Write( m_MappingInfo ) );
    }

    
//-----------------------------------------------------------------------------
// private: struct MappingInfo
//-----------------------------------------------------------------------------

        
    //-------------------------------------------------------------------------
    // private: reading / writing
    //-------------------------------------------------------------------------


        /// @brief  loads the prompt texture mappings from JSON
        /// @param  mappings    the mappings array to load into
        /// @param  data        the JSON data to read from
        void ControlPromptSystem::MappingInfo::loadMappings( std::vector< std::array< int, 2 > >& mappings, nlohmann::ordered_json const& data )
        {
            // loop through each mapping in the array
            for ( nlohmann::ordered_json const& mappingData : data )
            {
                // ensure the mapping follows the correct format
                if (
                    mappingData.is_array() == false || mappingData.size() != 2 ||
                    mappingData[ 0 ].is_number_integer() == false || mappingData[ 1 ].is_number_integer() == false
                )
                {
                    Debug() << "Warning: incorrect format at " << Stream::GetDebugLocation() << std::endl;
                    return;
                }

                // load the mapping
                mappings.emplace_back();
                mappings.back()[ 0 ] = mappingData[ 0 ].get< int >();
                mappings.back()[ 1 ] = mappingData[ 1 ].get< int >();
            }
        }

        /// @brief  loads the prompt texture mappings from JSON
        /// @param  mappings    the mappings array to save
        /// @return the JSON data of the mappings array
        nlohmann::ordered_json ControlPromptSystem::MappingInfo::saveMappings( std::vector< std::array< int, 2 > > const& mappings )
        {
            nlohmann::ordered_json json = nlohmann::ordered_json::array();

            for ( std::array< int, 2 > const& mapping : mappings )
            {
                json.push_back( mapping );
            }

            return json;
        }


        /// @brief  reads Texture to display keyboard prompts with
        /// @param  data    the JSON data to read from
        void ControlPromptSystem::MappingInfo::readKeyboardPromptsTexture( nlohmann::ordered_json const& data )
        {
            Stream::Read( M_KeyboardPromptsTexture, data );
        }

        /// @brief  reads Texture to display mouse prompts with
        /// @param  data    the JSON data to read from
        void ControlPromptSystem::MappingInfo::readMousePromptsTexture( nlohmann::ordered_json const& data )
        {
            Stream::Read( M_MousePromptsTexture, data );
        }

        /// @brief  reads Texture to display gamepad prompts with
        /// @param  data    the JSON data to read from
        void ControlPromptSystem::MappingInfo::readGamepadPromptsTexture( nlohmann::ordered_json const& data )
        {
            Stream::Read( M_GamepadPromptsTexture, data );
        }


        /// @brief  reads keyboard mappings (GLFW id, texture index)
        /// @param  data    the JSON data to read from
        void ControlPromptSystem::MappingInfo::readKeyboardMappings( nlohmann::ordered_json const& data )
        {
            loadMappings( M_KeyboardMappings, data );
        }

        /// @brief  reads mouse button mappings (GLFW id, texture index)
        /// @param  data    the JSON data to read from
        void ControlPromptSystem::MappingInfo::readMouseMappings( nlohmann::ordered_json const& data )
        {
            loadMappings( M_MouseMappings, data );
        }

        /// @brief  reads gamepad button Xbox mappings (GLFW id, texture index)
        /// @param  data    the JSON data to read from
        void ControlPromptSystem::MappingInfo::readGamepadButtonsXboxMappings( nlohmann::ordered_json const& data )
        {
            loadMappings( M_GamepadButtonsXboxMappings, data );
        }

        /// @brief  reads gamepad button Playstation mappings (GLFW id, texture index)
        /// @param  data    the JSON data to read from
        void ControlPromptSystem::MappingInfo::readGamepadButtonsPlaystationMappings( nlohmann::ordered_json const& data )
        {
            loadMappings( M_GamepadButtonsPlaystationMappings, data );
        }

        /// @brief  reads gamepad axes mappings (GLFW id, texture index)
        /// @param  data    the JSON data to read from
        void ControlPromptSystem::MappingInfo::readGamepadAxesMappings( nlohmann::ordered_json const& data )
        {
            loadMappings( M_GamepadAxesMappings, data );
        }


    //-------------------------------------------------------------------------
    // public: reading / writing
    //-------------------------------------------------------------------------


        /// @brief  gets this System's read methods
        /// @return this System's read methods
        ReadMethodMap< ISerializable > const& ControlPromptSystem::MappingInfo::GetReadMethods() const
        {
            static ReadMethodMap< MappingInfo > const readMethods = {
                { "KeyboardPromptsTexture"           , &MappingInfo::readKeyboardPromptsTexture            },
                { "MousePromptsTexture"              , &MappingInfo::readMousePromptsTexture               },
                { "GamepadPromptsTexture"            , &MappingInfo::readGamepadPromptsTexture             },
                { "KeyboardMappings"                 , &MappingInfo::readKeyboardMappings                  },
                { "MouseMappings"                    , &MappingInfo::readMouseMappings                     },
                { "GamepadButtonsXboxMappings"       , &MappingInfo::readGamepadButtonsXboxMappings        },
                { "GamepadButtonsPlaystationMappings", &MappingInfo::readGamepadButtonsPlaystationMappings },
                { "GamepadAxesMappings"              , &MappingInfo::readGamepadAxesMappings               }
            };

            return (ReadMethodMap< ISerializable > const&)readMethods;
        }


        /// @brief  writes this ControlPromptSystem to JSON
        /// @return the JSON data of this ControlPromptSystem
        nlohmann::ordered_json ControlPromptSystem::MappingInfo::Write() const
        {
            nlohmann::ordered_json json = nlohmann::ordered_json::object();

            json[ "KeyboardPromptsTexture"            ] = Stream::Write( M_KeyboardPromptsTexture );
            json[ "MousePromptsTexture"               ] = Stream::Write( M_MousePromptsTexture    );
            json[ "GamepadPromptsTexture"             ] = Stream::Write( M_GamepadPromptsTexture  );

            json[ "KeyboardMappings"                  ] = saveMappings( M_KeyboardMappings                  );
            json[ "MouseMappings"                     ] = saveMappings( M_MouseMappings                     );
            json[ "GamepadButtonsXboxMappings"        ] = saveMappings( M_GamepadButtonsXboxMappings        );
            json[ "GamepadButtonsPlaystationMappings" ] = saveMappings( M_GamepadButtonsPlaystationMappings );
            json[ "GamepadAxesMappings"               ] = saveMappings( M_GamepadAxesMappings               );

            return json;
        }


    //-------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// private: methods
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// public: inspection
//-----------------------------------------------------------------------------


    /// @brief Gets Called by the Debug system to display debug information
    void ControlPromptSystem::DebugWindow()
    {
        bool showWindow = GetDebugEnabled();

        if ( ImGui::Begin( "Control Prompt System", &showWindow ) )
        {
            // inspect textures
            std::pair< Texture*, char const* > const promptsTextures[] = {
                { &m_MappingInfo.M_KeyboardPromptsTexture, "Keyboard Prompts Texture" },
                { &m_MappingInfo.M_MousePromptsTexture   , "Mouse Prompts Texture"    },
                { &m_MappingInfo.M_GamepadPromptsTexture , "Gamepad Prompts Texture"  },
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


/// @brief  shorthand method for ControlPromptSystem::GetInstance()
/// @return the ControlPromptSystem instance
ControlPromptSystem* ControlPrompts()
{
    return ControlPromptSystem::GetInstance();
}
