/// @file   Stream.h
/// @author Aidan Straker (aidan.straker@digipen.edu)
/// @brief  JSON File reading/writing
/// @version 0.1
/// @date 2023-09-10
///
/// @copyright  © 2023 DigiPen (USA) Corporation.
/// ------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files
//------------------------------------------------------------------------------

#define STREAM_H

#include <string>            // std::string
#include <map>               // std::map
#include <glm/glm.hpp>       // glm::vec3
#include <nlohmann/json.hpp> // nlohman::ordered_json

#include <type_traits>
#include <iostream>

#include "ISerializable.h"

//------------------------------------------------------------------------------
// Forward references
//------------------------------------------------------------------------------

class ISerializable;

//------------------------------------------------------------------------------
// Class: Stream
//------------------------------------------------------------------------------
class Stream
{
//------------------------------------------------------------------------------
public: // file io
//------------------------------------------------------------------------------

	/// @brief	Opens and parses a json document.
	/// @param  filepath    name of the file to read from.
	/// @return	the parsed json data
	static nlohmann::ordered_json ReadFromFile( std::string const& filepath );

    /// @brief  Writes json data to a file
    /// @param  filepath    the path of the file to write to
    /// @param  json        the json data to write to the file
    static void WriteToFile( std::string const& filepath, nlohmann::ordered_json const& json );

    /// @brief Write a message to the trace log.
    /// @param traceMessage The message to write.
    static void WriteToTraceLog(std::string const& traceMessage);

//-----------------------------------------------------------------------------
public: // reading
//-----------------------------------------------------------------------------

    /// @brief  reads a basic type from json
    /// @tparam ValueType   the type to read
    /// @param  json        the json data to read from
    /// @return the read data
    template< typename ValueType >
    static ValueType Read( nlohmann::ordered_json const& json );

    /// @brief  reads a basic type from json
    /// @tparam ValueType   the type to read
    /// @param  value       pointer to where to store the read data
    /// @param  json        the json data to read from
    template< typename ValueType >
    static void Read( ValueType& value, nlohmann::ordered_json const& json );

    /// @brief  reads a glm vector from json
    /// @tparam size        the size of the vector
    /// @tparam ValueType   the value type of the vector
    /// @param  json        the json data to read from
    /// @return the read vector
    template< int size, typename ValueType >
    static glm::vec< size, ValueType > Read( nlohmann::ordered_json const& json );

    /// @brief  reads a glm vector from json
    /// @tparam size        the size of the vector
    /// @tparam ValueType   the value type of the vector
    /// @param  value       the vector to read into
    /// @param  json        the json data to read from
    template< int size, typename ValueType >
    static void Read( glm::vec< size, ValueType >* value, nlohmann::ordered_json const& json );

//------------------------------------------------------------------------------
public: // writing
//------------------------------------------------------------------------------

    /// @brief  writes data to json
    /// @tparam ValueType   the type of data to write to json
    /// @param  value       the data to write to json
    /// @return the written json data
    template< typename ValueType >
    static nlohmann::ordered_json Write( ValueType const& value );
    
    /// @brief  writes a glm vector to json
    /// @tparam ValueType   the type of value of the vector
    /// @tparam size        the size of the vector
    /// @param  value       the vector to write
    /// @return the json data of the vector
    template< int size, typename ValueType >
    static nlohmann::ordered_json Write( glm::vec< size, ValueType > const& value );

//-----------------------------------------------------------------------------
private: // pointer-aware methods
//-----------------------------------------------------------------------------

    /// @brief  used to differentiate between pointers and non-pointers
    /// @tparam B   whether ValueType is a pointer
    template< bool B > struct IsPointer {};

    /// @brief  reads data into a value by pointer
    /// @tparam ValueType   the type of value to read
    /// @param  value       the value to read data into
    /// @param  json        the json data to read from
    template< typename ValueType >
    static void Read( ValueType& value, nlohmann::ordered_json const& json, IsPointer<true> );

    /// @brief  reads data into a value by reference
    /// @tparam ValueType   the type of value to read
    /// @param  value       the value to read data into
    /// @param  json        the json data to read from
    template< typename ValueType >
    static void Read( ValueType& value, nlohmann::ordered_json const& json, IsPointer<false> );


    /// @brief  writes data into a value by pointer
    /// @tparam ValueType   the type of value to write
    /// @param  value       the value to write
    /// @return the written json data
    template< typename ValueType >
    static nlohmann::ordered_json Write( ValueType const& value, IsPointer<true> );
    
    /// @brief  writes data into a value by pointer
    /// @tparam ValueType   the type of value to write
    /// @param  value       the value to write
    /// @return the written json data
    template< typename ValueType >
    static nlohmann::ordered_json Write( ValueType const& value, IsPointer<false> );

//-----------------------------------------------------------------------------
private: // ISerializable-aware methods
//-----------------------------------------------------------------------------

    /// @brief  used to differentiate when types are derived from ISerializable
    /// @tparam B   whether ValueType is derived from ISerializable
    template< bool B > struct IsISerializable {};


    /// @brief  reads an ISerializable
    /// @tparam ValueType   the type to read
    /// @param  value       the value to read into
    /// @param  json        the json data to read from
    template< typename ValueType >
    static void Read( ValueType& value, nlohmann::ordered_json const& json, IsISerializable<true> );

    /// @brief  reads a basic type
    /// @tparam ValueType   the type to read
    /// @param  value       the value to read into
    /// @param  json        the json data to read from
    template< typename ValueType >
    static void Read( ValueType& value, nlohmann::ordered_json const& json, IsISerializable<false> );


    /// @brief  write an ISerializable
    /// @tparam ValueType   the type to write
    /// @param  value       the value to write
    /// @return the written json data
    template< typename ValueType >
    static nlohmann::ordered_json Write( ValueType const& value, IsISerializable<true> );

    /// @brief  write a basic type
    /// @tparam ValueType   the type to write
    /// @param  value       the value to write
    /// @return the written json data
    template< typename ValueType >
    static nlohmann::ordered_json Write( ValueType const& value, IsISerializable<false> );

//------------------------------------------------------------------------------
};


#ifndef STREAM_T_CPP
#include "Stream.t.cpp"
#endif