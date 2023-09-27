/// @file   Stream.h
/// @author Aidan Straker (aidan.straker@digipen.edu)
/// @brief  JSON File reading/writing
/// @version 0.1
/// @date 2023-09-10
///
/// @copyright  © 2023 DigiPen (USA) Corporation.
/// ------------------------------------------------------------------------------

#define STREAM_H

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------
#include <rapidjson/document.h>  // rapidjson::Document, Parse, HasParseError, GetParseError
#include <string>      // std::string
#include <fstream>     // std::ifstream
#include <glm/glm.hpp> // glm::vec3
#include <map>

//------------------------------------------------------------------------------
// Class: Stream
//------------------------------------------------------------------------------
class Stream
{
public: // static methods

	/// @brief		  Opens and parses a json document.
	/// @param name   Name of the file to read from.
	/// @return		  rapidjson document.
	static rapidjson::Document ReadFromJSON(const std::string& name);               

public: // constructor

	/// @brief			Creates a Stream wrapper of the root object in a 
	///					json document.
	/// @param document The json document.
	Stream( rapidjson::Document const& document );

	/// @brief		 Creates a stream wrapper from a json value.
	/// @param value The json value
	Stream( rapidjson::Value const& value );

public: // acessors

	/// @brief  Gets the rapidjson value as an object
	/// @return The rapidjson value
	rapidjson::GenericObject< true, rapidjson::Value > const& GetObject() const;

    /// @brief  Gets the rapidjson value as an object
    /// @return The rapidjson value
    rapidjson::GenericArray< true, rapidjson::Value > const& GetArray() const;

	/// @brief	  Reads a basic type from a json value
	/// @tparam T The type to read
	/// @return	  The value from the json
	template < typename T >
	T Read() const;

    /// @brief  Reads an int from a json value
    /// @return The value from the json
    template <>
    int Read<int>() const;

	/// @brief  Reads an int from a json value
	/// @return The value from the json
	template <>
	bool Read<bool>() const;

	/// @brief  Reads a basic type from a json value
	/// @return The value from the json
	template <>
	unsigned int Read<unsigned int>() const;

    /// @brief  Reads a float from a json value
    /// @return The value from the json
    template <>
    float Read<float>() const;

    /// @brief  Reads a string from a json value
    /// @return The value from the json
    template <>
    std::string Read<std::string>() const;

    /// @brief  Reads a vec3 from a json value
    /// @return The value from the json
    template <>
    glm::vec3 Read<glm::vec3>() const;

	/// @brief  Reads a vec4 from a json value
	/// @return The value from the json
	template <>
	glm::vec4 Read<glm::vec4>() const;

   /// @brief  Reads a vec2 from a json value
   /// @return The value from the json
	template <>
	glm::vec2 Read<glm::vec2>() const;

public: // methods

	/// @brief	  Reads data into an existing complex type
	/// @tparam T The type to read
	/// @param	  Object the oject to read data into
	template < typename T >
	void Read( T* object );

private: // member variables

	/// @brief The json value containing the data to read
	rapidjson::Value const& value;

};

template < typename T >
using ReadMethod = void (T::*)(Stream value);

template < typename T >
using ReadMethodMap = std::map< std::string, ReadMethod< T > >;

#ifndef STREAM_TC
#include "stream.t.cpp"
#endif