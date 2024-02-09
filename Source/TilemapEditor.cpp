/// @file       TilemapEditor.h
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      Component that allows direct editing of a Tilemap
/// @version    0.1
/// @date       2024-02-08
/// 
/// @copyright  Copyright (c) 2024 Digipen Institute of Technology

#pragma once

#include "TilemapEditor.h"

#include "Tilemap.h"

#include "Texture.h"
#include "AssetLibrarySystem.h"

#include "InputSystem.h"

#include "Inspection.h"

    
//-----------------------------------------------------------------------------
// public: constructor / Destructor
//-----------------------------------------------------------------------------


    /// @brief  default constructor
    TilemapEditor::TilemapEditor() :
        Component( typeid( TilemapEditor ) )
    {}


//-----------------------------------------------------------------------------
// public: methods
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// public: accessors
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// public: virtual override methods
//-----------------------------------------------------------------------------


    /// @brief  called once when entering the scene
    void TilemapEditor::OnInit()
    {
        m_Tilemap = GetEntity()->GetComponent< Tilemap< int > >();

        m_PreviewTexture = AssetLibrary< Texture >()->GetAsset( m_PreviewTextureName );
    }


    /// @brief  called once when exiting the scene
    void TilemapEditor::OnExit() {}


//-----------------------------------------------------------------------------
// private: methods
//-----------------------------------------------------------------------------

    
    /// @brief  gets the tile position of the mouse
    /// @return the tile position of the mouse
    glm::ivec2 TilemapEditor::getMouseTilePos() const
    {
       return m_Tilemap->WorldPosToTileCoord( Input()->GetMousePosWorld() );
    }


    /// @brief  updates the brush tool
    void TilemapEditor::updateBrushTool()
    {
        // don't use this tool if it's not selected
        if ( m_ToolButtons[ s_BrushToolIndex ] == -1 )
        {
            return;
        }

        // don't use this tool if the mouse button is up
        if ( Input()->GetMouseDown( m_ToolButtons[ s_BrushToolIndex ] ) == false )
        {
            return;
        }

        // when first starting to use this tool, register an undoable action
        if ( Input()->GetMouseTriggered( m_ToolButtons[ s_BrushToolIndex ] ) )
        {
            pushUndoableAction();
        }

        // get the mouse position
        glm::ivec2 mousePos = getMouseTilePos();

        // skip if the mouse is out of bounds
        if ( m_Tilemap->IsPositionWithinBounds( mousePos ) == false )
        {
            return;
        }

        // draw a tile at the mouse
        m_Tilemap->SetTile( mousePos, m_SelectedTileIndex );
    }

    /// @brief  updates the erase tool
    void TilemapEditor::updateEraseTool()
    {
        // don't use this tool if it's not selected
        if ( m_ToolButtons[ s_EraseToolIndex ] == -1 )
        {
            return;
        }

        // don't use this tool if the mouse button is up
        if ( Input()->GetMouseDown( m_ToolButtons[ s_EraseToolIndex ] ) == false )
        {
            return;
        }

        // when first starting to use this tool, register an undoable action
        if ( Input()->GetMouseTriggered( m_ToolButtons[ s_EraseToolIndex ] ) )
        {
            pushUndoableAction();
        }

        // get the mouse position
        glm::ivec2 mousePos = getMouseTilePos();

        // skip if the mouse is out of bounds
        if ( m_Tilemap->IsPositionWithinBounds( mousePos ) == false )
        {
            return;
        }

        // erase a tile at the mouse
        m_Tilemap->SetTile( mousePos, -1 );
    }

    /// @brief  updates the rectangle selection tool
    void TilemapEditor::updateSelectionTool()
    {

        // cut, copy, and paste
        if ( Input()->GetKeyDown( GLFW_KEY_LEFT_CONTROL ) )
        {
            if ( Input()->GetKeyTriggered( GLFW_KEY_X ) )
            {
                copySelection();
                eraseSelection();
            }
            else if ( Input()->GetKeyTriggered( GLFW_KEY_C ) )
            {
                copySelection();
            }

            if ( Input()->GetKeyTriggered( GLFW_KEY_V ) )
            {
                pasteSelection();
            }
        }

        // erase selection
        if ( Input()->GetKeyTriggered( GLFW_KEY_DELETE ) || Input()->GetKeyTriggered( GLFW_KEY_DELETE ) )
        {
            eraseSelection();
        }


        // don't use this tool if it's not selected
        if ( m_ToolButtons[ s_EraseToolIndex ] == -1 )
        {
            return;
        }

        // don't use this tool if the mouse button is up
        if ( Input()->GetMouseDown( m_ToolButtons[ s_EraseToolIndex ] ) == false )
        {
            return;
        }

        // get the mouse position
        glm::ivec2 mousePos = getMouseTilePos();

        // when first starting to use this tool, set first position
        if ( Input()->GetMouseTriggered( m_ToolButtons[ s_EraseToolIndex ] ) )
        {
            m_SelectionPos0 = mousePos;
        }

        // set the second pos wherever the mouse is as long as it's down
        m_SelectionPos1 = mousePos;
        
    }

    /// @brief  updates the picker tool
    void TilemapEditor::updatePickerTool()
    {
        // don't use this tool if it's not selected
        if ( m_ToolButtons[ s_PickerToolIndex ] == -1 )
        {
            return;
        }

        // do nothing if the key is not triggered
        if ( Input()->GetMouseTriggered( m_ToolButtons[ s_PickerToolIndex ] ) == false )
        {
            return;
        }

        // get the mouse position
        glm::ivec2 mousePos = getMouseTilePos();

        // skip if the mouse is out of bounds
        if ( m_Tilemap->IsPositionWithinBounds( mousePos ) == false )
        {
            return;
        }

        m_SelectedTileIndex = m_Tilemap->GetTile( mousePos );
    }


    /// @brief  clamps the selection to the Tilemap bounds and makes pos0 the top left
    void TilemapEditor::standardizeSelection()
    {
        glm::ivec2 min = glm::ivec2(
            std::min( std::min( m_SelectionPos0.x, m_SelectionPos1.x ), m_Tilemap->GetDimensions().x - 1 ),
            std::min( std::min( m_SelectionPos0.y, m_SelectionPos1.y ), m_Tilemap->GetDimensions().y - 1 )
        );

        glm::ivec2 max = glm::ivec2(
            std::max( std::max( m_SelectionPos0.x, m_SelectionPos1.x ), 0 ),
            std::max( std::max( m_SelectionPos0.y, m_SelectionPos1.y ), 0 )
        );

        m_SelectionPos0 = min;
        m_SelectionPos1 = max;
    }


    /// @brief  copies the current selection onto the clipboard
    void TilemapEditor::copySelection()
    {
        standardizeSelection();

        m_Clipboard.M_Size = m_SelectionPos1 - m_SelectionPos0 + glm::ivec2( 1, 1 );
        m_Clipboard.M_Tiles.clear();

        glm::ivec2 pos;
        for ( pos.y = m_SelectionPos0.y; pos.y <= m_SelectionPos1.y; ++pos.y )
        {
            for ( pos.x = m_SelectionPos0.x; pos.x <= m_SelectionPos1.x; ++pos.x )
            {
                m_Clipboard.M_Tiles.push_back( m_Tilemap->GetTile( pos ) );
            }
        }
    }

    /// @brief  erases all tiles in the current selection
    void TilemapEditor::eraseSelection()
    {
        standardizeSelection();

        pushUndoableAction();

        glm::ivec2 pos;
        for ( pos.y = m_SelectionPos0.y; pos.y <= m_SelectionPos1.y; ++pos.y )
        {
            for ( pos.x = m_SelectionPos0.x; pos.x <= m_SelectionPos1.x; ++pos.x )
            {
                m_Tilemap->SetTile( pos, -1 );
            }
        }
    }

    /// @brief  pastes the current selection into the tilemap
    void TilemapEditor::pasteSelection()
    {
        standardizeSelection();

        pushUndoableAction();

        glm::ivec2 pos;
        for ( pos.y = 0; pos.y < m_Clipboard.M_Size.y; ++pos.y )
        {
            for ( pos.x = 0; pos.x < m_Clipboard.M_Size.x; ++pos.x )
            {
                int tile = m_Clipboard.M_Tiles[ pos.y * m_Clipboard.M_Size.x + pos.x ];
                m_Tilemap->SetTile( pos + m_SelectionPos0, tile );
            }
        }
    }


    /// @brief  saves the current state of the tilemap to the undo stack
    void TilemapEditor::pushUndoableAction()
    {
        if ( m_UndoStackPosition == m_UndoStackCapacity )
        {
            m_UndoStack.pop_front();
            --m_UndoStackPosition;
        }

        if ( m_UndoStackPosition == m_UndoStack.size() )
        {
            m_UndoStack.push_back( Stream::Write( *m_Tilemap ) );
            ++m_UndoStackPosition;
            return;
        }

        m_UndoStack[ m_UndoStackPosition ] = Stream::Write( *m_Tilemap );
        ++m_UndoStackPosition;

        if ( m_UndoStackPosition < m_UndoStack.size() )
        {
            m_UndoStack.erase( m_UndoStack.begin() + m_UndoStackPosition, m_UndoStack.end() );
        }
    }

    /// @brief  undoes the most recent action
    void TilemapEditor::undo()
    {
        if ( m_UndoStackPosition < 1 )
        {
            return;
        }

        --m_UndoStackPosition;
        Stream::Read( *m_Tilemap, m_UndoStack[ m_UndoStackPosition ] );
    }

    /// @brief  redoes the most recent undo
    void TilemapEditor::redo()
    {
        if ( m_UndoStackPosition >= m_UndoStack.size() - 1 )
        {
            return;
        }

        ++m_UndoStackPosition;
        Stream::Read( *m_Tilemap, m_UndoStack[ m_UndoStackPosition ] );
    }


    /// @brief  displays the current selection
    void TilemapEditor::displaySelection() const
    {
        // TODO: display selection
    }


