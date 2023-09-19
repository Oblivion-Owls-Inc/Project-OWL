/// @file   Stream.h
/// @author Aidan Straker (aidan.straker@digipen.edu)
/// @brief  JSON File reading/writing
/// @version 0.1
/// @date 2023-09-10
///
/// @copyright  © 2023 DigiPen (USA) Corporation.

#define STREAM_H

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------
#include "document.h"  // rapidjson::Document, Parse, HasParseError, GetParseError
#include <string>      // std::string
#include <fstream>     // std::ifstream
#include "glm/glm.hpp" // glm::vec3
#include <map>

//------------------------------------------------------------------------------

class Stream
{
public: // static methods

	/// @brief Opens and parses a json document.
	/// @param name - name of the file to read from.
	/// @return - rapidjson document.
	static rapidjson::Document ReadFromJSON( const std::string& name );

public: // constructor

	/// @brief creates a Stream wrapper of the root object in a json document
	/// @param document the json document.
	Stream( rapidjson::Document const& document );

	/// @brief creates a stream wrapper from a json value.
	/// @param value the json value
	Stream( rapidjson::Value const& value );

public: // acessors

	/// @brief gets the rapidjson value as an object
	/// @return the rapidjson value
	rapidjson::GenericObject< true, rapidjson::Value > const& getObject() const;

	/// @brief reads a basic type from a json value
	/// @tparam T the type to read
	/// @return the value from the json
	template < typename T >
	T Read() const;

    /// @brief reads an int from a json value
    /// @return the value from the json
    template <>
    int Read<int>() const;

    /// @brief reads a float from a json value
    /// @return the value from the json
    template <>
    float Read<float>() const;

    /// @brief reads a string from a json value
    /// @return the value from the json
    template <>
    std::string Read<std::string>() const;

    /// @brief reads a vec3 from a json value
    /// @return the value from the json
    template <>
    glm::vec3 Read<glm::vec3>() const;

public: // methods

	/// @brief reads data into an existing complex type
	/// @tparam T the type to read
	/// @param object the oject to read data into
	template < typename T >
	void Read( T* object );

private: // member variables

	/// @brief the json value containing the data to read
	rapidjson::Value const& value;

};

template < typename T >
using ReadMethod = void (T::*)(Stream value);

template < typename T >
using ReadMethodMap = std::map< std::string, ReadMethod< T > >;

#ifndef STREAM_TC
#include "stream.t.cpp"
#endif