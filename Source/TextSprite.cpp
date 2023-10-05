/// @file     TextSprite.cpp
/// @author   Eli Tsereteli (ilya.tsereteli@digipen.edu)
/// 
/// @brief    A version of Sprite specifically for rendering text. Uses instancing to draw multiple 
///           letters simultaneously.
#include "TextSprite.h"
#include "Mesh.h"  
#include "Texture.h"
#include "Entity.h"         // parent
#include "Transform.h"
#include "RenderSystem.h"   // shader
#include "CameraSystem.h"   // projection matrices
#include "Stream.h"


//-----------------------------------------------------------------------------
// public: constructor / destructors
//-----------------------------------------------------------------------------

    /// @brief  default constructor
    TextSprite::TextSprite() :
        Sprite( typeid( TextSprite ) ),
        m_Text(),
        m_RowWidth( 43 ),
        m_StrideMultiplier( 1, 1 ),
        m_InstBufferID( 0 )
    {}


    /// @brief  manual constructor
    /// @param  texture             the texture for this TextSprite to use
    /// @param  strideMultiplier    (optional) Multiplier to adjust stride (spacing)
    /// @param  layer               (optional) Rendering layer: 0-4. 0 is back, 4 is front.
    TextSprite::TextSprite( Texture const* texture, float strideMultiplier, int layer ) :
        Sprite( texture, layer, typeid( TextSprite ) ),
        m_Text(),
        m_RowWidth( 43 ),
        m_StrideMultiplier( strideMultiplier, 1 ),
        m_InstBufferID( 0 )
    {}


    /// @brief  Destructor
    TextSprite::~TextSprite()
    {
        
    }


//-----------------------------------------------------------------------------
// public: methods
//-----------------------------------------------------------------------------

    /// @brief  Draws currently stored text using parent's transform.
    void TextSprite::Draw()
    {
        glm::mat4 trm(1);                       // transform matrix - identity by default
        glm::vec2 stridex(m_StrideMultiplier.x, 0);   // x stride vector - pointing right by default
        glm::vec2 stridey(0, -m_StrideMultiplier.y);  // y stride vector - down
        glm::vec2 uvsize = m_Mesh->GetUVsize(); // UV size (for the frames of spritesheet)

        // Calculate matrix and stride based on parent't transform
        if (GetParent() && GetParent()->GetComponent<Transform>())
        {
            Transform* tr = GetParent()->GetComponent<Transform>();
            glm::mat4 proj;

            if (tr->GetIsDiegetic())
                proj = Camera()->GetMat_WorldToClip();
            else
                proj = Camera()->GetMat_UItoClip();

            // get transform matrix, and its linear part (for stride)
            trm = tr->GetMatrix();

            // calculate stride vectors using linear transform (projected)
            glm::mat2 trm_linear = glm::mat2(proj) * glm::mat2(trm);
            stridex = trm_linear * stridex;
            stridey = trm_linear * stridey;

            // apply full projection to full transform, for the actual mesh position
            trm = proj * trm;
        }

        // Select the shader, and send all the uniform data to it
        Shader* sh_txt = Renderer()->SetActiveShader("text");
        glUniformMatrix4fv(sh_txt->GetUniformID("mvp"), 1, 0, &trm[0][0]);
        glUniform1f(sh_txt->GetUniformID("opacity"), m_Opacity);
        glUniform2f(sh_txt->GetUniformID("stridex"), stridex.x, stridex.y);
        glUniform2f(sh_txt->GetUniformID("stridey"), stridey.x, stridey.y);
        glUniform2f(sh_txt->GetUniformID("UVsize"), uvsize.x, uvsize.y);
        glUniform1i(sh_txt->GetUniformID("columns"), m_Texture->GetSheetDimensions().x);
        glUniform1i(sh_txt->GetUniformID("rowwidth"), m_RowWidth);
        glUniform4fv(sh_txt->GetUniformID("tint"), 1, &m_Color[0]);

        // Load the string into buffer. sigh... chars have to be converted to floats. 
        // (should be ints, but I can't get it to read ints from buffer correctly, 
        // so I'll just cast them on shader)
        int count = (int)m_Text.size();
        if (count)
        {
            std::vector<float> chars(count);
            for (int i = 0; i < count; i++)
                chars[i] = (float)m_Text[i];

            glBindBuffer(GL_ARRAY_BUFFER, m_InstBufferID);
            glBufferData(GL_ARRAY_BUFFER, sizeof(float) * count, &chars[0], GL_STREAM_DRAW);
        }


        // Bind the texture and render instanced mesh
        m_Texture->Bind();
        glBindVertexArray(m_Mesh->GetVAO());
        glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, m_Mesh->GetVertexCount(), count);
        glBindVertexArray(0);
    }

//-----------------------------------------------------------------------------
// private: virtual override methods
//-----------------------------------------------------------------------------

    /// @brief  called when entering the scene
    void TextSprite::OnInit()
    {
        Sprite::OnInit();

        // Init text shader if it ain't.
        if (!Renderer()->GetShader("text"))
        {
            Renderer()->AddShader("text", new Shader("Data/shaders/text_instancing.vert", "Data/shaders/text_instancing.frag"));
        }

        // For instancing, I'll need an extra buffer, and an additional attribute for this mesh
        glGenBuffers(1, &m_InstBufferID);
        glBindBuffer(GL_ARRAY_BUFFER, m_InstBufferID);

        glBindVertexArray(m_Mesh->GetVAO());
        glEnableVertexAttribArray(2);

        // First 2 attribute indices are already used for vertex data.
        // This third index will be used for instance data.
        // index 2: 1 float, auto stride, offset 0
        glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 0, 0);
        glVertexAttribDivisor(2, 1);

        glBindVertexArray(0);

    }

    /// @brief  called when exiting the scene
    void TextSprite::OnExit()
    {
        Sprite::OnExit();

        glDeleteBuffers( 1, &m_InstBufferID );
    }

//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------

    /// @brief          Read in the text this TextSprite displays
    /// @param  stream  The json to read from.
    void TextSprite::readText(Stream stream)
    {
        m_Text = stream.Read<std::string>();
    }

    /// @brief          Read in the stride multiplier
    /// @param  stream  The json to read from.
    void TextSprite::readStrideMultiplier(Stream stream)
    {
        m_StrideMultiplier = stream.Read<glm::vec2>();
    }

    /// @brief          Read in the amount of tile per row
    /// @param  stream  The json to read from.
    void TextSprite::readRowWidth(Stream stream)
    {
        m_RowWidth = stream.Read<int>();
    }

    /// @brief the map of read methods for this Component
    ReadMethodMap< TextSprite > const TextSprite::s_ReadMethods = {
        { "Texture"         , &readTexture          },
        { "Layer"           , &readLayer            },
        { "Color"           , &readColor            },
        { "Text"            , &readText             },
        { "StrideMultiplier", &readStrideMultiplier },
        { "RowWidth"        , &readRowWidth         }
    };

    /// @brief gets the map of read methods for this Component
    /// @return the map of read methods for this Component
    ReadMethodMap< Component > const& TextSprite::GetReadMethods() const
    {
        return (ReadMethodMap< Component> const&)s_ReadMethods;
    }

//-----------------------------------------------------------------------------
