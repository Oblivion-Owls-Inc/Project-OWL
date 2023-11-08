/// @file       Inspection.h
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      Helper methods for inspecting things
/// @version    0.1
/// @date       2023-09-13
/// 
/// @copyright  Copyright (c) 2023 Digipen Institute of Technology

#pragma once

#include "AssetLibrarySystem.h"

#include <string>

class Entity;

class Inspection
{
//-----------------------------------------------------------------------------
public: // public methods
//-----------------------------------------------------------------------------


    /// @brief  selects a file from a directory
    /// @param  label           the ImGui label of the dropdown selector
    /// @param  selectedFile    pointer to the currently selected filepath
    /// @param  directoryPath   path of the directory to select the file from
    /// @return whether a file was selected
    static bool SelectFileFromDirectory( char const* label, std::string* selectedFile, std::string const& directoryPath );


    /// @brief  selects an Asset from an AssetLibrary
    /// @tparam AssetType       the type of asset to select
    /// @param  label           the ImGui label of the dropdown selector
    /// @param  selectedAsset   pointer to the currently selected Asset
    /// @return Whether an asset was selected
    template< class AssetType >
    static bool SelectAssetFromLibrary( char const* label, AssetType const** selectedAsset );


    /// @brief  selects an Entity from the Scene
    /// @param  label           the ImGui label of the dropdown selector
    /// @param  selectedEntity  pointer to the currently selected Entity
    /// @return whether an Entity was selected
    static bool SelectEntityFromScene( char const* label, Entity** selectedEntity );


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


//-----------------------------------------------------------------------------
// public: tempalate implementations
//-----------------------------------------------------------------------------


    /// @brief  selects an Asset from an AssetLibrary
    /// @tparam AssetType       the type of asset to select
    /// @param  label           the ImGui label of the dropdown selector
    /// @param  selectedAsset   pointer to the currently selected Asset
    /// @return Whether an asset was selected
    template< class AssetType >
    bool Inspection::SelectAssetFromLibrary( char const* label, AssetType const** selectedAsset )
    {
        if ( ImGui::BeginCombo( label, AssetLibrary< AssetType >()->GetAssetName( *selectedAsset ) ) )
        {
            for ( auto const& [ name, asset ] : AssetLibrary< AssetType >()->GetAssets() )
            {
                if ( ImGui::Selectable( name.c_str(), asset == *selectedAsset) )
                {
                    *selectedAsset = asset;

                    ImGui::EndCombo();
                    return true;
                }
            }

            ImGui::EndCombo();
        }

        return false;
    }


//-----------------------------------------------------------------------------
