/*********************************************************************
* \file   WinState.cpp
* \brief  Keeps track of inventory of an entity, switches to a scene
*		  when specified amount of specified item is reached.
*
* \author Eli Tsereteli
*********************************************************************/
#include "BehaviorSystem.h"
#include "WinState.h"
#include "Entity.h"
#include "Inventory.h"
#include "SceneSystem.h"

#include "ComponentReference.t.h"

/// @brief  Default constructor
WinState::WinState() : Behavior(typeid(WinState)) {}

/// @brief  copy ctor
WinState::WinState(const WinState& other) : Behavior(other) {}

/// @brief  Return a copy of this component
WinState* WinState::Clone() const { return new WinState(*this); }



//-----------------------------------------------------------------------------
//              Overrides
//-----------------------------------------------------------------------------

/// @brief  Adds itself to behavior system
void WinState::OnInit() 
{ 
	Behaviors< Behavior >()->AddComponent(this); 

	// Supposedly, this entity's parent is the player. Get their inventory.
	m_Inventory.Init( GetEntity()->GetParent() );
}

/// @brief  Removes itself from behavior system
void WinState::OnExit() 
{ 
	Behaviors< Behavior >()->RemoveComponent(this);

	m_Inventory.Exit();
}


/// @brief  Keeps an eye on item count
void WinState::OnUpdate(float dt)
{
	if (!m_Inventory)
		return;

	std::vector< ItemStack > const& items = m_Inventory->GetItems();

	for (ItemStack const& item : items)
	{
		if (item.M_ItemId == m_Item && item.M_Count >= m_WinCount)
			Scenes()->SetNextScene(m_WinSceneName);
	}
}


/// @brief  Tweak properties in debug window
void WinState::Inspector()
{
	ImGui::InputInt("Winning item ID", &m_Item);
	ImGui::InputInt("How many to win", &m_WinCount);
	ImGui::InputText("Scene to switch to", &m_WinSceneName);
}


/// @brief  What to do when entity is re-parented
void WinState::OnHierarchyChange(Entity* previousParent)
{
	// re-init inventory reference
	m_Inventory.Exit();
	m_Inventory.Init( GetEntity()->GetParent() );
}


//-----------------------------------------------------------------------------
//              Reading / Writing
//-----------------------------------------------------------------------------

/// @brief read method map
ReadMethodMap<WinState> const WinState::s_ReadMethods =
{
	{"ItemID",		&readItemID},
	{"ItemCount",	&readItemCount},
	{"WinScene",    &readWinScene}
};

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

	data["ItemID"] = m_Item;
	data["ItemCount"] = m_WinCount;
	data["WinScene"] = m_WinSceneName;

	return data;
}

