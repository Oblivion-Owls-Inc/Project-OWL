/*****************************************************************//**
 * \file   Emitter.cpp
 * \brief  Controls timing of particle emissions, and their init data.
 * 
 * \author Eli
 * \date   October 2023
 *********************************************************************/
#include "Emitter.h"
#include "imgui.h"          // inspector
#include "RenderSystem.h"   // shader uniform locations
#include "Entity.h"         // parent transform
#include "Transform.h"
#include "ComponentReference.t.h"


/// @brief	    Defualt constructor
Emitter::Emitter() : Component( typeid( Emitter ) )
{
    // default (when added thru editor)
    m_PPS = 200.0f;
    m_Init.lifetime = 2.0f;
    m_Init.size = 0.2f;
    m_Init.dir_spread = 3.14f;
    m_Init.speed = 1.0f;
    m_Init.speed_spread = 0.5f;
}


/// @brief        Copy constructor
/// @param other  Component to copy
Emitter::Emitter(const Emitter& other) : Component(other),
    m_Init(other.m_Init),
    m_PPS(other.m_PPS),
    m_Continuous(other.m_Continuous),
    m_Delay(other.m_Delay),
    m_BufferSize(other.m_BufferSize)
{}


/// @return     A copy of this emitter
Component* Emitter::Clone() const { return new Emitter(*this); }


/// @brief    Destructor: calls OnExit if needed
Emitter::~Emitter()
{
    if (m_DataSSBO)
        OnExit();
}




//-----------------------------------------------------------------------------
//              Overrides
//-----------------------------------------------------------------------------

/// @brief  Initialization: add this emitter to particle system, init buffers
void Emitter::OnInit()
{
    Particles()->AddEmitter(this);

    // create/init buffers
    glGenBuffers(1, &m_MatSSBO);
    glGenBuffers(1, &m_DataSSBO);
    glGenBuffers(1, &m_OpacitySSBO);
    resizeBuffers();

    // uniform locations
    Shader* cs = Renderer()->GetShader("pCompute");
    m_Urange = cs->GetUniformID("range");
    m_Uoldest = cs->GetUniformID("oldest");
    m_UparentPos = cs->GetUniformID("parentPos");

    // parent transform
    m_Transform.Init(GetEntity());
}


/// @brief  Exit: emitter removes itself from behavior system; free GPU memory
void Emitter::OnExit()
{
    Particles()->RemoveEmitter(this);
    glDeleteBuffers(1, &m_DataSSBO);
    glDeleteBuffers(1, &m_MatSSBO);
    glDeleteBuffers(1, &m_OpacitySSBO);

    m_DataSSBO = 0;  // for destructor
}




//-----------------------------------------------------------------------------
//              Public methods
//-----------------------------------------------------------------------------

/// @brief          Called by particle system after loading emit data. Accepts
///                 some locations of uniforms on compute shader, dispatches
///                 the compute shader to update its particles.
/// @param dt       Deltatime
void Emitter::Update(float dt)
{
    // bind individual buffers
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_DataSSBO);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, m_OpacitySSBO);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, m_MatSSBO);

    // zero-init the buffer (by setting range to whole size)
    if (m_Zinit)
    {
        m_Zinit = false;
        glUniform2i(m_Urange, 0, m_BufferSize);
        glDispatchCompute(m_WGcount, 1, 1);

        return;
    }

    // --------------- TIMING -------------- //
    
    if (m_Continuous)
    {
        m_ParticleCount += m_PPS * dt;

        // if there's delay, only emit when the timer reaches it
        if (m_Delay)
        {
            m_DelayTimer += dt;

            if (m_DelayTimer >= m_Delay)
                m_DelayTimer -= m_Delay;
            else
            {
                // delay time not reached: no emitting. Just update
                glDispatchCompute(m_WGcount, 1, 1);

                return;
            }
        }
    }

    // -------------- EMITTING ------------- //

    // emit the integer part, keep the fractional part
    int count = (int)m_ParticleCount;
    m_ParticleCount -= glm::floor(m_ParticleCount);

    glUniform1i(m_Uoldest, m_CurrentIndex);

    if (count && count <= m_BufferSize)
    {
        // initialize particles in the next available range.
        if (m_CurrentIndex + count > m_BufferSize)
            m_CurrentIndex = 0;

        glUniform2i(m_Urange, m_CurrentIndex, m_CurrentIndex+count);


        if (m_Transform)
            glUniform2fv(m_UparentPos, 1, &m_Transform->GetTranslation().x);
        else
            m_Transform = GetEntity()->GetComponent<Transform>();


        m_CurrentIndex += count;
    }
    else
        glUniform2i(m_Urange, -1, -1); // (nothing to emit this time)

    
    glDispatchCompute(m_WGcount, 1, 1);
}




