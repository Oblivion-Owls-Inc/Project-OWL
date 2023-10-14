/// @file       ISerializable.h
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      interface for all serializable objects
/// @version    0.1
/// @date       2023-10-10
/// 
/// @copyright  Copyright (c) 2023 Digipen Institute of Technology

#pragma once

#include <map>
#include <string>

#include "Stream.h"

template < typename T >
using ReadMethod = void (T::*)( nlohmann::json const& json );

template < typename T >
using ReadMethodMap = std::map< std::string, ReadMethod< T > >;

/// @brief  interface for all serializable objects
class ISerializable
{
//-----------------------------------------------------------------------------
public: // virtual methods
//-----------------------------------------------------------------------------
    
    /// @brief  Gets the map of read methods for this object
    /// @return the map of read methods for htis object
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const
    {
        static ReadMethodMap< ISerializable > emptyMap = {};
        return emptyMap;
    }

    /// @brief  function which gets called after all data is read into this object on deserialization
    virtual void AfterLoad() {}

    /// @brief  writes this object to json
    /// @return the json data of this object
    virtual nlohmann::json Write() const { return {}; }

//-----------------------------------------------------------------------------
};
