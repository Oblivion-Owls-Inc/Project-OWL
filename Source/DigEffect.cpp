/*********************************************************************
* \file       DigEffect.cpp
* \author     Eli Tsereteli
*
* \brief      Controls the temporary entity used for emitting particles
*             when a tile breaks.
*
* \date       March 2024
* \copyright  Copyright (c) 2024 Digipen Instutute of Technology
*********************************************************************/
#include "BehaviorSystem.h"  // add/remove from behaviorsystem
#include "DigEffect.h"
#include "Emitter.h"
#include "Entity.h"			 // destroy/spawn entity
#include "MiningLaser.h"	 // tile break callback
#include "TilemapSprite.h"   // get tile texture
#include "EmitterSprite.h"   // (and assign it to particles)
#include "EntitySystem.h"	 // spawn entity
#include "Transform.h"	     // (and set its position)

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

	m_Emitter.Init( GetEntity() );
	if (m_Emitter == nullptr)
		return;

	// if timer is non-zero, this is temp entity
	if (m_Timer > 0.0f)
	{
		// if this is temporary entity, emit 50 particles and that's that.
		m_Emitter->SetPPS(50.0f);
		m_Emitter->Emit();
	}
	else if (m_Timer == 0.0f)
	{
		// if it's not, this should be laser's child entity. init references, set callback
		m_Archetype.SetOwnerName( GetEntity()->GetName() );
		m_Archetype.Init();

		m_Laser.Init( GetEntity()->GetParent() );
		if (m_Laser == nullptr)
			return;

		m_Laser->AddOnBreakTileCallback( GetId(), std::bind( &DigEffect::spawnTemp, 
															 this,
															 std::placeholders::_1, 
															 std::placeholders::_2, 
															 std::placeholders::_3) );
	}
}


/// @brief  Removes itself from behavior system, clears references
void DigEffect::OnExit() 
{ 
	Behaviors< Behavior >()->RemoveComponent(this);

	if (m_Emitter)
		m_Emitter.Exit();
	
	if (m_Laser)
	{
		m_Laser->RemoveOnBreakTileCallback( GetId() );
		m_Laser.Exit();
	}

	if (m_Archetype)
		m_Archetype.Clear();
}


/// @brief  Keeps an eye on item count
void DigEffect::OnUpdate(float dt)
{
	// if timer is not 0, it will run. And destroy entity once it runs out.
	if (m_Timer > 0.0f)
	{
		m_Timer -= dt;
		if (m_Timer <= 0.0f)
			GetEntity()->Destroy();
	}
	//else
	//	updateChild(dt);  // TODO: rename this to something less confusing
}


/// @brief  Tweak properties in debug window
void DigEffect::Inspector()
{
	ImGui::InputFloat("Timer", &m_Timer, 0.01f);
}



void DigEffect::spawnTemp(Tilemap< int >* tilemap, glm::ivec2 const& tilePos, int tileId)
{
	glm::vec2 pos = tilemap->TileCoordToWorldPos(tilePos);

	Entity* temp = m_Archetype->Clone();
	temp->GetComponent<Transform>()->SetTranslation(pos);
	temp->AddToScene();
	Debug() << "Broken tile pos:  " << pos.x << ", " << pos.y << "  ID: " << tileId << std::endl;
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

void DigEffect::readArchetype(nlohmann::ordered_json const& data)
{
	Stream::Read(m_Archetype, data);
}

/// @brief read method map
ReadMethodMap<DigEffect> const DigEffect::s_ReadMethods =
{
	{ "Timer",		&readTimer },
	{ "Archetype",	&readArchetype }
};


/// @brief	write to json
nlohmann::ordered_json DigEffect::Write() const
{
	nlohmann::ordered_json data;

	data["Timer"] = m_Timer;

	return data;
}

