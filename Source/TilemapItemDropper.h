/// @file       TilemapItemDropper.h
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      Drops an item whenever a tile is broken
/// @version    0.1
/// @date       2023-10-20
/// 
/// @copyright  Copyright (c) 2023 Digipen Institute of Technology

#pragma once

#include "Component.h"

#include "ItemStack.h"

template< typename TileType >
class Tilemap;



/// @brief  Drops an item whenever a tile is broken
class TilemapItemDropper : public Component
{
//-----------------------------------------------------------------------------
public: // types
//-----------------------------------------------------------------------------

    
//-----------------------------------------------------------------------------
private: // types
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
public: // accessors
//-----------------------------------------------------------------------------



    


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


//-----------------------------------------------------------------------------
private: // members
//-----------------------------------------------------------------------------


    /// @brief  the name of the Entity to drop items as
    std::string m_ItemArchetypeName;
    /// @brief  the archetype of the Entity to drop items as
    Entity const* m_ItemArchetype;


    /// @brief  the maximum initial velocity of a dropped item
    float m_MaxInitialVelocity;

    /// @brief  the square radius aroud the center of the tile to spawn the items in
    float m_ItemSpawnRadius;


    /// @brief  the Tilemap attached to this TilemapItemDropper
    Tilemap< int >* m_Tilemap;


//-----------------------------------------------------------------------------
private: // helper methods
//-----------------------------------------------------------------------------


    /// @brief  callback called whenever a tile is changed
    /// @param  tilemap         the tilemap that a tile was changed on
    /// @param  tilePos         the position of the changed tile; (-1, -1) if whole tilemap changed
    /// @param  previousValue   the previous value of the changed tile
    void onTilemapChangedCallback(
        Tilemap< int >* tilemap,
        glm::ivec2 const& tilePos,
        int const& previousValue
    ) const;

    /// @brief  drop an ItemStack from the specified tile position
    /// @param  itemStack   the item stack to drop
    /// @param  tilePos     the position to drop the item
    void dropItem( ItemStack const& itemStack, glm::ivec2 const& tilePos ) const;


//-----------------------------------------------------------------------------
private: // inspection
//-----------------------------------------------------------------------------

    
    /// @brief  displays this TilemapItemDropper in the Inspector
    virtual void Inspector() override;


//-----------------------------------------------------------------------------
private: // reading
//-----------------------------------------------------------------------------


    /// @brief  reads the name of the Entity to drop items as
    /// @param  data    the json data to read from
    void readItemArchetypeName( nlohmann::ordered_json const& data );


    /// @brief  reads the maximum initial velocity of a dropped item
    /// @param  data    the json data to read from
    void readMaxInitialVelocity( nlohmann::ordered_json const& data );

    /// @brief  reads the square radius aroud the center of the tile to spawn the items in
    /// @param  data    the json data to read from
    void readItemSpawnRadius( nlohmann::ordered_json const& data );


//-----------------------------------------------------------------------------
public: // reading / writing
//-----------------------------------------------------------------------------


    /// @brief  gets the map of read methods for this Component
    /// @return the map of read methods for this Component
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override;


    /// @brief  Write all TilemapItemDropper data to a JSON file.
    /// @return The JSON file containing the TilemapItemDropper data.
    virtual nlohmann::ordered_json Write() const override;


//-----------------------------------------------------------------------------
public: // constructor / Destructor
//-----------------------------------------------------------------------------


    /// @brief  constructor
    TilemapItemDropper();


//-----------------------------------------------------------------------------
public: // copying
//-----------------------------------------------------------------------------


    /// @brief  clones this TilemapItemDropper
    /// @return the newly created clone of this TilemapItemDropper
    virtual TilemapItemDropper* Clone() const override;


//-----------------------------------------------------------------------------
private: // copying
//-----------------------------------------------------------------------------


    /// @brief  copy-constructor for the TilemapItemDropper
    /// @param  other   the other TilemapItemDropper to copy
    TilemapItemDropper( const TilemapItemDropper& other );

    // diable = operator
    void operator =( TilemapItemDropper const& ) = delete;


//-----------------------------------------------------------------------------
};
