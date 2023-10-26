#include "glew.h"
#include "ParticleSystem.h"
#include "RenderSystem.h"
#include "CameraSystem.h"

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

    // raw data buffer (binding 0)
    glGenBuffers(1, &m_DataSSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_DataSSBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(Particle) * m_WGsize * m_WGcount, 
                                                              NULL, GL_STREAM_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_DataSSBO);


    // transform matrices buffer (binding 1)
    glGenBuffers(1, &m_MatSSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_MatSSBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(glm::mat4) * m_WGsize * m_WGcount, 
                                                               NULL, GL_STREAM_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, m_MatSSBO);
    // allow it to be read by vertex shader
    glMemoryBarrier(GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);


    // uniform block for init data (binding 2)
    glGenBuffers(1, &m_UBO);
    glBindBuffer(GL_UNIFORM_BUFFER, m_UBO);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(ShaderInitData)*m_MaxInitCount, NULL, GL_STREAM_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, m_UBO);


    // cache the uniform locations
    m_UinitCount = m_CShader->GetUniformID("initCount");
    m_Ut = m_CShader->GetUniformID("t");
    m_Udt = m_CShader->GetUniformID("dt");
    m_Uproj = m_CShader->GetUniformID("proj");
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


    float pi = 3.14f;
    if (Input()->GetKeyDown(GLFW_KEY_F))      // dir  spd   size   rot
        SpawnParticles({ 30, Input()->GetMousePosWorld(),  pi/2, 2.5f, 0.2f, 0.0f,    // center
                                                       {}, 2.0f, 2.5f, 0.0f, 1.0f});  // spread

    if (Input()->GetMouseTriggered(GLFW_MOUSE_BUTTON_2))
        ClearParticles();


    int size;
    if (m_Clear)
    {
        size = 1;
        m_Spawns.clear();
        m_Spawns.push_back({GetMaxParticleCount()}); // init everything to 0
        m_Clear = false;
    }
    else
    {
        // excess inits just get cut off
        size = (int) m_Spawns.size();
        if (size > m_MaxInitCount) 
            size = m_MaxInitCount;
    }

    // set uniforms : dt, time, projection matrix, init count
    m_CShader->use();
    glUniform1f(m_Udt, dt);
    glUniform1f(m_Ut, time);
    glUniformMatrix4fv(m_Uproj, 1, 0, &Cameras()->GetMat_WorldToClip()[0][0]);
    glUniform1i(m_UinitCount, size);


    // prep and load init data, if particles need to be spawned (or cleared)
    if (size)
    {
        // this preserves all the data except range. (range is garbage, set manually)
        ShaderInitData* init = reinterpret_cast<ShaderInitData*>(&m_Spawns[0]);

        // set range for each group.
        for (int i=0; i<size; ++i)
        {
            int amount = (int)m_Spawns[i].amount;

            // end index overflow: loop back to start
            if (m_NextIndex + amount > GetMaxParticleCount())
                m_NextIndex = 0;

            // ivec2 'range' overwrites long 'amount'
            int end = m_NextIndex + amount;
            init[i].range = {m_NextIndex, end};
            m_NextIndex = end;
        }

        // now that amounts have been replaced with ranges, load it all in.
        glBindBuffer(GL_UNIFORM_BUFFER, m_UBO);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(ShaderInitData)*size, init, GL_STREAM_DRAW);
    }


    // let compute shader do its thing
    glDispatchCompute(m_WGcount, 1, 1);
    m_Spawns.clear();
}


/// @brief  Called when system exits
void ParticleSystem::OnExit()
{
    glDeleteBuffers(1, &m_DataSSBO);
    glDeleteBuffers(1, &m_MatSSBO);
    glDeleteBuffers(1, &m_UBO);
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
