/// @file       UiBarSprite.cpp
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      sprite that renders a ui bar
/// @version    0.1
/// @date       2023-10-20
/// 
/// @copyright  Copyright (c) 2023 Digipen Institute of Technology


#include "UibarSprite.h"

#include "RenderSystem.h"
#include "CameraSystem.h"

#include "Transform.h"
#include "Texture.h"

#include "Inspection.h"


//-----------------------------------------------------------------------------
// public: class UiBarSection
//-----------------------------------------------------------------------------



    //-----------------------------------------------------------------------------
    // public: inspection
    //-----------------------------------------------------------------------------


        /// @brief  inspects this UiBarSection
        /// @return whether a change was made
        bool UiBarSprite::UiBarSection::Inspect()
        {
            bool changed = false;

            changed |= ImGui::ColorPicker4( "color", &M_Color[ 0 ] );

            changed |= ImGui::DragFloat( "value", &M_Value, 0.05f, 0.0f, 1.0f );

            changed |= ImGui::DragFloat( "slope", &M_Slope, 0.05f );

            return changed;
        }
        
        
    //-----------------------------------------------------------------------------
    // private: reading
    //-----------------------------------------------------------------------------


        /// @brief  reads the color of this UiBarSection
        /// @param  data    the json data to read from
        void UiBarSprite::UiBarSection::readColor( nlohmann::ordered_json const& data )
        {
            Stream::Read( &M_Color, data );
        }

        /// @brief  reads the value of this UiBarSection
        /// @param  data    the json data to read from
        void UiBarSprite::UiBarSection::readValue( nlohmann::ordered_json const& data )
        {
            Stream::Read( M_Value, data );
        }

        /// @brief  reads the slope of the border of this UiBarSection
        /// @param  data    the json data to read from
        void UiBarSprite::UiBarSection::readSlope( nlohmann::ordered_json const& data )
        {
            Stream::Read( M_Slope, data );
        }


    //-----------------------------------------------------------------------------
    // public: reading / writing
    //-----------------------------------------------------------------------------


        /// @brief  gets the map of read methods for this UiBarSection
        /// @return the map of read methods for this UiBarSection
        ReadMethodMap< ISerializable > const& UiBarSprite::UiBarSection::GetReadMethods() const
        {
            static ReadMethodMap< UiBarSection > const readMethods = {
                { "Color", &UiBarSection::readColor },
                { "Value", &UiBarSection::readValue },
                { "Slope", &UiBarSection::readSlope }
            };

            return (ReadMethodMap< ISerializable > const&)readMethods;
        }


        /// @brief  Write all UiBarSection data to a JSON file.
        /// @return The JSON file containing the UiBarSection data.
        nlohmann::ordered_json UiBarSprite::UiBarSection::Write() const
        {
            nlohmann::ordered_json json;

            json [ "Color" ] = Stream::Write( M_Color );
            json [ "Value" ] = Stream::Write( M_Value );
            json [ "Slope" ] = Stream::Write( M_Slope );

            return json;
        }


    //-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// public: accessors
//-----------------------------------------------------------------------------


    /// @brief  gets the UiBarSections of this UiBarSprite
    /// @return the UiBarSections of this UiBarSprite
    std::vector< UiBarSprite::UiBarSection >& UiBarSprite::GetSections()
    {
        return m_Sections;
    }


//-----------------------------------------------------------------------------
// public:  methods
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// private: virtual override methods
//-----------------------------------------------------------------------------


    /// @brief  called once when entering the scene
    void UiBarSprite::OnInit()
    {
        Sprite::OnInit();

        // init the shader if necessary
        if ( Renderer()->GetShader( "UiBar" ) == nullptr )
        {
            Renderer()->AddShader(
                "UiBar",
                new Shader(
                    "Data/shaders/vshader.vert", 
                    "Data/shaders/UiBar.frag"
                )
            );
        }
    }

    /// @brief  called once when exiting the scene
    void UiBarSprite::OnExit()
    {
        Sprite::OnExit();
    }


    /// @brief  renders this UiBarSprite
    void UiBarSprite::Draw()
    {
        if ( m_Texture == nullptr )
        {
            return;
        }

        m_Texture->Bind();

        prepareShader();

        // Render it with triangle strip mode
        Mesh const* mesh = m_Texture->GetMesh();
        glBindVertexArray( mesh->GetVAO() );
        glDrawArrays( GL_TRIANGLE_STRIP, 0, mesh->GetVertexCount() );
        glBindVertexArray( 0 );
    }


