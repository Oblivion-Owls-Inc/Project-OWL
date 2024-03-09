/// @file       MiningLaser.h
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      Digging laser controllable by an entity
/// @version    0.1
/// @date       2024-01-14
/// 
/// @copyright  Copyright (c) 2023 Digipen Institute of Technology

#pragma once

#include "pch.h" // precompiled header has to be included first

#include "basics.h"
#include "Behavior.h"

#include "CollisionLayerFlags.h"

#include "EntityReference.h"
#include "ComponentReference.h"
#include "DestructibleTilemap.h"
class Transform;


/// @brief  Digging laser controllable by an entity
class MiningLaser : public Behavior
{
//-----------------------------------------------------------------------------
public: // constructor / Destructor
//-----------------------------------------------------------------------------

    /// @brief  constructor
    MiningLaser();

    /// @brief  destructor
    ~MiningLaser() = default;

    
//-----------------------------------------------------------------------------
private: // types
//-----------------------------------------------------------------------------


    /// @brief  a tile currently in the process of being broken
    struct InProgressTile
    {
        /// @brief  the position of the tile
        glm::ivec2 M_TilePos;

        /// @brief  how much time remains until the tile is broken
        float remainingTime;
    };


//-----------------------------------------------------------------------------
public: // accessors
//-----------------------------------------------------------------------------

    
    /// @brief  gets the range
    /// @return the range
    float GetRange() const;

    /// @brief  sets the range
    /// @param  range   the new range
    void SetRange( float range );


    /// @brief  gets how quickly the laser breaks blocks
    /// @return how quickly the laser breaks blocks
    float GetMiningSpeed() const;

    /// @brief  sets how quickly the laser breaks blocks
    /// @param  miningSpeed  how quickly the laser breaks blocks
    void SetMiningSpeed( float miningSpeed );


    /// @brief  gets the beam color
    /// @return the beam color
    glm::vec4 const& GetBeamColor() const;

    /// @brief  sets the beam color
    /// @param  beamColor   the beam color
    void SetBeamColor( glm::vec4 const& beamColor );


    /// @brief  gets the beam width
    /// @return the beam width
    float GetBeamWidth() const;

    /// @brief  sets the beam width
    /// @param  beamWidth   the beam width
    void SetBeamWidth( float beamWidth );


    /// @brief  gets how much damage per second the laser deals
    /// @return how much damage per second the laser deals
    float GetDamageRate() const;

    /// @brief  sets how much damage per second the laser deals
    /// @param  dps how much damage per second the laser deals
    void SetDamageRate( float dps );


    /// @brief  gets which layers the laser collides with
    /// @return which layers the laser collides with
    CollisionLayerFlags GetCollisionLayers() const;

    /// @brief  sets which layers the laser collides with
    /// @param  collisionLayers which layers the laser collides with
    void SetCollisionLayers( CollisionLayerFlags collisionLayers );


    /// @brief  gets the direction that the beam is firing in
    /// @return the direction that the beam is firing in
    glm::vec2 const& GetDirection() const;

    /// @brief  sets the direction that the beam is firing in
    /// @param  direction   the direction that the beam is firing in
    void SetDirection( glm::vec2 const& direction );


    /// @brief  gets whether the beam is firing
    /// @return whether the beam is firing
    bool GetIsFiring() const;

    /// @brief  sets whether the beam is firing
    /// @return whether the beam is firing
    void SetIsFiring( bool isFiring );


    /// @brief  gets the Transform attached to this MiningLaser
    /// @return the Transform attached to this MiningLaser
    Transform* GetTransform();



//-----------------------------------------------------------------------------
public: //  methods
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
private: // virtual override methods
//-----------------------------------------------------------------------------


    /// @brief  called once when entering the scene
    virtual void OnInit() override;

    /// @brief  called once when exiting the scene
    virtual void OnExit() override;

    /// @brief  called every graohics frame
    virtual void OnUpdate( float dt ) override;

    /// @brief  called every simulation frame
    virtual void OnFixedUpdate() override;


//-----------------------------------------------------------------------------
private: // members
//-----------------------------------------------------------------------------


    /// @brief  the Transform attached to this Entity
    ComponentReference< Transform > m_Transform;


    /// @brief  the Tilemap this mining laser digs in
    ComponentReference< DestructibleTilemap > m_DestructibleTilemap;

    /// @brief  the Entity that the target Tilemap is a part of
    EntityReference m_TilemapEntity = EntityReference( { &m_DestructibleTilemap } );


