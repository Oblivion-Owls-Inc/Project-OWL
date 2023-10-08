/// @file   Stream.cpp
/// @author Aidan Straker (aidan.straker@digipen.edu)
/// @brief  JSON File reading/writing
/// @version 0.1
/// @date 2023-09-10
///
/// @copyright  © 2023 DigiPen (USA) Corporation.

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "Stream.h"

#include <iostream>
#include <map>
#include <rapidjson/istreamwrapper.h>

//-----------------------------------------------------------------------------
// public static methods
//-----------------------------------------------------------------------------

/// @brief              Open and parse a json file.
/// @param filepath     Name of the json file. 
/// @return             rapid::json document parsed from the specified file.
rapidjson::Document Stream::ReadFromJSON(const std::string& filepath)
{
    // Check if the string is empty.
    if (filepath.empty())
    {
        throw "File was not found";
    }
    // Open the json file for reading.
    std::ifstream file(filepath);
    // Check if the file was opened.
    if (!file.is_open())
    {
        //TODO: Talk with team about failed file openings.
        throw "File could not be opened!";
    }

    rapidjson::IStreamWrapper isw(file);
    // Create a document object.
    rapidjson::Document doc;
    // Parse the document from the stream.
    doc.ParseStream(isw);

    // Check if the json was parsed correctly.
    if (doc.HasParseError())
    {
        std::cerr << "ERROR parsing JSON: " << doc.GetParseError() << std::endl;
        file.close();
        throw (std::runtime_error("Parse Error"));
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
Stream::Stream(rapidjson::Document const& document) :
    value(document.GetObject())
{}

/// @brief creates a stream wrapper from a json value.
/// @param value the json value
Stream::Stream( rapidjson::Value const& value_ ) :
    value(value_)
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

//-----------------------------------------------------------------------------
// Public Templated Reads
//-----------------------------------------------------------------------------

/// @brief  Reads a basic type from a json value
/// @return The value from the json
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

/// @brief  Reads a boolean value from a json value
/// @return The value from the json
template <>
bool Stream::Read<bool>() const
{
    if (value.IsBool() == false)
    {
        throw std::runtime_error(
            "JSON error: unexpected value type while trying to read Boolean type"
        );
    }
    return value.GetBool();
}

/// @brief  Reads an unsigned int value from a json value
/// @return The value from the json
template <>
unsigned int Stream::Read<unsigned int>() const
{
    if (value.IsNumber() == false)
    {
        throw std::runtime_error(
            "JSON error: unexpected value type while trying to read Unsigned Int"
            "type"
        );
    }
    return value.GetUint();
}

/// @brief  Reads a float value from a json value
/// @return The value from the json
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

/// @brief  Reads a string from a json value
/// @return The value from the json
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

/// @brief  Reads a glm::vec1 from a json value
/// @return The value from the json
template <>
glm::vec1 Stream::Read<glm::vec1>() const
{
    return glm::vec1( Read<float>() );
}


/// @brief  Reads a glm::vec2 from a json value
/// @return The value from the json
template <>
glm::vec2 Stream::Read<glm::vec2>() const
{
    if (value.IsArray() == false)
    {
        throw std::runtime_error(
            "JSON error: unexpected value type while trying to read Vec2"
        );
    }

    if ( value.GetArray().Size() > 2 )
    {
        throw std::runtime_error(
            "JSON error: unexpected array size while trying to read Vec2"
        );
    }

    glm::vec2 vector = {};
    for (int i = 0; i < value.GetArray().Size(); i++)
    {
        if ( value[i].IsNumber() == false )
        {
            throw std::runtime_error(
                "JSON error: unexpected value type while trying to read float in vec2"
            );
        }
        vector[i] = value[i].GetFloat();
    }
    return vector;
}

/// @brief  Reads a glm::vec3 from a json value
/// @return The value from the json
template <>
glm::vec3 Stream::Read<glm::vec3>() const
{
    if ( value.IsArray() == false )
    {
        throw std::runtime_error(
            "JSON error: unexpected value type while trying to read Vec3"
        );
    }

    if ( value.GetArray().Size() > 3 )
    {
        throw std::runtime_error(
            "JSON error: unexpected array size while trying to read Vec3"
        );
    }

    glm::vec3 vector = {};
    for (int i = 0; i < value.GetArray().Size(); i++)
    {
        if ( value[i].IsNumber() == false )
        {
            throw std::runtime_error(
                "JSON error: unexpected value type while trying to read float in vec3"
            );
        }
        vector[i] = value[i].GetFloat();
    }
    return vector;
}

/// @brief  Reads a glm::vec4 from a json value
/// @return The value from the json
template <>
glm::vec4 Stream::Read<glm::vec4>() const
{
    if (value.IsArray() == false)
    {
        throw std::runtime_error(
            "JSON error: unexpected value type while trying to read Vec4"
        );
    }

    if ( value.GetArray().Size() > 4 )
    {
        throw std::runtime_error(
            "JSON error: unexpected array size while trying to read Vec4"
        );
    }

    glm::vec4 vector = {};
    for (int i = 0; i < value.GetArray().Size(); i++)
    {
        if ( value[i].IsNumber() == false )
        {
            throw std::runtime_error(
                "JSON error: unexpected value type while trying to read float in vec4"
            );
        }
        vector[i] = value[i].GetFloat();
    }
    return vector;
}

/// @brief  Reads an ivec2 from a json value
/// @return The value from the json
template <>
glm::ivec2 Stream::Read<glm::ivec2>() const
{
    if (value.IsArray() == false)
    {
        throw std::runtime_error(
            "JSON error: unexpected value type while trying to read ivec2"
        );
    }

    glm::ivec2 vector = {};
    for (int i = 0; i < value.GetArray().Size(); i++)
    {
        if ( value[i].IsInt() == false )
        {
            throw std::runtime_error(
                "JSON error: unexpected value type while trying to read integer in ivec2"
            );
        }
        vector[i] = value[i].GetInt();
    }
    return vector;
}