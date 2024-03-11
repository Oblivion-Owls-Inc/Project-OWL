/*********************************************************************
* \file       DigEffect.h
* \author     Eli Tsereteli
* 
* \brief      Controls the temporary entity used for emitting particles
*             when a tile breaks. 
* 
* \date       March 2024
* \copyright  Copyright (c) 2024 Digipen Instutute of Technology
*********************************************************************/
#pragma once
#include "Behavior.h"
#include "ComponentReference.h"
#include "AssetReference.h"
#include "Tilemap.h"


// fwd refs
class Emitter;
class MiningLaser;
class TilemapSprite;

/// @brief    
class DigEffect : public Behavior
{
//-----------------------------------------------------------------------------
public: // constructors / destructors
//-----------------------------------------------------------------------------

    /// @brief  Default constructor
    DigEffect();

    /// @brief  Returns copy of this behavior
    DigEffect* Clone() const override;
    
private:
    /// @brief  copy ctor
    DigEffect(const DigEffect& other);



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

    __inline void SetTimer(float sec) { m_Timer = sec; }


//-----------------------------------------------------------------------------
//              Data
//-----------------------------------------------------------------------------
private:

    /// @brief   When timer runs out, entity is destroyed. If timer is 0, this
    ///          isn't considered a temporary entity.
    float m_Timer = 0.0f;

    TilemapSprite* m_TSprite = nullptr;  // TODO: Link errors when using component reference. Figure it out.

    /// @brief   Reference to archetype version of this entity
    AssetReference< Entity > m_Archetype;


//-----------------------------------------------------------------------------
//              Helpers
//-----------------------------------------------------------------------------
private:

    /// @brief          Spawns new temporary entity at the broken tile's location
    /// @param tilemap  pointer to tilemap whose tile just broke
    /// @param tilePos  2D index of the broken tile
    /// @param tileId   old ID/frame of the changed tile
    void spawnTemp(Tilemap< int >* tilemap, glm::ivec2 const& tilePos, int prevTileId);


//-----------------------------------------------------------------------------
//              Reading / Writing
//-----------------------------------------------------------------------------
private:

    /// @brief        reads the timer value
    /// @param data   json to read from
    void readTimer(nlohmann::ordered_json const& data);

    void readArchetype(nlohmann::ordered_json const& data);


    /// @brief   the map of read methods for this Component
    static ReadMethodMap< DigEffect > const s_ReadMethods;


public:

    /// @brief   gets the map of read methods for this Component
    /// @return  the map of read methods for this Component
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override
    {
        return (ReadMethodMap< ISerializable > const&)s_ReadMethods;
    }

    /// @brief      Write all DigEffect component data to a JSON file.
    /// @return     The JSON file containing the DigEffect component data.
    virtual nlohmann::ordered_json Write() const override;
};

