///*****************************************************************/
/// @file	    DoomsDay.h
/// @author     Tyler Birdsall (tyler.birdsall@digipen.edu)
/// @date	    04/14/2024
/// @brief      DoomsDay class header
/// @copyright  Digipen LLC (c) 2024
///*****************************************************************/

#pragma once

#include "Component.h"

#include "ComponentReference.h"
#include "AudioPlayer.h"
#include "Transform.h"
#include "Collider.h"
#include "Health.h"
#include "Sprite.h"
#include "Emitter.h"
#include "EmitterSprite.h"
#include "PathfinderTarget.h"
#include "Light.h"
#include "Interactable.h"
#include "ItemStack.h"

class DoomsDay : public Behavior
{
//-----------------------------------------------------------------------------
public: // constructor / destructors
//-----------------------------------------------------------------------------

    /// @brief  constructor
    DoomsDay();

    /// @brief DoomsDay clone
    /// @return Component pointer of the cloned DoomsDay behavior
    virtual DoomsDay* Clone() const override;

//-----------------------------------------------------------------------------
public: // virtual override methods
//-----------------------------------------------------------------------------

    /// @brief called when DoomsDay initializes
    virtual void OnInit() override;

    /// @brief called when DoomsDay exits
    virtual void OnExit() override;

    /// @brief  called every frame
    /// @param  dt delta time
    virtual void OnFixedUpdate() override;

//-----------------------------------------------------------------------------
private: // copying
//-----------------------------------------------------------------------------

    /// @brief copy ctor
    DoomsDay(const DoomsDay& other);

//-----------------------------------------------------------------------------
public: // accessors
//-----------------------------------------------------------------------------

    

//-----------------------------------------------------------------------------
public: // methods
//-----------------------------------------------------------------------------

    void SetHasLost();

//-----------------------------------------------------------------------------
private: // variables
//-----------------------------------------------------------------------------

    /// @brief  event to cast on loss
    std::string m_LossEventName;

    /// @brief  movement speeds
    float m_CatchupSpeed;
    float m_NormalSpeed;
    float m_CloseSpeed;

    /// @brief  speed distances
    float m_CatchupDistance;
    float m_NormalDistance;
    float m_LoseDistance;

    /// @brief  have we lost the game
    bool m_HasLost = false;

    ComponentReference<Transform> m_Transform;

//-----------------------------------------------------------------------------
private: // private functions
//-----------------------------------------------------------------------------

    void setCatchupSpeed();
    void setNormalSpeed();
    void setCloseSpeed();

//-----------------------------------------------------------------------------
private: // inspector methods
//-----------------------------------------------------------------------------

    /// @brief Used by the Debug System to display information about this Component
    virtual void Inspector() override;

//-----------------------------------------------------------------------------
private: // reading
//-----------------------------------------------------------------------------

    /// @brief the map of read methods for this Component
    static ReadMethodMap< DoomsDay > const s_ReadMethods;

    /// @brief gets the map of read methods for this Component
    /// @return the map of read methods for this Component
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override
    {
        return (ReadMethodMap< ISerializable > const&)s_ReadMethods;
    }

    /// @brief read the catchup speed
    void readCatchupSpeed(nlohmann::ordered_json const& json);

    /// @brief read the normal speed
    void readNormalSpeed(nlohmann::ordered_json const& json);

    /// @brief read the close speed
    void readCloseSpeed(nlohmann::ordered_json const& json);

    /// @brief read the catchup distance
    void readCatchupDistance(nlohmann::ordered_json const& json);

    /// @brief read the normal distance
    void readNormalDistance(nlohmann::ordered_json const& json);

    /// @brief read the lose distance
    void readLoseDistance(nlohmann::ordered_json const& json);


//-----------------------------------------------------------------------------
public: // writing
//-----------------------------------------------------------------------------

    /// @brief Write all DoomsDay data to a JSON file.
    /// @return The JSON file containing the WavesBehavior data.
    virtual nlohmann::ordered_json Write() const override;

//-----------------------------------------------------------------------------
};