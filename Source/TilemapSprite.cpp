#include "TilemapSprite.h"
#include "RenderSystem.h"
#include "CameraSystem.h"
#include "Entity.h"
#include "Transform.h"
#include "Texture.h"
#include "glew.h"

/// @brief              Default constructor
TilemapSprite::TilemapSprite() : Sprite( typeid(TilemapSprite) )
{}


/// @brief              Inherited constructor
/// @param type         typeid(DerivedClass)
TilemapSprite::TilemapSprite(std::type_index type) : Sprite(type) {}


/// @brief              Textured constructor
/// 
/// @param texture      Texture (spritesheet) to use for tiling
/// @param stride_mult  (optional) Multiplier to adjust stride (spacing)
/// @param layer        (optional) Rendering layer: 0-4. 0 is back, 4 is front.
/// @param type         (for derived) Component type
TilemapSprite::TilemapSprite(Texture* texture, float stride_mult, int layer, 
                             std::type_index type) :
    Sprite(texture, layer, type),
    m_StrideMult(stride_mult)
{}


//-----------------------------------------------------------------------------
// private: virtual override methods
//-----------------------------------------------------------------------------

/// @brief  called when entering the scene
void TilemapSprite::OnInit()
{
    Sprite::OnInit();

    // Init text shader if it ain't.
    if (!Renderer()->GetShader("text"))
    {
        Renderer()->AddShader("text", new Shader("Data/shaders/text_instancing.vert", 
                                                 "Data/shaders/text_instancing.frag"));
    }

    // For instancing, I'll need an extra buffer, and an additional attribute
    // for current mesh
    glGenBuffers(1, &m_InstBufferID);
    glBindBuffer(GL_ARRAY_BUFFER, m_InstBufferID);

    glBindVertexArray(m_Texture->GetMesh()->GetVAO());
    glEnableVertexAttribArray(2);

    // First 2 attribute indices are already used for vertex data.
    // This third index will be used for instance data.
    // index 2: 1 float, auto stride, offset 0
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 0, 0);
    glVertexAttribDivisor(2, 1);

    glBindVertexArray(0);

}

/// @brief  called when exiting the scene
void TilemapSprite::OnExit()
{
    Sprite::OnExit();

    glDeleteBuffers( 1, &m_InstBufferID );
}



/// @brief          Draws tilemap using currently loaded array.
void TilemapSprite::Draw()
{
    Mesh const* mesh = m_Texture->GetMesh();
    glm::mat4 trm(1);                       // transform matrix - identity by default
    glm::vec2 stridex(m_StrideMult.x, 0);   // x stride vector - pointing right by default
    glm::vec2 stridey(0, -m_StrideMult.y);  // y stride vector - down
    glm::vec2 uvsize = mesh->GetUVsize();   // UV size (for the frames of spritesheet)

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

    // Bind the texture and render instanced mesh
    m_Texture->Bind();
    glBindVertexArray(mesh->GetVAO());
    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, mesh->GetVertexCount(), m_TileCount);
    glBindVertexArray(0);
}


/// @brief              Loads the tile array from a raw char array.
///                     TODO: overloads for string / int vector? if needed
/// @param char_array   tile IDs  (spritesheet frames)
/// @param size         array size
void TilemapSprite::LoadTileArray(const char* char_array, int size)
{
    // Load the string into buffer. sigh... chars have to be converted to floats. 
    // (should be ints, but I can't get it to read ints from buffer correctly, 
    // so I'll just cast them on shader)
    std::vector<float> chars(size);
    for (int i = 0; i < size; i++)
        chars[i] = (float)char_array[i];

    glBindBuffer(GL_ARRAY_BUFFER, m_InstBufferID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * size, &chars[0], GL_DYNAMIC_DRAW);

    m_TileCount = size;
}



/// @brief            Read in the stride multiplier
/// @param  stream    The json to read from.
void TilemapSprite::readStrideMultiplier(Stream stream)
{
    m_StrideMult = stream.Read<glm::vec2>();
}

/// @brief            Read in the amount of tiles per row
/// @param  stream    The json to read from.
void TilemapSprite::readRowWidth(Stream stream)
{
    m_RowWidth = stream.Read<int>();
}

/// @brief the map of read methods for this Component
ReadMethodMap< TilemapSprite > const TilemapSprite::s_ReadMethods = {
    { "Texture"         , &readTexture          },
    { "Layer"           , &readLayer            },
    { "Color"           , &readColor            },
    { "StrideMultiplier", &readStrideMultiplier },
    { "RowWidth"        , &readRowWidth         },
    { "Opacity"         , &readOpacity          }
};

/// @brief gets the map of read methods for this Component
/// @return the map of read methods for this Component
ReadMethodMap< Component > const& TilemapSprite::GetReadMethods() const
{
    return (ReadMethodMap< Component> const&)s_ReadMethods;
}
