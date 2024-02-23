/// @file       Inspection.cpp
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      Helper methods for inspecting things
/// @version    0.1
/// @date       2023-09-13
/// 
/// @copyright  Copyright (c) 2023 Digipen Institute of Technology

#include "Inspection.h"

#include "Entity.h"
#include "EntitySystem.h"

#include "CollisionSystem.h"

#include <filesystem>
#include <imgui.h>

//-----------------------------------------------------------------------------
// public: public methods
//-----------------------------------------------------------------------------


    /// @brief  selects a file from a directory
    /// @param  label           the ImGui label of the dropdown selector
    /// @param  selectedFile    pointer to the currently selected filepath
    /// @param  directoryPath   path of the directory to select the file from
    /// @return whether a new file was selected
    bool Inspection::SelectFileFromDirectory( char const* label, std::string* selectedFile, std::string const& directoryPath )
    {
        if ( ImGui::BeginCombo( label, selectedFile->c_str() + directoryPath.size() + 1) )
        {
            if ( selectFileFromDirectoryHelper( selectedFile, directoryPath ) )
            {
                ImGui::EndCombo();
                return true;
            }
            ImGui::EndCombo();
        }
        return false;
    }

//-----------------------------------------------------------------------------
// private: helper methods
//-----------------------------------------------------------------------------


    /// @brief  selects a file from a directory (recursive for subdirectories)
    /// @param  selectedFile    pointer to the currently selected filepath
    /// @param  directoryPath   path of the directory to select the file from
    /// @return whether a new file was selected
    bool Inspection::selectFileFromDirectoryHelper( std::string* selectedFile, std::string const& directoryPath )
    {
        for ( auto const& file : std::filesystem::directory_iterator( directoryPath ) )
        {
            std::string filepath = file.path().filename().string();
            if ( file.is_directory() )
            {
                if ( ImGui::TreeNode( filepath.c_str() ) )
                {
                    if ( selectFileFromDirectoryHelper( selectedFile, directoryPath + "/" + filepath ) )
                    {
                        ImGui::TreePop();
                        return true;
                    }
                    ImGui::TreePop();
                }
            }
            else
            {
                if ( ImGui::Selectable( filepath.c_str(), filepath == *selectedFile ) )
                {
                    *selectedFile = directoryPath + "/" + filepath;
                    return true;
                }
            }
        }
        return false;
    }


//-----------------------------------------------------------------------------
