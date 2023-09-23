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
#include <rapidjson/istreamwrapper.h>

//-----------------------------------------------------------------------------
// public static methods
//-----------------------------------------------------------------------------

/// @brief Open and parse a json file.
/// @param name - name of the json file. 
/// @return rapid::json document parsed from the specified file.
rapidjson::Document Stream::ReadFromJSON( const std::string& filepath )
{
    // Check if the string is empty.
    if ( filepath.empty() )
    {
        throw "File was not found";
    }
    // Open the json file for reading.
    std::ifstream file( filepath );
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

#pragma warning ( push )
#pragma warning ( disable: 4172 )
/// @brief gets the rapidjson value as an object
/// @return the rapidjson value
rapidjson::GenericObject< true, rapidjson::Value > const& Stream::GetObject() const
{
    if ( value.IsObject() == false )
    {
        throw std::runtime_error(
            "JSON error: unexpected value type while trying to read Object type"
        );
    }
    return value.GetObject();
}
#pragma warning ( pop )

#pragma warning ( push )
#pragma warning ( disable: 4172 )
/// @brief gets the rapidjson value as an object
/// @return the rapidjson value
rapidjson::GenericArray< true, rapidjson::Value > const& Stream::GetArray() const
{
    if ( value.IsArray() == false )
    {
        throw std::runtime_error(
            "JSON error: unexpected value type while trying to read Array type"
        );
    }
    return value.GetArray();
}
#pragma warning ( pop )

/// @brief reads an int from a json value
/// @return the value from the json
template <>
int Stream::Read<int>() const
{
    if ( value.IsNumber() == false )
    {
        throw std::runtime_error(
            "JSON error: unexpected value type while trying to read Int type"
        );
    }
    return value.GetInt();
}

/// @brief reads a float from a json value
/// @return the value from the json
template <>
float Stream::Read<float>() const
{
    if ( value.IsNumber() == false )
    {
        throw std::runtime_error(
            "JSON error: unexpected value type while trying to read Float type"
        );
    }
    return value.GetFloat();
}

/// @brief reads a bool from a json value
/// @return the value from the json
template <>
bool Stream::Read<bool>() const
{
    if ( value.IsBool() == false )
    {
        throw std::runtime_error(
            "JSON error: unexpected value type while trying to read Bool type"
        );
    }
    return value.GetBool();
}

/// @brief reads string from a json value
/// @return the value from the json
template <>
std::string Stream::Read<std::string>() const
{
    if ( value.IsString() == false )
    {
        throw std::runtime_error(
            "JSON error: unexpected value type while trying to read String type"
        );
    }
    return value.GetString();
}

/// @brief reads a vec3 from a json value
/// @return the value from the json
template <>
glm::vec3 Stream::Read<glm::vec3>() const
{
    if ( value.IsArray() == false )
    {
        throw std::runtime_error(
            "JSON error: unexpected value type while trying to read Vec3 type"
        );
    }

    glm::vec3 vector = {};
    for ( int i = 0; i < 3; i++ )
    {
        assert( value[i].IsNumber() );
        vector[i] = value[i].GetFloat();
    }
    return vector;
}

//-----------------------------------------------------------------------------
