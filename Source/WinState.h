/*********************************************************************
* \file   WinState.h
* \brief  Keeps track of inventory of an entity, switches to a scene
*		  when specified amount of specified item is reached.
*
* \author Eli Tsereteli
*********************************************************************/
#pragma once
#include "Behavior.h"

class WinState : public Behavior
{
//-----------------------------------------------------------------------------
public: // constructor / destructors
//-----------------------------------------------------------------------------

    /// @brief  Default constructor
    WinState();

    /// @brief  Returns copy of this behavior
    Component* Clone() const override;
    
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



//-----------------------------------------------------------------------------
//              Data
//-----------------------------------------------------------------------------
private:
    // (temporary, will need names once the items have them)
    
    /// @brief  Which item to keep track of
    int m_Item = 0;

    /// @brief  How many needed to win
    int m_WinCount = 10;

    /// @brief  Name of entity that holds items (Player probably)
    std::string m_EntityName = "Player";

    std::string m_WinSceneName = "GameWin";


//-----------------------------------------------------------------------------
//              Reading / Writing
//-----------------------------------------------------------------------------
private:
    /// @brief   the map of read methods for this Component
    static ReadMethodMap< WinState > const s_ReadMethods;

    /// @brief		 Reads name of the entity that holds items
    /// @param data  json data to read
    void readEntityName(nlohmann::ordered_json const& data);

    /// @brief		 Reads winning item ID
    /// @param data  json data to read
    void readItemID(nlohmann::ordered_json const& data);

    /// @brief		 Reads winning item count
    /// @param data  json data to read
    void readItemCount(nlohmann::ordered_json const& data);

    /// @brief		 Reads win scene's name
    /// @param data  json data to read
    void readWinScene(nlohmann::ordered_json const& data);

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

