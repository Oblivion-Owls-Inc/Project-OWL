///--------------------------------------------------------------------------//
/// @file   WavesInspector.cpp
/// @brief  Definitions for wave and spawning editing in game
/// 
/// @author Tyler Birdsall (tyler.birdsall)
/// @date   10/27/23
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
#include "RenderSystem.h"
#include "Texture.h"
#include "InputSystem.h"
#include "Inspection.h"



//-----------------------------------------------------------------------------
// private: inspector methods
//-----------------------------------------------------------------------------

/// @brief displays wave data to edit
void WavesBehavior::guiWaves()
{
	ImGui::Text("Total Waves: %i", numWaves);
	// add/remove waves
	if (ImGui::Button("Add Wave"))
	{
		guiAddWave();
	}
	if (ImGui::Button("Remove Wave"))
	{
		guiRemoveWave();
	}
	
	if (numWaves > 0)
	{

		ImGui::InputInt("Wave in View", &inspectorWave);
		if (inspectorWave >= numWaves)
		{
			inspectorWave = numWaves - 1;
		}
		else if (inspectorWave < 0)
		{
			inspectorWave = 0;
		}
		// allows reordering of waves
		ImGui::PushID(4);
		if (ImGui::BeginCombo("Move Wave", "Move Wave in View to Wave X:"))
		{
			for (int i = 0; i < numWaves; i++)
			{
				char text[32] = { 0 };
				snprintf(text, sizeof(text), "Wave %i", i);
				if (ImGui::Selectable(text, false))
				{
					int offset = inspectorWave - i;
					if (offset == 0)
					{
						break;
					}
					else if (offset < 0)
					{
						for (int j = 0; j > offset; j--)
						{
							std::swap(waves[inspectorWave - j], 
								waves[inspectorWave - j + 1]);
						}
					}
					else if (offset > 0)
					{
						for (int j = 0; j < offset; j++)
						{
							std::swap(waves[inspectorWave - j],
								waves[inspectorWave - j - 1]);
						}
					}
					break;
				}
			}
			ImGui::EndCombo();
		}
		ImGui::PopID();
		// time values
		ImGui::InputFloat("Time to Next Wave",
			&waves[inspectorWave].timeToNextWave);

		ImGui::Text("");
	}
}

/// @brief displays group data to edit
void WavesBehavior::guiGroups()
{
	ImGui::Text("Number of Groups in this Wave: %i", waves[inspectorWave].groups.size());
	// buttons to add/remove a group
	ImGui::PushID(1);
	if (ImGui::Button("Add Group"))
	{
		guiAddGroup();
	}
	if (ImGui::Button("Remove Group"))
	{
		guiRemoveGroup();
	}
	
	ImGui::PopID();
	if (waves[inspectorWave].groups.size() > 0)
	{
		if (inspectorGroup >= waves[inspectorWave].groups.size())
		{
			inspectorGroup = (int)waves[inspectorWave].groups.size() - 1;
		}
		ImGui::InputInt("Group in View", &inspectorGroup);
		if (inspectorGroup >= waves[inspectorWave].groups.size())
		{
			inspectorGroup = (int)waves[inspectorWave].groups.size() - 1;
		}
		else if (inspectorGroup < 0)
		{
			inspectorGroup = 0;
		}
		// allows reordering groups
		ImGui::PushID(4);
		if (ImGui::BeginCombo("Move Group", "Move Group in View to Group X:"))
		{
			for (int i = 0; i < waves[inspectorWave].groups.size(); i++)
			{
				char text[32] = { 0 };
				snprintf(text, sizeof(text), "Group %i", i);
				if (ImGui::Selectable(text, false))
				{
					int offset = inspectorGroup - i;
					if (offset == 0)
					{
						break;
					}
					else if (offset < 0)
					{
						for (int j = 0; j > offset; j--)
						{
							std::swap(waves[inspectorWave].groups[inspectorGroup - j],
								waves[inspectorWave].groups[inspectorGroup - j + 1]);
						}
					}
					else if (offset > 0)
					{
						for (int j = 0; j < offset; j++)
						{
							std::swap(waves[inspectorWave].groups[inspectorGroup - j],
								waves[inspectorWave].groups[inspectorGroup - j - 1]);
						}
					}
					break;
				}
			}
			ImGui::EndCombo();
		}
		ImGui::PopID();

		ImGui::PushID(3);
		waves[inspectorWave].groups[inspectorGroup].enemy.Inspect( "enemy prefab" );
		ImGui::PopID();

		// general group info
		ImGui::InputInt("Enemies", &waves[inspectorWave].groups[inspectorGroup].enemyAmount);

		ImGui::InputInt("Spawner", &waves[inspectorWave].groups[inspectorGroup].spawner);
		int size = (int)spawners.size();
		if (waves[inspectorWave].groups[inspectorGroup].spawner >= size)
		{
			waves[inspectorWave].groups[inspectorGroup].spawner = size - 1;
		}
		if (waves[inspectorWave].groups[inspectorGroup].spawner < 0)
		{
			waves[inspectorWave].groups[inspectorGroup].spawner = 0;
		}

		ImGui::InputFloat("Spawn Interval", &waves[inspectorWave].groups[inspectorGroup].spawnInterval);
		ImGui::InputFloat("Initial Spawn Offset", &waves[inspectorWave].groups[inspectorGroup].offset);

	}
	ImGui::Text("");
}

