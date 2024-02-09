/// @file       TilemapEditor.h
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      Component that allows direct editing of a Tilemap
/// @version    0.1
/// @date       2024-02-08
/// 
/// @copyright  Copyright (c) 2024 Digipen Institute of Technology

#pragma once

#include "Component.h"

#include "glew.h"
#include "glfw3.h"

#include <deque>

template < typename TileType >
class Tilemap;

class Texture;


/// @brief  Component that allows direct editing of a Tilemap
class TilemapEditor : public Component
{
//-----------------------------------------------------------------------------
private: // types
//-----------------------------------------------------------------------------

    /// @brief  a rectangular selection on the clipboard
    struct RectangleSelection
    {
    //-------------------------------------------------------------------------
        
        /// @brief  the size of the selection
        glm::ivec2 M_Size = { 0, 0 };

        /// @brief  the tiles in the selection
        std::vector< int > M_Tiles = {};

    //-------------------------------------------------------------------------
    };

    /// @brief  GLFW mouse button ID
    using MouseButtonId = int;
    
//-----------------------------------------------------------------------------
public: // constructor / Destructor
//-----------------------------------------------------------------------------


    /// @brief  default constructor
    TilemapEditor();


//-----------------------------------------------------------------------------
public: // methods
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
public: // accessors
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
public: // virtual override methods
//-----------------------------------------------------------------------------


    /// @brief  called once when entering the scene
    virtual void OnInit() override;

    /// @brief  called once when exiting the scene
    virtual void OnExit() override;


//-----------------------------------------------------------------------------
private: // members
//-----------------------------------------------------------------------------


    /// @brief  the index of the currently selected tile type
    int m_SelectedTileIndex = 0;


    /// @brief  the name of the preview texture asset
    std::string m_PreviewTextureName = "";
    
    /// @brief  the preview textures of the placeable tiles
    Texture const*  m_PreviewTexture = nullptr;


    /// @brief  the index in the tool buttons array of the specified tool
    static constexpr int s_BrushToolIndex = 0, s_EraseToolIndex = 1, s_PickerToolIndex = 2, s_SelectionToolIndex = 3;

    /// @brief  the mouse buttons assigned to each tool
    MouseButtonId m_ToolButtons[ 4 ] = {
        GLFW_MOUSE_BUTTON_1,
        GLFW_MOUSE_BUTTON_2,
        GLFW_MOUSE_BUTTON_3,
        GLFW_MOUSE_BUTTON_4
    };


    /// @brief  the maximum number of undoable actions that will be kept track of
    int m_UndoStackCapacity = 64;


    /// @brief  the tilemap attached to this Entity to edit
    Tilemap< int >* m_Tilemap = nullptr;


    /// @brief  the first corner of the selection
    glm::ivec2 m_SelectionPos0 = { 0, 0 };

    /// @brief  the second corner of the selection
    glm::ivec2 m_SelectionPos1 = { 0, 0 };


    /// @brief  the current position on the UndoStack
    int m_UndoStackPosition = 0;

    /// @brief  stack of changes that can be un/redone
    std::deque< nlohmann::ordered_json > m_UndoStack = {};


    /// @brief  the clipboard which can be copy-pasted
    RectangleSelection m_Clipboard;


//-----------------------------------------------------------------------------
private: // methods
//-----------------------------------------------------------------------------

    
    /// @brief  gets the tile position of the mouse
    /// @return the tile position of the mouse
    glm::ivec2 getMouseTilePos() const;


    /// @brief  updates the brush tool
    void updateBrushTool();

    /// @brief  updates the erase tool
    void updateEraseTool();

    /// @brief  updates the rectangle selection tool
    void updateSelectionTool();

    /// @brief  updates the picker tool
    void updatePickerTool();


    /// @brief  clamps the selection to the Tilemap bounds and makes pos0 the top left
    void standardizeSelection();


    /// @brief  copies the current selection onto the clipboard
    void copySelection();

    /// @brief  erases all tiles in the current selection
    void eraseSelection();

    /// @brief  pastes the current selection into the tilemap
    void pasteSelection();


    /// @brief  saves the current state of the tilemap to the undo stack
    void pushUndoableAction();

    /// @brief  undoes the most recent action
    void undo();

    /// @brief  redoes the most recent undo
    void redo();


    /// @brief  displays the current selection
    void displaySelection() const;


//-----------------------------------------------------------------------------
public: // inspection
//-----------------------------------------------------------------------------


    /// @brief  shows the inspector for TilemapEditor
    virtual void Inspector() override;


    // TODO: add OnAddComponent method


//-----------------------------------------------------------------------------
private: // reading
//-----------------------------------------------------------------------------


    /// @brief  reads the name of the preview texture asset
    /// @param  data    the json data to read from
    void readPreviewTextureName( nlohmann::ordered_json const& data );

    /// @brief  reads the mouse buttons assigned to each tool
    /// @param  data    the json data to read from
    void readToolButtons( nlohmann::ordered_json const& data );

    /// @brief  reads the maximum number of undoable actions that will be kept track of
    /// @param  data    the json data to read from
    void readUndoStackCapacity( nlohmann::ordered_json const& data );


//-----------------------------------------------------------------------------
public: // reading / writing
//-----------------------------------------------------------------------------


    /// @brief  gets the map of read methods for this Component
    /// @return the map of read methods for this Component
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override;


    /// @brief  Writes all AudioPlayr data to a JSON file.
    /// @return The JSON file containing the data.
    virtual nlohmann::ordered_json Write() const override;

    
//-----------------------------------------------------------------------------
public: // copying
//-----------------------------------------------------------------------------


    /// @brief  clones this TilemapEditor
    /// @return the newly created clone of this TilemapEditor
    virtual TilemapEditor* Clone() const override;


//-----------------------------------------------------------------------------
private: // copying
//-----------------------------------------------------------------------------


    /// @brief  copy-constructor for the TilemapEditor
    /// @param  other   the other TilemapEditor to copy
    TilemapEditor( TilemapEditor const& other );


    // diable = operator
    void operator =( TilemapEditor const& ) = delete;


//-----------------------------------------------------------------------------
};