// ----------------------------------------------------------------------------
//      ImGui
// ----------------------------------------------------------------------------

/// @brief  Imgui inspector
void Emitter::Inspector()
{
    bool initChanged = false;

    ImGui::Checkbox("Continuous", &m_Continuous);
    if (!m_Continuous)
    {
        static int amt = 5;

        ImGui::SliderInt("Amount", &amt, 1, m_BufferSize);

        if (ImGui::Button("Emit"))
            m_ParticleCount = (float)amt;
    }
    else
    {
        ImGui::Text("Particles per sec");
        ImGui::SliderFloat("###PPS", &m_PPS, 1.0f, 2000.0f);
        ImGui::SliderFloat("Delay", &m_Delay, 0.0f, 2.0f);
    }
    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Text("Lifetime");
    initChanged |= ImGui::DragFloat("sec ###lifetime", &m_Init.lifetime, 0.01f, 0.01f, 15.0f);
    ImGui::Spacing();
    ImGui::Text("Offset");
    initChanged |= ImGui::DragFloat2("###pos", &m_Init.offset.x, 0.01f);
    initChanged |= ImGui::DragFloat2("spread ###posspread", &m_Init.pos_spread.x,0.01f);
    ImGui::Spacing();
    ImGui::Text("Direction");
    initChanged |= ImGui::SliderFloat("###Direction", &m_Init.direction, 0.0f, 6.28f);
    initChanged |= ImGui::SliderFloat("spread ###dirspread", &m_Init.dir_spread, 0.0f, 3.14f);
    ImGui::Text("Speed");
    initChanged |= ImGui::SliderFloat("###Speed", &m_Init.speed, 0.0f, 10.0f);
    initChanged |= ImGui::SliderFloat("spread ###spdspread", &m_Init.speed_spread, 0.0f, m_Init.speed);
    ImGui::Text("Size");
    initChanged |= ImGui::InputFloat("###Size", &m_Init.size, 0.01f, 0.05f, "%.2f");
    initChanged |= ImGui::SliderFloat("spread ###szspread", &m_Init.size_spread, 0.0f, m_Init.size);
    initChanged |= ImGui::InputFloat("change/sec ###sizedt", &m_Init.sizePerSec, 0.01f, 0.05f, "%.2f");
    ImGui::Spacing();
    ImGui::Text("Start ahead");
    initChanged |= ImGui::DragFloat("###ahead", &m_Init.startAhead, 0.01f, 0.0f);
    ImGui::Text("Fade duration");
    initChanged |= ImGui::DragFloat("in ###fadein", &m_Init.fadeInDuration, 0.01f, 0.0f, m_Init.lifetime);
    initChanged |= ImGui::DragFloat("out ###fadeout", &m_Init.fadeOutDuration, 0.01f, 0.0f, m_Init.lifetime);
    ImGui::Spacing();
    ImGui::Text("Acceleration");
    initChanged |= ImGui::DragFloat2("###acc", &m_Init.acceleration.x, 0.1f);
    ImGui::Text("Initial direction accel factor");
    initChanged |= ImGui::DragFloat("###diracc", &m_Init.dirAcc, 0.1f);

    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Text("Maximum rendered particles");
    if (ImGui::DragInt("###maxp", &m_BufferSize, 128, 128, 32768))
    {
        resizeBuffers();
        initChanged = true;
    }
    ImGui::TextWrapped("Note: if it starts acting up when adjusting this number, "\
                        "just readjust again until it's ok. (This only happens " \
                        "during real-time adjustments)");

    if (initChanged)
        Particles()->SetEmitDataDirty();
}




//-----------------------------------------------------------------------------
//              Helpers
//-----------------------------------------------------------------------------

/// @brief   (re)allocates buffers based on m_BufferSize.
void Emitter::resizeBuffers()
{
    // ensure buffer size is multiple of 128
    m_BufferSize = int(std::ceil(m_BufferSize / 128.0f) * 128);
    m_Init.bufferSize = m_BufferSize;
    m_WGcount = m_BufferSize / Particles()->GetWorkGroupSize();

    // raw data buffer
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_DataSSBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER, (sizeof(ParticleSystem::Particle)) * 
                                            m_BufferSize, NULL, GL_STREAM_DRAW);

    // opacities buffer
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_OpacitySSBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(float) * 
                    m_BufferSize, NULL, GL_STREAM_DRAW);
    // allow it to be read by vertex shader

    // transform matrices buffer
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_MatSSBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(glm::mat4) * 
                    m_BufferSize, NULL, GL_STREAM_DRAW);
    // allow it to be read by vertex shader

    m_Zinit = true;
}




