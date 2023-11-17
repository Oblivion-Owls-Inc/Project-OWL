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
#include <shlobj_core.h>


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
#ifdef NDEBUG //These are backwards for some reason but it works

        char* appData = nullptr;
        size_t size = 0;
        errno_t err = _dupenv_s(&appData, &size, "APPDATA");

        if (err || appData == nullptr) 
        {
            std::cerr << "Error: Unable to retrieve APPDATA environment variable." << std::endl;
            return;
        }

        std::string gameDirectory = std::string(appData) + "\\Dig_Deeper"; // Create the game directory path
        free(appData); // Free the memory allocated by _dupenv_s

        // Check if the game directory exists
        DWORD ftyp = GetFileAttributesA(gameDirectory.c_str());
        if (ftyp == INVALID_FILE_ATTRIBUTES) 
        {
            // Directory doesn't exist, attempt to create it
            if (!CreateDirectoryA(gameDirectory.c_str(), NULL)) 
            {
                std::cerr << "Error: Unable to create directory \"" << gameDirectory << "\"" << std::endl;
                return;
            }
        }
        else if (!(ftyp & FILE_ATTRIBUTE_DIRECTORY))  // Check if the path is a directory
        {
            // A file with the same name as the directory exists
            std::cerr << "Error: Game directory path \"" << gameDirectory << "\" is a file!" << std::endl;
            return;
        }

        std::string traceFilePath = gameDirectory + "\\trace.log";

#else
    		std::string traceFilePath = "trace.log";
#endif

        // Attempt to open the trace log for writing.
        static std::ofstream traceFile(traceFilePath.c_str());
        if (!traceFile.is_open())
        {
            std::cerr << "Warning: unable to open file \"" << traceFilePath << "\"" << std::endl;
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