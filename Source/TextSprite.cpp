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

/// @brief      Temporary. Allow switching to a different texture, while
///             keeping track of original one. If called without argument,
///             resets texture to original.
/// @param t    texture pointer - remember to free this one manually.
void TextSprite::SetTexture(Texture* t)
{
    if (!t)
        m_Texture = m_Original;
    else
        m_Texture = t;
}


/// @brief              Default constructor: does nothing
TextSprite::TextSprite() : Sprite() {}


/// @brief              Constructor: unlike base Sprite constructor, this one 
///                     requires texture and columns/rows, since it's specifically
///                     for text.
///                     NOTE: Spritesheet should begin with space (ASCII 32), 
///                     and have the rest of the characters in proper ASCII order.
/// 
/// @param image_file   Path to the image file to load (should be spritesheet)
/// @param columns      Columns of the spritesheet
/// @param rows         Rows of the spritesheet
/// @param stride_mult  (optional) Multiplier to adjust stride (spacing)
/// @param layer        (optional) Rendering layer: 0-4. 0 is back, 4 is front.
TextSprite::TextSprite(const char* image_file, int columns, int rows, float stride_mult, int layer) :
                Sprite(nullptr, columns, rows, layer), 
                m_StrideMult(stride_mult,1.0f)
{
    // Init text shader if it ain't.
    if (!Renderer()->GetShader("text"))
        Renderer()->AddShader("text", new Shader("Data/shaders/text_instancing.vert", "Data/shaders/text_instancing.frag"));

    m_Mesh = new Mesh(true, rows, columns);    // TODO: obtain it from mesh library
    m_Texture = new Texture(image_file);
    m_Original= m_Texture;
    calcHeightMult();


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


/// @brief      Destructor: frees the instance buffer
TextSprite::~TextSprite()
{
    glDeleteBuffers(1, &m_InstBufferID);
    SetTexture();   // reset to original texture so base destructor can free it
}



/// @brief      Feed text into this function just like with std::cout. 
/// @return     Reference to string stream to feed text into.
std::ostringstream & TextSprite::Text() 
{
    m_Stream.str(std::string()); // clear current string
    
    return m_Stream;
}

/// @brief      Sets the width of a single row of tilemap (amount of columns)
void TextSprite::SetRowWidth(int columns) { m_RowWidth = columns; }


/// @brief      Draws currently stored text using parent's transform.
void TextSprite::Draw()
{
    glm::mat4 trm(1);                       // transform matrix - identity by default
    glm::vec2 stridex(m_StrideMult.x, 0);   // x stride vector - pointing right by default
    glm::vec2 stridey(0, -m_StrideMult.y);  // y stride vector - down
    glm::vec2 uvsize = m_Mesh->GetUVsize(); // UV size (for the frames of spritesheet)

    // Calculate matrix and stride based on parent't transform
    if (GetParent() && GetParent()->GetComponent<Transform>())
    {
        Transform* tr = GetParent()->GetComponent<Transform>();
        glm::mat4 proj;
#if 0
        if (tr->getIsDiegetic())
            proj = Camera()->GetMat_WorldToClip();
        else
#endif
            proj = Camera()->GetMat_UItoClip();

        // get transform matrix, and its linear part (for stride)
        trm = *tr->GetMatrix();

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
    glUniform1i(sh_txt->GetUniformID("columns"), m_Columns);
    glUniform1i(sh_txt->GetUniformID("rowwidth"), m_RowWidth);
    glUniform4fv(sh_txt->GetUniformID("tint"), 1, &m_Color[0]);

    // Load the string into buffer. sigh... chars have to be converted to floats. 
    // (should be ints, but I can't get it to read ints from buffer correctly, 
    // so I'll just cast them on shader)
    std::string str = m_Stream.str();
    int count = (int)str.size();
    if (count)
    {
        std::vector<float> chars(count);
        for (int i = 0; i < count; i++)
            chars[i] = (float)str[i];

        glBindBuffer(GL_ARRAY_BUFFER, m_InstBufferID);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * count, &chars[0], GL_STREAM_DRAW);
    }


    // Bind the texture and render instanced mesh
    m_Texture->Bind();
    glBindVertexArray(m_Mesh->GetVAO());
    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, m_Mesh->GetVertCount(), count);
    glBindVertexArray(0);
}