    /// @brief  the range of the mining laser
    float m_Range = 2.0f;


    /// @brief  how quickly the laser breaks tiles
    float m_MiningSpeed = 1.0f;


    /// @brief  the color of the beam
    glm::vec4 m_BeamColor = { 1.0f, 0.0f, 0.0f, 1.0f };

    /// @brief  the width of the beam
    float m_BeamWidth = 0.1f;


    /// @brief  how much damage per second the laser deals
    float m_DamageRate = 1.0f;

    /// @brief Used to distribute across frames.
    float m_AccumulatedDamage = 1.0f;


    /// @brief  which layers the laser collides with
    CollisionLayerFlags m_CollisionLayers = 0;


    /// @brief  the direction to aim the laser in
    glm::vec2 m_Direction = { 1.0f, 0.0f };

    /// @brief  whether the laser is active
    bool m_IsFiring = false;


    /// @brief  the laser's current length
    float m_beamLength = 0.0f;


//-----------------------------------------------------------------------------
private: // helper methods
//-----------------------------------------------------------------------------

    
    /// @brief  fires the laser
    /// @param  tileDamage  the amount of damage to deal to tiles. May be called recursively if damage overkills
    void fireLaser( float tileDamage );


    /// @brief  tries to damages the specified entity
    /// @param  entity  the entity to damage
    /// @param  damage  the amount of damage to deal to the entity
    void tryDamageEntity( Entity* entity, float damage );

    /// @brief  damages the specified tile
    /// @param  tilePos the tile to damage
    /// @param  damage  the amount of damage to deal to the tile
    /// @return the amount of overkill damage dealt, if the tile was destroyed
    float damageTile( glm::ivec2 const& tilePos, float damage );


//-----------------------------------------------------------------------------
private: // inspector methods
//-----------------------------------------------------------------------------

    
    /// @brief  displays this MiningLaser in the Inspector
    virtual void Inspector() override;


//-----------------------------------------------------------------------------
private: // reading
//-----------------------------------------------------------------------------


    /// @brief  reads the name of the Tilemap entity
    /// @param  data the json data to read from
    void readTilemapEntity( nlohmann::ordered_json const& data );


    /// @brief  reads the range of the laser
    /// @param  data the json data to read from
    void readRange( nlohmann::ordered_json const& data );


    /// @brief  reads how quickly the mining laser breaks tiles
    /// @param  data the json data to read from
    void readMiningSpeed( nlohmann::ordered_json const& data );


    /// @brief  reads the color of the beam
    /// @param  data the json data to read from
    void readBeamColor( nlohmann::ordered_json const& data );

    /// @brief  reads the width of the beam
    /// @param  data the json data to read from
    void readBeamWidth( nlohmann::ordered_json const& data );


    /// @brief  reads the damage rate
    /// @param  data the json data to read from
    void readDamageRate( nlohmann::ordered_json const& data );


    /// @brief  reads the collision layers the laser hits
    /// @param  data the json data to read from
    void readCollideWithLayers( nlohmann::ordered_json const& data );


    /// @brief  reads the direction the laser fires in
    /// @param  data the json data to read from
    void readDirection( nlohmann::ordered_json const& data );

    /// @brief  reads whether the beam is firing
    /// @param  data the json data to read from
    void readIsFiring( nlohmann::ordered_json const& data );



    /// @brief  map of the read methods for this Component
    static ReadMethodMap< MiningLaser > s_ReadMethods;


//-----------------------------------------------------------------------------
public: // reading / writing
//-----------------------------------------------------------------------------


    /// @brief  gets the map of read methods for this Component
    /// @return the map of read methods for this Component
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override
    {
        return (ReadMethodMap< ISerializable > const&)s_ReadMethods;
    }


    /// @brief  Write all MiningLaser data to a JSON file.
    /// @return The JSON file containing the MiningLaser data.
    virtual nlohmann::ordered_json Write() const override;


//-----------------------------------------------------------------------------
private: // copying
//-----------------------------------------------------------------------------

    /// @brief  copy-constructor for the MiningLaser
    /// @param  other   the other MiningLaser to copy
    MiningLaser( const MiningLaser& other );

    /// @brief  clones this MiningLaser
    /// @return the newly created clone of this MiningLaser
    __inline virtual Component* Clone() const override { return new MiningLaser( *this ); }

//-----------------------------------------------------------------------------
public: // copying
//-----------------------------------------------------------------------------

    // diable = operator
    void operator =( const MiningLaser& ) = delete;

//-----------------------------------------------------------------------------
};
