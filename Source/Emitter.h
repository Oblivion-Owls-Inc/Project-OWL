/*****************************************************************//**
 * \file   Emitter.h
 * \brief  Controls timing of particle emissions, and their init data.
 * 
 * \author Eli
 * \date   October 2023
 *********************************************************************/
#pragma once
#include "Component.h"
#include "ParticleSystem.h"


class Emitter : public Component
{
public:

	/// @brief	  Defualt constructor
	Emitter();

    /// @return   A copy of this emitter
    virtual Component* Clone() const override;



//-----------------------------------------------------------------------------
//              Public methods
//-----------------------------------------------------------------------------

    /// @brief   Sets whether the emitter should emit continuously
    __inline void SetContinuous(bool continuous) { m_Continuous = continuous; }

    /// @brief   Sets the amount of particles per second to emit
    __inline void SetPPS(float particles_per_sec) { m_PPS = particles_per_sec; }

    /// @brief   Sets the delay between emissions
    __inline void SetDelay(float delay) { m_Delay = delay; }

    /// @return  Size of the particle buffer(s), which is also maximum particle count 
    __inline int GetBufferSize() const { return m_BufferSize; }

    /// @return  ID of the shader storage buffer for matrices
    __inline unsigned GetMatSSBO() const { return m_MatSSBO; }

    /// @return  ID of the shader storage buffer for opacity values
    __inline unsigned GetOpacitySSBO() const { return m_OpacitySSBO; }

    /// @return  Emit data (particle system needs it)
    __inline ParticleSystem::EmitData const& GetEmitData() const { return m_Init; }

    /// @brief          Called by particle system after loading emit data. Accepts
    ///                 some locations of uniforms on compute shader, dispatches
    ///                 the compute shader to update its particles.
    /// @param dt       Deltatime
    /// @param urange   Uniform location of 'range' ivec2
    /// @param uoldest  Uniform location of 'oldest' int
    void Update(float dt, unsigned int urange, unsigned int uoldest);


//-----------------------------------------------------------------------------
//              Overrides
//-----------------------------------------------------------------------------
private: 

    /// @brief  Initialization: emitter adds itself to particle system
    virtual void OnInit() override;

    /// @brief  Exit: emitter removes itself from particle system
    virtual void OnExit() override;

    /// @brief  Tweak properties in debug window
    virtual void Inspector() override;



//-----------------------------------------------------------------------------
//              Data
//-----------------------------------------------------------------------------
private:
    bool m_Continuous = false;  /// @brief  If true, emits automatically.
    float m_PPS = 0.0f;         /// @brief  Particles per second
    float m_Delay = 0.0f;       /// @brief  Delay between emissions.
    float m_DelayTimer = 0.0f;  /// @brief  Timer for the delay
    int m_CurrentIndex = 0;     /// @brief  Index to emit new particles at

    /// @brief  Maximum amount of rendered particles for this emitter
    int m_BufferSize = 2048;

    /// @brief  How many work groups to dispatch
    int m_WGcount = 0;
    
    /// @brief   "Accumulation" of particles as a float. Whenever delay timer hits, 
    ///          integer part of this number is used as amt of particles to emit
    float m_ParticleCount = 0.0f;

    /// @brief   Data to initialize particles with on each emission
    ParticleSystem::EmitData m_Init = {};

    /// @brief   Shader storage buffers for particle data. Raw data, matrices,
    ///          and opacities.
    unsigned int m_DataSSBO = 0, 
                 m_MatSSBO = 0, 
                 m_OpacitySSBO = 0;


//-----------------------------------------------------------------------------
//              Reading / Writing
//-----------------------------------------------------------------------------
private:

    /// @brief          Reads in the boolean for continuous (active) flag 
    /// @param  data    JSON data to read
    void readContinuous(nlohmann::ordered_json const& data);

    /// @brief          Reads in the value for particles per second
    /// @param  data    JSON data to read
    void readPPS(nlohmann::ordered_json const& data);

    /// @brief          Reads in the value for delay between emissions
    /// @param  data    JSON data to read
    void readDelay(nlohmann::ordered_json const& data);

    /// @brief          Reads in value for maximum rendered particles
    /// @param  data    JSON data to read
    void readMax(nlohmann::ordered_json const& data);

    /// @brief          Reads in the array of floats that represent init data.
    /// @param  data    JSON data to read
    void readData(nlohmann::ordered_json const& data);

	/// @brief  map of read methods
	static ReadMethodMap< Emitter > const s_ReadMethods;

    /// @brief  gets the map of read methods
    /// @return the map of read methods
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override
    {
        return (ReadMethodMap< ISerializable > const&)s_ReadMethods;
    }


public:

    /// @brief    Writes component data to a JSON file.
    /// @return   JSON data to write
    virtual nlohmann::ordered_json Write() const override;
};
