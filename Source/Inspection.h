/// @file       Inspection.h
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      Helper methods for inspecting things
/// @version    0.1
/// @date       2023-09-13
/// 
/// @copyright  Copyright (c) 2023 Digipen Institute of Technology

#pragma once

#include <string>

class Inspection
{
//-----------------------------------------------------------------------------
public: // public methods
//-----------------------------------------------------------------------------


    /// @brief  selects a file from a directory
    /// @param  label           the ImGui label of the dropdown selector
    /// @param  selectedFile    pointer to the currently selected filepath
    /// @param  directoryPath   path of the directory to select the file from
    /// @return whether a new file was selected
    static bool SelectFileFromDirectory( char const* label, std::string* selectedFile, std::string const& directoryPath );


//-----------------------------------------------------------------------------
private: // helper methods
//-----------------------------------------------------------------------------


    /// @brief  selects a file from a directory (recursive for subdirectories)
    /// @param  selectedFile    pointer to the currently selected filepath
    /// @param  directoryPath   path of the directory to select the file from
    /// @return whether a new file was selected
    static bool selectFileFromDirectoryHelper( std::string* selectedFile, std::string const& directoryPath );


//-----------------------------------------------------------------------------
};
