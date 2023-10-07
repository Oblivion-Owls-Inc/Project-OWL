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


//-----------------------------------------------------------------------------
// public: constructor / destructors
//-----------------------------------------------------------------------------

    /// @brief  default constructor
    Sprite::Sprite() :
        Component( typeid( Sprite ) ),
        m_Color( { 0, 0, 0, 1, } ),
        m_Opacity( 1.0f ),
        m_Layer( 2 ),
        m_Mesh( nullptr ),
        m_IsTextured( false ),
        m_Texture( nullptr ),
        m_FrameIndex( 0 )
    {}

    /// @brief  Textured constructor
    /// @param  texture the texture for this Sprite to use
    /// @param  layer   (optional) Rendering layer: 0-4. 0 is back, 4 is front.
    Sprite::Sprite( Texture const* texture, int layer, std::type_index type ) :
        Component( type ),
        m_Color( { 0, 0, 0, 1, } ),
        m_Opacity( 1.0f ),
        m_Layer( layer ),
        m_IsTextured( true ),
        m_Texture( texture ),
        m_FrameIndex( 0 )
    {
        glm::ivec2 sheetDimensions = m_Texture->GetSheetDimensions();
        m_Mesh = new Mesh( true, sheetDimensions.x, sheetDimensions.y );
    }


    /// @brief  Plain square constructor
    /// @param  init_square true/false - initialize the square or nah?
    /// @param  color       (optional) Color to initialize the square to
    /// @param  layer       (optional) Rendering layer: 0-4. 0 is back, 4 is front.
    Sprite::Sprite( bool init_square, glm::vec4 const& color, int layer ) :
        Component( typeid( Sprite ) ),
        m_Color( color ),
        m_Opacity( 1.0f ),
        m_Layer( layer ),
        m_IsTextured( false ),
        m_Texture( nullptr ),
        m_FrameIndex( 0 )
    {

        if (init_square)
            m_Mesh = new Mesh(true, 1, 1);
    }

    /// @brief      Clears out memory, removes this sprite's pointer from RenderSystem.
    Sprite::~Sprite()
    {
        // TODO: remove this after data library is implemented
        delete m_Mesh;
    }

//-----------------------------------------------------------------------------
// protected: constructor
//-----------------------------------------------------------------------------

    /// @brief          Default constructor - does nothing
    Sprite::Sprite( std::type_index type ) :
        Component( type )
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
        m_Mesh(       nullptr            ),
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
        Shader* sh;

        // Render texture if one is present, or color if not
        if (m_Texture)
        {
            sh = Renderer()->SetActiveShader("texture");
            m_Texture->Bind();
            glm::vec2 uv_offset = calcUVoffset();
            glUniform2f(sh->GetUniformID("UV_offset"), uv_offset.x, uv_offset.y);
        }
        else
        {
            sh = Renderer()->SetActiveShader("color");
            glUniform4fv(sh->GetUniformID("color"), 1, &m_Color.x);
        }

        // Stuff they both have in common: transform and opacity
        glm::mat4 mat(1);   // it can still draw without parent and transform
        if (GetParent())
        {
            Transform* t = GetParent()->GetComponent<Transform>();
            if (t)
            {
                mat = t->GetMatrix();

                // world or UI space
                if (t->GetIsDiegetic())
                    mat = Camera()->GetMat_WorldToClip() * mat;
                else
                    mat = Camera()->GetMat_UItoClip() * mat;
            }
        }

        glUniformMatrix4fv(sh->GetUniformID("mvp"), 1, 0, &mat[0][0]);
        glUniform1f(sh->GetUniformID("opacity"), m_Opacity);

        // Render it with triangle strip mode
        glBindVertexArray(m_Mesh->GetVAO());
        glDrawArrays(GL_TRIANGLE_STRIP, 0, m_Mesh->GetVertexCount());
        glBindVertexArray(0);
    }


