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

#include <string>      // std::string
#include <map>

#include <glm/glm.hpp> // glm::vec3

#include <nlohmann/json.hpp>

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
public: // file io
//------------------------------------------------------------------------------

	/// @brief	Opens and parses a json document.
	/// @param  filepath    name of the file to read from.
	/// @return	the parsed json data
	static nlohmann::ordered_json ReadFromFile( std::string const& filepath );

    /// @brief  Writes json data to a file
    /// @param  filepath    the path of the file to write to
    /// @param  json        the json data to write to the file
    static void WriteToFile( std::string const& filepath, nlohmann::ordered_json const& json );

//------------------------------------------------------------------------------
public: // reading
//------------------------------------------------------------------------------

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
    static void Read( ValueType* value, nlohmann::ordered_json const& json );

    /// @brief  reads a serializable object from json
    /// @param  object  the object to read from json
    /// @param  json    the json data to read from
    template<>
    static void Read< ISerializable >( ISerializable* object, nlohmann::ordered_json const& json );

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

//------------------------------------------------------------------------------
public: // writing
//------------------------------------------------------------------------------
    
    /// @brief  writes a glm vector to json
    /// @tparam ValueType   the type of value of the vector
    /// @tparam size        the size of the vector
    /// @param  value       the vector to write
    /// @return the json data of the vector
    template< int size, typename ValueType >
    static nlohmann::ordered_json Write( glm::vec< size, ValueType > const& value );

//------------------------------------------------------------------------------
};

//------------------------------------------------------------------------------
// template read method definitions
//------------------------------------------------------------------------------

    /// @brief  reads a basic type from json
    /// @tparam ValueType   the type to read
    /// @param  json        the json data to read from
    /// @return the read data
    template< typename ValueType >
    ValueType Stream::Read( nlohmann::ordered_json const& json )
    {
        // TODO: error handling
        return json.get< ValueType >();
    }

    /// @brief  reads a basic type from json
    /// @tparam ValueType   the type to read
    /// @param  value       pointer to where to store the read data
    /// @param  json        the json data to read from
    template< typename ValueType >
    void Stream::Read( ValueType* value, nlohmann::ordered_json const& json )
    {
        // TODO: error handling
        *value = json.get< ValueType >();
    }


    /// @brief  reads a glm vector from json
    /// @tparam size        the size of the vector
    /// @tparam ValueType   the value type of the vector
    /// @param  json        the json data to read from
    /// @return the read vector
    template< int size, typename ValueType >
    glm::vec< size, ValueType > Stream::Read( nlohmann::ordered_json const& json )
    {
        glm::vec< size, ValueType > value;
        Read< size, ValueType >( &value, json );
        return value;
    }

    /// @brief  reads a glm vector from json
    /// @tparam size        the size of the vector
    /// @tparam ValueType   the value type of the vector
    /// @param  value       the vector to read into
    /// @param  json        the json data to read from
    template< int size, typename ValueType >
    void Stream::Read( glm::vec< size, ValueType >* value, nlohmann::ordered_json const& json )
    {
        if ( json.is_array() == false )
        {
            throw std::runtime_error(
                std::string() + "Error: unexpected json type \"" + json.type_name() +
                "\" encountered while trying to read vector of " + std::to_string(size) + " " + typeid(ValueType).name() + "s"
            );
        }

        int count = size;
        if ( json.size() != size )
        {
            // TODO: throw warning, not error
            count = std::min( (int)json.size(), size );
        }

        for ( int i = 0; i < count; ++i )
        {
            (*value)[ i ] = json[ i ].get< ValueType >();
        }
    }

//------------------------------------------------------------------------------
// template read method definitions
//------------------------------------------------------------------------------

    /// @brief  writes a glm vector to json
    /// @tparam ValueType   the type of value of the vector
    /// @tparam size        the size of the vector
    /// @param  value       the vector to write
    /// @return the json data of the vector
    template< int size, typename ValueType >
    nlohmann::ordered_json Stream::Write( glm::vec< size, ValueType > const& value )
    {
        nlohmann::ordered_json json;
        json.array( value );
        return json;
    }

//------------------------------------------------------------------------------
