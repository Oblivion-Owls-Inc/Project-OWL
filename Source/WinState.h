/*********************************************************************
* \file   WinState.h
* \brief  Keeps track of inventory of an entity, switches to a scene
*		  when specified amount of specified item is reached.
*
* \author Eli Tsereteli
* \copyright Copyright (c) 2024 Digipen Institute of Technology
*********************************************************************/
#pragma once
#include "Behavior.h"
#include "ComponentReference.h"
#include "Inventory.h"
#include "EventSystem.h"
#include "EventListener.h"

#include "EntityReference.h"
class SceneTransition;

class WinState : public Behavior
{
//-----------------------------------------------------------------------------
public: // constructors / destructors
//-----------------------------------------------------------------------------

    /// @brief  Default constructor
    WinState();

    /// @brief  Returns copy of this behavior
    WinState* Clone() const override;
    
private:
    /// @brief  copy ctor
    WinState(const WinState& other);



//-----------------------------------------------------------------------------
//              Overrides
//-----------------------------------------------------------------------------
private:
    /// @brief  Adds itself to behavior system
    virtual void OnInit() override;

    /// @brief  Keeps an eye on item count
    virtual void OnUpdate(float dt) override;

    /// @brief  Removes itself from behavior system
    virtual void OnExit() override;
    
    /// @brief  Tweak properties in debug window
    virtual void Inspector() override;

    /// @brief  What to do when entity is re-parented
    virtual void OnHierarchyChange(Entity* previousParent);


//-----------------------------------------------------------------------------
//              Public methods
//-----------------------------------------------------------------------------
public:
    /// @brief   Sets the ID of winning item 
    __inline void SetWinItemID(int id) { m_Item = id; }

    /// @return  ID of winning item
    __inline int GetWinItemID() const { return m_Item; }

    /// @brief   Sets the item count required to win
    __inline void SetWinItemCount(int count) { m_WinCount = count; }

    /// @return  Item count required to win
    __inline int GetWinItemCount() const { return m_WinCount; }

    /// @brief   Sets the scene to switch to upon meeting item count requirement 
    __inline void SetWinSceneName(std::string scene) { m_WinSceneName = scene; }

    /// @return  Current win scene name 
    __inline std::string GetWinSceneName() const { return m_WinSceneName; }

    /// @brief   Win the game
    void WinTheGame();



//-----------------------------------------------------------------------------
//              Data
//-----------------------------------------------------------------------------
private:
    // (temporary, will need names once the items have them)
    
    /// @brief  Which item to keep track of
    int m_Item = 0;

    /// @brief  How many needed to win
    int m_WinCount = 10;

    /// @brief  Name of the scene to switch to upon meeting the requirement
    std::string m_WinSceneName = "GameWin";

    /// @brief Event Listener
    EventListener<std::string> m_Listener;

    /// @brief  the name of the event to listen for
    std::string m_EventName;

    /// @brief  the SceneTransition Component responsible for changing scenes
    ComponentReference< SceneTransition > m_SceneTransition;

    /// @brief  the Entity the SceneTransition Component is attached to
    EntityReference m_SceneTransitionEntity = EntityReference( { &m_SceneTransition } );


    /// @brief  Item holder's inventory
    ComponentReference< Inventory > m_Inventory;

//-----------------------------------------------------------------------------
//              Reading / Writing
//-----------------------------------------------------------------------------
private:
    /// @brief   the map of read methods for this Component
    static ReadMethodMap< WinState > const s_ReadMethods;

    /// @brief		 Reads winning item ID
    /// @param data  json data to read
    void readItemID(nlohmann::ordered_json const& data);

    /// @brief		 Reads winning item count
    /// @param data  json data to read
    void readItemCount(nlohmann::ordered_json const& data);

    /// @brief		 Reads win scene's name
    /// @param data  json data to read
    void readWinScene(nlohmann::ordered_json const& data);

    /// @brief  reads the Entity the SceneTransition Component is attached to
    /// @param  data    the JSON data to read from
    void readSceneTransitionEntity( nlohmann::ordered_json const& data );

    /// @brief  reads the Event name to listen for
    /// @param  data    the JSON data to read from
    void readEventName(nlohmann::ordered_json const& data);

public:

    /// @brief   gets the map of read methods for this Component
    /// @return  the map of read methods for this Component
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override
    {
        return (ReadMethodMap< ISerializable > const&)s_ReadMethods;
    }

    /// @brief      Write all WinState component data to a JSON file.
    /// @return     The JSON file containing the WinState component data.
    virtual nlohmann::ordered_json Write() const override;
};