//-----------------------------------------------------------------------------
// private: virtual override methods
//-----------------------------------------------------------------------------

    /// @brief  called when entering the scene
    void Sprite::OnInit()
    {

        glm::ivec2 sheetDimension = m_IsTextured ? m_Texture->GetSheetDimensions() : glm::ivec2( 1, 1 );
        m_Mesh = new Mesh(true, sheetDimension.y, sheetDimension.x);

        Renderer()->AddSprite( this );
    }

    /// @brief  called when exiting the scene
    void Sprite::OnExit()
    {
        Renderer()->RemoveSprite( this );
    }

    void Sprite::Inspector()
    {
        if (ImGui::DragInt("Layer", &m_Layer, 1, 0, 4))
        {
            SetLayer(m_Layer);
        }
        if (ImGui::DragFloat("Opacity", &m_Opacity, 0.01f, 0.0f, 1.0f))
        {
			SetOpacity(m_Opacity);
		}
        if (!m_Texture)
        {
            return;
        }
        if (ImGui::DragInt("Frame Index", &m_FrameIndex, 1, 0, 
            m_Texture->GetSheetDimensions().x * m_Texture->GetSheetDimensions().y))
        {
			SetFrameIndex(m_FrameIndex);
		}
        if (ImGui::ColorEdit4("Color", &m_Color.x))
        {
            SetColor(m_Color);
        }

        // Texture selection
        std::vector<std::string> textureNames;
        for (const auto& pair : AssetLibrary<Texture>()->GetAssets())
        {
            textureNames.push_back(pair.first);
        }

        static int currentTextureIdx = 0;  // you can set this according to the current texture of the sprite if needed

        if (ImGui::BeginCombo("Textures", textureNames[currentTextureIdx].c_str())) // Default displayed
        {
            for (int n = 0; n < textureNames.size(); ++n)
            {
                bool isSelected = (currentTextureIdx == n);
                if (ImGui::Selectable(textureNames[n].c_str(), isSelected))
                {
                    currentTextureIdx = n;
                    // Set the texture here
                    SetTexture(AssetLibrary<Texture>()->GetAsset(textureNames[currentTextureIdx]));
                }
                if (isSelected)
                {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
        }

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
        glm::vec2 uv = m_Mesh->GetUVsize() * glm::vec2( col, row );
        return uv;
    }


/// @brief      Pre-calculates height multiplier based on texture dimensions
// void Sprite::calcHeightMult()
// {
//     if (!m_Texture || !m_Mesh)
//         return;
// 
//     glm::vec2 size = m_Texture->GetPixelDimensions();
//     glm::vec2 uvsize = m_Mesh->GetUVsize();
//     m_HeightMult = (size.y / size.x) * (uvsize.y / uvsize.x);
// }



//-----------------------------------------------------------------------------
// protected: reading
//-----------------------------------------------------------------------------

    /// @brief  reads the Texture of this Sprite
    /// @param  stream  the json data to read from
    void Sprite::readTexture( Stream stream )
    {
        m_Texture = AssetLibrary<Texture>()->GetAsset( stream.Read<std::string>() );
        m_IsTextured = true;
    }

    /// @brief  reads the frame index of this Sprite
    /// @param  stream  the json data to read from
    void Sprite::readFrameIndex( Stream stream )
    {
        m_FrameIndex = stream.Read<int>();
    }

    /// @brief        Read in the colour for a sprite.
    /// @param stream The json to read from.
    void Sprite::readColor( Stream stream )
    {
        m_Color = stream.Read<glm::vec4>();
    }

    /// @brief  Read in the opacity for a sprite.
    /// @param  stream  The json to read from.
    void Sprite::readOpacity( Stream stream )
    {
        m_Opacity = stream.Read<float>();
    }

    /// @brief        Read in the layer for a sprite.
    /// @param stream The json to read from.
    void Sprite::readLayer( Stream stream )
    {
        int layer = stream.Read<int>();
        m_Layer = std::max( 0, std::min( layer, 4 ) );
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

    /// @brief gets the map of read methods for this Component
    /// @return the map of read methods for this Component
    ReadMethodMap< Component > const& Sprite::GetReadMethods() const
    {
        return (ReadMethodMap< Component> const&)s_ReadMethods;
    }

//-----------------------------------------------------------------------------
