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
public: // methods
//------------------------------------------------------------------------------

	/// @brief	Opens and parses a json document
    /// @param  object      the object to read the JSON data into
	/// @param  filepath    name of the file to read from
	static void ReadFromFile( ISerializable* object, std::string const& filepath );


    /// @brief Read the SDL Controller Mappings
    /// @param filepath - The .txt file containing the mappings.
    /// @return A string holding all the mappings
    static std::string ReadFromTXTFile(std::string const& filepath);

    /// @brief  Writes json data to a file
    /// @param  filepath    the path of the file to write to
    /// @param  json        the json data to write to the file
    static void WriteToFile( std::string const& filepath, nlohmann::ordered_json const& json );

    /// @brief Write a message to the trace log.
    /// @param traceMessage The message to write.
    static void WriteToTraceLog(std::string const& traceMessage);


    /// @brief  copies a value to the clipbaord
    /// @tparam ValueType   the type of value to copy to the clipboard
    /// @param  value       the value to copy to the clipboard
    template< typename ValueType >
    static void CopyToClipboard( ValueType const& value );

    /// @brief  pastes the clipboard into a value
    /// @tparam ValueType   the type of value to paste the clipboard into
    /// @param  value       the value to paste the clipboard into
    template< typename ValueType >
    static void PasteFromClipboard( ValueType& value );


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

    /// @brief Reads a vector of a standard type from a JSON file.
    /// @tparam ValueType - the type of data to read into the vector,
    /// @param vector     - the vector to read into.
    /// @param json       - the JSON to read from.
    template<typename ValueType>
    static void Read(std::vector<ValueType>* vec_data, nlohmann::ordered_json const& json);

    /// @brief Reads an array of a standard type from a JSON file.
    /// @tparam ValueType - the data type of the array.
    /// @tparam Size      - the size of the array.
    /// @param array      - the array to read into.
    template<typename ValueType, int Size>
    static void Read(ValueType* array_data, nlohmann::ordered_json const& json);

    /// @brief Reads a map with a string key and any standard data type from a JSON.
    /// @tparam ValueType - the type of map value.
    /// @param map_data   - pointer tot he map to read into.
    /// @param json       - the JSON to read from.
    template<typename ValueType>
    static void Read(std::map<std::string, ValueType>* map_data, nlohmann::ordered_json const& json);

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

    /// @brief Write a vector of any standard data type to a JSON
    /// @tparam ValueType - type of data stored by the vector.
    /// @param vec_data   - the vector to write to JSON.
    /// @return The JSON object containing the vector data.
    template<typename ValueType>
    static nlohmann::ordered_json Write(std::vector<ValueType> const& vec_data);

    /// @brief Write an array of any standard data type to a JSON.
    /// @tparam ValueType - the type of data stored in the array
    /// @tparam Size      - the size of the array
    /// @param array_data - pointer to the array.
    /// @return The JSON object containing the array.
    template<typename ValueType, int Size>
    static nlohmann::ordered_json Write( ValueType const* array_data );

    /// @brief Write a map of string keys and any standard data type to a JSON object.
    /// @tparam ValueType - the data type of the map values.
    /// @param map_data   - the map to write to a JSON object.
    /// @return The JSON object containing the written map data.
    template<typename ValueType>
    static nlohmann::ordered_json Write(std::map<std::string, ValueType> map_data);

//-----------------------------------------------------------------------------
private: // static variables
//-----------------------------------------------------------------------------
    

    /// @brief  clipboard used for copy and pasting data around the editor
    static nlohmann::ordered_json s_Clipboard;

    
//-----------------------------------------------------------------------------
public: // debug stack
//-----------------------------------------------------------------------------


    /// @brief  pushes a Debug Location name to the DebugLocationStack
    /// @param  locationName    the name of the location to push
    static void PushDebugLocation( std::string const& locationName );


    /// @brief  pops a Debug Location name from the DebugLocationStack
    static void PopDebugLocation();


    /// @brief  gets the DebugLocation as a string to print
    /// @return the DebugLocation as a string
    static std::string GetDebugLocation();


//-----------------------------------------------------------------------------
private: // debug stack
//-----------------------------------------------------------------------------


    /// @brief  stack of strings representing the current location in the JSON file
    static std::vector< std::string > s_DebugLocationStack;
    

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