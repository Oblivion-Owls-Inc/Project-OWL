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

#include <iostream> // std::ifstream, is_open
#include <map>      // std::map
#include "istreamwrapper.h"

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

/// @brief  Gets the rapidjson value as an object
/// @return The rapidjson value
rapidjson::GenericObject<true, rapidjson::Value> const& Stream::getObject() const
{
    assert(value.IsObject());
    return value.GetObject();
}

/// @brief  Gets the rapidjson value as an object
/// @return The rapidjson value
rapidjson::GenericArray<true, rapidjson::Value> const& Stream::getArray() const
{
    assert(value.IsArray());
    return value.GetArray();
}

//-----------------------------------------------------------------------------
// Public Templated Reads
//-----------------------------------------------------------------------------

/// @brief  Reads a basic type from a json value
/// @tparam T the type to read
/// @return The value from the json
template <>
int Stream::Read<int>() const
{
    assert(value.IsInt());
    return value.GetInt();
}

/// @brief  Reads a basic type from a json value
/// @tparam T the type to read
/// @return The value from the json
template <>
unsigned int Stream::Read<unsigned int>() const
{
    assert(value.IsUint());
    return value.GetUint();
}

/// @brief  Reads a basic type from a json value
/// @tparam T the type to read
/// @return The value from the json
template <>
float Stream::Read<float>() const
{
    assert(value.IsNumber());
    return value.GetFloat();
}

/// @brief  Reads a basic type from a json value
/// @tparam T the type to read
/// @return The value from the json
template <>
std::string Stream::Read<std::string>() const
{
    assert(value.IsString());
    return value.GetString();
}

/// @brief  Reads a basic type from a json value
/// @tparam T the type to read
/// @return The value from the json
template <>
glm::vec3 Stream::Read<glm::vec3>() const
{
    assert(value.IsArray());
    glm::vec3 vector = {};
    for (int i = 0; i < 3; i++)
    {
        assert(value[i].IsNumber());
        vector[i] = value[i].GetFloat();
    }
    return vector;
}

/// @brief  Reads a basic type from a json value
/// @tparam T the type to read
/// @return The value from the json
template <>
glm::vec2 Stream::Read<glm::vec2>() const
{
    assert(value.IsArray());
    glm::vec2 vector = {};
    for (int i = 0; i < 2; i++)
    {
        assert(value[i].IsNumber());
        vector[i] = value[i].GetFloat();
    }
    return vector;
}