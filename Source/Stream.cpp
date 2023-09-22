/**
 * @file   Stream.h
 * @author Aidan Straker (aidan.straker@digipen.edu)
 * @brief  JSON File reading/writing
 * @version 0.1
 * @date 2023-09-10
 *
 * @copyright  © 2023 DigiPen (USA) Corporation.
 *
 */
#include "Stream.h"
#include <iostream>
#include "istreamwrapper.h"

/// @brief Open and parse a json file.
/// @param name - name of the json file. 
/// @return rapid::json document parsed from the specified file.
rapidjson::Document Stream::ReadFromJSON( const std::string& name )
{
	// Check if the string is empty.
	if ( name.empty() )
	{
		throw "File was not found";
	}
	// Parse the filepath from the name.
	std::string filePath( "Data/" + name + ".json" );
	// Open the json file for reading.
	std::ifstream file( filePath );
	// Check if the file was opened.
	if ( !file.is_open() )
	{
//TODO: Talk with team about failed file openings.
		throw "File could not be opened!";
	}

	rapidjson::IStreamWrapper isw( file );
	// Create a document object.
	rapidjson::Document doc;
	// Parse the document from the stream.
	doc.ParseStream( isw );

	// Check if the json was parsed correctly.
	if ( doc.HasParseError() )
	{
		std::cerr << "ERROR parsing JSON: " << doc.GetParseError() << std::endl;
		file.close();
		throw;
	}
	file.close();
	// If no parse errors, then the file was successfully opened.
	return doc;
}

Stream::Stream(){}
Stream::~Stream(){}

/// @brief Reads an integer from a JSON File.
/// @param data  - the json file to read from.
/// @param value - read the data from te json into this variable.
/// @param key   - the label of the data to search for.
void Stream::Read( const rapidjson::Value& data, int& value, const char* key )
{
	// Only read if the value is in the json file.
	if ( data.HasMember(key) )
	{
		value = data[key].GetInt();
	}
}

/// @brief Reads a float from a JSON File.
/// @param data  - the json file to read from.
/// @param value - read the data from the json into this variable.
/// @param key   - the label of the data to search for.
void Stream::Read( const rapidjson::Value& data, float& value, const char* key )
{
	// Only read if the data is in the json file.
	if ( data.HasMember(key))
	{
		value = data[key].GetFloat();
	}
}

/// @brief Reads a string from a JSON File.
/// @param data - the json file to read from.
/// @param name - read the name from the json into this variable.
/// @param key  - the label of the data to search for.
void Stream::Read( const rapidjson::Value& data, std::string& name, const char* key )
{
	// Only read if the name is in the json file.
	if ( data.HasMember(key) )
	{
		name = data[key].GetString();
	}
}

/// @brief Reads a vector from a JSON File.
/// @param data  - the json file to read from.
/// @param value - read the data from the json into this variable.
/// @param key   - the label of the data to search for.
void Stream::Read( const rapidjson::Value& data, glm::vec3& value, const char* key )
{
	// Only read if the value is in the json file.
	if ( data.HasMember( key ) )
	{
		ReadVector( data.GetArray(), value );
	}
}

/// @brief Reads a double from a JSON File.
/// @param data  - the json file to read from.
/// @param value - read the data from the json into this variable.
/// @param key   - the label of the data to search for.
void Stream::Read( const rapidjson::Value& data, double& value, const char* key )
{
	// Only read if the value is in the json file.
	if ( data.HasMember(key) )
	{
		value = data[key].GetDouble();
	}
}

/// @brief Reads an unsigned integer from a JSON File.
/// @param data  - the json file to read from.
/// @param value - read the data from the json into this variable.
/// @param key   - the label of the data to search for.
void Stream::Read( const rapidjson::Value& data, unsigned int& value, const char* key )
{
	// Only read if the value is in the file.
	if ( data.HasMember(key) )
	{
		value = data[key].GetUint();
	}
}


/// @brief Reads in a vector from a JSON File.
/// @param data   - the json file to read from
/// @param value  - read the data from the json into this variable.
void Stream::ReadVector( const rapidjson::Value& data, glm::vec3& value )
{
	// Make sure both the array and vector are valid.
	assert( data.IsArray() );
	// Gte the x and y values from the array.
	value.x = data[0].GetFloat();
	value.y = data[1].GetFloat();
	value.z = data[2].GetFloat();
}