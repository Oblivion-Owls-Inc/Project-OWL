/// @file       DestructibleTilemap.h
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      Component responsible for handling the breaking of tiles with varrying strengths
/// @version    0.1
/// @date       2024-03-07
/// 
/// @copyright  Copyright (c) 2024 Digipen Institute of Technology

#pragma once

#include "Component.h"

#include "ComponentReference.h"
template < typename TileType >
class Tilemap;

/// @brief  Component responsible for handling the breaking of tiles with varrying strengths
class DestructibleTilemap : public Component
{
//-----------------------------------------------------------------------------
public: // constructor / Destructor
//-----------------------------------------------------------------------------


    /// @brief  default constructor
    DestructibleTilemap();


//-----------------------------------------------------------------------------
public: // methods
//-----------------------------------------------------------------------------


    /// @brief  damages a tile in the tilemap
    /// @param  tilePos the tile to damage
    /// @param  damage  the amount to damage the tile
    /// @return the amount of overkill damage dealt, if the tile was destroyed
    float DamageTile( glm::ivec2 const& tilePos, float damage );


    /// @brief  gets the health proportion of the specified tile pos
    /// @param  tilePos the position of the tile to get the health of
    /// @return the health of the tile from 1 to 0
    float GetTileHealthProportion( glm::ivec2 const& tilePos ) const;


    /// @brief  gets the maximum health of tile type
    /// @param  tileId  the ID of the tile type to get the max heath of
    /// @return the max health of the specified tile type
    float GetMaxHealth( int tileId ) const;


//-----------------------------------------------------------------------------
public: // accessors
//-----------------------------------------------------------------------------


    /// @brief  gets the source Tilemap attached to this Entity
    /// @return the source Tilemap attached to this Entity
    Tilemap< int > const* GetTilemap() const;

    /// @brief  gets the health Tilemap attached to this Entity
    /// @return the health Tilemap attached to this Entity
    Tilemap< float > const* GetHealthTilemap() const;


//-----------------------------------------------------------------------------
public: // virtual override methods
//-----------------------------------------------------------------------------


    /// @brief  called once when entering the scene
    virtual void OnInit() override;

    /// @brief  called once when exiting the scene
    virtual void OnExit() override;


//-----------------------------------------------------------------------------
private: // members
//-----------------------------------------------------------------------------


    /// @brief  the default health of each type of tile
    std::vector< float > m_TileHealths = {};


    /// @brief  the Tilemap attached to this Entity
    ComponentReference< Tilemap< int > > m_Tilemap;

    /// @brief  the health each tile in the map has
    ComponentReference< Tilemap< float > > m_HealthTilemap;


//-----------------------------------------------------------------------------
private: // methods
//-----------------------------------------------------------------------------


    /// @brief  callback called when the source tilemap changes
    /// @param  tilemap         the tilemap this callback was called from
    /// @param  tilepos         the position of the tile that was changed - will be (-1, -1) if whole tilemap changed
    /// @param  previousValue   the value of the tile before it was changed - garbage if whole tilemap changed
    void onTilemapChangedCallback(
        Tilemap< int >* tilemap,
        glm::ivec2 const& tilePos,
        int const& previousValue
    );


    /// @brief  reloads the health tilemap from the source tilemap
    void reloadTileHealths();


//-----------------------------------------------------------------------------
public: // inspection
//-----------------------------------------------------------------------------


    /// @brief  shows the inspector for DestructibleTilemap
    virtual void Inspector() override;


//-----------------------------------------------------------------------------
private: // reading
//-----------------------------------------------------------------------------


    /// @brief  reads the default health of each type of tile
    /// @param  data    the JSON data to read from
    void readTileTypeHealths( nlohmann::ordered_json const& data );


//-----------------------------------------------------------------------------
public: // reading / writing
//-----------------------------------------------------------------------------


    /// @brief  gets the map of read methods for this DestructibleTilemap
    /// @return the map of read methods for this DestructibleTilemap
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override;


    /// @brief  writes this DestructibleTilemap to JSON
    /// @return the JSON data of this DestructibleTilemap
    virtual nlohmann::ordered_json Write() const override;

    
//-----------------------------------------------------------------------------
public: // copying
//-----------------------------------------------------------------------------


    /// @brief  clones this DestructibleTilemap
    /// @return the newly created clone of this DestructibleTilemap
    virtual DestructibleTilemap* Clone() const override;


//-----------------------------------------------------------------------------
private: // copying
//-----------------------------------------------------------------------------


    /// @brief  copy-constructor for the DestructibleTilemap
    /// @param  other   the other DestructibleTilemap to copy
    DestructibleTilemap( DestructibleTilemap const& other );


    // disable assignment operator
    void operator =( DestructibleTilemap const& ) = delete;


//-----------------------------------------------------------------------------
};
