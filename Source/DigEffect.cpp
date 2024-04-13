/***************************************************************************
* \file       DigEffect.cpp
* \author     Eli Tsereteli
*
* \brief      Controls the temporary entity used for emitting particles
*             when a tile breaks. If timer is initialized to 0, acts as
*             the spawner of temporary entities (should be attached to
*             the map entity). If timer is greater than 0, simply destroys
*             parent entty when timer runs out. (should be attached to prefab)
*
* \date       March 2024
* \copyright  Copyright (c) 2024 Digipen Instutute of Technology
****************************************************************************/
#include "pch.h"
#include "BehaviorSystem.h"  // add/remove from behaviorsystem
#include "DigEffect.h"
#include "Emitter.h"
#include "Entity.h"			 // destroy/spawn entity
#include "MiningLaser.h"	 // tile break callback
#include "TilemapSprite.h"   // callback
#include "EmitterSprite.h"   // emitter texture/frame
#include "EntitySystem.h"	 // spawn entity
#include "Transform.h"	     // (and set its position)
#include "AudioPlayer.h"	 // block breaking
#include "Texture.h"

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

	// if timer is non-zero, this is temp entity
	if (m_Timer > 0.0f)
	{
		// if this is temporary entity, emit particles and init audio
		Emitter* emitter = GetEntity()->GetComponent<Emitter>();
		if (emitter != nullptr)
			emitter->Emit();

		m_AudioPlayer.Init( GetEntity() );
	}
	else if (m_Timer == 0.0f)
	{
		// if it's not, this should be part of tilemap entity. It will be spawning archetypes.
		Tilemap<int>* tm = GetEntity()->GetComponent< Tilemap<int> >();
		if (tm)
			tm->AddOnTilemapChangedCallback( GetId(), std::bind( &DigEffect::spawnTemp,
																 this,
																 std::placeholders::_1,
																 std::placeholders::_2,
																 std::placeholders::_3 ) );

		m_Archetype.SetOwnerName( GetEntity()->GetName() );
		m_Archetype.Init();
		m_Texture.SetOwnerName( GetEntity()->GetName() );
		m_Texture.Init();

		for (auto& sound : m_BreakSounds)
		{
			sound.SetOwnerName( GetName() );
			sound.Init();
		}
	}
}


/// @brief  Removes itself from behavior system, clears references
void DigEffect::OnExit() 
{ 
	Behaviors< Behavior >()->RemoveComponent(this);

	if (m_Archetype)
		m_Archetype.Clear();

	Tilemap<int>* tm = GetEntity()->GetComponent< Tilemap<int> >();
	if (tm)
		tm->RemoveOnTilemapChangedCallback( GetId() );
}


/// @brief  Runs the timer
void DigEffect::OnUpdate(float dt)
{
	// if timer is not 0, it will run. And destroy parent entity once it runs out.
	if (m_Timer > 0.0f)
	{
		m_Timer -= dt;
		if (m_Timer <= 0.0f)
			GetEntity()->Destroy();
	}
}


/// @brief  Tweak properties in debug window
void DigEffect::Inspector()
{
	if (m_Timer == 0.0f)
	{
		std::string message("Be advised: this is the spawner component. To edit timer (lifetime) and AudioPlayer properties, find the entity prefab named \"");
		message += m_Archetype.GetName();
		message += '\"';
		ImGui::TextWrapped(message.c_str());

		ImGui::PushID(0);
		m_Archetype.Inspect("Archetype (to spawn when tile breaks)");
		ImGui::PopID();
		ImGui::PushID(1);
		m_Texture.Inspect("Texture");
		ImGui::PopID();
		ImGui::TextWrapped("(frames of the texture should match parent tilemap IDs)");
		ImGui::Spacing();
		ImGui::Text("Break sounds");
		for (int i = 0; i < m_BreakSounds.size(); ++i)
		{
			char id[4] = {};
			sprintf_s(id, 4, "%i", i);
			m_BreakSounds[i].Inspect(id);
		}
		if (ImGui::Button("Add sound"))
			m_BreakSounds.push_back(AssetReference<Sound>());

		if (m_BreakSounds.size() != 0 && ImGui::Button("Delete last"))
			m_BreakSounds.erase(m_BreakSounds.end() - 1);

		ImGui::TextWrapped("(order of the sounds should match parent tilemap IDs)");
	}
	else
		ImGui::InputFloat("Timer", &m_Timer, 0.01f);
}


/// @brief          Spawns new temporary entity at the broken tile's location.
/// @param tilemap  pointer to tilemap whose tile just broke
/// @param tilePos  2D index of the broken tile
/// @param tileId   old ID/frame of the changed tile
void DigEffect::spawnTemp(Tilemap< int >* tilemap, glm::ivec2 const& tilePos, int prevTileId)
{
	if ( !m_Archetype || !m_Texture || tilePos == glm::ivec2(-1) )
		return;

	// skip if the tile wasn't changed to empty
	int tileId = tilemap->GetTile(tilePos);
	if (tileId != -1)
		return;

	glm::vec2 pos = tilemap->TileCoordToWorldPos(tilePos);

	// spawn from archetype at broken tile pos
	Entity* temp = m_Archetype->Clone();
	temp->GetComponent<Transform>()->SetTranslation(pos);

	// set emitter's texture and frame
	EmitterSprite* es = temp->GetComponent<EmitterSprite>();
	es->SetTexture( m_Texture );
	es->SetFrameIndex( prevTileId );

	// set audio to respective sound
	if (prevTileId < m_BreakSounds.size())
	{
		AudioPlayer* ap = temp->GetComponent<AudioPlayer>();
		if (ap)
		{
			ap->SetSound( m_BreakSounds[prevTileId] );
			ap->Play();
		}
	}

	temp->AddToScene();
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

/// @brief        reads the archetype to spawn
/// @param data   json to read from
void DigEffect::readArchetype(nlohmann::ordered_json const& data)
{
	Stream::Read(m_Archetype, data);
}

/// @brief        reads the texture to use for particles
/// @param data   json to read from
void DigEffect::readTexture(nlohmann::ordered_json const& data)
{
	Stream::Read(m_Texture, data);
}

/// @brief        reads the breaking sounds
/// @param data   json to read from
void DigEffect::readSounds(nlohmann::ordered_json const& data)
{
	if (data.size() == 0)
		return;

	m_BreakSounds.resize( data.size() );
	for (int i=0; i<m_BreakSounds.size(); ++i)
		Stream::Read( m_BreakSounds[i], data[i] );
}


/// @brief read method map
ReadMethodMap<DigEffect> const DigEffect::s_ReadMethods =
{
	{ "Timer",		&readTimer     },
	{ "Archetype",	&readArchetype },
	{ "Texture",    &readTexture   },
	{ "Sounds",     &readSounds    }
};


/// @brief	write to json
nlohmann::ordered_json DigEffect::Write() const
{
	nlohmann::ordered_json data;

	data["Timer"] = m_Timer;
	data["Archetype"] = Stream::Write(m_Archetype);
	data["Texture"] = Stream::Write(m_Texture);

	if (m_BreakSounds.size() != 0)
	{
		nlohmann::ordered_json& soundarray = data["Sounds"];
		for (auto& sound : m_BreakSounds)
			soundarray.push_back( Stream::Write(sound) );
	}

	return data;
}

