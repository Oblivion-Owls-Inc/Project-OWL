/*****************************************************************//**
 * \file   ParticleSystem.h
 * \brief  Updates all particles using compute shader, spawns new ones
 *         in accordance with emitters' timing.
 * 
 * \author Eli Tsereteli
 * \date   October 2023
 *********************************************************************/
#pragma once
#include "System.h"
#include <vector>

class Shader; // fwd ref


/// @brief Example System meant to be copy-pasted when creating new Systems
using namespace std;
class ParticleSystem : public System
{
public:
    /// @brief  Not really utilized CPU-side, but useful for reference. Mirrors the
    ///         Particle struct declared on the compute shader.
    struct Particle
    {
        glm::vec2 pos; float size,    rotation;
        glm::vec2 vel; float size_d,  rotation_d;   
        glm::vec2 acc; float drag;
        float opacity, fade_startTime, fade_rate;   
    };

    /// @brief  Data to initialize particles with.
    struct InitData
    {                           // long - to match the padding of the shader-side 
        long long int amount;   // struct (which has ivec2). makes copying easier.
        glm::vec2 position;
        float direction,  speed,        size,         rotation;
        glm::vec2 pos_spread;
        float dir_spread, speed_spread, size_spread,  rotation_spread;
        float fade_startTime, fade_rate;
    };

    /// @return  Maximum amount of particles
    __inline int GetMaxParticleCount() { return m_WGcount * m_WGsize; }

    /// @return  ID of the Shader Storage Buffer that stores transform matrices
    __inline unsigned int GetMatSSBO() { return m_MatSSBO; }

    /// @brief           Spawns a group of particles with provided initial data.
    /// @param initData  Range, position, velocity, etc.
    __inline void SpawnParticles(const InitData& initData) { m_Spawns.push_back(initData); }

    /// @brief   Clears all particles.
    __inline void ClearParticles() { m_Clear = true; }


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



    //-----------------------------------------------------------------------------
    //              Data
    //-----------------------------------------------------------------------------
private:
    /// @brief  Groups of particles to spawn. 10 groups per frame max. Excess gets
    ///         cut off. (won't show up on screen)
    std::vector< InitData > m_Spawns;  

    unsigned int m_DataSSBO = 0;    /// @brief  ID of particle raw data buffer
    unsigned int m_MatSSBO = 0;     /// @brief  ID of transform matrices buffer
    unsigned int m_UBO = 0;         /// @brief  ID of buffer used by uniform block
    Shader* m_CShader = nullptr;    /// @brief  Ptr to compute shader
    int m_NextIndex = 0;
    bool m_Clear = false;           /// @brief  When true, clears all particles

    /// @brief  Uniform locations
    unsigned int m_UinitCount = -1, 
                 m_Udt = -1, 
                 m_Ut = -1,
                 m_Uproj = -1;


    // -------------
    // Constants. 
    // -------------

    /// @brief  Size of a single work group (amount of individual invocations
    ///         within it). Make sure it matches what's in the shader.
    const int m_WGsize = 1024;


    // TODO: make these 2 deserializable?

    /// @brief  Amount of work groups that compute shader will use. Don't set 
    ///         this number to more than 1024. Total particles = WGsize x WGcount
    const int m_WGcount = 32;

    /// @brief  Maximum amount of emissions (groups of particles) to init per frame
    const int m_MaxInitCount = 10;


    /// @brief  Mirrors the struct on the shader. Used for spawning new particles.
    ///         This one is for internal use, allows proper UBO allocation/loading.
    struct ShaderInitData
    {
        glm::ivec2 range;
        glm::vec2 position;
        float direction,  speed,        size,         rotation;
        glm::vec2 pos_spread;
        float dir_spread, speed_spread, size_spread,  rotation_spread;
        float fade_startTime, fade_rate;
    };


    //-----------------------------------------------------------------------------
    //              Singleton stuff
    //-----------------------------------------------------------------------------
private:

    /// @brief  Private constructor
    ParticleSystem();

    /// @brief  The singleton instance of ParticleSystem
    static ParticleSystem * s_Instance;

public:

    /// @return   The singleton instance
    static ParticleSystem * GetInstance();

    // Prevent copying
    ParticleSystem(ParticleSystem& other) = delete;
    void operator=(const ParticleSystem&) = delete;
};

/// @brief      Convenient function for getting ParticleSystem instance.
__inline ParticleSystem* Particles() { return ParticleSystem::GetInstance(); }
