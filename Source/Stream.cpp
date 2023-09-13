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
rapidjson::Document Stream::ReadEntityFromJSON( const std::string& name )
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

Stream::Stream()
{
}

Stream::~Stream()
{
}

void Stream::Read( const rapidjson::Value& data, int& value, const char* key )
{
	if ( data.HasMember(key) )
	{
		value = data[key].GetInt();
	}
}

void Stream::Read( const rapidjson::Value& data, float& value, const char* key )
{
	if ( data.HasMember(key))
	{
		value = data[key].GetFloat();
	}
}

void Stream::Read( const rapidjson::Value& data, glm::vec3& value, const char* key )
{
	if ( data.HasMember( key ) )
	{
		ReadVector( data.GetArray(), value );
	}
}



/// @brief Reads in a vector from a JSON File.
/// @param doc - the json file to read from
/// @param property - name of the vector to be read.
void Stream::ReadVector( const rapidjson::Value& data, glm::vec3& vector )
{
	// Make sure both the array and vector are valid.
	assert( data.IsArray() );
	// Gte the x and y values from the array.
	vector.x = data[0].GetFloat();
	vector.y = data[1].GetFloat();
	vector.z = data[2].GetFloat();
}
