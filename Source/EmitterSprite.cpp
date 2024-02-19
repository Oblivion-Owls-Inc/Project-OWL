/*****************************************************************//**
 * \file   EmitterSprite.cpp
 * \brief  Renders particles using data from SSBO.
 * 
 * \author Eli Tsereteli
 * \date   October 2023
 *********************************************************************/
#include "glew.h"
#include "EmitterSprite.h"
#include "Texture.h"
#include "Emitter.h"
#include "ParticleSystem.h" // SSBO
#include "RenderSystem.h"   // shader
#include "Entity.h"         // parent


/// @brief          Default constructor
EmitterSprite::EmitterSprite() : 
    Sprite( typeid(EmitterSprite) )
{}

/// @brief          Copy constructor
/// @param other    Component to copy
EmitterSprite::EmitterSprite(const EmitterSprite& other) :
    Sprite ( other )
{}

/// @return  Copy of this component
EmitterSprite* EmitterSprite::Clone() const { return new EmitterSprite(*this); }

/// @brief      Destructor: calls OnExit if needed
EmitterSprite::~EmitterSprite()
{
    if (m_VAO)
        OnExit();
}



//-----------------------------------------------------------------------------
//          Virtual overrides
//-----------------------------------------------------------------------------

/// @brief   Inits VAO and shader when entering scene
void EmitterSprite::OnInit()
{
    Sprite::OnInit();
    initInstancingStuff();
}


/// @brief   Deletes VAO when exiting
void EmitterSprite::OnExit()
{
    Sprite::OnExit();
    glDeleteVertexArrays(1, &m_VAO);
    m_VAO = 0;
}


/// @brief     Draws particles using gpu instancing.
void EmitterSprite::Draw()
{
    if (!m_IsTextured || !m_VAO)
    {
        if (!m_VAO)
            initInstancingStuff();

        return;
    }

    Shader* sh = Renderer()->SetActiveShader("particles");
    glUniform1f(sh->GetUniformID("opacity"), m_Opacity);
    glUniform4fv(sh->GetUniformID("tint"), 1, &m_Color[0]);
    glm::vec2 uv_offset = calcUVoffset();
    glUniform2f(sh->GetUniformID("UV_offset"), uv_offset.x, uv_offset.y);

    // Bind the texture and render instanced mesh using ParticleSystem's VAO
    glBindVertexArray(m_VAO);
    m_Texture->Bind();
    const Mesh* mesh = m_Texture->GetMesh();
    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, mesh->GetVertexCount(), 
                                                m_Emitter->GetBufferSize());
    glBindVertexArray(0);
}



//-----------------------------------------------------------------------------
//              Helpers
//-----------------------------------------------------------------------------

/// @brief      Initializes VAO for using SSBO as a VBO  (what have I become...)
void EmitterSprite::initInstancingStuff()
{
    if (m_VAO || !m_Texture)
        return;

    m_Emitter = GetEntity()->GetComponent<Emitter>();
    if (!m_Emitter)
        return;

    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    // Attributes linked to mesh buffer (per vertex):
    // Vertex position    (attribute 0, 2 floats)
    // Vertex UV          (attribute 1, 2 floats)
    glBindBuffer(GL_ARRAY_BUFFER, m_Texture->GetMesh()->GetBuffer());
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Mesh::Vertex), 0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Mesh::Vertex), (void*)offsetof(Mesh::Vertex, UV));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    // Attributes linked to SSBO (per instance):
    // Mesh opacity       (attribute 2, 1 float)
    glBindBuffer(GL_ARRAY_BUFFER, m_Emitter->GetOpacitySSBO());
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(float), 0);
    glVertexAttribDivisor(2, 1);
    glEnableVertexAttribArray(2);
    // Transform matrix   (attribute 3, occupies 4 attribute spaces - vec4 each)
    glBindBuffer(GL_ARRAY_BUFFER, m_Emitter->GetMatSSBO());
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), 0);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*) sizeof(glm::vec4));
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)*2));
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)*3));
    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);
    glVertexAttribDivisor(5, 1);
    glVertexAttribDivisor(6, 1);
    glEnableVertexAttribArray(3);
    glEnableVertexAttribArray(4);
    glEnableVertexAttribArray(5);
    glEnableVertexAttribArray(6);

    // It's still declared as regular mat4 on the shader. Go figure.

    glBindVertexArray(0);

    if (!Renderer()->GetShader("particles"))
        Renderer()->AddShader("particles", new Shader("Data/Shaders/particles.vert",
                              "Data/Shaders/particles.frag"));
}
