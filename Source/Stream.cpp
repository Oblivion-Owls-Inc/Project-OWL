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

#include "DebugSystem.h"

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
            Debug() << "Warning: unable to open file \"" << filepath << "\"" << std::endl;
            return nlohmann::ordered_json();
        }
        // TODO: error handling?
        return nlohmann::ordered_json::parse( file );
    }

    /// @brief Write a meesage to the trace log.
    /// @param traceMessage The message to be written.
    void Stream::WriteToTraceLog(std::string const& traceMessage)
    {
        // Attempt to open the trace log for writing.
        static std::ofstream traceFile("trace.log");
        if (!traceFile.is_open())
        {
            Debug() << "Warning: unable to open file \"trace.log\"" << std::endl;
            return;
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
            Debug() << "Warning: unable to open file \"" << filepath << "\"" << std::endl;
            return;
        }
        file << std::setw( 4 ) << json << std::endl;
    }

//-----------------------------------------------------------------------------
// private: static variables
//-----------------------------------------------------------------------------

    /// @brief  clipboard used for copy and pasting data around the editor
    nlohmann::ordered_json Stream::m_Clipboard = nlohmann::ordered_json();

//-----------------------------------------------------------------------------