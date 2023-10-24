#include "WavesBehavior.h"
#include "BehaviorSystem.h"
#include "EntitySystem.h"
#include "Transform.h"
#include "Animation.h"
#include "DebugSystem.h"
#include "AssetLibrarySystem.h"
#include "Engine.h"
#include "Tilemap.h"

WavesBehavior::WavesBehavior() :
	Behavior(typeid(WavesBehavior)),
	numWaves(0),
	currentWave(0),
	inspectorWave(0),
	inspectorGroup(0)
{
	waves.clear();
}

WavesBehavior::WavesBehavior(const WavesBehavior& other)
	             : Behavior(typeid(WavesBehavior)), 
					numWaves(other.numWaves),
					currentWave(other.currentWave),
					inspectorWave(other.inspectorWave),
					inspectorGroup(other.inspectorGroup)
{
	waves = other.waves;
}

WavesBehavior::~WavesBehavior()
{
}

WavesBehavior::Wave::Wave() :
	remainingTime(0),
	timeToNextWave(0),
	numGroups(0)
{}

WavesBehavior::enemyGroup::enemyGroup() :
	enemyAmount(0),
	spawnInterval(0),
	timer(0),
	offset(0),
	spawner(0)
{}

Component* WavesBehavior::Clone() const
{
	return (Component*)new WavesBehavior(*this);
}

void WavesBehavior::OnInit()
{
	/// Add this behavior to the behavior system
	BehaviorSystem<WavesBehavior>::GetInstance()->AddBehavior(this);
}

void WavesBehavior::OnExit()
{
	/// Remove this behavior from the behavior system
	BehaviorSystem<WavesBehavior>::GetInstance()->RemoveBehavior(this);
}

void WavesBehavior::OnUpdate(float dt)
{
 /// Do Waves Debug Stuff
}

void WavesBehavior::OnFixedUpdate()
{
	if (currentWave < numWaves)
	{
		float dt = Engine::GetInstance()->GetFixedFrameDuration();
		if (waves[currentWave]->remainingTime > 0.0f)
		{
			waves[currentWave]->remainingTime -= dt;
			int groupNum = 0;
			for (auto groups : waves[currentWave]->groups)
			{
				if (groups->enemyAmount > 0)
				{
					groups->timer -= dt;
					if (groups->timer < 0.0f)
					{
						groups->timer += groups->spawnInterval;
						groups->enemyAmount--;
						Spawn(groups->name, groupNum);
					}
				}
				groupNum++;
			}
		}
		else
		{
			waves[currentWave]->remainingTime = 0.0f;
		}
		waves[currentWave]->timeToNextWave -= dt;
		if (waves[currentWave]->timeToNextWave < 0.0f)
		{
			waves[currentWave]->timeToNextWave = 0.0f;
			currentWave++;
		}
	}
}

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
		&waves[inspectorWave]->remainingTime);
	ImGui::InputFloat("Time to Next Wave",
		&waves[inspectorWave]->timeToNextWave);
	
	ImGui::Text("");
}

void WavesBehavior::GuiGroups()
{
	ImGui::Text("Groups in Wave: %i", waves[inspectorWave]->numGroups);
	if (waves[inspectorWave]->numGroups > 0)
	{
		ImGui::Text("Group: %i", inspectorGroup + 1);
		ImGui::Text("Enemy Type: %s", waves[inspectorWave]->groups[inspectorGroup]->name.c_str());
		ImGui::InputInt("Group", &inspectorGroup);
		if (inspectorGroup >= waves[inspectorWave]->numGroups)
		{
			inspectorGroup = waves[inspectorWave]->numGroups - 1;
		}
		else if (inspectorGroup < 0)
		{
			inspectorGroup = 0;
		}
		ImGui::InputInt("Enemies", &waves[inspectorWave]->groups[inspectorGroup]->enemyAmount);
		ImGui::InputInt("Spawner", &waves[inspectorWave]->groups[inspectorGroup]->spawner);
		int size = (int)spawners.size();
		if (waves[inspectorWave]->groups[inspectorGroup]->spawner >= size)
		{
			waves[inspectorWave]->groups[inspectorGroup]->spawner = size - 1;
		}
		else if (waves[inspectorWave]->groups[inspectorGroup]->spawner < 0)
		{
			waves[inspectorWave]->groups[inspectorGroup]->spawner = 0;
		}
		ImGui::InputFloat("Timer", &waves[inspectorWave]->groups[inspectorGroup]->timer);
		
	}
	ImGui::Text("");
}

void WavesBehavior::GuiCurrentWave()
{
	
	if (currentWave < numWaves)
	{
		ImGui::Text("Wave: %i", currentWave + 1);
		ImGui::Text("Remaining Time: %f", waves[currentWave]->remainingTime);
		ImGui::Text("Time to Next Wave: %f", waves[currentWave]->timeToNextWave);
	}
	else
	{
		ImGui::Text("No Current Wave");
	}
}