/// @brief displays the currently active wave data
void WavesBehavior::guiInViewWave()
{

	if (numWaves > 0)
	{
		ImGui::Text("Wave: %i", inspectorWave);
		ImGui::Text("Time to Next Wave: %f", waves[inspectorWave].timeToNextWave);
	}
	else
	{
		ImGui::Text("No Waves");
	}
}

/// @brief displays the currently active enemy groups data
void WavesBehavior::guiInViewGroups()
{
	if (numWaves > 0)
	{
		for (int i = 0; i < waves[inspectorWave].groups.size(); i++)
		{
			ImGui::Text("Group: %i", i + 1);
			ImGui::Text("Enemy Type: %s", waves[inspectorWave].groups[i].enemy.GetName().c_str());
			ImGui::Text("Amount: %i", waves[inspectorWave].groups[i].enemyAmount);
			ImGui::Text("Spawner: %i", waves[inspectorWave].groups[i].spawner);
			ImGui::Text("Timer: %f", waves[inspectorWave].groups[i].timer);
			ImGui::Text("");
		}
	}
}

/// @brief displays the currently active wave data
void WavesBehavior::guiCurrentWave()
{

	if (currentWave < numWaves)
	{
		ImGui::Text("Wave: %i", currentWave + 1);
		ImGui::Text("Time to Next Wave: %f", waves[currentWave].timeToNextWave);
	}
	else
	{
		ImGui::Text("No Current Wave");
	}
}

/// @brief displays the currently active enemy groups data
void WavesBehavior::guiCurrentGroups()
{
	if (currentWave < numWaves)
	{
		for (int i = 0; i < waves[currentWave].groups.size(); i++)
		{
			ImGui::Text("Group: %i", i + 1);
			ImGui::Text("Enemy Type: %s", waves[currentWave].groups[i].enemy.GetName().c_str());
			ImGui::Text("Amount: %i", waves[currentWave].groups[i].enemyAmount);
			ImGui::Text("Spawner: %i", waves[currentWave].groups[i].spawner);
			ImGui::Text("Timer: %f", waves[currentWave].groups[i].timer);
			ImGui::Text("");
		}
	}
}

/// @brief calls the in view gui handlers
void WavesBehavior::guiInView()
{
	char title[20];
	snprintf(title, sizeof(title), "Inspected Wave Data:");
	if (ImGui::TreeNode(title))
	{
		guiInViewWave();
		ImGui::Text("");
		guiInViewGroups();
		ImGui::TreePop();
	}
}

/// @brief calls the current gui handlers
void WavesBehavior::guiCurrent()
{
	char title[20];
	snprintf(title, sizeof(title), "Current Wave Data:");
	if (ImGui::TreeNode(title))
	{
		guiCurrentWave();
		ImGui::Text("");
		guiCurrentGroups();
		ImGui::TreePop();
	}
}

/// @brief  allows dragging of Transforms with the mouse
void WavesBehavior::debugDrag(int number)
{
	float constexpr maxDragRange = 1.0f;

	static unsigned draggedTransformId = 0;
	static float closestDistanceSquared = maxDragRange * maxDragRange;
	static int num = -1;

	if (Input()->GetMouseTriggered(GLFW_MOUSE_BUTTON_1))
	{
		glm::vec2 mousePos = true ? Input()->GetMousePosWorld() : Input()->GetMousePosUI();
		glm::vec2 offset = spawners[number] - mousePos;
		float distanceSquared = glm::dot(offset, offset);
		if (distanceSquared <= closestDistanceSquared)
		{
			closestDistanceSquared = distanceSquared;
			draggedTransformId = GetId();
			num = number;
		}
	}
	else if (
		draggedTransformId == GetId() &&
		Input()->GetMouseDown(GLFW_MOUSE_BUTTON_1)
		&& num == number)
	{
		glm::vec2 mousePos = true ? Input()->GetMousePosWorld() : Input()->GetMousePosUI();
		spawners[number] = mousePos;
	}
	else if (Input()->GetMouseReleased(GLFW_MOUSE_BUTTON_1))
	{
		draggedTransformId = 0;
		closestDistanceSquared = maxDragRange * maxDragRange;
		num = -1;
	}
}

