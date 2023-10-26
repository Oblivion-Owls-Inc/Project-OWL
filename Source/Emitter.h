#pragma once
#include "Behavior.h"
#include "ParticleSystem.h"


class Emitter : public Behavior
{
public:

	/// @brief	        Defualt constructor
	Emitter();

    /// @return         A copy of this emitter
    virtual Component* Clone() const override;



//-----------------------------------------------------------------------------
//              Public methods
//-----------------------------------------------------------------------------

    __inline void SetContinuous(bool continuous) { m_Continuous = continuous; }

    __inline void SetEPS(float emissions_per_sec) { m_PPS = emissions_per_sec; }

    __inline void SetDelay(float delay) { m_Delay = delay; }



//-----------------------------------------------------------------------------
//              Overrides
//-----------------------------------------------------------------------------
private: 

    /// @brief  Initialization: emitter adds itself to behavior system
    virtual void OnInit() override;

    /// @brief  Exit: emitter removes itself from behavior system
    virtual void OnExit() override;

	/// @brief  Controls timing of continuous emissions
	virtual void OnUpdate(float dt) override;

    virtual void Inspector() override;

//-----------------------------------------------------------------------------
//              Data
//-----------------------------------------------------------------------------
private:
    bool m_Continuous = false;  /// @brief  If true, emits automatically.
    float m_PPS = 0.0f;         /// @brief  Particles per second
    float m_Delay = 0.0f;       /// @brief  Delay between emissions.
    float m_DelayTimer = 0.0f;  /// @brief  Timer for the delay
    
    /// @brief   Accumulation... or something. Not sure what the real mathematical
    ///          name for this is. It increases by EPS*dt each frame, and emits
    ///          (integer part) amount of particles whenever delay timer hits.
    float m_Emission = 0.0f;

    /// @brief   Data to initialize particles with on each emission
    ParticleSystem::InitData m_Init = {};



//-----------------------------------------------------------------------------
//              Reading
//-----------------------------------------------------------------------------
private:

	/// @brief  map of read methods
	static ReadMethodMap< Emitter > const s_ReadMethods;

    /// @brief  gets the map of read methods
    /// @return the map of read methods
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override
    {
        return (ReadMethodMap< ISerializable > const&)s_ReadMethods;
    }


};
