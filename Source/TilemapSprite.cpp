/// @file     TilemapSprite.cpp
/// @author   Eli Tsereteli (ilya.tsereteli@digipen.edu)
/// 
/// @brief    A version of Sprite specifically for rendering tiles.
#include "TilemapSprite.h"
#include "RenderSystem.h"
#include "Tilemap.h"        // callback for when tilemap array updates
#include "CameraSystem.h"   // projection matrices
#include "Entity.h"         // parent
#include "Transform.h"
#include "Texture.h"
#include "glew.h"
#include <functional>       // callback binding

/// @brief              Default constructor
TilemapSprite::TilemapSprite() : 
    Sprite( typeid(TilemapSprite) )
{}


/// @brief              Textured constructor
/// 
/// @param texture      Texture (spritesheet) to use for tiling
/// @param stride_mult  (optional) Multiplier to adjust stride (spacing)
/// @param layer        (optional) Rendering layer: 0-4. 0 is back, 4 is front.
/// @param type         (for derived) Component type
TilemapSprite::TilemapSprite(Texture* texture, float stride_mult, int layer, 
                             std::type_index type) :
    Sprite(texture, layer, type)
{}


/// @brief              Loads the tile array from a raw char array.
/// @param tiles        tile IDs  (spritesheet frames)
/// @param size         array size
void TilemapSprite::LoadTileArray(const char* tiles, int size)
{
    m_TileCount = size;

    // Until I figure out what's wrong with reading ints from buffer, convert to floats.
    std::vector<float> chars(m_TileCount);
    for (int i = 0; i < m_TileCount; i++)
        chars[i] = (float)tiles[i];

    // Load the array into buffer.
    glBindBuffer(GL_ARRAY_BUFFER, m_InstBufferID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m_TileCount, &chars[0], GL_DYNAMIC_DRAW);
}


/// @brief              Loads the tile array from a vector of ints.
/// @param tiles        tile IDs  (spritesheet frames)
void TilemapSprite::LoadTileArray(std::vector<int> tiles)
{
    m_TileCount = (int)tiles.size();

    // Until I figure out what's wrong with reading ints from buffer, convert to floats.
    std::vector<float> chars(m_TileCount);         // (this one will load directly)
    for (int i = 0; i < m_TileCount; i++)
        chars[i] = (float)tiles[i];

    // Load the array into buffer.
    glBindBuffer(GL_ARRAY_BUFFER, m_InstBufferID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m_TileCount, &chars[0], GL_DYNAMIC_DRAW);
}




//-----------------------------------------------------------------------------
//          Virtual overrides
//-----------------------------------------------------------------------------

/// @brief  called when entering the scene
void TilemapSprite::OnInit()
{
    Sprite::OnInit();
    initInstancingStuff();

    // Init text shader if it ain't.
    if (!Renderer()->GetShader("tile"))
    {
        Renderer()->AddShader("tile", new Shader("Data/shaders/tile_instancing.vert", 
                                                 "Data/shaders/tile_instancing.frag"));
    }

    // Set up callback for when Tilemap array changes
    if (GetParent())
    {
        Tilemap<int>* tm = GetParent()->GetComponent< Tilemap<int> >();
        if (tm)
            tm->AddOnTilemapChangedCallback(this, std::bind(&TilemapSprite::onTilemapChanged, this));
    }
}


/// @brief  called when exiting the scene
void TilemapSprite::OnExit()
{
    Sprite::OnExit();

    glDeleteBuffers( 1, &m_InstBufferID );
    glDeleteVertexArrays(1, &m_VAO);

    if (GetParent())            // TODO: cache instead
    {
        Tilemap<int>* tm = GetParent()->GetComponent< Tilemap<int> >();
        if (tm)
            tm->RemoveOnTilemapChangedCallback(this);
    }
}


/// @brief          Draws tilemap using currently loaded array.
///                 Note: currently, it draws with or without transform. Should I
///                       just NOT allow transform-less rendering?...
void TilemapSprite::Draw()
{
    Mesh const* mesh = m_Texture->GetMesh();
    Entity* parent = GetParent();
    glm::mat4 trm(1);                       // transform matrix - identity by default
    glm::vec2 uvsize = mesh->GetUVsize();   // UV size (for the frames of spritesheet)

    // Get data from Tilemap component, if it has been updated.
    if (m_TilemapChanged)
    {
        Tilemap<int>* tm = parent->GetComponent< Tilemap<int> >();  // TODO: cache

        LoadTileArray(tm->GetTilemap());
        m_StrideMult = tm->GetTileScale();
        m_RowWidth = tm->GetTilemapWidth();
        m_TileCount = (int)tm->GetTilemap().size();

        m_TilemapChanged = false;
    }


    // Calculate matrix and stride based on parent't transform

    glm::vec2 stridex = {m_StrideMult.x,0};  // right vector (x stride)
    glm::vec2 stridey = {0,-m_StrideMult.y}; // down vector (y stride)

    if (parent && parent->GetComponent<Transform>())
    {
        Transform* tr = parent->GetComponent<Transform>();
        trm = tr->GetMatrix();

        // world or camera projection
        glm::mat4 proj;
        if (tr->GetIsDiegetic())
            proj = Camera()->GetMat_WorldToClip();
        else
            proj = Camera()->GetMat_UItoClip();


        // calculate stride vectors using linear transform (projected)
        glm::mat2 trm_linear = glm::mat2(proj) * glm::mat2(trm);
        stridex = trm_linear * stridex;
        stridey = trm_linear * stridey;

        // apply full projection to full transform, for the actual mesh position
        trm = proj * trm;
    }

    // Select the shader, and send all the uniform data to it
    Shader* sh_txt = Renderer()->SetActiveShader("tile");
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
    glBindVertexArray(m_VAO);
    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, mesh->GetVertexCount(), m_TileCount);
    glBindVertexArray(0);
}




//-----------------------------------------------------------------------------
//              Helpers
//-----------------------------------------------------------------------------

/// @brief      Initializes instancing buffer and its VAO
void TilemapSprite::initInstancingStuff()
{
    // This sprite needs its own VAO that references both the shared mesh buffer, 
    // and the individual instance buffer.
    // (yeah it's a bit messy, but more efficient this way)
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    // First, make sure it's using same mesh attributes as regular mesh VAO
    // (position and UV), which refer to the shared buffer
    glBindBuffer(GL_ARRAY_BUFFER, m_Texture->GetMesh()->GetBuffer());
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Mesh::Vertex), 0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Mesh::Vertex), (void*)offsetof(Mesh::Vertex, UV));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    // For instancing, I'll need an individual instance buffer...
    glGenBuffers(1, &m_InstBufferID);
    glBindBuffer(GL_ARRAY_BUFFER, m_InstBufferID);

    // ...and an extra attribute that refers to this buffer in particular.
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 0, 0);  // 1 float, no offset
    glVertexAttribDivisor(2, 1);                            // 1 per instance
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}


