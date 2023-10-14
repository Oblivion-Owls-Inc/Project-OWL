/// @file   Stream.cpp
/// @author Aidan Straker (aidan.straker@digipen.edu)
/// @brief  JSON File reading/writing
/// @version 0.1
/// @date 2023-09-10
///
/// @copyright  © 2023 DigiPen (USA) Corporation.
/// ------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Include Files
//------------------------------------------------------------------------------

#include "Stream.h"
#include "ISerializable.h"

#include <fstream>     // std::ifstream

//------------------------------------------------------------------------------
// public: static methods
//------------------------------------------------------------------------------

    /// @brief	Opens and parses a json document.
    /// @param  filepath    name of the file to read from.
    /// @return	the parsed json data
    nlohmann::json Stream::ReadFromFile( std::string const& filepath )
    {
        // TODO: error handling
        return nlohmann::json::parse( std::ifstream( filepath ) );
    }

    /// @brief  Writes json data to a file
    /// @param  filepath    the path of the file to write to
    /// @param  json        the json data to write to the file
    void Stream::WriteToFile( std::string const& filepath, nlohmann::json const& json )
    {
        // TODO: error handling
        std::ofstream( filepath ) << json;
    }


    /// @brief  reads a basic type from json
    /// @tparam ValueType   the type to read
    /// @param  json        the json data to read from
    /// @return the read data
    template< typename ValueType >
    ValueType Stream::Read( nlohmann::json const& json )
    {
        // TODO: error handling
        return json.get< ValueType >();
    }

    /// @brief  reads a basic type from json
    /// @tparam ValueType   the type to read
    /// @param  value       pointer to where to store the read data
    /// @param  json        the json data to read from
    template< typename ValueType >
    void Stream::Read( ValueType* value, nlohmann::json const& json )
    {
        // TODO: error handling
        *value = json.get< ValueType >();
    }


    /// @brief  reads a serializable object from json
    /// @param  object  the object to read from json
    /// @param  json    the json data to read from
    template<>
    void Stream::Read< ISerializable >( ISerializable* object, nlohmann::json const& json )
    {
        if ( json.is_object() == false )
        {
            // TODO: error handling
        }

        ReadMethodMap< ISerializable > const& readMethods = object->GetReadMethods();
        for ( auto& [ key, value ] : json.items() )
        {
            auto it = readMethods.find( key );
            if ( it == readMethods.end() )
            {
                // TODO: error handling
            }

            ReadMethod< ISerializable > const& readMethod = it->second;
            (object->*it->second)( value );
        }

        object->AfterLoad();
    }


    /// @brief  reads a glm vector from json
    /// @tparam size        the size of the vector
    /// @tparam ValueType   the value type of the vector
    /// @param  json        the json data to read from
    /// @return the read vector
    template< int size, typename ValueType >
    glm::vec< size, ValueType > Stream::Read( nlohmann::json const& json )
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
    void Stream::Read( glm::vec< size, ValueType >* value, nlohmann::json const& json )
    {
        if ( json.is_array() == false )
        {
            // TODO: error handling
        }

        int count = size;
        if ( json.size() != size )
        {
            // TODO: throw warning, not error
            int count = std::min( json.size(), size );
        }

        for ( int i = 0; i < count, ++i )
        {
            (*value)[ i ] = json[ i ].get< ValueType >();
        }
    }

//------------------------------------------------------------------------------
