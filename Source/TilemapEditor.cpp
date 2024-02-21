/// @file       TilemapEditor.h
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      Component that allows direct editing of a Tilemap
/// @version    0.1
/// @date       2024-02-08
/// 
/// @copyright  Copyright (c) 2024 Digipen Institute of Technology

#pragma once

#include "TilemapEditor.h"

#include "ComponentReference.t.h"
#include "Tilemap.h"

#include "Texture.h"
#include "AssetLibrarySystem.h"

#include "InputSystem.h"
#include "RenderSystem.h"

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
        m_Tilemap.SetOnConnectCallback(
            [ this ]()
            {
                pushUndoableAction();
            }
        );

        m_Tilemap.Init( GetEntity() );

        m_PreviewTexture.SetOwnerName( GetName() );
        m_PreviewTexture.Init();
    }


    /// @brief  called once when exiting the scene
    void TilemapEditor::OnExit()
    {
        m_Tilemap.Exit( GetEntity() );
    }


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

        static bool changeMade = false;

        // don't use this tool if the mouse button is up
        if ( Input()->GetMouseDown( m_ToolButtons[ s_BrushToolIndex ] ) == false )
        {
            if ( changeMade && Input()->GetMouseReleased( m_ToolButtons[ s_BrushToolIndex ] ) )
            {
                pushUndoableAction();
            }
            return;
        }


        // when first starting to use this tool, register an undoable action
        if ( Input()->GetMouseTriggered( m_ToolButtons[ s_BrushToolIndex ] ) )
        {
            changeMade = false;
        }

        // get the mouse position
        glm::ivec2 mousePos = getMouseTilePos();

        // skip if the mouse is out of bounds
        if ( m_Tilemap->IsPositionWithinBounds( mousePos ) == false )
        {
            return;
        }

        // draw a tile at the mouse
        if ( changeMade == false )
        {
            changeMade = true;
        }
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
        
        static bool changeMade = false;
        // don't use this tool if the mouse button is up
        if ( Input()->GetMouseDown( m_ToolButtons[ s_EraseToolIndex ] ) == false )
        {
            if ( changeMade && Input()->GetMouseReleased( m_ToolButtons[ s_EraseToolIndex ] ) )
            {
                pushUndoableAction();
            }
            return;
        }

        // when first starting to use this tool, register an undoable action
        if ( Input()->GetMouseTriggered( m_ToolButtons[ s_EraseToolIndex ] ) )
        {
            changeMade = false;
        }

        // get the mouse position
        glm::ivec2 mousePos = getMouseTilePos();

        // skip if the mouse is out of bounds
        if ( m_Tilemap->IsPositionWithinBounds( mousePos ) == false )
        {
            return;
        }

        // erase a tile at the mouse
        if ( changeMade == false )
        {
            changeMade = true;
        }
        m_Tilemap->SetTile( mousePos, -1 );
    }

    /// @brief  updates the rectangle selection tool
    void TilemapEditor::updateSelectionTool()
    {

        // don't use this tool if it's not selected
        if ( m_ToolButtons[ s_SelectionToolIndex ] == -1 )
        {
            return;
        }

        // don't use this tool if the mouse button is up
        if ( Input()->GetMouseDown( m_ToolButtons[ s_SelectionToolIndex ] ) == false )
        {
            return;
        }

        // get the mouse position
        glm::ivec2 mousePos = getMouseTilePos();

        // when first starting to use this tool, set first position
        if ( Input()->GetMouseTriggered( m_ToolButtons[ s_SelectionToolIndex ] ) || m_SelectionPos0 == glm::ivec2( -1, -1 ) )
        {
            m_SelectionPos0 = mousePos;
        }

        if ( m_Tilemap->IsPositionWithinBounds( mousePos ) == false )
        {
            return;
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


    /// @brief  updates the hotkeys the TilemapEditor uses for copy/paste, undo/redo, and similar
    void TilemapEditor::updateHotkeys()
    {
        if ( Input()->GetKeyDown( GLFW_KEY_LEFT_CONTROL ) )
        {
            // copy/cut/paste
            if ( Input()->GetKeyTriggered( GLFW_KEY_C ) )
            {
                copySelection();
            }
            if ( Input()->GetKeyTriggered( GLFW_KEY_X ) )
            {
                copySelection();
                fillSelection( -1 );
            }
            if ( Input()->GetKeyTriggered( GLFW_KEY_V ) )
            {
                pasteSelection();
            }

            // undo / redo
            if ( Input()->GetKeyTriggered( GLFW_KEY_Z ) )
            {
                if ( Input()->GetKeyDown( GLFW_KEY_LEFT_SHIFT ) )
                {
                    redo();
                }
                else
                {
                    undo();
                }
            }
            if ( Input()->GetKeyTriggered( GLFW_KEY_Y ) )
            {
                redo();
            }
        }

        // erase selection
        if ( Input()->GetKeyTriggered( GLFW_KEY_DELETE ) || Input()->GetKeyTriggered( GLFW_KEY_BACKSPACE ) )
        {
            fillSelection( -1 );
        }

        // fill selection
        if ( Input()->GetKeyTriggered( GLFW_KEY_F ) )
        {
            fillSelection( m_SelectedTileIndex );
        }
    }


    /// @brief  clamps the selection to the Tilemap bounds and makes pos0 the top left
    /// @return whether the selection is valid
    bool TilemapEditor::standardizeSelection()
    {
        if ( m_SelectionPos0 == glm::ivec2( -1, -1 ) )
        {
            return false;
        }

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

        return true;
    }


    /// @brief  copies the current selection onto the clipboard
    void TilemapEditor::copySelection()
    {
        bool selectionValid = standardizeSelection();
        if ( selectionValid == false )
        {
            return;
        };

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

    /// @brief  fills the selection with the specified tile
    void TilemapEditor::fillSelection( int tileId )
    {
        bool selectionValid = standardizeSelection();
        if ( selectionValid == false )
        {
            return;
        };

        glm::ivec2 pos;
        for ( pos.y = m_SelectionPos0.y; pos.y <= m_SelectionPos1.y; ++pos.y )
        {
            for ( pos.x = m_SelectionPos0.x; pos.x <= m_SelectionPos1.x; ++pos.x )
            {
                m_Tilemap->SetTile( pos, tileId );
            }
        }

        pushUndoableAction();
    }

    /// @brief  pastes the current selection into the tilemap
    void TilemapEditor::pasteSelection()
    {
        bool selectionValid = standardizeSelection();
        if ( selectionValid == false )
        {
            return;
        };

        glm::ivec2 pos;
        for ( pos.y = 0; pos.y < m_Clipboard.M_Size.y; ++pos.y )
        {
            int destY = pos.y + m_SelectionPos0.y;
            if ( destY < 0 )
            {
                continue;
            }
            if ( destY >= m_Tilemap->GetDimensions().y )
            {
                break;
            }

            for ( pos.x = 0; pos.x < m_Clipboard.M_Size.x; ++pos.x )
            {
                int destX = pos.x + m_SelectionPos0.x;
                if ( destX < 0 )
                {
                    continue;
                }
                if ( destX >= m_Tilemap->GetDimensions().x )
                {
                    break;
                }

                int tile = m_Clipboard.M_Tiles[ pos.y * m_Clipboard.M_Size.x + pos.x ];
                m_Tilemap->SetTile( glm::ivec2( destX, destY ), tile);
            }
        }

        pushUndoableAction();
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
        if ( m_UndoStackPosition <= 1 )
        {
            return;
        }

        --m_UndoStackPosition;
        Stream::Read( *m_Tilemap, m_UndoStack[ m_UndoStackPosition - 1 ] );
    }

    /// @brief  redoes the most recent undo
    void TilemapEditor::redo()
    {
        if ( m_UndoStackPosition >= m_UndoStack.size() )
        {
            return;
        }

        Stream::Read( *m_Tilemap, m_UndoStack[ m_UndoStackPosition ] );
        ++m_UndoStackPosition;
    }


    /// @brief  displays the current selection
    void TilemapEditor::displaySelection() const
    {
        if ( m_SelectionPos0 == glm::ivec2( -1, -1 ) )
        {
            return;
        }

        glm::vec2 pos0 = m_Tilemap->TileCoordToWorldPos( m_SelectionPos0 );
        glm::vec2 pos1 = m_Tilemap->TileCoordToWorldPos( m_SelectionPos1 );

        glm::vec2 middle = 0.5f * ( pos0 + pos1 );
        glm::vec2 scale = glm::vec2( std::abs( pos1.x - pos0.x ), std::abs( pos1.y - pos0.y ) );
        scale += m_Tilemap->GetTileScale();

        Renderer()->DrawRect( middle, scale, 0.0f, m_SelectionColor, m_SelectionAlpha );
    }


    /// @brief  displays the preview tile and grid
    void TilemapEditor::displayPreview() const
    {

        glm::ivec2 tilePos = m_Tilemap->WorldPosToTileCoord( Input()->GetMousePosWorld() );
        if ( m_Tilemap->IsPositionWithinBounds( tilePos ) == false )
        {
            return;
        }
        glm::vec2 pos = m_Tilemap->TileCoordToWorldPos( tilePos );

        static Texture const grid = Texture( "Data/Textures/Debug/Grid.png" );
        Renderer()->DrawTexture( &grid, pos, glm::vec2( 5.0f, 5.0f ), 0.0f, glm::vec4( 0.0f ), m_GridAlpha );

        if ( m_SelectedTileIndex != -1 )
        {
            Renderer()->DrawTexture( m_PreviewTexture, pos, glm::vec2( 1.0f, 1.0f ), 0.0f, glm::vec4( 0.0f ), m_PreviewAlpha, true, m_SelectedTileIndex );
        }
    }


//-----------------------------------------------------------------------------
// public: inspection
//-----------------------------------------------------------------------------


    /// @brief  shows the inspector for TilemapEditor
    void TilemapEditor::Inspector()
    {
        if ( m_Tilemap == nullptr )
        {
            ImGui::Text( "this TilemapEditor cannot find a tilemap to edit" );
            return;
        }

        ImGui::InputInt( "selected tile index", &m_SelectedTileIndex, 1, 5 );


        if ( m_PreviewTexture != nullptr )
        {
            glm::ivec2 sheetDimensions = m_PreviewTexture->GetSheetDimensions();
            for ( int i = 0; i < sheetDimensions.x * sheetDimensions.y; ++i )
            {
                if ( i % sheetDimensions.x != 0 )
                {
                    ImGui::SameLine();
                }
                glm::vec4 color = m_SelectedTileIndex == i ? glm::vec4( 1.0f, 1.0f, 1.0f, 1.0f ) : glm::vec4( 0.0f );
                m_PreviewTexture->DisplayInInspector( i, (float)m_PreviewTexture->GetPixelDimensions().x / sheetDimensions.x, glm::vec4( 1.0f ), color );
                if ( ImGui::IsItemClicked() )
                {
                    m_SelectedTileIndex = i;
                }
            }
        }


        ImGui::DragInt2( "selection Pos 1", &m_SelectionPos0[ 0 ], 0.05f, 0, INT_MAX );
        ImGui::DragInt2( "selection Pos 2", &m_SelectionPos1[ 0 ], 0.05f, 0, INT_MAX );

        if ( ImGui::Button( "undo" ) )
        {
            undo();
        }

        if ( ImGui::Button( "redo" ) )
        {
            redo();
        }

        ImGui::ColorEdit4( "selection highlight color", &m_SelectionColor[ 0 ] );
        ImGui::DragFloat( "selection alpha", &m_SelectionAlpha );
        ImGui::DragFloat( "grid alpha", &m_GridAlpha );
        ImGui::DragFloat( "preview alpha", &m_PreviewAlpha );

       m_PreviewTexture.Inspect( "preview texture" );

        updateBrushTool();
        updateEraseTool();
        updatePickerTool();
        updateSelectionTool();
        updateHotkeys();

        displaySelection();
        displayPreview();
    }


//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------


    /// @brief  reads the name of the preview texture asset
    /// @param  data    the json data to read from
    void TilemapEditor::readPreviewTexture( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_PreviewTexture, data );
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

    /// @brief  reads the color to highlight the selection with
    /// @param  data    the json data to read from
    void TilemapEditor::readSelectionColor( nlohmann::ordered_json const& data )
    {
        Stream::Read( &m_SelectionColor, data );
    }

    /// @brief  reads the transparency of the selection
    /// @param  data    the json data to read from
    void TilemapEditor::readSelectionAlpha( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_SelectionAlpha, data );
    }

    /// @brief  reads the transparency of the grid
    /// @param  data    the json data to read from
    void TilemapEditor::readGridAlpha( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_GridAlpha, data );
    }

    /// @brief  reads the transparency of the preview
    /// @param  data    the json data to read from
    void TilemapEditor::readPreviewAlpha( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_PreviewAlpha, data );
    }

