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
#include "Behavior.h"
#include "BehaviorSystem.h"

//-----------------------------------------------------------------------------
// constructor / destructor 
//-----------------------------------------------------------------------------

/// @brief  constructor
GeneratorBehavior::GeneratorBehavior() : Behavior(typeid(GeneratorBehavior))
{
	m_isActive = false;
	m_powerRadius = 0.0f;
	m_activationRadius = 0.0f;
	m_depth = 0;
}

/// @brief  dtor
GeneratorBehavior::~GeneratorBehavior()
{
}

/// @brief  clone
GeneratorBehavior* GeneratorBehavior::Clone() const
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

	GetEntity()->GetComponent< CircleCollider >()->AddOnCollisionEnterCallback(
		GetId(),
		std::bind(&GeneratorBehavior::onCollisionEnter, this, std::placeholders::_1)
	);

	m_AudioPlayer = GetEntity()->GetComponent<AudioPlayer>();

	m_Transform = GetEntity()->GetComponent<Transform>();

	//const std::string name = GetEntity()->GetName();
}

/// @brief	called on exit, handles loss state
void GeneratorBehavior::OnExit()
{
	BehaviorSystem<GeneratorBehavior>::GetInstance()->RemoveComponent(this);
	//BasicEntityBehavior::OnExit();
}

//-----------------------------------------------------------------------------
// copying
//-----------------------------------------------------------------------------

/// @brief  copy ctor
GeneratorBehavior::GeneratorBehavior(const GeneratorBehavior& other) : Behavior(other)
{
	m_isActive = other.m_isActive;
	m_powerRadius = other.m_powerRadius;
	m_activationRadius = other.m_activationRadius;
	m_depth = other.m_depth;
}

//-----------------------------------------------------------------------------
// accessors
//-----------------------------------------------------------------------------

/// @brief	returns the lowest generator in the level
/// @return entity pointer to the lowest generator
Entity* GeneratorBehavior::GetLowestGenerator()
{
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

//-----------------------------------------------------------------------------
// private functions
//-----------------------------------------------------------------------------

/// @brief collision callback for generators
void GeneratorBehavior::onCollisionEnter(Collider* other)
{
	if (m_isActive)
	{
		EnemyBehavior* enemy = other->GetEntity()->GetComponent<EnemyBehavior>();
		if (!enemy)
		{
			return;
		}
		Health* health = GetEntity()->GetComponent<Health>();
		//health->GetHealth()->SetCurrent(health->GetHealth()->GetCurrent() - enemy->GetDamage());
		health->TakeDamage(enemy->GetDamage());
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
			health->GetHealth()->GetMaximum());
			if (GetEntity()->GetComponent<BaseBehavior>())
			{
				GetEntity()->GetComponent<BaseBehavior>()->Destroy();
			}
		}
	}
}

//-----------------------------------------------------------------------------
// inspector methods
//-----------------------------------------------------------------------------

/// @brief	inspector for generators
void GeneratorBehavior::Inspector()
{
	ImGui::InputFloat("Radius", &m_powerRadius, 0.5f, 1.0f);
	ImGui::InputFloat("Activate Radius", &m_activationRadius, 0.5f, 1.0f);
	ImGui::InputInt("Depth", &m_depth, 1, 5);
	ImGui::Checkbox("Active", &m_isActive);
}

//-----------------------------------------------------------------------------
// reading
//-----------------------------------------------------------------------------

/// @brief read method map
ReadMethodMap<GeneratorBehavior> const GeneratorBehavior::s_ReadMethods =
{
	{ "Radius",	  &readRadius},
	{ "ActivateRadius",&readARadius},
	{ "Depth",	  &readDepth},
	{ "Active",	  &readActive},
};

/// @brief	read the raidus from json
void GeneratorBehavior::readRadius(nlohmann::ordered_json const& json)
{
	m_powerRadius = Stream::Read<float>(json);
}

/// @brief	read the activation radius from json
void GeneratorBehavior::readARadius(nlohmann::ordered_json const& json)
{
	m_activationRadius = Stream::Read<float>(json);
}

/// @brief	read the depth from json
void GeneratorBehavior::readDepth(nlohmann::ordered_json const& json)
{
	m_depth = Stream::Read<int>(json);
}

/// @brief	read if the generator starts active from json
void GeneratorBehavior::readActive(nlohmann::ordered_json const& json)
{
	m_isActive = Stream::Read<bool>(json);
}

//-----------------------------------------------------------------------------
// writing
//-----------------------------------------------------------------------------

/// @brief	write to json
nlohmann::ordered_json GeneratorBehavior::Write() const
{
	nlohmann::ordered_json data;

	data["Radius"] = m_powerRadius;
	data["ActivateRadius"] = m_activationRadius;
	data["Depth"] = m_depth;
	data["Active"] = m_isActive;


	return data;
}


