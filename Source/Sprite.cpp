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
    m_Color(      other.m_Color      ),
    m_Opacity(    other.m_Opacity    ),
    m_Layer(      other.m_Layer      ),
    m_IsTextured( other.m_IsTextured ),
    m_Texture(    other.m_Texture    ),
    m_FrameIndex( other.m_FrameIndex )
{}

//-----------------------------------------------------------------------------
// public: methods
//-----------------------------------------------------------------------------


/// @brief          Draws the mesh with texture (if one is present), or color.
void Sprite::Draw()
{
    if (!m_Texture)
        return;

    Shader* sh = Renderer()->SetActiveShader("texture");
    m_Texture->Bind();
    glm::vec2 uv_offset = calcUVoffset();
    glUniform2f(sh->GetUniformID("UV_offset"), uv_offset.x, uv_offset.y);
    

    // Stuff they both have in common: transform and opacity
    glm::mat4 mat(1);   // it can still draw without parent and transform
    if (GetEntity())
    {
        Transform* t = GetEntity()->GetComponent<Transform>();
        if (t)
        {
            mat = t->GetMatrix();

            // world or UI space
            if (t->GetIsDiegetic())
                mat = Cameras()->GetMat_WorldToClip() * mat;
            else
                mat = Cameras()->GetMat_UItoClip() * mat;
        }
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
// private: virtual override methods
//-----------------------------------------------------------------------------

/// @brief  called when entering the scene
void Sprite::OnInit()
{
    glm::ivec2 sheetDimension = m_IsTextured ? m_Texture->GetSheetDimensions() : glm::ivec2( 1, 1 );

    Renderer()->AddSprite( this );
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

        Inspection::SelectAssetFromLibrary( "Texture", &m_Texture );

        if (!m_Texture)
        {
            return;
        }

        if ( ImGui::DragInt(
            "Frame Index", &m_FrameIndex, 1, 0, 
            m_Texture->GetSheetDimensions().x * m_Texture->GetSheetDimensions().y
        ) )
        {
			SetFrameIndex(m_FrameIndex);
		}

        // // Texture selection
        // std::vector<std::string> textureNames;
        // for (const auto& pair : AssetLibrary<Texture>()->GetAssets())
        // {
        //     textureNames.push_back(pair.first);
        // }
        // 
        // static int currentTextureIdx = 0;  // you can set this according to the current texture of the sprite if needed
        // 
        // if (ImGui::BeginCombo("Textures", textureNames[currentTextureIdx].c_str())) // Default displayed
        // {
        //     for (int n = 0; n < textureNames.size(); ++n)
        //     {
        //         bool isSelected = (currentTextureIdx == n);
        //         if (ImGui::Selectable(textureNames[n].c_str(), isSelected))
        //         {
        //             currentTextureIdx = n;
        //             // Set the texture here
        //             SetTexture(AssetLibrary<Texture>()->GetAsset(textureNames[currentTextureIdx]));
        //         }
        //         if (isSelected)
        //         {
        //             ImGui::SetItemDefaultFocus();
        //         }
        //     }
        //     ImGui::EndCombo();
        // }

    }

//-----------------------------------------------------------------------------
// private: methods
//-----------------------------------------------------------------------------

/// @brief  Calculates UV offset based on current frameIndex
/// @return the UV offset
glm::vec2 Sprite::calcUVoffset() const
{
    int width =  m_Texture->GetSheetDimensions().x;
    int row = m_FrameIndex / width;
    int col = m_FrameIndex % width;
    glm::vec2 uv = m_Texture->GetMesh()->GetUVsize() * glm::vec2( col, row );
    return uv;
}


//-----------------------------------------------------------------------------
// protected: reading
//-----------------------------------------------------------------------------

/// @brief  reads the Texture of this Sprite
/// @param  data  the json data to read from
void Sprite::readTexture( nlohmann::ordered_json const& data )
{
    m_Texture = AssetLibrary<Texture>()->GetAsset( Stream::Read<std::string>( data ) );
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
    int layer = Stream::Read<int>( data );
    m_Layer = std::max( 0, std::min( layer, 4 ) );
}

/// @brief Write all Sprite component data to a JSON file.
/// @return The JSON file containing the Sprite component data.
nlohmann::ordered_json Sprite::Write() const
{
    nlohmann::ordered_json data;

    data["Layer"] = m_Layer;
    data["Color"] = Stream::Write(m_Color);
    data["Opacity"] = m_Opacity;
    data["FrameIndex"] = m_FrameIndex;
    if ( m_Texture != nullptr )
    {
        std::string const& name = AssetLibrary<Texture>()->GetAssetName(m_Texture);
        if (!name.empty())
        {
            data["Texture"] = name;
        }
    }

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
