///--------------------------------------------------------------------------//
/// @file   WavesBehavior.cpp
/// @brief  Definitions for wave and spawning behavior
/// 
/// @author Tyler Birdsall (tyler.birdsall)
/// @date   10/24/23
///
/// @copyright (c) 2023 DigiPen (USA) Corporation.
///--------------------------------------------------------------------------//

#include "WavesBehavior.h"
#include "BehaviorSystem.h"
#include "EntitySystem.h"
#include "Transform.h"
#include "Animation.h"
#include "DebugSystem.h"
#include "AssetLibrarySystem.h"
#include "Engine.h"
#include "Tilemap.h"

//-----------------------------------------------------------------------------
// public: constructor / destructor
//-----------------------------------------------------------------------------

/// @brief default constructor
WavesBehavior::WavesBehavior() :
	Behavior(typeid(WavesBehavior)),
	numWaves(0),
	currentWave(0),
	inspectorWave(0),
	inspectorGroup(0)
{
	waves.clear();
}

/// @brief cpy ctor
WavesBehavior::WavesBehavior(const WavesBehavior& other)
	: Behavior(typeid(WavesBehavior)),
	numWaves(other.numWaves),
	currentWave(other.currentWave),
	inspectorWave(other.inspectorWave),
	inspectorGroup(other.inspectorGroup)
{
	waves = other.waves;
}

/// @brief dtor
WavesBehavior::~WavesBehavior()
{
}

/// @brief wave default constructor
WavesBehavior::Wave::Wave() :
	remainingTime(0),
	timeToNextWave(0)
{}

/// @brief enemyGroup default constructor
WavesBehavior::EnemyGroup::EnemyGroup() :
	enemyAmount(0),
	spawnInterval(0),
	timer(0),
	offset(0),
	spawner(0)
{}

//-----------------------------------------------------------------------------
// private: copying
//-----------------------------------------------------------------------------

Component* WavesBehavior::Clone() const
{
	return (Component*)new WavesBehavior(*this);
}

//-----------------------------------------------------------------------------
// private: virtual override methods
//-----------------------------------------------------------------------------

/// @brief initialize WavesBehavior
void WavesBehavior::OnInit()
{
	/// Add this behavior to the behavior system
	BehaviorSystem<WavesBehavior>::GetInstance()->AddBehavior(this);
}

/// @brief cleanup WavesBehavior
void WavesBehavior::OnExit()
{
	/// Remove this behavior from the behavior system
	BehaviorSystem<WavesBehavior>::GetInstance()->RemoveBehavior(this);
}

/// @brief update, currently not used
void WavesBehavior::OnUpdate(float dt)
{
	/// Do Waves Debug Stuff
}

/// @brief update waves behavior and handle when things should spawn
void WavesBehavior::OnFixedUpdate()
{
	if (currentWave < numWaves)
	{
		float dt = Engine::GetInstance()->GetFixedFrameDuration();
		if (waves[currentWave].remainingTime > 0.0f)
		{
			waves[currentWave].remainingTime -= dt;
			for (int i = 0; i < waves[currentWave].groups.size(); i++)
			{
				if (waves[currentWave].groups[i].enemyAmount > 0)
				{
					waves[currentWave].groups[i].timer -= dt;
					if (waves[currentWave].groups[i].timer < 0.0f)
					{
						waves[currentWave].groups[i].timer +=
							waves[currentWave].groups[i].spawnInterval;
						waves[currentWave].groups[i].enemyAmount--;
						Spawn(waves[currentWave].groups[i].name, i);
					}
				}
			}
		}
		else
		{
			waves[currentWave].remainingTime = 0.0f;
		}
		waves[currentWave].timeToNextWave -= dt;
		if (waves[currentWave].timeToNextWave < 0.0f)
		{
			waves[currentWave].timeToNextWave = 0.0f;
			currentWave++;
		}
	}
}

//-----------------------------------------------------------------------------
// private: inspector methods
//-----------------------------------------------------------------------------

/// @brief displays wave data to edit
void WavesBehavior::GuiWaves()
{
	ImGui::Text("Total Waves: %i", numWaves);

	ImGui::Text("Wave: %i", inspectorWave + 1);
	ImGui::InputInt("Wave", &inspectorWave);
	if (inspectorWave >= numWaves)
	{
		inspectorWave = numWaves - 1;
	}
	else if (inspectorWave < 0)
	{
		inspectorWave = 0;
	}
	ImGui::InputFloat("Remaining Time",
		&waves[inspectorWave].remainingTime);
	ImGui::InputFloat("Time to Next Wave",
		&waves[inspectorWave].timeToNextWave);

	ImGui::Text("");
}