//-----------------------------------------------------------------------------
// public: reading / writing
//-----------------------------------------------------------------------------


    /// @brief  gets the map of read methods for this Component
    /// @return the map of read methods for this Component
    ReadMethodMap< ISerializable > const& TilemapEditor::GetReadMethods() const
    {
        static ReadMethodMap< TilemapEditor > const readMethods = {
            { "PreviewTexture"   , &TilemapEditor::readPreviewTexture    },
            { "ToolButtons"      , &TilemapEditor::readToolButtons       },
            { "UndoStackCapacity", &TilemapEditor::readUndoStackCapacity },
            { "SelectionColor"   , &TilemapEditor::readSelectionColor    },
            { "SelectionAlpha"   , &TilemapEditor::readSelectionAlpha    },
            { "GridAlpha"        , &TilemapEditor::readGridAlpha         },
            { "PreviewAlpha"     , &TilemapEditor::readPreviewAlpha      }
        };

        return (ReadMethodMap< ISerializable > const&)readMethods;
    }


    /// @brief  Writes all AudioPlayr data to a JSON file.
    /// @return The JSON file containing the data.
    nlohmann::ordered_json TilemapEditor::Write() const
    {
        nlohmann::ordered_json json;

        json[ "PreviewTexture"    ] = Stream::Write( m_PreviewTexture    );
        json[ "UndoStackCapacity" ] = Stream::Write( m_UndoStackCapacity );
        json[ "SelectionColor"    ] = Stream::Write( m_SelectionColor    );
        json[ "SelectionAlpha"    ] = Stream::Write( m_SelectionAlpha    );
        json[ "GridAlpha"         ] = Stream::Write( m_GridAlpha         );
        json[ "PreviewAlpha"      ] = Stream::Write( m_PreviewAlpha      );

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
        m_UndoStackCapacity( other.m_UndoStackCapacity ),
        m_SelectionColor   ( other.m_SelectionColor    ),
        m_SelectionAlpha   ( other.m_SelectionAlpha    ),
        m_GridAlpha        ( other.m_GridAlpha         ),
        m_PreviewAlpha     ( other.m_PreviewAlpha      ),

        m_PreviewTexture( other.m_PreviewTexture )
    {
        for ( int i = 0; i < 4; ++i )
        {
            m_ToolButtons[ i ] = other.m_ToolButtons[ i ];
        }
    }


//-----------------------------------------------------------------------------
