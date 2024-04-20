/*********************************************************************
* \file   WinState.cpp
* \brief  Keeps track of inventory of an entity, switches to a scene
*		  when specified amount of specified item is reached.
*
* \author Eli Tsereteli
*********************************************************************/

#include "pch.h" // precompiled header has to be included first
#include "BehaviorSystem.h"
#include "WinState.h"
#include "Entity.h"
#include "Inventory.h"
#include "SceneSystem.h"

#include "ComponentReference.t.h"
#include "SceneTransition.h"

#include "Sprite.h"

/// @brief  Default constructor
WinState::WinState() :
    Behavior(typeid(WinState))
{}

/// @brief  copy ctor
WinState::WinState(const WinState& other) :
    Behavior(other),
    m_Item                 ( other.m_Item         ),
    m_WinCount             ( other.m_WinCount     ),
    m_WinSceneName         ( other.m_WinSceneName ),
    m_SceneTransitionEntity( other.m_SceneTransitionEntity, { &m_SceneTransition } ),
    m_EventName            ( other.m_EventName )
{}

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

    m_SceneTransitionEntity.SetOwnerName( GetName() );
    m_SceneTransitionEntity.Init();

    /// Set the filter function for the listener
    m_Listener.SetFilterFunction([&](std::string const& EventNameEnd) -> bool
    {
        return EventNameEnd == m_EventName;
    });

    /// Set the Callback function for the listener
    m_Listener.SetResponseFunction([&](std::string const& EventNameEnd)
    {
        WinTheGame();
    });

    m_Listener.Init();
}

/// @brief  Removes itself from behavior system
void WinState::OnExit() 
{ 
    Behaviors< Behavior >()->RemoveComponent(this);

    m_Inventory.Exit();

    m_SceneTransitionEntity.Exit();
    m_Listener.Exit();
}


/// @brief  Keeps an eye on item count
void WinState::OnUpdate(float dt)
{
    if (!m_Inventory)
        return;

    std::vector< ItemStack > const& items = m_Inventory->GetItems();

    for (ItemStack const& item : items)
    {
        if ( item.M_ItemId == m_Item && item.M_Count >= m_WinCount )
        {
            if ( m_SceneTransition != nullptr )
            {
                m_SceneTransition->StartTransition( m_WinSceneName );
            }
        }
    }
}

/// @brief  Win the game
void WinState::WinTheGame()
{
    if (m_SceneTransition != nullptr)
    {
        m_SceneTransition->StartTransition(m_WinSceneName);
    }
}


/// @brief  Tweak properties in debug window
void WinState::Inspector()
{
    ImGui::InputInt("Winning item ID", &m_Item);
    ImGui::InputInt("How many to win", &m_WinCount);
    Scenes()->InspectorSelectScene( "Scene to switch to", &m_WinSceneName );

    m_SceneTransitionEntity.Inspect( "Scene Transition Entity" );

    ImGui::InputText("Event Name", &m_EventName);
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
    { "ItemID"               , &WinState::readItemID                },
    { "ItemCount"            , &WinState::readItemCount             },
    { "WinScene"             , &WinState::readWinScene              },
    { "SceneTransitionEntity", &WinState::readSceneTransitionEntity },
    { "EventName"            , &WinState::readEventName             },
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

/// @brief  reads the Entity the SceneTransition Component is attached to
/// @param  data    the JSON data to read from
void WinState::readSceneTransitionEntity( nlohmann::ordered_json const& data )
{
    Stream::Read( m_SceneTransitionEntity, data );
}

/// @brief  reads the Event name to listen for
/// @param  data    the JSON data to read from
void WinState::readEventName(nlohmann::ordered_json const& data)
{
    Stream::Read(m_EventName, data);
}


/// @brief	write to json
nlohmann::ordered_json WinState::Write() const
{
    nlohmann::ordered_json data;

    data[ "ItemID"                ] = Stream::Write( m_Item                  );
    data[ "ItemCount"             ] = Stream::Write( m_WinCount              );
    data[ "WinScene"              ] = Stream::Write( m_WinSceneName          );
    data[ "SceneTransitionEntity" ] = Stream::Write( m_SceneTransitionEntity );
    data[ "EventName"        ] = m_EventName;

    return data;
}

