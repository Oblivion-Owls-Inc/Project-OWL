/// @file       Nineslice.cpp
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      sprite that renders a nineslice
/// @version    0.1
/// @date       2024-02-20
/// 
/// @copyright  Copyright (c) 2024 Digipen Institute of Technology

#include "Nineslice.h"

#include "RenderSystem.h"
#include "CameraSystem.h"

//-----------------------------------------------------------------------------
// public: accessors
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// public:  methods
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// private: virtual override methods
//-----------------------------------------------------------------------------


    /// @brief  called once when entering the scene
    void Nineslice::OnInit()
    {
        Sprite::OnInit();

        // init the shader if necessary
        if ( Renderer()->GetShader( "Nineslice" ) == nullptr )
        {
            Renderer()->AddShader(
                "Nineslice",
                new Shader(
                    "Data/shaders/nineslice.vert", 
                    "Data/shaders/nineslice.frag"
                )
            );
        }
    }

    /// @brief  called once when exiting the scene
    void Nineslice::OnExit()
    {
        Sprite::OnExit();
    }


    /// @brief  renders this Nineslice
    void Nineslice::Draw()
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
    void Nineslice::prepareShader() const
    {
        Shader* shader = Renderer()->SetActiveShader( "Nineslice" );
        if ( shader == nullptr )
        {
            Debug() << "Error: unable to find shader \"Nineslice\"" << std::endl;
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
        glUniform2fv( shader->GetUniformID( "uvOffset" ), 1, &m_Texture->GetUvOffset( m_FrameIndex )[ 0 ] );

        glUniform1f( shader->GetUniformID( "opacity" ), m_Opacity );

        glUniform4fv( shader->GetUniformID( "tint" ), 1, &m_Color[ 0 ] );

        
        glUniform2fv( shader->GetUniformID( "uvSize" ), 1, &m_Texture->GetMesh()->GetUVsize()[ 0 ] );

        glUniform2fv( shader->GetUniformID( "minInternalEdge" ), 1, &m_MinInternalEdge[ 0 ] );
        glUniform2fv( shader->GetUniformID( "maxInternalEdge" ), 1, &m_MaxInternalEdge[ 0 ] );

        glm::vec2 minBorderWidths = m_MinBorderWidths / m_Transform->GetScale();
        glm::vec2 maxBorderWidths = m_MaxBorderWidths / m_Transform->GetScale();
        glUniform2fv( shader->GetUniformID( "minBorderWidths" ), 1, &minBorderWidths[ 0 ] );
        glUniform2fv( shader->GetUniformID( "maxBorderWidths" ), 1, &maxBorderWidths[ 0 ] );
    }


//-----------------------------------------------------------------------------
// private: inspection
//-----------------------------------------------------------------------------

    
    /// @brief  displays this Nineslice in the Inspector
    void Nineslice::Inspector()
    {
        Sprite::Inspector();

        ImGui::DragFloat2( "min internal edge", &m_MinInternalEdge[ 0 ], 0.05f, 0.0f, 1.0f );
        ImGui::DragFloat2( "max internal edge", &m_MaxInternalEdge[ 0 ], 0.05f, 0.0f, 1.0f );
        ImGui::NewLine();

        ImGui::DragFloat2( "min border widths", &m_MinBorderWidths[ 0 ], 0.05f, 0.0f, 1.0f );
        ImGui::DragFloat2( "max border widths", &m_MaxBorderWidths[ 0 ], 0.05f, 0.0f, 1.0f );
    }


//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------


    /// @brief  reads the internal edges of the nineslice in negative directions
    /// @param  data    the JSON data to read from
    void Nineslice::readMinInternalEdge( nlohmann::ordered_json const& data )
    {
        Stream::Read( &m_MinInternalEdge, data );
    }

    /// @brief  reads the internal edges of the nineslice in positive directions
    /// @param  data    the JSON data to read from
    void Nineslice::readMaxInternalEdge( nlohmann::ordered_json const& data )
    {
        Stream::Read( &m_MaxInternalEdge, data );
    }


    /// @brief  reads the widths of the minimum borders
    /// @param  data    the JSON data to read from
    void Nineslice::readMinBorderWidths( nlohmann::ordered_json const& data )
    {
        Stream::Read( &m_MinBorderWidths, data );
    }

    /// @brief  reads the widths of the maximum borders
    /// @param  data    the JSON data to read from
    void Nineslice::readMaxBorderWidths( nlohmann::ordered_json const& data )
    {
        Stream::Read( &m_MaxBorderWidths, data );
    }


//-----------------------------------------------------------------------------
// public: reading / writing
//-----------------------------------------------------------------------------


    /// @brief  gets the map of read methods for this Component
    /// @return the map of read methods for this Component
    ReadMethodMap< ISerializable > const& Nineslice::GetReadMethods() const
    {
        static ReadMethodMap< Nineslice > const readMethods = {
            { "MinInternalEdge", &Nineslice::readMinInternalEdge },
            { "MaxInternalEdge", &Nineslice::readMaxInternalEdge },
            { "MinBorderWidths", &Nineslice::readMinBorderWidths },
            { "MaxBorderWidths", &Nineslice::readMaxBorderWidths },
            { "Texture"        , &Nineslice::readTexture         },
            { "Opacity"        , &Nineslice::readOpacity         },
            { "FrameIndex"     , &Nineslice::readFrameIndex      },
            { "Color"          , &Nineslice::readColor           }
        };

        return (ReadMethodMap< ISerializable > const&)readMethods;
    }


    /// @brief  Write all Nineslice data to a JSON file.
    /// @return The JSON file containing the Nineslice data.
    nlohmann::ordered_json Nineslice::Write() const
    {
        nlohmann::ordered_json json;

        json[ "MinInternalEdge" ] = Stream::Write( m_MinInternalEdge );
        json[ "MaxInternalEdge" ] = Stream::Write( m_MaxInternalEdge );
        json[ "MinBorderWidths" ] = Stream::Write( m_MinBorderWidths );
        json[ "MaxBorderWidths" ] = Stream::Write( m_MaxBorderWidths );
        json[ "Texture"         ] = Stream::Write( m_Texture         );
        json[ "Opacity"         ] = Stream::Write( m_Opacity         );
        json[ "FrameIndex"      ] = Stream::Write( m_FrameIndex      );
        json[ "Color"           ] = Stream::Write( m_Color           );

        return json;
    }


//-----------------------------------------------------------------------------
// public: constructor / Destructor
//-----------------------------------------------------------------------------


    /// @brief  constructor
    Nineslice::Nineslice() :
        Sprite( typeid( Nineslice ) )
    {}


//-----------------------------------------------------------------------------
// public: copying
//-----------------------------------------------------------------------------


    /// @brief  clones this Nineslice
    /// @return the newly created clone of this Nineslice
    Nineslice* Nineslice::Clone() const
    {
        return new Nineslice( *this );
    }


//-----------------------------------------------------------------------------
// private: copying
//-----------------------------------------------------------------------------


    /// @brief  copy-constructor for the Nineslice
    /// @param  other   the other Nineslice to copy
    Nineslice::Nineslice( const Nineslice& other ) :
        Sprite( other ),
        m_MinInternalEdge( other.m_MinInternalEdge ),
        m_MaxInternalEdge( other.m_MaxInternalEdge ),
        m_MinBorderWidths( other.m_MinBorderWidths ),
        m_MaxBorderWidths( other.m_MaxBorderWidths )
    {}


//-----------------------------------------------------------------------------
