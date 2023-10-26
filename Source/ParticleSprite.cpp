/*****************************************************************//**
 * \file   ParticleSprite.cpp
 * \brief  Renders particles using data from SSBO.
 * 
 * \author Eli Tsereteli
 * \date   October 2023
 *********************************************************************/
#include "glew.h"
#include "ParticleSprite.h"
#include "ParticleSystem.h" // SSBO
#include "RenderSystem.h"   // shader
#include "Texture.h"


/// @brief              Default constructor
ParticleSprite::ParticleSprite() : 
    Sprite( typeid(ParticleSprite) )
{}

// for testing
ParticleSprite::ParticleSprite(Texture const* texture, int first, int afterLast, int layer) : 
    Sprite(texture, layer,  typeid(ParticleSprite) ), m_Range{first, afterLast}
{
    OnInit();
}



//-----------------------------------------------------------------------------
//          Virtual overrides
//-----------------------------------------------------------------------------

/// @brief  called when entering the scene
void ParticleSprite::OnInit()
{
    Sprite::OnInit();
    initInstancingStuff();

    m_Range = {0, Particles()->GetMaxParticleCount()};  // TODO: deserialize

    if (!Renderer()->GetShader("particles"))
        Renderer()->AddShader("particles", new Shader("Data/Shaders/particles.vert",
                                                        "Data/Shaders/texture.frag"));
}


void ParticleSprite::OnExit()
{
    Sprite::OnExit();
    if (m_VAO)
        glDeleteVertexArrays(1, &m_VAO);
    m_VAO = 0;
}

// for testing
ParticleSprite::~ParticleSprite()
{
    if (m_VAO)
        glDeleteVertexArrays(1, &m_VAO);
    m_VAO = 0;
}



/// @brief          Draws tilemap using currently loaded array.
///                 Note: currently, it draws with or without transform. Should I
///                       just NOT allow transform-less rendering?...
void ParticleSprite::Draw()
{
    Shader* sh = Renderer()->GetShader("particles");
    sh->use();

    glUniform1f(sh->GetUniformID("opacity"), m_Opacity);

    // Bind the texture and render instanced mesh using ParticleSystem's VAO
    glBindVertexArray(m_VAO);
    m_Texture->Bind();
    const Mesh* mesh = m_Texture->GetMesh();
    glDrawArraysInstanced(GL_TRIANGLE_STRIP, m_Range.x, mesh->GetVertexCount(), 
                                                          m_Range.y - m_Range.x);
    glBindVertexArray(0);
}



//-----------------------------------------------------------------------------
//              Helpers
//-----------------------------------------------------------------------------

/// @brief      Initializes VAO for using SSBO as a VBO  (what have I become...)
void ParticleSprite::initInstancingStuff()
{
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    // Set the usual attributes linked to mesh buffer:
    // Vertex position    (attribute 0, 2 floats)
    // Vertex UV          (attribute 1, 2 floats)
    glBindBuffer(GL_ARRAY_BUFFER, m_Texture->GetMesh()->GetBuffer());
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Mesh::Vertex), 0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Mesh::Vertex), (void*)offsetof(Mesh::Vertex, UV));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    // Now, link the buffer of matrices calculated by compute shader
    // Transform matrix   (occupies 4 attribute spaces, vec4 each)
    glBindBuffer(GL_ARRAY_BUFFER, ParticleSystem::GetInstance()->GetMatSSBO());
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), 0);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*) sizeof(glm::vec4));
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)*2));
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)*3));
    glVertexAttribDivisor(2, 1);
    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);
    glVertexAttribDivisor(5, 1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    glEnableVertexAttribArray(4);
    glEnableVertexAttribArray(5);

    // It's still declared as regular mat4 on the shader. Go figure.

    glBindVertexArray(0);
}