/// @brief	adds a wave to the behavior
void WavesBehavior::guiAddWave()
{
	numWaves++;
	if (waves.size() >= numWaves)
	{
		inspectorWave = numWaves - 1;
		return;
	}
	else
	{
		Wave newWave = Wave();
		waves.push_back(newWave);
		inspectorWave = numWaves - 1;
	}
}

/// @brief	removes a wave from the behavior
void WavesBehavior::guiRemoveWave()
{
	if (numWaves > 0)
	{
		numWaves--;
	}
}

/// @brief	adds a group to the behavior
void WavesBehavior::guiAddGroup()
{
	EnemyGroup newGroup = EnemyGroup();
	waves[inspectorWave].groups.push_back(newGroup);
	inspectorGroup = (int)waves[inspectorWave].groups.size() - 1;
}

/// @brief	removes a group from the behavior
void WavesBehavior::guiRemoveGroup()
{
	if (waves[inspectorWave].groups.size() > 0)
	{
		waves[inspectorWave].groups.pop_back();
	}
}

/// @brief	adds a spawner to the behavior
void WavesBehavior::guiAddSpawner()
{
	glm::vec2 vec(0, 0);
	spawners.push_back(vec);
}

/// @brief	removes a spawner from the behavior
void WavesBehavior::guiRemoveSpawner()
{
	if (spawners.size() > 0)
	{
		spawners.pop_back();
	}
}

/// @brief lists all spawners and locations
void WavesBehavior::guiSpawners()
{
	// general inspector info
	char title[20];
	snprintf(title, sizeof(title), "Spawners:");
	if (ImGui::TreeNode(title))
	{
		for (int i = 0; i < spawners.size(); i++)
		{
			ImGui::Text("Spawner: %i", i);
			char location[20] = {0};
			snprintf(location, sizeof(location), "Location %i:", i);
			ImGui::DragFloat2("", &spawners[i][0], 0.05f);
			int size = (int)spawners.size();
			ImGui::PushID(i);
			// arrow buttons to reorder spawners
			if (size > 1)
			{
				ImGui::SameLine();
				if (i == 0)
				{
					if (ImGui::ArrowButton("D", 3))
					{
						std::swap(spawners[0], spawners[1]);
					}
					ImGui::SameLine();
					if (ImGui::ArrowButton("U", 2))
					{
						std::swap(spawners[0], spawners[spawners.size() - 1]);
					}
				}
				else if (i + 1 == size)
				{
					if (ImGui::ArrowButton("D", 3))
					{
						std::swap(spawners[i], spawners[0]);
					}
					ImGui::SameLine();
					if (ImGui::ArrowButton("U", 2))
					{
						std::swap(spawners[i], spawners[i - 1]);
					}
				}
				else
				{
					if (ImGui::ArrowButton("D", 3))
					{
						std::swap(spawners[i], spawners[i + 1]);
					}
					ImGui::SameLine();
					if (ImGui::ArrowButton("U", 2))
					{
						std::swap(spawners[i], spawners[i - 1]);
					}
				}

			}
			ImGui::PopID();
			Renderer()->DrawTexture(Transform::GetWidgetTexture(), spawners[i], glm::vec2(1), 0, glm::vec4(0), 1.0f, true);
			debugDrag(i);
		}
		// buttons to add/remove spawners
		ImVec2 vector;
		vector.x = 20;
		vector.y = 20;
		ImGui::PushID(2);
		if (ImGui::Button("-", vector))
		{
			guiRemoveSpawner();
		}
		ImGui::SameLine();
		if (ImGui::Button("+", vector))
		{
			guiAddSpawner();
		}
		ImGui::PopID();
		ImGui::TreePop();
	}
}

/// @brief handles displaying ImGui inspectors
void WavesBehavior::Inspector()
{
	///Edit the Behavior of the Waves 
	guiWaves();

	if (numWaves > 0)
	{
		guiGroups();
	}

	guiInView();

	guiCurrent();

	guiSpawners();
	
}
