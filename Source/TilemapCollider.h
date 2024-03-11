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

#include "ComponentReference.h"
#include "Tilemap.h"

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
public: // virtual overrides
//-----------------------------------------------------------------------------


    /// @brief  called once when entering the scene
    virtual void OnInit() override;

    /// @brief  called once when exiting the scene
    virtual void OnExit() override;


    /// @brief  draws the collision shape of this collider for debug purposes
    virtual void DebugDraw() const override;


//-----------------------------------------------------------------------------
private: // members
//-----------------------------------------------------------------------------


    /// @brief  the Tilemap component associated with this TilemapCollider
    ComponentReference< Tilemap< int > > m_Tilemap;


//-----------------------------------------------------------------------------
public: // reading / writing
//-----------------------------------------------------------------------------


    /// @brief gets the map of read methods for this Component
    /// @return the map of read methods for this Component
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override;


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


    // disable assignment operator
    void operator =( TilemapCollider const& ) = delete;


//-----------------------------------------------------------------------------
};
