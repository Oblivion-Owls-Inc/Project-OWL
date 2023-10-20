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

#include <fstream> // std::ifstream

//------------------------------------------------------------------------------
// public: static methods
//------------------------------------------------------------------------------

    /// @brief	Opens and parses a json document.
    /// @param  filepath    name of the file to read from.
    /// @return	the parsed json data
    nlohmann::ordered_json Stream::ReadFromFile( std::string const& filepath )
    {
        std::ifstream file( filepath );
        if ( file.is_open() == false )
        {
            throw std::runtime_error(
                std::string() + "Error: unable to open file \"" + filepath + "\""
            );
        }
        // TODO: error handling?
        return nlohmann::ordered_json::parse( file );
    }

    /// @brief Write a meesage to the trace log.
    /// @param traceMessage The message to be written.
    void Stream::WriteToTraceLog(std::string const& traceMessage)
    {
        // Attempt to open the trace log for writing.
        std::ofstream traceFile("trace.log");
        if (!traceFile.is_open())
        {
            throw std::runtime_error(
                std::string() + "Error: unable to open trace log"
            );
        }

        // Write the trace message to the log.
        traceFile << traceMessage;
    }

    /// @brief  Writes json data to a file
    /// @param  filepath    the path of the file to write to
    /// @param  json        the json data to write to the file
    void Stream::WriteToFile( std::string const& filepath, nlohmann::ordered_json const& json )
    {
        std::ofstream file( filepath );
        if ( file.is_open() == false )
        {
            throw std::runtime_error(
                std::string() + "Error: unable to open file \"" + filepath + "\""
            );
        }
        file << std::setw( 4 ) << json << std::endl;
    }


    /// @brief  reads a serializable object from json
    /// @param  object  the object to read from json
    /// @param  json    the json data to read from
    template<>
    void Stream::Read< ISerializable >( ISerializable* object, nlohmann::ordered_json const& json )
    {
        if ( json.is_object() == false )
        {
            throw std::runtime_error(
                "Error: json is not an object"
            );
        }

        ReadMethodMap< ISerializable > const& readMethods = object->GetReadMethods();
        for ( auto& [ key, value ] : json.items() )
        {
            auto it = readMethods.find( key );
            if ( it == readMethods.end() )
            {
                throw std::runtime_error(
                    std::string() + "Error: unrecognized token \"" + key + "\" encountered"
                );
            }

            ReadMethod< ISerializable > const& readMethod = it->second;
            (object->*it->second)( value );
        }

        object->AfterLoad();
    }
//-----------------------------------------------------------------------------