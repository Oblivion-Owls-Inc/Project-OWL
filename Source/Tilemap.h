/// @file     Tilemap.h
/// @author   Eli Tsereteli (ilya.tsereteli@digipen.edu)
/// 
/// @brief    A version of Sprite specifically for rendering text. Uses GPU
///           instancing to draw multiple letters simultaneously.
#pragma once
#include "Component.h"
#include <vector>
#include <sstream>  // accept new text


/// @brief       Uses TilemapSprite attached to parent entity to draw text.
class Tilemap : public Component
{
public:
    /// @brief   Default constructor
    Tilemap();

    // Inherited via Component
    virtual Component * Clone() const override;


    /// @brief          Retrieve entire tilemap
    /// @return         array (vector) of tile IDs
    __inline std::vector<int> const& GetTilemap() const { return m_Tilemap; }


    /// @brief          Sets the whole tilemap to a given array (vector)
    /// @param  tiles   vector of tile IDs
    __inline void SetTilemap( std::vector<int> const& tiles ) { m_Tilemap = tiles; 
                                                                loadTilemapIntoSprite(); } 


    /// @brief          Gets the index of the tile at given coordinate.
    /// @param x        column
    /// @param y        row
    /// @return         index of the tile
    __inline int GetTile(int x, int y) const { return m_Tilemap[y*m_RowWidth + x]; }


    /// @brief          Sets the tile at given coordinate to given index.
    /// @param x        column
    /// @param y        row
    /// @param tileID   index to change the tile to
    void SetTile(int x, int y, int tileID);


    //-----------------------------------------------------------------------------
private: // overrides
    //-----------------------------------------------------------------------------
    /// @brief  called when entering a scene
    virtual void OnInit() override;


    //-----------------------------------------------------------------------------
private: // member variables / helpers
    //-----------------------------------------------------------------------------

    /// @brief  text displayed by the textSprite
    std::vector<int> m_Tilemap;

    /// @brief  Keep a local copy of TilemapSprite's row width for 2D array accessors
    int m_RowWidth = 0;

    /// @brief  Loads tilemap array into TilemapSprite of the parent for rendering.
    void loadTilemapIntoSprite();


    //-------------------------------------------------------------------------
private: // Reading
    //-------------------------------------------------------------------------

    /// @brief  Read in the text this Tilemap displays
    /// @param  stream  The json to read from.
    void readTilemap(Stream stream);

    /// @brief the map of read methods for this Component
    static ReadMethodMap< Tilemap > const s_ReadMethods;

    /// @brief gets the map of read methods for this Component
    /// @return the map of read methods for this Component
    virtual ReadMethodMap< Component > const& GetReadMethods() const override;

    //-------------------------------------------------------------------------
};
