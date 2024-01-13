/// @file       MiningLaser.h
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      Digging laser controllable by an entity
/// @version    0.1
/// @date       2023-10-20
/// 
/// @copyright  Copyright (c) 2023 Digipen Institute of Technology

#pragma once

#include "Behavior.h"
#include "basics.h"

#include "CollisionSystem.h"

#include <deque>

class Transform;
class AudioPlayer;
class Emitter;
class health;

template< typename T >
class Tilemap;

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


    /// @brief  gets the laser source offset
    /// @return the laser source offset
    glm::vec2 const& GetSourceOffset() const;

    /// @brief  sets the laser source offset
    /// @param  sourceOffset    the laser source offset
    void SetSourceOffset( glm::vec2 const& sourceOffset );


    /// @brief  gets how quickly the laser breaks blocks
    /// @return how quickly the laser breaks blocks
    float GetMiningSpeed() const;

    /// @brief  sets how quickly the laser breaks blocks
    /// @param  miningSpeed  how quickly the laser breaks blocks
    void SetMiningSpeed( float miningSpeed );


    /// @brief  gets the max in progress tiles
    /// @return the max in progress tiles
    int GetMaxInProgressTiles() const;

    /// @brief  sets the max in progress tiles
    /// @param  maxInProgressTiles  the max in progress tiles
    void SetMaxInProgressTiles( int maxInProgressTiles );


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


//-----------------------------------------------------------------------------
public: //  methods
//-----------------------------------------------------------------------------


    /// @brief  gets the world emission position
    /// @return the world emission position
    glm::vec2 GetWorldEmissionPosition() const;


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
    Transform* m_Transform = nullptr;


    /// @brief  the name of the Tilemap entity
    std::string m_TilemapName = "";

    /// @brief  the Tilemap this mining laser digs in
    Tilemap< int >* m_Tilemap = nullptr;


    /// @brief  the AudioPlayer this mining laser uses to play sounds
    AudioPlayer* m_AudioPlayer = nullptr;


    /// @brief  the particleEmitter the laser particles emit with
    Emitter* m_ParticleEmitter = nullptr;



    /// @brief  the range of the mining laser
    float m_Range = 2.0f;


    /// @brief  the offset from the center of the Transform that the laser starts from
    glm::vec2 m_SourceOffset = { 0.0f, 0.0f };

    /// @brief  how quickly the laser breaks tiles
    float m_MiningSpeed = 1.0f;


    /// @brief  how many tiles can be in the process of being destroyed at once
    int m_MaxInProgressTiles = 32;


    /// @brief  all tiles currently in the process of being destroyed
    std::deque< InProgressTile > m_InProgressTiles;


    /// @brief  the color of the beam
    glm::vec4 m_BeamColor = { 1.0f, 0.0f, 0.0f, 1.0f };

    /// @brief  the width of the beam
    float m_BeamWidth = 0.1f;


    /// @brief  how much damage per second the laser deals
    float m_DamageRate = 1.0f;


    /// @brief  which layers the laser collides with
    CollisionLayerFlags m_CollideWithLayers = 0;


    /// @brief  the direction to aim the laser in
    glm::vec2 m_Direction = { 1.0f, 0.0f };

    /// @brief  whether the laser is active
    bool m_IsFiring = false;


    /// @brief  the laser's current length
    float m_beamLength = 0.0f;


//-----------------------------------------------------------------------------
private: // helper methods
//-----------------------------------------------------------------------------

    
    /// @brief  tries to damages the specified entity
    /// @param  entity  the entity to damage
    void tryDamageEntity( Entity* entity ) const;


    /// @brief  gets the tile position of a raycast hit
    /// @param  hit the RayCastHit to get the tile position of
    /// @return the tile position of the raycast hit
    glm::ivec2 getTargettedTile( RayCastHit const& hit ) const;


    /// @brief  damages the specified tile
    /// @param  tilePos the tile to damage
    void damageTile( glm::ivec2 const& tilePos );

    /// @brief  breaks the specified tile
    /// @param  tilePos the tile to break
    void breakTile( std::deque< MiningLaser::InProgressTile >::iterator const& tilePos );


    /// @brief  gets an in progress tile, and adds it to the queue if it doesn't exist yet
    /// @param  tilePos the position of the tile to get
    /// @return the in progress tile data for that tile
    std::deque< MiningLaser::InProgressTile >::iterator getInProgressTile( glm::ivec2 tilePos );


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
    void readTilemapName( nlohmann::ordered_json const& data );


    /// @brief  reads the range of the laser
    /// @param  data the json data to read from
    void readRange( nlohmann::ordered_json const& data );


    /// @brief  reads offset of the laser source
    /// @param  data the json data to read from
    void readSourceOffset( nlohmann::ordered_json const& data );

    /// @brief  reads how quickly the mining laser breaks tiles
    /// @param  data the json data to read from
    void readMiningSpeed( nlohmann::ordered_json const& data );


    /// @brief  reads how many tiles the laser can damage at once
    /// @param  data the json data to read from
    void readMaxInProgressTiles( nlohmann::ordered_json const& data );


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
