/// @file   Stream.cpp
/// @author Aidan Straker (aidan.straker@digipen.edu)
/// @brief  JSON File reading/writing
/// @version 0.1
/// @date 2023-09-10
///
/// @copyright  © 2023 DigiPen (USA) Corporation.

#include "Stream.h"

#include <iostream>
#include <map>
#include "istreamwrapper.h"

//-----------------------------------------------------------------------------
// public static methods
//-----------------------------------------------------------------------------

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

//-----------------------------------------------------------------------------
// public constructors
//-----------------------------------------------------------------------------

/// @brief creates a Stream wrapper of the root object in a json document
/// @param document the json document.
Stream::Stream( rapidjson::Document const& document ) :
    value( document.GetObject() )
{}

/// @brief creates a stream wrapper from a json value.
/// @param value the json value
Stream::Stream( rapidjson::Value const& value_ ) :
    value( value_ )
{}

//-----------------------------------------------------------------------------
// public accessors
//-----------------------------------------------------------------------------

/// @brief gets the rapidjson value as an object
/// @return the rapidjson value
rapidjson::GenericObject< true, rapidjson::Value > const& Stream::getObject() const
{
    assert( value.IsObject() );
    return value.GetObject();
}

/// @brief reads a basic type from a json value
/// @tparam T the type to read
/// @return the value from the json
template <>
int Stream::Read<int>() const
{
    assert( value.IsInt() );
    return value.GetInt();
}

/// @brief reads a basic type from a json value
/// @tparam T the type to read
/// @return the value from the json
template <>
float Stream::Read<float>() const
{
    assert( value.IsFloat() );
    return value.GetFloat();
}

/// @brief reads a basic type from a json value
/// @tparam T the type to read
/// @return the value from the json
template <>
std::string Stream::Read<std::string>() const
{
    assert( value.IsString() );
    return value.GetString();
}

/// @brief reads a basic type from a json value
/// @tparam T the type to read
/// @return the value from the json
template <>
glm::vec3 Stream::Read<glm::vec3>() const
{
    assert( value.IsArray() );
    glm::vec3 vector;
    for ( int i = 0; i < 3; i++ )
    {
        assert( value[i].IsFloat() );
        vector[i] = value[i].GetFloat();
    }
    return vector;
}

//-----------------------------------------------------------------------------
