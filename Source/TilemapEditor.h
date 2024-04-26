/// @file       TilemapEditor.h
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      Component that allows direct editing of a Tilemap
/// @version    0.1
/// @date       2024-02-08
/// 
/// @copyright  Copyright (c) 2024 Digipen Institute of Technology

#pragma once

#include "pch.h" 









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
private: // constants
//-----------------------------------------------------------------------------

    
    /// @brief  The different Tools the TileMapEditor can bind to mouse buttons
    enum class MouseTool : int
    {
        None = 0,
        Brush = 1,
        Eraser = 2,
        Selection = 3,
        Picker = 4,
        _Count = 5
    };


//-----------------------------------------------------------------------------
private: // members
//-----------------------------------------------------------------------------


    /// @brief  the preview textures of the placeable tiles
    AssetReference< Texture > m_PreviewTexture;


    /// @brief  the tools bound to each mouse button
    std::vector< MouseTool > m_ToolBindings = { MouseTool::Brush, MouseTool::Eraser, MouseTool::None, MouseTool::Selection, MouseTool::Picker };


    /// @brief  the color to highlight the selection with
    glm::vec4 m_SelectionColor = { 0.3f, 0.3f, 1.0f, 0.0f };

    /// @brief  the transparency of the selection
    float m_SelectionAlpha = 0.4f;

    /// @brief  the transparency of the grid
    float m_GridAlpha = 0.4f;

    /// @brief  the transparency of the preview
    float m_PreviewAlpha = 0.4f;


    /// @brief  the index of the currently selected tile type
    int m_SelectedTileIndex = 0;


    /// @brief  the first corner of the selection
    glm::ivec2 m_SelectionPos0 = { 0, 0 };

    /// @brief  the second corner of the selection
    glm::ivec2 m_SelectionPos1 = { 0, 0 };


    /// @brief  the maximum number of undoable actions that will be kept track of
    int m_UndoStackCapacity = 64;

    /// @brief  the current position on the UndoStack
    int m_UndoStackPosition = 0;

    /// @brief  stack of changes that can be un/redone
    std::deque< nlohmann::ordered_json > m_UndoStack = {};


    /// @brief  the clipboard which can be copy-pasted
    RectangleSelection m_Clipboard;


    /// @brief  the tilemap attached to this Entity to edit
    ComponentReference< Tilemap< int > > m_Tilemap;


//-----------------------------------------------------------------------------
private: // methods
//-----------------------------------------------------------------------------

    
    /// @brief  gets the tile position of the mouse
    /// @return the tile position of the mouse
    glm::ivec2 getMouseTilePos() const;


    /// @brief  updates the brush tool
    /// @param  mouseButton the mouse button this tool is bound to
    void updateBrushTool( int mouseButton );

    /// @brief  updates the erase tool
    /// @param  mouseButton the mouse button this tool is bound to
    void updateEraseTool( int mouseButton );

    /// @brief  updates the rectangle selection tool
    /// @param  mouseButton the mouse button this tool is bound to
    void updateSelectionTool( int mouseButton );

    /// @brief  updates the picker tool
    /// @param  mouseButton the mouse button this tool is bound to
    void updatePickerTool( int mouseButton );


    /// @brief  updates the hotkeys the TilemapEditor uses for copy/paste, undo/redo, and similar
    void updateHotkeys();


    /// @brief  clamps the selection to the Tilemap bounds and makes pos0 the top left
    /// @return whether the selection is valid
    bool standardizeSelection();


    /// @brief  copies the current selection onto the clipboard
    void copySelection();

    /// @brief  fills the selection with the specified tile
    void fillSelection( int tileId );

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


    /// @brief  displays the preview tile and grid
    void displayPreview() const;


//-----------------------------------------------------------------------------
public: // inspection
//-----------------------------------------------------------------------------


    /// @brief  shows the inspector for TilemapEditor
    virtual void Inspector() override;

    
//-----------------------------------------------------------------------------
private: // inspection
//-----------------------------------------------------------------------------


    /// @brief  inspects a MouseTool
    /// @param  mouseTool   the MouseTool to inspect
    static bool inspectMouseTool( MouseTool* mouseTool );


//-----------------------------------------------------------------------------
private: // reading
//-----------------------------------------------------------------------------


    /// @brief  reads the name of the preview texture asset
    /// @param  data    the json data to read from
    void readPreviewTexture( nlohmann::ordered_json const& data );

    /// @brief  reads the tools bound to each mouse button
    /// @param  data    the json data to read from
    void readToolBindings( nlohmann::ordered_json const& data );

    /// @brief  reads the maximum number of undoable actions that will be kept track of
    /// @param  data    the json data to read from
    void readUndoStackCapacity( nlohmann::ordered_json const& data );

    /// @brief  reads the color to highlight the selection with
    /// @param  data    the json data to read from
    void readSelectionColor( nlohmann::ordered_json const& data );

    /// @brief  reads the transparency of the selection
    /// @param  data    the json data to read from
    void readSelectionAlpha( nlohmann::ordered_json const& data );

    /// @brief  reads the transparency of the grid
    /// @param  data    the json data to read from
    void readGridAlpha( nlohmann::ordered_json const& data );

    /// @brief  reads the transparency of the preview
    /// @param  data    the json data to read from
    void readPreviewAlpha( nlohmann::ordered_json const& data );


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