void WavesBehavior::GuiCurrentGroups()
{
	if (currentWave < numWaves)
	{
		for (int i = 0; i < waves[currentWave]->groups.size(); i++)
		{
			ImGui::Text("Group: %i", i + 1);
			ImGui::Text("Enemy Type: %s", waves[currentWave]->groups[i]->name.c_str());
			ImGui::Text("Amount: %i", waves[currentWave]->groups[i]->enemyAmount);
			ImGui::Text("Spawner: %i", waves[currentWave]->groups[i]->spawner);
			ImGui::Text("Timer: %f", waves[currentWave]->groups[i]->timer);
			ImGui::Text("");
		}
	}
}

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

void WavesBehavior::GuiSpawners()
{
	char title[20];
	snprintf(title, sizeof(title), "Spawners:");
	if (ImGui::TreeNode(title))
	{
		for (int i = 0; i < spawners.size(); i++)
		{
			glm::vec2 location = spawners[i]->GetTranslation();
			ImGui::Text("Spawner: %i", i);
			ImGui::Text("Location: [ %f %f ]", location.x, location.y);
		}
		ImGui::TreePop();
	}
}

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

float WavesBehavior::getTimer()
{
	if (waves[currentWave]->remainingTime > 0.0f)
	{
		return waves[currentWave]->remainingTime;
	}
	else
	{
		return waves[currentWave]->timeToNextWave;
	}
}

void WavesBehavior::Spawn(std::string name, int group)
{
	const Entity* entity = AssetLibrary<Entity>()->GetAsset(name);
	Entity* copy = new Entity;
	*copy = *entity;
	Transform* copyTransform = copy->GetComponent<Transform>();
	Transform* spawnerTransform;
	
	spawnerTransform = spawners[waves[currentWave]->groups[group]->spawner];
	copyTransform->SetTranslation(spawnerTransform->GetTranslation());
	Entities()->AddEntity(copy);
}

void WavesBehavior::readWaveData(nlohmann::ordered_json const& data)
{
	for (nlohmann::ordered_json const& items : data)
	{
		glm::vec2 times = Stream::Read<2, float>(items);
		Wave* insert = new Wave;
		insert->remainingTime = times[0];
		insert->timeToNextWave = times[1];
		insert->numGroups = 0;
		waves.push_back(insert);
	}
	numWaves = (int)waves.size();
	currentWave = 0;
	inspectorWave = 0;
}

void WavesBehavior::readSpawners(nlohmann::ordered_json const& data)
{
	Transform* home = GetParent()->GetComponent<Transform>();
	spawners.push_back(home);
	for (nlohmann::ordered_json const& entityData : data)
	{
		Entity* entity = new Entity();
		entity->SetName("spawner");
		Transform* transform = new Transform();
		entity->AddComponent(transform);
		transform->SetTranslation(Stream::Read<2, float>(entityData));
		Entities()->AddEntity(entity);
		spawners.push_back(entity->GetComponent<Transform>());
	}
}

void WavesBehavior::readEData(nlohmann::ordered_json const& data)
{
	for (int i = 0; i < data.size(); i++)
	{
		assert(data[i].is_array());
		enemyGroup* insert = new enemyGroup;
		int wave = Stream::Read<int>(data[i][0]);
		insert->name = Stream::Read<std::string>(data[i][1]);
		insert->enemyAmount = Stream::Read<int>(data[i][2]);
		insert->spawnInterval = Stream::Read<float>(data[i][3]);
		insert->offset = Stream::Read<float>(data[i][4]);
		insert->timer = insert->offset;
		insert->spawner = Stream::Read<int>(data[i][5]);
		waves[wave]->groups.push_back(insert);
		waves[wave]->numGroups++;
	}
}

/// @brief Write all WavesBehavior data to a JSON file.
/// @return The JSON file containing the WavesBehavior data.
nlohmann::ordered_json WavesBehavior::Write() const
{
	nlohmann::ordered_json data;

	data["Waves"] = numWaves;
	/*data["range"] = m_Range;
	data["bulletdamage"] = m_BulletDamage;
	data["bulletspeed"] = m_BulletSpeed;
	data["bulletsize"] = m_BulletSize;*/

	return data;
}

ReadMethodMap<WavesBehavior> const WavesBehavior::s_ReadMethods =
{
	{ "WaveData",			  &readWaveData},
	{ "AlternateSpawners",	  &readSpawners},
	{ "EnemyData",				&readEData}
	
	/*{ "bulletsize",		    &readBulletSize }*/
};
