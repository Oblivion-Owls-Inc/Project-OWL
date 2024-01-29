///*****************************************************************/
/// @file	    GeneratorBehavior.cpp
/// @author     Tyler Birdsall (tyler.birdsall@digipen.edu)
/// @date	    01/12/2024
/// @brief      GeneratorBehavior class implimentation
/// @copyright  Digipen LLC (c) 2024
///*****************************************************************/

#include "GeneratorBehavior.h"
#include "Entity.h"
#include "CircleCollider.h"
#include "CollisionSystem.h"
#include "EnemyBehavior.h"
#include "SceneSystem.h"
#include "AudioPlayer.h"
#include "BehaviorSystem.h"
#include "Health.h"
#include "BaseBehavior.h"

//-----------------------------------------------------------------------------
// constructor / destructor 
//-----------------------------------------------------------------------------

/// @brief  constructor
GeneratorBehavior::GeneratorBehavior() : BasicEntityBehavior(typeid(GeneratorBehavior))
{
	m_isActive = false;
	m_radius = 0.0f;
	m_activateRadius = 0.0f;
	m_depth = 0;
}

/// @brief  copy ctor
GeneratorBehavior::GeneratorBehavior(const GeneratorBehavior& other) : BasicEntityBehavior(other)
{
	m_isActive = other.m_isActive;
	m_radius = other.m_radius;
	m_activateRadius = other.m_activateRadius;
	m_depth = other.m_depth;
	//Behaviors<GeneratorBehavior>()->GetComponents();
}

/// @brief  dtor
GeneratorBehavior::~GeneratorBehavior()
{
}

/// @brief  clone
Component* GeneratorBehavior::Clone() const
{
	return new GeneratorBehavior(*this);
}

//-----------------------------------------------------------------------------
// virtual override methods
//-----------------------------------------------------------------------------

/// @brief	initialize Generator
void GeneratorBehavior::OnInit()
{
	BehaviorSystem<GeneratorBehavior>::GetInstance()->AddComponent(this);
	//BasicEntityBehavior::OnInit();

	GetEntity()->GetComponent< CircleCollider >()->AddOnCollisionCallback(
		GetId(),
		std::bind(&GeneratorBehavior::onCollision, this, std::placeholders::_1, std::placeholders::_2)
	);

	m_AudioPlayer = GetEntity()->GetComponent<AudioPlayer>();

	const std::string name = GetEntity()->GetName();
	if (name == "Base")
	{
		m_isActive = true;
	}
	else
	{
		m_isActive = true;
	}
}

/// @brief	called on exit, handles loss state
void GeneratorBehavior::OnExit()
{
	BehaviorSystem<GeneratorBehavior>::GetInstance()->RemoveComponent(this);
	//BasicEntityBehavior::OnExit();
}


void GeneratorBehavior::Inspector()
{
	ImGui::InputFloat("Radius", &m_radius, 0.5f, 1.0f);
	ImGui::InputFloat("Activate Radius", &m_activateRadius, 0.5f, 1.0f);
	ImGui::InputInt("Depth", &m_depth, 1, 5);
	if (ImGui::Checkbox("Active", &m_isActive)) {}
}

Entity* GeneratorBehavior::GetLowestGenerator()
{
	int first = 1;
	GeneratorBehavior* give = Behaviors<GeneratorBehavior>()->GetComponents().front();
	for (auto& generator : Behaviors<GeneratorBehavior>()->GetComponents())
	{
		if (generator->m_depth > give->m_depth)
		{
			give = generator;
		}
	}
	return give->GetEntity();
}

void GeneratorBehavior::onCollision(Collider* other, CollisionData const& collisionData)
{
	if (m_isActive)
	{
		EnemyBehavior* enemy = other->GetEntity()->GetComponent<EnemyBehavior>();
		if (!enemy)
		{
			return;
		}
		Health* health = GetEntity()->GetComponent<Health>();
		health->GetHealth()->SetCurrent(health->GetHealth()->GetCurrent() - enemy->GetDamage());
		//BasicEntityBehavior::TakeDamage(enemy->GetDamage());

		if (m_AudioPlayer)
		{
			m_AudioPlayer->Play();
		}
		enemy->GetEntity()->Destroy();

		if (health->GetHealth()->GetCurrent() <= 0)
		{
			m_isActive = false;
			health->GetHealth()->SetCurrent(
			health->GetHealth()->GetDefault());
			if (GetEntity()->GetComponent<BaseBehavior>())
			{
				GetEntity()->GetComponent<BaseBehavior>()->Destroy();
			}
		}
	}
}

//-----------------------------------------------------------------------------
// reading
//-----------------------------------------------------------------------------

/// @brief read method map
ReadMethodMap<GeneratorBehavior> const GeneratorBehavior::s_ReadMethods =
{
	{ "Radius",	  &readRadius},
	{ "Activate Range",&readARadius},
	{ "Depth",	  &readDepth},
	{ "Active",	  &readActive},
};

//-----------------------------------------------------------------------------
// writing
//-----------------------------------------------------------------------------

void GeneratorBehavior::readRadius(nlohmann::ordered_json const& json)
{
	m_radius = Stream::Read<float>(json);
}

void GeneratorBehavior::readARadius(nlohmann::ordered_json const& json)
{
	m_activateRadius = Stream::Read<float>(json);
}

void GeneratorBehavior::readDepth(nlohmann::ordered_json const& json)
{
	m_depth = Stream::Read<int>(json);
}

void GeneratorBehavior::readActive(nlohmann::ordered_json const& json)
{
	m_isActive = Stream::Read<bool>(json);
}

/// @brief	write to json
nlohmann::ordered_json GeneratorBehavior::Write() const
{
	nlohmann::ordered_json data;

	data["Radius"] = m_radius;
	data["ActivateRadius"] = m_activateRadius;
	data["Depth"] = m_depth;
	data["Active"] = m_isActive;


	return data;
}


