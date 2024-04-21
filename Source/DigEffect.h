/***************************************************************************
* \file       DigEffect.h
* \author     Eli Tsereteli (ilya.tsereteli@digipen.edu)
* 
* \brief      Controls the temporary entity used for emitting particles
*             when a tile breaks. If timer is initialized to 0, acts as 
*             the spawner of temporary entities (should be attached to 
*             the map entity). If timer is greater than 0, simply destroys
*             parent entty when timer runs out. (should be attached to prefab)
* 
* \date       March 2024
* \copyright  Copyright (c) 2024 Digipen Instutute of Technology
****************************************************************************/
#pragma once
#include "Behavior.h"
#include "ComponentReference.h"
#include "AssetReference.h"
#include "Tilemap.h"
#include "Sound.h"


// fwd refs
class Emitter;
class MiningLaser;
class Texture;
class AudioPlayer;

/// @brief    Behavior class for the dig particle effect.
class DigEffect : public Behavior
{
//-----------------------------------------------------------------------------
//              Constructors/destructors
//-----------------------------------------------------------------------------
public:

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
    /// @brief  Adds itself to behavior system, inits references/callbacks
    virtual void OnInit() override;

    /// @brief  Executed every frame: runs the timer
    virtual void OnUpdate(float dt) override;

    /// @brief  Removes itself from behavior system, clears references
    virtual void OnExit() override;
    
    /// @brief  Tweak properties in debug window
    virtual void Inspector() override;


//-----------------------------------------------------------------------------
//              Accessors
//-----------------------------------------------------------------------------
public:

    /// @brief      Timer setter
    /// @param sec  time, in seconds
    __inline void SetTimer(float sec) { m_Timer = sec; }

    /// @brief      Timer getter
    /// @return     time, in seconds
    __inline float GetTimer() const { return m_Timer; }


//-----------------------------------------------------------------------------
//              Data
//-----------------------------------------------------------------------------
private:

    /// @brief   When timer runs out, entity is destroyed. If timer is 0, this
    ///          isn't a temporary entity, but rather one that spawns temps.
    float m_Timer = 0.0f;

    /// @brief   Texture to use for the particles
    AssetReference< Texture > m_Texture;

    /// @brief   Audio player for breaking sound
    ComponentReference< AudioPlayer > m_AudioPlayer;

    /// @brief   Different breaking sound for each type of block
    std::vector< AssetReference< Sound > > m_BreakSounds;

    /// @brief   Archetype to spawn when a tile breaks
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

    /// @brief        reads the archetype to spawn
    /// @param data   json to read from
    void readArchetype(nlohmann::ordered_json const& data);
    
    /// @brief        reads the texture to use for particles
    /// @param data   json to read from
    void readTexture(nlohmann::ordered_json const& data);

    /// @brief        reads the breaking sounds
    /// @param data   json to read from
    void readSounds(nlohmann::ordered_json const& data);


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

