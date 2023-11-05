/*****************************************************************//**
 * \file   ParticleSystem.cpp
 * \brief  Updates all particles using compute shader, spawns new ones
 *         in accordance with emitters' timing.
 * 
 * \author Eli Tsereteli
 * \date   October 2023
 *********************************************************************/
#include "glew.h"
#include "ParticleSystem.h"
#include "RenderSystem.h"
#include "CameraSystem.h"
#include "Emitter.h"

// for debugging
#include "InputSystem.h"
#include "glfw3.h"
#include <iostream>


//-----------------------------------------------------------------------------
//              Virtual overrides
//-----------------------------------------------------------------------------

/// @brief  Called when system starts: initialize compute shader, buffers, uniforms
void ParticleSystem::OnInit()
{
    // shader
    m_CShader = new Shader("Data/shaders/particles_compute.glsl");
    Renderer()->AddShader("pCompute", m_CShader);

    // uniform block for init data (binding 2, first 2 are taken by emitters' buffers)
    glGenBuffers(1, &m_UBO);
    glBindBuffer(GL_UNIFORM_BUFFER, m_UBO);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(EmitData), NULL, GL_STREAM_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, m_UBO);

    // cache the uniform locations
    m_Ut = m_CShader->GetUniformID("t");
    m_Udt = m_CShader->GetUniformID("dt");
    m_Uproj = m_CShader->GetUniformID("proj");
    m_Urange = m_CShader->GetUniformID("range");
    m_UinitIndex = m_CShader->GetUniformID("initIndex");
    m_Uoldest = m_CShader->GetUniformID("oldest");
}


/// @brief  Compute shader updates all particles every frame. Since RenderSystem 
///         uses normal update, this should also happen here to ensure 
///         consistent buffer sync.
void ParticleSystem::OnUpdate(float dt)
{
    if (!Cameras()->GetActiveCamera())
        return;

    static float time = 0.0f;
    time += dt;

    m_CShader->use();

    // set common uniforms : dt, time, projection matrix
    glUniform1f(m_Udt, dt);
    glUniform1f(m_Ut, time);
    glUniformMatrix4fv(m_Uproj, 1, 0, &Cameras()->GetMat_WorldToClip()[0][0]);

    // Put all of the emitters' init data together and send it to GPU.
    // If each emitter was to load its own data instead, executions would not be
    // parallelized.    (bindings and basic uniforms are ok)
    if (m_InitDataDirty)
    {
        m_InitDataDirty = false;
        
        std::vector<EmitData> inits;
        for (auto& emitter : m_Emitters)
            inits.push_back(emitter.second->GetEmitData());

        glBindBuffer(GL_UNIFORM_BUFFER, m_UBO);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(EmitData) * (int)inits.size(), 
                     &inits[0], GL_DYNAMIC_DRAW);
    }
    
    // let each emitter execute compute shader for its buffers
    int index = 0;
    for (auto& emitter : m_Emitters)
    {
        glUniform1i(m_UinitIndex, index++);
        emitter.second->Update(dt, m_Urange, m_Uoldest);
    }

    // This makes computations of particles little slower compared to doing it
    // all in one go with single buffer. But, it's more manageable this way.
    // The other version was too OP anyway.
}


/// @brief  Called when system exits
void ParticleSystem::OnExit()
{
    glDeleteBuffers(1, &m_UBO);
}



void ParticleSystem::AddEmitter(Emitter* emitter)
{
    m_Emitters[emitter->GetId()] = emitter;
    SetEmitDataDirty();
}


void ParticleSystem::RemoveEmitter(Emitter* emitter)
{
    m_Emitters.erase(emitter->GetId());
    SetEmitDataDirty();
}



//-----------------------------------------------------------------------------
//              Singleton stuff
//-----------------------------------------------------------------------------

/// @brief    Private constructor
ParticleSystem::ParticleSystem() :
    System( "ParticleSystem" )
{}

/// @brief    The singleton instance of ParticleSystem
ParticleSystem * ParticleSystem::s_Instance = nullptr;

/// @return   The singleton instance
ParticleSystem * ParticleSystem::GetInstance()
{
    if ( s_Instance == nullptr )
    {
        s_Instance = new ParticleSystem();
    }
    return s_Instance;
}



