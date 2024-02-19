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
#include "Emitter.h"
#include "RenderSystem.h"   // shader
#include "CameraSystem.h"   // projection matrix


//-----------------------------------------------------------------------------
//              Virtual overrides
//-----------------------------------------------------------------------------

/// @brief  Called when system starts: initialize compute shader, buffers, uniforms
void ParticleSystem::OnInit()
{
    // shader
    Shader* CShader = new Shader("Data/shaders/particles_compute.glsl");
    Renderer()->AddShader("pCompute", CShader);

    // uniform block for init data (binding 2, first 2 are taken by emitters' buffers)
    glGenBuffers(1, &m_UBO);
    glBindBuffer(GL_UNIFORM_BUFFER, m_UBO);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(EmitData), NULL, GL_STREAM_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, m_UBO);

    // cache the uniform locations
    m_Ut = CShader->GetUniformID("t");
    m_Udt = CShader->GetUniformID("dt");
    m_Uproj = CShader->GetUniformID("proj");
    m_UinitIndex = CShader->GetUniformID("initIndex");
}


/// @brief  Compute shader updates all particles every frame. Since RenderSystem 
///         uses normal update, this should also happen here to ensure 
///         consistent buffer sync.
void ParticleSystem::OnUpdate(float dt)
{
    if (m_Emitters.empty() || !Cameras()->GetActiveCamera())
        return;

    Renderer()->SetActiveShader("pCompute");

    // Put all of the emitters' init data together and send it to GPU.
    // If each emitter was to load its own data instead, executions would not be
    // parallelized / queued.    (bindings and basic uniforms are ok)
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

    static float time = 0.0f;  // for the shader-side PRNG
    if (dt > 0.2f)
        dt = 0.016f;
    
    m_FastForward += dt;
    while (m_FastForward >= 0.0f)
    {
        m_FastForward -= dt;

        // set common uniforms : dt, time, projection matrix
        time += dt;
        glUniform1f(m_Udt, dt);
        glUniform1f(m_Ut, time);
        glUniformMatrix4fv(m_Uproj, 1, 0, &Cameras()->GetMat_WorldToClip()[0][0]);
    
        // let each emitter execute compute shader for its buffers
        int index = 0;
        for (auto& emitter : m_Emitters)
        {
            glUniform1i(m_UinitIndex, index++);
            emitter.second->Update(dt);
        }
        // This makes computations of particles little slower compared to doing it
        // all in one go with single buffer. But, it's more manageable this way.
        // The other version was too OP anyway.
    }
}


/// @brief  Called when system exits
void ParticleSystem::OnExit()
{
    glDeleteBuffers(1, &m_UBO);
}


/// @brief  Called when entering new scene: fast-forward 300 frames
void ParticleSystem::OnSceneInit()
{
    m_FastForward = 5.0f;
}


//-----------------------------------------------------------------------------
//              Public methods
//-----------------------------------------------------------------------------

/// @brief          Adds new emitter to the system
/// @param emitter  Pointer to new emitter
void ParticleSystem::AddEmitter(Emitter* emitter)
{
    m_Emitters[emitter->GetId()] = emitter;
    SetEmitDataDirty();
}


/// @brief          Removes emitter from system
/// @param emitter  Pointer to emitter
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


