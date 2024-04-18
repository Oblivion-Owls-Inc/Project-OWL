/// @file       BeamSprite.cpp
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      Sprite that renders a beam with a scrollable texture
/// @version    0.1
/// @date       April 2024
/// 
/// @copyright  Copyright (c) 2024 Digipen Institute of Technology

#include "pch.h"

#include "BeamSprite.h"

#include "RenderSystem.h"
#include "CameraSystem.h"


//-----------------------------------------------------------------------------
// public: constructor / Destructor
//-----------------------------------------------------------------------------


    /// @brief  default constructor
    BeamSprite::BeamSprite() :
        Sprite( typeid( BeamSprite ) )
    {}


//-----------------------------------------------------------------------------
// public: methods
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// public: accessors
//-----------------------------------------------------------------------------


    /// @brief  gets the length of the beam
    /// @return the length of the beam
    float BeamSprite::GetLength() const
    {
        return m_Length;
    }

    /// @brief  sets the length of the beam
    /// @param  length  the length of the beam
    void BeamSprite::SetLength( float length )
    {
        m_Length = length;
    }


    /// @brief  gets the phase of the texture along the beam
    /// @return the phase of the texture along the beam
    float BeamSprite::GetPhase() const
    {
        return m_Phase;
    }

    /// @brief  sets the phase of the texture along the beam
    /// @param  phase   the phase of the texture along the beam
    void BeamSprite::SetPhase( float phase )
    {
        m_Phase = phase;
    }


//-----------------------------------------------------------------------------
// public: virtual override methods
//-----------------------------------------------------------------------------


    /// @brief  called once when entering the scene
    void BeamSprite::OnInit()
    {
        Sprite::OnInit();

        // init the shader if necessary
        if ( Renderer()->GetShader( "Beam" ) == nullptr )
        {
            Renderer()->AddShader(
                "Beam",
                new Shader(
                    "Data/shaders/beam.vert",
                    "Data/shaders/texture.frag"
                )
            );
        }
    }

    /// @brief  called once when exiting the scene
    void BeamSprite::OnExit()
    {
        Sprite::OnExit();
    }


    /// @brief  draws this BeamSprite
    void BeamSprite::Draw()
    {
        if ( m_Transform == nullptr || m_Texture == nullptr )
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
// private: methods
//-----------------------------------------------------------------------------


    /// @brief  prepares the shader to render
    void BeamSprite::prepareShader() const
    {
        Shader* shader = Renderer()->SetActiveShader( "Beam" );
        if ( shader == nullptr )
        {
            Debug() << "Error: unable to find shader \"Beam\"" << std::endl;
            return;
        }

        // mvp matrix
        float horizontalScale = m_Length / (m_Transform->GetScale().x * m_Texture->GetMesh()->GetScale().x);
        glm::mat4 mvp = m_Transform->GetMatrix() * glm::scale( glm::mat4( 1.0f ), glm::vec3( horizontalScale, 1.0f, 1.0f ) );
        if ( m_Transform->GetIsDiegetic() )
        {
            mvp = Cameras()->GetMat_WorldToClip() * mvp;
        }
        else
        {
            mvp = Cameras()->GetMat_UiToClip() * mvp;
        }
        glUniformMatrix4fv( shader->GetUniformID( "mvp" ), 1, false, &mvp[ 0 ][ 0 ] );

        // vertex uniforms
        glUniform1f( shader->GetUniformID( "length" ), m_Length / (m_Transform->GetScale().x * m_Texture->GetMesh()->GetScale().x) );

        glUniform1f( shader->GetUniformID( "phase" ), m_Phase );

        // fragment uniforms
        glUniform1f( shader->GetUniformID( "opacity" ), m_Opacity );

        glUniform4fv( shader->GetUniformID( "tint" ), 1, &m_Color[ 0 ] );

    }


//-----------------------------------------------------------------------------
// public: inspection
//-----------------------------------------------------------------------------


    /// @brief  shows the inspector for BeamSprite
    void BeamSprite::Inspector()
    {
        if ( m_Texture.Inspect( "texture" ) && GetEntity()->IsInScene() )
        {
            OnExit();
            OnInit();
        }

        ImGui::NewLine();

        if ( ImGui::ColorEdit3( "Color", &m_Color[ 0 ] ) )
        {
            SetColor( m_Color );
        }
        if ( ImGui::DragFloat( "Opacity", &m_Opacity, 0.01f, 0.0f, 1.0f ) )
        {
            SetOpacity( m_Opacity );
        }

        ImGui::NewLine();

        if ( ImGui::DragInt( "Layer", &m_Layer, 0.05f ) )
        {
            SetLayer( m_Layer );
        }

        ImGui::NewLine();

        ImGui::DragFloat( "beam length", &m_Length, 0.05f, 0.0f, INFINITY );
        ImGui::DragFloat( "beam phase" , &m_Phase , 0.05f );
    }


//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------


    /// @brief  reads the length of the beam
    /// @param  data    the JSON data to read from
    void BeamSprite::readLength( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_Length, data );
    }

    /// @brief  reads the phase of the texture along the beam
    /// @param  data    the JSON data to read from
    void BeamSprite::readPhase( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_Phase, data );
    }


//-----------------------------------------------------------------------------
// public: reading / writing
//-----------------------------------------------------------------------------


    /// @brief  gets the map of read methods for this BeamSprite
    /// @return the map of read methods for this BeamSprite
    ReadMethodMap< ISerializable > const& BeamSprite::GetReadMethods() const
    {
        static ReadMethodMap< BeamSprite > const readMethods = {
            { "Length" , &BeamSprite::readLength  },
            { "Phase"  , &BeamSprite::readPhase   },
            { "Texture", &BeamSprite::readTexture },
            { "Opacity", &BeamSprite::readOpacity },
            { "Color"  , &BeamSprite::readColor   },
            { "Layer"  , &BeamSprite::readLayer   }
        };

        return (ReadMethodMap< ISerializable > const&)readMethods;
    }


    /// @brief  writes this BeamSprite to JSON
    /// @return the JSON data of this BeamSprite
    nlohmann::ordered_json BeamSprite::Write() const
    {
        nlohmann::ordered_json json = nlohmann::ordered_json::object();

        json[ "Length"  ] = Stream::Write( m_Length  );
        json[ "Phase"   ] = Stream::Write( m_Phase   );
        json[ "Texture" ] = Stream::Write( m_Texture );
        json[ "Opacity" ] = Stream::Write( m_Opacity );
        json[ "Color"   ] = Stream::Write( m_Color   );
        json[ "Layer"   ] = Stream::Write( m_Layer   );

        return json;
    }

    
//-----------------------------------------------------------------------------
// public: copying
//-----------------------------------------------------------------------------


    /// @brief  clones this BeamSprite
    /// @return the newly created clone of this BeamSprite
    BeamSprite* BeamSprite::Clone() const
    {
        return new BeamSprite( *this );
    }


//-----------------------------------------------------------------------------
// private: copying
//-----------------------------------------------------------------------------


    /// @brief  copy-constructor for the BeamSprite
    /// @param  other   the other BeamSprite to copy
    BeamSprite::BeamSprite( BeamSprite const& other ) :
        Sprite( other ),
        m_Length( other.m_Length ),
        m_Phase ( other.m_Phase  )
    {}


//-----------------------------------------------------------------------------
