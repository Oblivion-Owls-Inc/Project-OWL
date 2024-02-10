/// @file     Sprite.cpp
/// @author   Eli Tsereteli (ilya.tsereteli@digipen.edu)
/// 
/// @brief    Implementation of Sprite component.
#include "Sprite.h"
#include "RenderSystem.h"
#include "Mesh.h"
#include "Texture.h"
#include "Transform.h"      // matrix
#include "Entity.h"         // parent
#include "CameraSystem.h"   // projection matrix
#include <algorithm>        // min/max
#include <iostream>         // error msg

#include "AssetLibrarySystem.h"
#include "Inspection.h"

//-----------------------------------------------------------------------------
// public: constructor / destructors
//-----------------------------------------------------------------------------

/// @brief  default constructor
Sprite::Sprite() :
    Component( typeid( Sprite ) ),
    m_Color( { 0, 0, 0, 0 } ),
    m_Opacity( 1.0f ),
    m_Layer( 2 ),
    m_IsTextured( false ),
    m_Texture( nullptr ),
    m_FrameIndex( 0 )
{}

/// @brief              Textured constructor
/// @param  texture     the texture for this Sprite to use
/// @param  layer       (optional) Rendering layer: 0-4. 0 is back, 4 is front.
/// @param  type        for use ONLY when deriving with this Constructor
Sprite::Sprite( Texture const* texture, int layer, std::type_index type ) :
    Component( type ),
    m_Color( { 1, 1, 1, 1 } ),
    m_Opacity( 1.0f ),
    m_Layer( layer ),
    m_IsTextured( true ),
    m_Texture( texture ),
    m_FrameIndex( 0 )
{}


//-----------------------------------------------------------------------------
// protected: constructor
//-----------------------------------------------------------------------------

/// @brief              Inherited constructor
/// @param type         typeid(DerivedClass)
Sprite::Sprite( std::type_index type ) :
    Component( type ),
    m_Color( { 0, 0, 0, 0 } ),
    m_Opacity( 1.0f ),
    m_Layer( 2 ),
    m_IsTextured( false ),
    m_Texture( nullptr ),
    m_FrameIndex( 0 )
{}

//-----------------------------------------------------------------------------
// public: copying
//-----------------------------------------------------------------------------

/// @brief  Creates new Sprite using copy constructor.
/// @return pointer to copied Sprite component.
Component* Sprite::Clone() const { return new Sprite( *this ); }
    
//-----------------------------------------------------------------------------
// public: copying
//-----------------------------------------------------------------------------

/// @brief  Copy constructor
Sprite::Sprite(Sprite const& other) :
    Component( other ),
    m_Color      ( other.m_Color       ),
    m_Opacity    ( other.m_Opacity     ),
    m_Layer      ( other.m_Layer       ),
    m_IsTextured ( other.m_IsTextured  ),
    m_Texture    ( other.m_Texture     ),
    m_TextureName( other.m_TextureName ),
    m_FrameIndex ( other.m_FrameIndex  )
{}