/// @brief displays group data to edit
void WavesBehavior::GuiGroups()
{
	ImGui::Text("Groups in Wave: %i", waves[inspectorWave].groups.size());
	if (waves[inspectorWave].groups.size() > 0)
	{
		if (inspectorGroup >= waves[inspectorWave].groups.size())
		{
			inspectorGroup = (int)waves[inspectorWave].groups.size() - 1;
		}
		ImGui::Text("Group: %i", inspectorGroup + 1);
		ImGui::Text("Enemy Type: %s", waves[inspectorWave].groups[inspectorGroup].name.c_str());
		ImGui::InputInt("Group", &inspectorGroup);
		if (inspectorGroup >= waves[inspectorWave].groups.size())
		{
			inspectorGroup = (int)waves[inspectorWave].groups.size() - 1;
		}
		else if (inspectorGroup < 0)
		{
			inspectorGroup = 0;
		}
		ImGui::InputInt("Enemies", &waves[inspectorWave].groups[inspectorGroup].enemyAmount);
		ImGui::InputInt("Spawner", &waves[inspectorWave].groups[inspectorGroup].spawner);
		int size = (int)spawners.size();
		if (waves[inspectorWave].groups[inspectorGroup].spawner >= size)
		{
			waves[inspectorWave].groups[inspectorGroup].spawner = size - 1;
		}
		else if (waves[inspectorWave].groups[inspectorGroup].spawner < 0)
		{
			waves[inspectorWave].groups[inspectorGroup].spawner = 0;
		}
		ImGui::InputFloat("Timer", &waves[inspectorWave].groups[inspectorGroup].timer);

	}
	ImGui::Text("");
}

/// @brief displays the currently active wave data
void WavesBehavior::GuiCurrentWave()
{

	if (currentWave < numWaves)
	{
		ImGui::Text("Wave: %i", currentWave + 1);
		ImGui::Text("Remaining Time: %f", waves[currentWave].remainingTime);
		ImGui::Text("Time to Next Wave: %f", waves[currentWave].timeToNextWave);
	}
	else
	{
		ImGui::Text("No Current Wave");
	}
}

/// @brief displays the currently active enemy groups data
void WavesBehavior::GuiCurrentGroups()
{
	if (currentWave < numWaves)
	{
		for (int i = 0; i < waves[currentWave].groups.size(); i++)
		{
			ImGui::Text("Group: %i", i + 1);
			ImGui::Text("Enemy Type: %s", waves[currentWave].groups[i].name.c_str());
			ImGui::Text("Amount: %i", waves[currentWave].groups[i].enemyAmount);
			ImGui::Text("Spawner: %i", waves[currentWave].groups[i].spawner);
			ImGui::Text("Timer: %f", waves[currentWave].groups[i].timer);
			ImGui::Text("");
		}
	}
}

/// @brief calls the current gui handlers
void WavesBehavior::GuiCurrent()
{
	char title[20];
	snprintf(title, sizeof(title), "Current Wave Data:");
	if (ImGui::TreeNode(title))
	{
		GuiCurrentWave();
		ImGui::Text("");
		GuiCurrentGroups();
		ImGui::TreePop();
	}
}

/// @brief lists all spawners and locations
void WavesBehavior::GuiSpawners()
{
	char title[20];
	snprintf(title, sizeof(title), "Spawners:");
	if (ImGui::TreeNode(title))
	{
		for (int i = 0; i < spawners.size(); i++)
		{
			//glm::vec2 location = spawners[i]GetTranslation();
			//ImGui::Text("Spawner: %i", i);
			//ImGui::Text("Location: [ %f %f ]", location.x, location.y);
		}
		ImGui::TreePop();
	}
}

/// @brief handles displaying ImGui inspectors
void WavesBehavior::Inspector()
{
	///Edit the Behavior of the Waves 
	ImGui::Begin("WaveData");

	GuiWaves();

	GuiGroups();

	GuiCurrent();

	GuiSpawners();

	ImGui::End();
}

//-----------------------------------------------------------------------------
// public: accessors
//-----------------------------------------------------------------------------

/// @brief get the timer for a UI element
/// @brief this should probably be fancier
float WavesBehavior::getTimer()
{
	if (waves[currentWave].remainingTime > 0.0f)
	{
		return waves[currentWave].remainingTime;
	}
	else
	{
		return waves[currentWave].timeToNextWave;
	}
}

//-----------------------------------------------------------------------------
// private: methods
//-----------------------------------------------------------------------------

