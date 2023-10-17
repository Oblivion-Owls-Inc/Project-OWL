/// @file       TilemapCollider.h
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      tilemap collider
/// @version    0.1
/// @date       2023-10-16
/// 
/// @copyright  Copyright (c) 2023 Digipen Institute of Technology

#pragma once

#include "Collider.h"
#include "Stream.h"

template< typename TileType >
class Tilemap;

/// @class  TilemapCollider
/// @brief  Circle-shaped Collider
class TilemapCollider : public Collider
{
//-----------------------------------------------------------------------------
public: // constructor
//-----------------------------------------------------------------------------

    /// @brief  default constructor
    TilemapCollider();

//-----------------------------------------------------------------------------
public: // accessors
//-----------------------------------------------------------------------------

    /// @brief  gets the Tilemap associated with this TilemapCollider
    /// @return the tilemap
    Tilemap< int > const* GetTilemap() const { return m_Tilemap; }

//-----------------------------------------------------------------------------
private: // virtual overrides
//-----------------------------------------------------------------------------

    /// @brief  called once when entering the scene
    virtual void OnInit() override;

//-----------------------------------------------------------------------------
private: // members
//-----------------------------------------------------------------------------

    /// @brief  the Tilemap component associated with this TilemapCollider
    Tilemap< int >* m_Tilemap = nullptr;

//-----------------------------------------------------------------------------
private: // reading
//-----------------------------------------------------------------------------

    /// @brief map of the read methods for this Component
    static ReadMethodMap< TilemapCollider > s_ReadMethods;

    /// @brief gets the map of read methods for this Component
    /// @return the map of read methods for this Component
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override
    {
        return (ReadMethodMap< ISerializable > const&)s_ReadMethods;
    }

//-----------------------------------------------------------------------------
public: // writing
//-----------------------------------------------------------------------------

    /// @brief Write all CircleCollider component data to a JSON file.
    /// @return The JSON file containing the CircleCollider component data.
    virtual nlohmann::ordered_json Write() const override;

//-----------------------------------------------------------------------------
public: // copying
//-----------------------------------------------------------------------------

    /// @brief  virtual component clone function
    /// @return new clone of component
    virtual TilemapCollider* Clone() const override;

//-----------------------------------------------------------------------------
private: // copying
//-----------------------------------------------------------------------------

    /// @brief  copy-constructor
    /// @param  other   the collider to copy
    TilemapCollider( TilemapCollider const& other );

//-----------------------------------------------------------------------------
};
