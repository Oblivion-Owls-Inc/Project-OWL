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

//------------------------------------------------------------------------------
