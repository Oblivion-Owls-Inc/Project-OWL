/*********************************************************************
* \file   WinState.cpp
* \brief  Keeps track of inventory of an entity, switches to a scene
*		  when specified amount of specified item is reached.
*
* \author Eli Tsereteli
*********************************************************************/
#include "BehaviorSystem.h"
#include "WinState.h"
#include "EntitySystem.h"
#include "Entity.h"
#include "Inventory.h"
#include "SceneSystem.h"

/// @brief  Default constructor
WinState::WinState() : Behavior(typeid(WinState)) {}

/// @brief  Return a copy of this component
Component* WinState::Clone() const {return new WinState(*this); }

/// @brief  copy ctor
WinState::WinState(const WinState& other) : Behavior(other) {}



//-----------------------------------------------------------------------------
//              Overrides
//-----------------------------------------------------------------------------

/// @brief  Adds itself to behavior system
void WinState::OnInit() { Behaviors< Behavior >()->AddComponent(this); }

/// @brief  Removes itself from behavior system
void WinState::OnExit() { Behaviors< Behavior >()->RemoveComponent(this); }


/// @brief  Keeps an eye on item count
void WinState::OnUpdate(float dt)
{
	// Caching is a pain in the ass and crash-prone. I'll just let it be a bit slow for now.
	Entity* potentialItemHolder = Entities()->GetEntity(m_EntityName);
	if (!potentialItemHolder)
		return;
	
	Inventory* inv = potentialItemHolder->GetComponent<Inventory>();
	if (!inv)
		return;

	std::vector< ItemStack > const& items = inv->GetItems();

	for (ItemStack const& item : items)
	{
		if (item.M_ItemId == m_Item && item.M_Count >= m_WinCount)
			Scenes()->SetNextScene(m_WinSceneName);
	}
}


/// @brief  Tweak properties in debug window
void WinState::Inspector()
{
	ImGui::InputText("Entity that holds items", &m_EntityName);
	ImGui::InputInt("Winning item ID", &m_Item);
	ImGui::InputInt("How many to win", &m_WinCount);
	ImGui::InputText("Scene to switch to", &m_WinSceneName);
}



//-----------------------------------------------------------------------------
//              Reading / Writing
//-----------------------------------------------------------------------------

/// @brief read method map
ReadMethodMap<WinState> const WinState::s_ReadMethods =
{
	{"Entity",		&readEntityName},
	{"ItemID",		&readItemID},
	{"ItemCount",	&readItemCount},
	{"WinScene",    &readWinScene}
};

/// @brief		 Reads name of the entity that holds items
/// @param data  json data to read
void WinState::readEntityName(nlohmann::ordered_json const& data)
{
	Stream::Read(m_EntityName, data);
}


/// @brief		 Reads winning item ID
/// @param data  json data to read
void WinState::readItemID(nlohmann::ordered_json const& data)
{
	Stream::Read(m_Item, data);
}


/// @brief		 Reads winning item count
/// @param data  json data to read
void WinState::readItemCount(nlohmann::ordered_json const& data)
{
	Stream::Read(m_WinCount, data);
}


/// @brief		 Reads winning scene name
/// @param data  json data to read
void WinState::readWinScene(nlohmann::ordered_json const& data)
{
	Stream::Read(m_WinSceneName, data);
}


/// @brief	write to json
nlohmann::ordered_json WinState::Write() const
{
	nlohmann::ordered_json data;

	data["Entity"] = m_EntityName;
	data["ItemID"] = m_Item;
	data["ItemCount"] = m_WinCount;
	data["WinScene"] = m_WinSceneName;

	return data;
}

