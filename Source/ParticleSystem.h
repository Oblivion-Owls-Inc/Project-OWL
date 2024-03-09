/*****************************************************************//**
 * \file   ParticleSystem.h
 * \brief  System in charge of managing compute shader and emitters.
 * 
 * \author Eli Tsereteli
 * \date   October 2023
 *********************************************************************/
#pragma once
#include "pch.h" // precompiled header has to be included first
#include "System.h"


// fwd reference
class Emitter;

/// @brief  System in charge of managing compute shader and emitters.
class ParticleSystem : public System
{
public:
    /// @brief  Not really utilized CPU-side, but useful for reference. Mirrors the
    ///         Particle struct declared on the compute shader.
    struct Particle
    {
        glm::vec2 pos, vel, acc, vec2padding;
        float size, rotation,
              dirAcc, drag,   // dirAcc - magnitude of accelerating in the initial direction
              lifetime, time,
              fadeIn, fadeOut,
              sizePerSec, align;
    };

    /// @brief  Mirrors the struct on the shader. Used for spawning new particles.
    ///         This one is for internal use, allows proper UBO allocation/loading.
    struct EmitData
    {
        glm::vec2 offset, pos_spread;
        glm::vec2 acceleration; float direction, speed;
        float size, rotation, dir_spread, speed_spread;
        float size_spread, rotation_spread, fadeInDuration, fadeOutDuration;
        float lifetime, startAhead, dirAcc, sizePerSec;
        int bufferSize, align1;
    };



//-----------------------------------------------------------------------------
//              Public methods
//-----------------------------------------------------------------------------
public:

    /// @return  Thread size of each work group. 
    __inline int GetWorkGroupSize() const { return m_WGsize; }

    /// @brief          Adds new emitter to the system
    /// @param emitter  Pointer to new emitter
    void AddEmitter(Emitter* emitter);

    /// @brief          Removes emitter from system
    /// @param emitter  Pointer to emitter
    void RemoveEmitter(Emitter* emitter);

    /// @brief    Sets emit data dirty flag (uniform block array will be re-loaded)
    void SetEmitDataDirty() { m_InitDataDirty = true; }


//-----------------------------------------------------------------------------
//              Virtual overrides
//-----------------------------------------------------------------------------
private:

    /// @brief  Called when system starts: inits shader/buffers
    virtual void OnInit() override;

    /// @brief  Called every frame: manages particle spawning
    virtual void OnUpdate(float dt) override;

    /// @brief  Called when system exits
    virtual void OnExit() override;

    /// @brief  Called when scene changes - reinits fast-forward
    virtual void OnSceneInit() override;



//-----------------------------------------------------------------------------
//              Data
//-----------------------------------------------------------------------------
private:

    unsigned int m_UBO = 0;         /// @brief  ID of buffer used by uniform block
    bool m_InitDataDirty = true;    /// @brief  When true, re-load init data buffer
    float m_FastForward = 5.0f;     /// @brief  Fast forward 5 sec at start

    /// @brief  All the emitters.
    std::map<int, Emitter*> m_Emitters;

    /// @brief  Uniform locations
    unsigned int m_Udt = -1, 
                 m_Ut = -1,
                 m_Uproj = -1,
                 m_UinitIndex = -1;

    /// @brief  Size of each work group. This is also minimum amount of particles
    ///         per emitter. Make sure it matches what's in the shader.
    const int m_WGsize = 128;



//-----------------------------------------------------------------------------
//              Singleton stuff
//-----------------------------------------------------------------------------
private:

    /// @brief  Private constructor
    ParticleSystem();

public:

    /// @return   The singleton instance
    static ParticleSystem * GetInstance();

    // Prevent copying
    ParticleSystem(ParticleSystem& other) = delete;
    void operator=(const ParticleSystem&) = delete;
};

/// @brief      Convenient function for getting ParticleSystem instance.
__inline ParticleSystem* Particles() { return ParticleSystem::GetInstance(); }