//-----------------------------------------------------------------------------
// private: helper methods
//-----------------------------------------------------------------------------


    /// @brief  prepares the shader to render
    void UiBarSprite::prepareShader() const
    {
        Shader* shader = Renderer()->SetActiveShader( "UiBar" );
        if ( shader == nullptr )
        {
            Debug() << "Error: unable to find shader \"UiBar\"" << std::endl;
            return;
        }


        // mvp matrix
        glm::mat4 mvp = m_Transform->GetMatrix();
        if ( m_Transform->GetIsDiegetic() )
        {
            mvp = Cameras()->GetMat_WorldToClip() * mvp;
        }
        else
        {
            mvp = Cameras()->GetMat_UItoClip() * mvp;
        }
        glUniformMatrix4fv( shader->GetUniformID( "mvp" ), 1, false, &mvp[0][0] );

        // UiBar has no uv offset
        glUniform2f( shader->GetUniformID( "UV_offset" ), 0.0f, 0.0f );

        // collect shader parameters into arrays for easy passing
        std::vector< glm::vec4 > sectionColors = {};
        std::vector< float > sectionValues = {};
        std::vector< float > sectionSlopes = {};
        for ( UiBarSection const& section : m_Sections )
        {
            sectionColors.push_back( section.M_Color );
            sectionValues.push_back( section.M_Value );
            sectionSlopes.push_back( section.M_Slope );
        }

        if ( m_Sections.size() > 0 )
        {
            glUniform4fv( shader->GetUniformID( "sectionColors" ), (GLsizei)m_Sections.size(), &sectionColors[ 0 ][ 0 ] );
            glUniform1fv( shader->GetUniformID( "sectionValues" ), (GLsizei)m_Sections.size(), &sectionValues[ 0 ] );
            glUniform1fv( shader->GetUniformID( "sectionSlopes" ), (GLsizei)m_Sections.size(), &sectionSlopes[ 0 ] );
        }

        glUniform2fv( shader->GetUniformID( "borderPositions" ), 1, &m_BorderPositions[ 0 ] );

        glUniform2fv( shader->GetUniformID( "size" ), 1, &m_Transform->GetScale()[ 0 ] );

        glUniform1i( shader->GetUniformID( "numSections" ), (int)m_Sections.size() );

        glUniform1f( shader->GetUniformID( "opacity" ), m_Opacity );

        glUniform1f( shader->GetUniformID( "rotationPosition" ), m_RotationPosition );
    }


//-----------------------------------------------------------------------------
// private: inspection
//-----------------------------------------------------------------------------

    
    /// @brief  displays this UiBarSprite in the Inspector
    void UiBarSprite::Inspector()
    {
       m_Texture.Inspect( "texture" );

        ImGui::DragInt( "layer", &m_Layer, 0.05f );

        ImGui::DragFloat( "opacity", &m_Opacity, 0.05f, 0.0f, 1.0f );

        Inspection::InspectArray< UiBarSection >(
            "sections",
            &m_Sections,
            []( UiBarSection* section ) -> bool
            {
                return section->Inspect();
            }
        );

        ImGui::DragFloat2( "border positions", &m_BorderPositions[ 0 ], 0.05f, -1.0f, 2.0f );

        ImGui::DragFloat( "rotation position", &m_RotationPosition, 0.05f, 0.0f, 1.0f );
    }


//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------

    
    /// @brief  the sections in this UiBar
    /// @param  data    the json data to read for
    void UiBarSprite::readSections( nlohmann::ordered_json const& data )
    {
        m_Sections.resize( data.size() );
        for ( int i = 0; i < data.size(); ++i )
        {
            Stream::Read( m_Sections[ i ], data[ i ] );
        }
    }


    /// @breif  reads the offset from the left edge of the sprite that the bar starts and ends
    /// @param  data    the json data to read from
    void UiBarSprite::readBorderPositions( nlohmann::ordered_json const& data )
    {
        Stream::Read( &m_BorderPositions, data );
    }

    /// @breif  reads the vertical position of the slope rotation
    /// @param  data    the json data to read from
    void UiBarSprite::readRotationPosition( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_RotationPosition, data );
    }


//-----------------------------------------------------------------------------
// public: reading / writing
//-----------------------------------------------------------------------------


    /// @brief  gets the map of read methods for this Component
    /// @return the map of read methods for this Component
    ReadMethodMap< ISerializable > const& UiBarSprite::GetReadMethods() const
    {
        static ReadMethodMap< UiBarSprite > const readMethods = {
            { "Texture"         , &UiBarSprite::readTexture          },
            { "Layer"           , &UiBarSprite::readLayer            },
            { "Opacity"         , &UiBarSprite::readOpacity          },
            { "Sections"        , &UiBarSprite::readSections         },
            { "BorderPositions" , &UiBarSprite::readBorderPositions  },
            { "RotationPosition", &UiBarSprite::readRotationPosition }
        };

        return (ReadMethodMap< ISerializable > const&)readMethods;
    }


    /// @brief  write all UiBarSprite data to json
    /// @return the json containing the UiBarSprite data
    nlohmann::ordered_json UiBarSprite::Write() const
    {
        nlohmann::ordered_json json;

        nlohmann::ordered_json& sectionData = json[ "Sections" ];
        for ( UiBarSection const& section : m_Sections )
        {
            sectionData.push_back( Stream::Write( section ) );
        }

        json[ "Texture"          ] = Stream::Write( m_Texture          );
        json[ "Layer"            ] = Stream::Write( m_Layer            );
        json[ "Opacity"          ] = Stream::Write( m_Opacity          );
        json[ "BorderPositions"  ] = Stream::Write( m_BorderPositions  );
        json[ "RotationPosition" ] = Stream::Write( m_RotationPosition );

        return json;
    }


//-----------------------------------------------------------------------------
// public: constructor / Destructor
//-----------------------------------------------------------------------------


    /// @brief  constructor
    UiBarSprite::UiBarSprite() :
        Sprite( typeid( UiBarSprite ) )
    {}


//-----------------------------------------------------------------------------
// public: copying
//-----------------------------------------------------------------------------


    /// @brief  clones this UiBarSprite
    /// @return the newly created clone of this UiBarSprite
    UiBarSprite* UiBarSprite::Clone() const
    {
        return new UiBarSprite( *this );
    }


//-----------------------------------------------------------------------------
// private: copying
//-----------------------------------------------------------------------------


    /// @brief  copy-constructor for the UiBarSprite
    /// @param  other   the other UiBarSprite to copy
    UiBarSprite::UiBarSprite( UiBarSprite const& other ) :
        Sprite( other ),
        m_Sections        ( other.m_Sections         ),
        m_BorderPositions ( other.m_BorderPositions  ),
        m_RotationPosition( other.m_RotationPosition )
    {}


//-----------------------------------------------------------------------------
