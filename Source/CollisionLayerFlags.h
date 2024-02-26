/// @file       CollisionLayerFlags.h
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      struct that determines which collision layers to interact with
/// @version    0.1
/// @date       2024-02-16
/// 
/// @copyright  Copyright (c) 2024 Digipen Institute of Technology


#pragma once

#include "ISerializable.h"

class CollisionLayerFlags : public ISerializable
{
//-----------------------------------------------------------------------------
public: // constructor
//-----------------------------------------------------------------------------

    
    /// @brief  constructor
    /// @param  flags   the bitflags of this CollisionLayerFlags
    CollisionLayerFlags( unsigned flags = 0 );


//-----------------------------------------------------------------------------
public: // methods
//-----------------------------------------------------------------------------


    /// @brief  Checks whether this CollisionLayerFlags contains a collisionLayer
    /// @param  collisionLayer  the layer to check if these flags include
    /// @return whether the given layer is included
    bool Includes( unsigned collisionLayer ) const;

    
//-----------------------------------------------------------------------------
public: // accessors
//-----------------------------------------------------------------------------


    /// @brief  implicit cast to unsigned
    /// @return the internal bitflags
    operator unsigned() const;

    
    /// @brief  assignment operator
    /// @param  bitflags    the bitflags to assign to this CollisionLayerFlags
    void operator =( unsigned bitflags );


//-----------------------------------------------------------------------------
private: // members
//-----------------------------------------------------------------------------


    /// @brief  the bitflags of which CollisionLayers are included
    unsigned m_Flags = 0;

    
//-----------------------------------------------------------------------------
public: // inspection
//-----------------------------------------------------------------------------


    /// @brief  inspects this CollisionLayerFlags
    /// @param  label   the label of this Inspector
    /// @return whether a change was made
    bool Inspect( char const* label );


//-----------------------------------------------------------------------------
private: // reading
//-----------------------------------------------------------------------------


    /// @brief  reads the bitflags of which CollisionLayers are included
    /// @param  data    the JSON data to read from
    void readBitFlags( nlohmann::ordered_json const& data );


//-----------------------------------------------------------------------------
public: // reading / writing
//-----------------------------------------------------------------------------


    /// @brief  gets the map of read methods for this CollisionLayerFlags
    /// @return the map of read methods for this CollisionLayerFlags
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override;


    /// @brief  Write all CollisionLayerFlags data to a JSON file.
    /// @return The JSON file containing the CollisionLayerFlags data.
    virtual nlohmann::ordered_json Write() const override;


//-----------------------------------------------------------------------------
};
