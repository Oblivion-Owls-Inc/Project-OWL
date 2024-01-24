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
#include <vector>
#include <functional>

class Entity;

/// @brief  bit flags of which layers a collider collides with
using CollisionLayerFlags = unsigned; 

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
    /// @param  selectedName    pointer to the name of the selectd Asset
    /// @return Whether an asset was selected
    template< class AssetType >
    static bool SelectAssetFromLibrary( char const* label, AssetType const** selectedAsset, std::string* selectedAssetName );


    /// @brief  selects an Entity from the Scene
    /// @param  label           the ImGui label of the dropdown selector
    /// @param  selectedEntity  pointer to the currently selected Entity
    /// @return whether an Entity was selected
    static bool SelectEntityFromScene( char const* label, Entity** selectedEntity );


    /// @brief  inspects a (non-reorderable) array of elements
    /// @tparam DataType            the type of data in the array
    /// @param  label               the label of the array inspector
    /// @param  array               the array to inspect
    /// @param  elementInspector    the function to use to inspect each element of the array
    /// @return (bool)              whether the array was modified
    template< typename DataType >
    static bool InspectArray(
        char const* label,
        std::vector< DataType >* array,
        std::function< bool ( DataType* ) > elementInspector
    );


    /// @brief  inspects a CollisionLayerFlags
    /// @param  label           the label of the inspector
    /// @param  collisionLayers the collisionLayerFlags to inspect
    /// @return whether the CollisionLayerFlags was modified
    static bool InspectCollisionLayerFlags( char const* label, CollisionLayerFlags* collisionLayers );


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


#include <imgui.h>

//-----------------------------------------------------------------------------
// public: tempalate implementations
//-----------------------------------------------------------------------------


    /// @brief  selects an Asset from an AssetLibrary
    /// @tparam AssetType       the type of asset to select
    /// @param  label           the ImGui label of the dropdown selector
    /// @param  selectedAsset   pointer to the currently selected Asset
    /// @param  selectedName    pointer to the name of the selectd Asset
    /// @return Whether an asset was selected
    template< class AssetType >
    bool Inspection::SelectAssetFromLibrary( char const* label, AssetType const** selectedAsset, std::string* selectedAssetName )
    {
        if ( ImGui::BeginCombo( label, selectedAssetName->c_str() ) )
        {
            for ( auto const& [ name, asset ] : AssetLibrary< AssetType >()->GetAssets() )
            {
                if ( ImGui::Selectable( name.c_str(), asset == *selectedAsset) )
                {
                    *selectedAssetName = name;
                    *selectedAsset = asset;

                    ImGui::EndCombo();
                    return true;
                }
            }

            ImGui::EndCombo();
        }

        return false;
    }



    /// @brief  inspects a reorderable array of elements
    /// @tparam DataType            the type of data in the array
    /// @param  label               the label of the array inspector
    /// @param  array               the array to inspect
    /// @param  elementInspector    the function to use to inspect each element of the array
    /// @return (bool)              whether the array was modified
    template< typename DataType >
    bool Inspection::InspectArray(
        char const* label,
        std::vector< DataType >* array,
        std::function< bool ( DataType* ) > elementInspector
    )
    {

        bool modified = false;

        // get the ID of this ListBox to prevent dragging and dropping across different arrays
        std::string fullId = label;
        fullId += std::to_string( ImGui::GetItemID() );

        ImGui::BeginTable( label, 3, ImGuiTableFlags_Borders );

        ImGui::TableSetupColumn( "index", ImGuiTableColumnFlags_WidthFixed, 45.0f );
        ImGui::TableSetupColumn( label );
        ImGui::TableSetupColumn( "remove", ImGuiTableColumnFlags_WidthFixed, 30.0f );
        ImGui::TableHeadersRow();

        for ( int i = 0; i < array->size(); ++i )
        {
            ImGui::PushID( i );

            ImGui::TableNextRow();
            ImGui::TableNextColumn();

            // allow dragging and dropping of the indices to reorder the array
            ImGui::Text( "%3i", i );
            if ( ImGui::BeginDragDropSource( ImGuiDragDropFlags_SourceAllowNullID ) )
            {
                ImGui::SetDragDropPayload( fullId.c_str(), &i, sizeof(int));

                ImGui::Text( "%3i", i );

                ImGui::EndDragDropSource();
            }
            if ( ImGui::BeginDragDropTarget() )
            {
                if ( ImGuiPayload const* payload = ImGui::AcceptDragDropPayload( fullId.c_str() ) )
                {
                    if ( payload->DataSize != sizeof( int ) )
                    {
                        Debug() << "Warning: ImGui drag/drop payload does not match expected size" << std::endl;
                    }
                    else
                    {
                        int fromIndex = *(const int*)payload->Data;

                        DataType movedValue = (*array)[ fromIndex ];
                        array->erase( array->begin() + fromIndex );
                        array->insert( array->begin() + i, movedValue );

                        modified = true;
                    }
                }
                ImGui::EndDragDropTarget();
            }

            // inspect the element
            ImGui::TableNextColumn();
            if ( elementInspector( &(*array)[ i ] ) )
            {
                modified = true;
            }

            // button to delete the element
            ImGui::TableNextColumn();
            if ( ImGui::Button( "X", ImVec2( 23, 23 ) ) )
            {
                array->erase( array->begin() + i );
                modified = true;
            }

            ImGui::PopID();
        }

        ImGui::TableNextRow();
        ImGui::TableNextColumn();

        // button to add element
        if ( ImGui::Button( "+", ImVec2( 40, 23 ) ) )
        {
            array->push_back( DataType() );
            modified = true;
        }

        ImGui::EndTable();

        return modified;
    }


//-----------------------------------------------------------------------------
