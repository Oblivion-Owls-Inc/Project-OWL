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


    /// @brief	opens and parses a json document
    /// @param  filepath    name of the file to read from
    /// @return the parsed JSON data
    nlohmann::ordered_json Stream::ParseFromFile( std::string const& filepath )
    {
        std::ifstream file( filepath );
        if ( file.is_open() == false )
        {
            Debug() << "Warning: unable to open file \"" << filepath << "\"" << std::endl;
            return nlohmann::ordered_json();
        }

        try
        {
            return nlohmann::ordered_json::parse( file );
        }
        catch ( std::runtime_error const& error )
        {
            Debug() << "WARNING: unable to parse JSON file \"" << filepath << "\" - " << error.what() << std::endl;
            return nlohmann::ordered_json();
        }
    }


    /// @brief	Opens and parses a json document.
    /// @param  object      the object to read the JSON data into
    /// @param  filepath    name of the file to read from
    void Stream::ReadFromFile( ISerializable* object, std::string const& filepath )
    {
        nlohmann::ordered_json json = ParseFromFile( filepath );
        if ( json.is_null() )
        {
            return;
        }

        s_DebugLocationStack.push_back( filepath + "::" );

        Stream::Read( *object, json );

        s_DebugLocationStack.pop_back();
    }


    /// @brief Read the SDL Controller Mappings
    /// @param filepath - The .txt file containing the mappings.
    /// @return A string holding all the mappings
    std::string Stream::ReadFromTXTFile(std::string const& filepath)
    {
        std::ifstream file(filepath);
        std::string mappings;

        if (file.is_open() == false)
        {
            Debug() << "Warning: unable to open file \"" << filepath << "\"" << std::endl;
            return mappings;
        }

        file >> mappings;

        return mappings;
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
    nlohmann::ordered_json Stream::s_Clipboard = nlohmann::ordered_json();

    
//-----------------------------------------------------------------------------
// public: debug stack
//-----------------------------------------------------------------------------


    /// @brief  pushes a Debug Location name to the DebugLocationStack
    /// @param  locationName    the name of the location to push
    void Stream::PushDebugLocation( std::string const& locationName )
    {
        s_DebugLocationStack.push_back( locationName );
    }


    /// @brief  pops a Debug Location name from the DebugLocationStack
    void Stream::PopDebugLocation()
    {
        s_DebugLocationStack.pop_back();
    }


    /// @brief  gets the DebugLocation as a string to print
    /// @return the DebugLocation as a string
    std::string Stream::GetDebugLocation()
    {
        std::string result = "";

        for ( std::string const& string : s_DebugLocationStack )
        {
            result += string;
        }

        if ( result.empty() == false )
        {
            result.pop_back();
        }

        return result;
    }


//-----------------------------------------------------------------------------
// private: debug stack
//-----------------------------------------------------------------------------


    /// @brief  stack of strings representing the current location in the JSON file
    std::vector< std::string > Stream::s_DebugLocationStack = {};
    

//-----------------------------------------------------------------------------