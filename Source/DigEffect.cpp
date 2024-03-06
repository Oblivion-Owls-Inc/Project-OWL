/*********************************************************************
* \file   DigEffect.cpp
* \brief  
*
* \author Eli Tsereteli
*********************************************************************/
#include "BehaviorSystem.h"
#include "DigEffect.h"
#include "Entity.h"
#include "Emitter.h"

/// @brief  Default constructor
DigEffect::DigEffect() : Behavior(typeid(DigEffect)) {}

/// @brief  copy ctor
DigEffect::DigEffect(const DigEffect& other) : 
	Behavior(other), m_Timer(other.m_Timer) {}

/// @brief  Return a copy of this component
DigEffect* DigEffect::Clone() const { return new DigEffect(*this); }



//-----------------------------------------------------------------------------
//              Overrides
//-----------------------------------------------------------------------------

/// @brief  Adds itself to behavior system
void DigEffect::OnInit() 
{
	Behaviors< Behavior >()->AddComponent(this);

	m_Emitter.Init(GetEntity());

	if (m_Emitter)
	{

		if (m_Timer)
		{
			// if this is temporary entity, emit 50 particles and that's that.
			m_Emitter->SetContinuous(false);
			m_Emitter->SetPPS(50.0f);
			m_Emitter->Emit();
		}
		else
			m_Emitter->SetContinuous(true);  // TBD.
	}
}

/// @brief  Removes itself from behavior system
void DigEffect::OnExit() 
{ 
	Behaviors< Behavior >()->RemoveComponent(this);
}


/// @brief  Keeps an eye on item count
void DigEffect::OnUpdate(float dt)
{
	// if timer is not 0, it will run. And destroy entity once it runs out.
	if (m_Timer)
	{
		m_Timer -= dt;
		if (m_Timer <= 0.0f)
			GetEntity()->Destroy();
	}
}


/// @brief  Tweak properties in debug window
void DigEffect::Inspector()
{
	ImGui::InputFloat("Timer", &m_Timer, 0.01f);
}


//-----------------------------------------------------------------------------
//              Reading / Writing
//-----------------------------------------------------------------------------

/// @brief		 Reads stuff
/// @param data  json data to read
void DigEffect::readTimer(nlohmann::ordered_json const& data)
{
	Stream::Read(m_Timer, data);
}

/// @brief read method map
ReadMethodMap<DigEffect> const DigEffect::s_ReadMethods =
{
	{ "Timer",		&readTimer }
};


/// @brief	write to json
nlohmann::ordered_json DigEffect::Write() const
{
	nlohmann::ordered_json data;

	data["Timer"] = m_Timer;

	return data;
}