//-----------------------------------------------------------------------------
// public: inspection
//-----------------------------------------------------------------------------


    /// @brief  shows the inspector for TilemapEditor
    void TilemapEditor::Inspector()
    {
        ImGui::InputInt( "selected tile index", &m_SelectedTileIndex, 1, 5 );

        if ( ImGui::Button( "undo" ) )
        {
            undo();
        }

        if ( ImGui::Button( "redo" ) )
        {
            redo();
        }

        // TODO: rest of inspector

        updateBrushTool();
        updateEraseTool();
        updatePickerTool();
        updateSelectionTool();
    }


//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------


    /// @brief  reads the name of the preview texture asset
    /// @param  data    the json data to read from
    void TilemapEditor::readPreviewTextureName( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_PreviewTextureName, data );
    }

    /// @brief  reads the mouse buttons assigned to each tool
    /// @param  data    the json data to read from
    void TilemapEditor::readToolButtons( nlohmann::ordered_json const& data )
    {
        if ( data.size() != 4 )
        {
            Debug() << "WARNING: json TilemapEditor ToolButtons was not an array of size 4" << std::endl;
            return;
        }

        for ( int i = 0; i < 4; ++i )
        {
            Stream::Read( m_ToolButtons[ i ], data[ i ] );
        }
    }

    /// @brief  reads the maximum number of undoable actions that will be kept track of
    /// @param  data    the json data to read from
    void TilemapEditor::readUndoStackCapacity( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_UndoStackCapacity, data );
    }