//-----------------------------------------------------------------------------
// public: methods
//-----------------------------------------------------------------------------


    /// @brief          Draws the mesh with texture (if one is present), or color.
    void Sprite::Draw()
    {
        if (!m_Texture)
            return;

        if ( m_Transform == nullptr )
        {
            m_Transform = GetEntity()->GetComponent< Transform >();
            if ( m_Transform == nullptr )
            {
                Debug() << "WARNING: Sprite component must have an attached Transform on entity \"" << GetEntity()->GetName() << "\"" << std::endl;
                return;
            }
        }

        if ( m_Texture->GetMesh() == nullptr )
        {
            Debug() << "WARNING: texture attached to sprite on \"" << GetEntity()->GetName() << "\" does not have an associated mesh"<< std::endl;
            return;
        }

        Shader* sh = Renderer()->SetActiveShader("texture");
        m_Texture->Bind();
        glm::vec2 uv_offset = calcUVoffset();
        glUniform2f(sh->GetUniformID("UV_offset"), uv_offset.x, uv_offset.y);
    

        // Stuff they both have in common: transform and opacity
        glm::mat4 mat(1);   // it can still draw without parent and transform
        if (GetEntity())
        {
            if ( m_Transform == nullptr)
            {
                m_Transform = GetEntity()->GetComponent< Transform >();
            }

            mat = m_Transform->GetMatrix();

            // world or UI space
            if ( m_Transform->GetIsDiegetic() )
                mat = Cameras()->GetMat_WorldToClip() * mat;
            else
                mat = Cameras()->GetMat_UItoClip() * mat;
        }

        glUniformMatrix4fv(sh->GetUniformID("mvp"), 1, false, &mat[0][0]);
        glUniform1f(sh->GetUniformID("opacity"), m_Opacity);
        glUniform4fv(sh->GetUniformID("tint"), 1, &m_Color[0]);

        // Render it with triangle strip mode
        Mesh const* m = m_Texture->GetMesh();
        glBindVertexArray(m->GetVAO());
        glDrawArrays(GL_TRIANGLE_STRIP, 0, m->GetVertexCount());
        glBindVertexArray(0);
    }


//-----------------------------------------------------------------------------
// public: accessors
//-----------------------------------------------------------------------------

    /// @brief  gets the frame index
    /// @return the frame index
    int Sprite::GetFrameIndex() const
    {
        return m_FrameIndex;
    }

    /// @brief  Sets current frame index of the spritesheet.
    /// @param  frameIndex  New frame index
    void Sprite::SetFrameIndex( int frameIndex )
    {
        m_FrameIndex = frameIndex;
    }

    /// @brief  gets the layer
    /// @return the layer
    int Sprite::GetLayer() const
    {
        return m_Layer;
    }

    /// @brief  Sets the rendering layer : 0 - 4.  0 is back, 4 is front.
    /// @param  layer   Rendering layer to move this sprite to.
    void Sprite::SetLayer( int layer )
    {
        m_Layer = layer;
    }

    /// @brief  gets the opacity
    /// @return the opacity
    float Sprite::GetOpacity() const
    {
        return m_Opacity;
    }

    /// @brief  Sets the opacity.
    /// @param  opacity I'm not explaining this.
    void Sprite::SetOpacity( float opacity )
    {
        m_Opacity = opacity;
    }

    /// @brief  gets the color
    /// @return the color
    glm::vec4 const& Sprite::GetColor() const
    {
        return m_Color;
    }

    /// @brief  sets the color
    /// @param  color   the color to apply
    void Sprite::SetColor( glm::vec4 const& color )
    {
        m_Color = color;
    }

    /// @brief  gets the Texture this Sprite is using
    /// @return the Texture this Sprite is using
    Texture const* Sprite::GetTexture() const
    {
        if ( m_Texture != nullptr )
        {
            return m_Texture;
        }

        return AssetLibrary< Texture >()->GetAsset( m_TextureName );
    }

    /// @brief  sets the Texture this Sprite is using
    /// @param  texture the Texture to set this Sprite to use
    void Sprite::SetTexture( Texture const* texture )
    {
        m_Texture = texture;
    }

    /// @brief  gets the Transform component attached to this Sprite
    /// @return the Transform component attached to this Sprite
    Transform* Sprite::GetTransform()
    {
        return m_Transform;
    }


//-----------------------------------------------------------------------------
// private: virtual override methods
//-----------------------------------------------------------------------------

/// @brief  called when entering the scene
void Sprite::OnInit()
{
    Renderer()->AddSprite( this );

    m_Transform = GetEntity()->GetComponent< Transform >();
    
    if ( m_TextureName.empty() == false )
    {
        m_Texture = AssetLibrary< Texture >()->GetAsset( m_TextureName );
    }
}