// ----------------------------------------------------------------------------
//      Reading / writing
// ----------------------------------------------------------------------------

/// @brief          Reads in the boolean for continuous (active) flag 
/// @param  data    JSON data to read
void Emitter::readContinuous(nlohmann::ordered_json const& data)
{
    m_Continuous = Stream::Read< bool >(data);
}

/// @brief          Reads in the value for particles per second
/// @param  data    JSON data to read
void Emitter::readPPS(nlohmann::ordered_json const& data)
{
    m_PPS = Stream::Read< float >(data);
}

/// @brief          Reads in the value for delay between emissions
/// @param  data    JSON data to read
void Emitter::readDelay(nlohmann::ordered_json const& data)
{
    m_Delay = Stream::Read< float >(data);
}

/// @brief          Reads in value for maximum rendered particles
/// @param  data    JSON data to read
void Emitter::readMax(nlohmann::ordered_json const& data)
{
    m_BufferSize = Stream::Read< int >(data);
    m_BufferSize = int(std::ceil(m_BufferSize / 128.0f) * 128);
}


/// @brief          Reads in the array that represents init data.
/// @param  data    JSON data to read
void Emitter::readData(nlohmann::ordered_json const& data)
{
    // ordered consistently with ParticleSystem::EmitData
    int i = 0;
    m_Init.offset = Stream::Read< 2,float >(data[i++]);
    m_Init.pos_spread = Stream::Read< 2,float >(data[i++]);
    m_Init.acceleration = Stream::Read< 2,float >(data[i++]);
    m_Init.direction = Stream::Read< float >(data[i++]);
    m_Init.speed = Stream::Read< float >(data[i++]);
    m_Init.size = Stream::Read< float >(data[i++]);
    m_Init.rotation = Stream::Read< float >(data[i++]);
    m_Init.dir_spread = Stream::Read< float >(data[i++]);
    m_Init.speed_spread = Stream::Read< float >(data[i++]);
    m_Init.size_spread = Stream::Read< float >(data[i++]);
    m_Init.rotation_spread = Stream::Read< float >(data[i++]);
    m_Init.fadeInDuration = Stream::Read< float >(data[i++]);
    m_Init.fadeOutDuration = Stream::Read< float >(data[i++]);
    m_Init.lifetime = Stream::Read< float >(data[i++]);
    m_Init.startAhead = Stream::Read< float >(data[i++]);
    m_Init.dirAcc = Stream::Read< float >(data[i++]);
    if (i < (int)data.size())
        m_Init.sizePerSec = Stream::Read< float >(data[i++]);
}


/// @brief  map of read methods
ReadMethodMap< Emitter > const Emitter::s_ReadMethods = {
    {"Continuous", &readContinuous  },
    {"PPS"       , &readPPS         },
    {"Delay"     , &readDelay       },
    {"Max"       , &readMax         },
    {"EmitData"  , &readData        }
};




/// @brief    Writes component data to a JSON file.
/// @return   JSON data to write
nlohmann::ordered_json Emitter::Write() const
{
    nlohmann::ordered_json data;

    data["PPS"] = m_PPS;
    data["Delay"] = m_Delay;
    data["Max"] = m_BufferSize;
    data["Continuous"] = m_Continuous;

    // ordered consistently with ParticleSystem::EmitData
    nlohmann::ordered_json& emitData = data[ "EmitData" ];
    emitData.push_back( Stream::Write( m_Init.offset ) );
    emitData.push_back( Stream::Write( m_Init.pos_spread ) );
    emitData.push_back( Stream::Write( m_Init.acceleration ) );
    emitData.push_back( Stream::Write( m_Init.direction ) );
    emitData.push_back( Stream::Write( m_Init.speed ) );
    emitData.push_back( Stream::Write( m_Init.size ) );
    emitData.push_back( Stream::Write( m_Init.rotation ) );
    emitData.push_back( Stream::Write( m_Init.dir_spread ) );
    emitData.push_back( Stream::Write( m_Init.speed_spread ) );
    emitData.push_back( Stream::Write( m_Init.size_spread ) );
    emitData.push_back( Stream::Write( m_Init.rotation_spread ) );
    emitData.push_back( Stream::Write( m_Init.fadeInDuration ) );
    emitData.push_back( Stream::Write( m_Init.fadeOutDuration ) );
    emitData.push_back( Stream::Write( m_Init.lifetime ) );
    emitData.push_back( Stream::Write( m_Init.startAhead ) );
    emitData.push_back( Stream::Write( m_Init.dirAcc ) );
    emitData.push_back( Stream::Write( m_Init.sizePerSec ) );

    return data;
}