//-----------------------------------------------------------------------------
// public: reading / writing
//-----------------------------------------------------------------------------


    /// @brief  gets the map of read methods for this Component
    /// @return the map of read methods for this Component
    ReadMethodMap< ISerializable > const& TilemapEditor::GetReadMethods() const
    {
        static ReadMethodMap< TilemapEditor > const readMethods = {
            { "PreviewTextureName", &TilemapEditor::readPreviewTextureName },
            { "ToolButtons"       , &TilemapEditor::readToolButtons        },
            { "UndoStackCapacity" , &TilemapEditor::readUndoStackCapacity  }
        };

        return (ReadMethodMap< ISerializable > const&)readMethods;
    }


    /// @brief  Writes all AudioPlayr data to a JSON file.
    /// @return The JSON file containing the data.
    nlohmann::ordered_json TilemapEditor::Write() const
    {
        nlohmann::ordered_json json;

        json[ "PreviewTextureName" ] = Stream::Write( m_PreviewTextureName );
        json[ "UndoStackCapacity"  ] = Stream::Write( m_UndoStackCapacity  );

        nlohmann::ordered_json& toolButtons = json[ "ToolButtons" ];
        for ( int button : m_ToolButtons )
        {
            toolButtons.push_back( button );
        }

        return json;
    }

    
//-----------------------------------------------------------------------------
// public: copying
//-----------------------------------------------------------------------------


    /// @brief  clones this TilemapEditor
    /// @return the newly created clone of this TilemapEditor
    TilemapEditor* TilemapEditor::Clone() const
    {
        return new TilemapEditor( *this );
    }


//-----------------------------------------------------------------------------
// private: copying
//-----------------------------------------------------------------------------


    /// @brief  copy-constructor for the TilemapEditor
    /// @param  other   the other TilemapEditor to copy
    TilemapEditor::TilemapEditor( TilemapEditor const& other ) :
        Component( other ),
        m_PreviewTextureName( other.m_PreviewTextureName )
    {}


//-----------------------------------------------------------------------------