/// @brief  called when exiting the scene
void Sprite::OnExit()
{
    Renderer()->RemoveSprite( this );
}


    void Sprite::Inspector()
    {
        if ( ImGui::DragInt( "Layer", &m_Layer, 0.05f ) )
        {
            SetLayer(m_Layer);
        }
        if ( ImGui::ColorEdit3( "Color", &m_Color[ 0 ] ) )
        {
            SetColor(m_Color);
        }
        if ( ImGui::DragFloat( "Opacity", &m_Opacity, 0.01f, 0.0f, 1.0f ) )
        {
            SetOpacity(m_Opacity);
        }

        Inspection::SelectAssetFromLibrary( "Texture", &m_Texture, &m_TextureName );

        if (!m_Texture)
        {
            m_IsTextured = false;
            return;
        }
        m_IsTextured = true;

        if ( ImGui::DragInt(
            "Frame Index", &m_FrameIndex, 1, 0, 
            m_Texture->GetSheetDimensions().x * m_Texture->GetSheetDimensions().y
        ) )
        {
			SetFrameIndex(m_FrameIndex);
		}

        m_Texture->DisplayInInspector( m_FrameIndex );
    }


    /// @brief  called whenever another component is added to this component's Entity in the inspector
    /// @param  component   the component that was added
    void Sprite::OnInspectorAddComponent( Component* component )
    {
        if ( dynamic_cast< Transform* >( component ) != nullptr )
        {
            m_Transform = dynamic_cast< Transform* >( component );
        }
    }

    /// @brief  called whenever another component is removed from this component's Entity in the inspector
    /// @param  component   the component that will be removed
    void Sprite::OnInspectorRemoveComponent( Component* component )
    {
        if ( dynamic_cast< Transform* >( component ) == m_Transform )
        {
            m_Transform = nullptr;
        }
    }


//-----------------------------------------------------------------------------
// private: methods
//-----------------------------------------------------------------------------

/// @brief  Calculates UV offset based on current frameIndex
/// @return the UV offset
glm::vec2 Sprite::calcUVoffset() const
{
    return m_Texture->GetUvOffset( m_FrameIndex );
}


//-----------------------------------------------------------------------------
// protected: reading
//-----------------------------------------------------------------------------

/// @brief  reads the Texture of this Sprite
/// @param  data  the json data to read from
void Sprite::readTexture( nlohmann::ordered_json const& data )
{
    Stream::Read( m_TextureName, data );
    m_IsTextured = true;
}

/// @brief  reads the frame index of this Sprite
/// @param  data  the json data to read from
void Sprite::readFrameIndex( nlohmann::ordered_json const& data )
{
    m_FrameIndex = Stream::Read<int>( data );
}

/// @brief        Read in the colour for a sprite.
/// @param data The json to read from.
void Sprite::readColor( nlohmann::ordered_json const& data )
{
    m_Color = Stream::Read< 4, float >( data );
}

/// @brief  Read in the opacity for a sprite.
/// @param  data  The json to read from.
void Sprite::readOpacity( nlohmann::ordered_json const& data )
{
    m_Opacity = Stream::Read<float>( data );
}

/// @brief        Read in the layer for a sprite.
/// @param data The json to read from.
void Sprite::readLayer( nlohmann::ordered_json const& data )
{
    m_Layer = Stream::Read<int>( data );
}

/// @brief Write all Sprite component data to a JSON file.
/// @return The JSON file containing the Sprite component data.
nlohmann::ordered_json Sprite::Write() const
{
    nlohmann::ordered_json data;

    data[ "Layer"      ] = Stream::Write( m_Layer       );
    data[ "Color"      ] = Stream::Write( m_Color       );
    data[ "Opacity"    ] = Stream::Write( m_Opacity     );
    data[ "FrameIndex" ] = Stream::Write( m_FrameIndex  );
    data[ "Texture"    ] = Stream::Write( m_TextureName );

    return data;
}

//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------

/// @brief the map of read methods for this Component
ReadMethodMap< Sprite > const Sprite::s_ReadMethods = {
    { "Texture"   , &readTexture    },
    { "Layer"     , &readLayer      },
    { "Color"     , &readColor      },
    { "Opacity"   , &readOpacity    },
    { "FrameIndex", &readFrameIndex }
};

//-----------------------------------------------------------------------------