/// @brief spawn an enemy with given name from given entity group
/// @param name  - the name of the enemy to spawn
/// @param group - enemyGroup to spawn from
void WavesBehavior::Spawn(std::string name, int group)
{
	Entity* copy = AssetLibrary<Entity>()->GetAsset(name)->Clone();
	Transform* copyTransform = copy->GetComponent<Transform>();

	copyTransform->SetTranslation(spawners[waves[currentWave].groups[group].spawner]);
	Entities()->AddEntity(copy);
}

//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------

/// @brief read the waves data
/// @param data the data to read from.
void WavesBehavior::readWaves(nlohmann::ordered_json const& data)
{
	numWaves = (int)data.size();
	waves.resize(data.size());
	for (int i = 0; i < data.size(); ++i)
	{
		Stream::Read(waves[i], data[i]);
	}
}

/// @brief read the alternate spawners data
/// @param data the data to read from.
void WavesBehavior::readSpawners(nlohmann::ordered_json const& data)
{
	for (nlohmann::ordered_json const& location : data)
	{
		spawners.push_back(Stream::Read<2, float>(location));
	}
}

void WavesBehavior::Wave::readWaveTime(nlohmann::ordered_json const& json)
{
	remainingTime = Stream::Read<float>(json);
}

void WavesBehavior::Wave::readNextTime(nlohmann::ordered_json const& json)
{
	timeToNextWave = Stream::Read<float>(json);
}

void WavesBehavior::Wave::readGroups(nlohmann::ordered_json const& json)
{
	groups.resize(json.size());
	for (int i = 0; i < json.size(); ++i)
	{
		Stream::Read(groups[i], json[i]);
	}
}

void WavesBehavior::EnemyGroup::readName(nlohmann::ordered_json const& json)
{
	name = Stream::Read<std::string>(json);
}

void WavesBehavior::EnemyGroup::readAmount(nlohmann::ordered_json const& json)
{
	enemyAmount = Stream::Read<int>(json);
}

void WavesBehavior::EnemyGroup::readInterval(nlohmann::ordered_json const& json)
{
	spawnInterval = Stream::Read<float>(json);
}

void WavesBehavior::EnemyGroup::readOffset(nlohmann::ordered_json const& json)
{
	offset = Stream::Read<float>(json);
	timer = offset;
}

void WavesBehavior::EnemyGroup::readSpawner(nlohmann::ordered_json const& json)
{
	spawner = Stream::Read<int>(json);
}

/// @brief Write all WavesBehavior data to a JSON file.
/// @return The JSON file containing the WavesBehavior data.
nlohmann::ordered_json WavesBehavior::Write() const
{
	nlohmann::ordered_json data;

	nlohmann::ordered_json& altSpawn = data["AlternateSpawners"];
	for (int i = 0; i < spawners.size(); i++)
	{
		altSpawn.push_back(Stream::Write(spawners[i]));
	}

	nlohmann::ordered_json& writeWaves = data["Waves"];
	for (int i = 0; i < waves.size(); i++)
	{
		writeWaves.push_back(waves[i].Write());
	}

	return data;
}

nlohmann::ordered_json WavesBehavior::Wave::Write() const
{
	nlohmann::ordered_json data;

	data["WaveTime"] = remainingTime;
	data["WaveNextTime"] = timeToNextWave;
	nlohmann::ordered_json& eGroups = data["EnemyGroups"];
	for (int i = 0; i < groups.size(); i++)
	{
		eGroups.push_back(groups[i].Write());
	}

	return data;
}

nlohmann::ordered_json WavesBehavior::EnemyGroup::Write() const
{
	nlohmann::ordered_json data;

	data["EnemyName"] = name;
	data["Amount"] = enemyAmount;
	data["SpawnInterval"] = spawnInterval;
	data["InitialOffset"] = offset;
	data["Spawner"] = spawner;


	return data;
}

/// @brief read method map
ReadMethodMap<WavesBehavior> const WavesBehavior::s_ReadMethods =
{
	{ "AlternateSpawners",	  &readSpawners},
	{ "Waves",				  &readWaves},
};

ReadMethodMap<WavesBehavior::Wave> const WavesBehavior::Wave::s_ReadMethods =
{
	{ "WaveTime",			  &readWaveTime},
	{ "WaveNextTime",		  &readNextTime},
	{ "EnemyGroups",		  &readGroups},
};

ReadMethodMap<WavesBehavior::EnemyGroup> const WavesBehavior::EnemyGroup::s_ReadMethods =
{
	{ "EnemyName",			  &readName},
	{ "Amount",				  &readAmount},
	{ "SpawnInterval",		  &readInterval},
	{ "InitialOffset",		  &readOffset},
	{ "Spawner",			  &readSpawner},
};