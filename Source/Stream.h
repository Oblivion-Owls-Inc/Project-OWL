/**
 * @file   Stream.h
 * @author Aidan Straker (aidan.straker@digipen.edu)
 * @brief  JSON File reading/writing
 * @version 0.1
 * @date 2023-09-10
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "document.h"  // rapidjson::Document, Parse, HasParseError, GetParseError
#include <string>      // std::string
#include <fstream>     // std::ifstream
#include "glm/glm.hpp" // glm::vec3

//------------------------------------------------------------------------------

class Stream
{
public:
	Stream();
	~Stream();

public:
	// Read an integer from a json document.
	void Read( const rapidjson::Value& data, int& value, const char* key );
	// Read a float from a json document.
	void Read( const rapidjson::Value& data, float& value, const char* key );
	// Read a string from a json document.
	void Read( const rapidjson::Value& data, std::string& name, const char* key );
	// Read a vector from a json document.
	void Read( const rapidjson::Value& data, glm::vec3& value, const char* key );
	// Read a double from a json document.
	void Read( const rapidjson::Value& data, double& value, const char* key );
	// Read an unsigned integer from a json document.
	void Read( const rapidjson::Value& data, unsigned int& value, const char* key );

public:
	// Opens and parses a json document.
	rapidjson::Document ReadFromJSON( const std::string& name );


private:
    /// @brief Read the data for a vector from a document.
    /// @param doc    - the document to read from.
    /// @param vector - read the data in the document into the variable.
	void ReadVector( const rapidjson::Value& doc, glm::vec3& value );
};