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
	m_powerRadius = 0.0f;
	m_activationRadius = 0.0f;
	m_depth = 0;
}

/// @brief  copy ctor
GeneratorBehavior::GeneratorBehavior(const GeneratorBehavior& other) : BasicEntityBehavior(other)
{
	m_isActive = other.m_isActive;
	m_powerRadius = other.m_powerRadius;
	m_activationRadius = other.m_activationRadius;
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
// accessor
//-----------------------------------------------------------------------------

/// @brief	returns the lowest generator in the level
/// @return entity pointer to the lowest generator
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
}

/// @brief	called on exit, handles loss state
void GeneratorBehavior::OnExit()
{
	BehaviorSystem<GeneratorBehavior>::GetInstance()->RemoveComponent(this);
	//BasicEntityBehavior::OnExit();
}

/// @brief	inspector for generators
void GeneratorBehavior::Inspector()
{
	ImGui::InputFloat("Radius", &m_powerRadius, 0.5f, 1.0f);
	ImGui::InputFloat("Activate Radius", &m_activationRadius, 0.5f, 1.0f);
	ImGui::InputInt("Depth", &m_depth, 1, 5);
	if (ImGui::Checkbox("Active", &m_isActive)) {}
}

/// @brief collision callback for generators
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


