/// @file     Tilemap.h
/// @author   Eli Tsereteli (ilya.tsereteli@digipen.edu)
/// 
/// @brief    Tilemap component - loads and manages a tilemap array.
#pragma once
#include "Component.h"
#include <vector>
#include <sstream>  // accept new text


/// @brief        Loads and manages a tilemap array.
class Tilemap : public Component
{
public:

    /// @brief    Default constructor
    Tilemap();


    /// @return   A copy of this component
    virtual Component * Clone() const override;



//-----------------------------------------------------------------------------
//          Public methods
//-----------------------------------------------------------------------------
public:

    /// @brief          Retrieve entire tilemap
    /// @return         array (vector) of tile IDs
    __inline std::vector<int> const& GetTilemap() const { return m_Tilemap; }

    /// @brief          Sets the whole tilemap to a given array (vector)
    /// @param  tiles   vector of tile IDs
    void SetTilemap(std::vector<int> const& tiles);


    /// @brief          Gets the index of the tile at given coordinate.
    /// @param coord    tile 2D coordinate (within the tilemap)
    /// @return         index of the tile
    __inline int GetTile(glm::ivec2 coord) const { return m_Tilemap[coord.y*m_RowWidth + coord.x]; }

    /// @brief          Sets the tile at given coordinate to given index.
    /// @param coord    Tile 2D coordinate (within the tilemap)
    /// @param tileID   Index to change the tile to
    void SetTile(glm::ivec2 coord, int tileID);


    /// @brief          Gets tilemap coordinate of the tile at given world position.
    /// @param pos      Position in world space
    /// @return         Tilemap coordinate of the tile. If the provided location
    ///                 is outside the tilemap, returns (-1,-1)
    glm::ivec2 WorldPosToTileCoord(glm::vec2 pos);


    /// @brief          Gets world position of the given tilemap coordinates.
    /// @param coord    Tilemap coordinate
    /// @return         World position of the tile at given tilemap coordinate
    glm::vec2 TileCoordToWorldPos(glm::ivec2 coord);



//-----------------------------------------------------------------------------
//          Virtual overrides
//-----------------------------------------------------------------------------
private:

    /// @brief  called when entering a scene
    virtual void OnInit() override;



//-----------------------------------------------------------------------------
//              Data
//-----------------------------------------------------------------------------
private:

    /// @brief   Tilemap array
    std::vector<int> m_Tilemap;

    /// @brief   Keep a local copy of TilemapSprite's row width for coord accessors
    int m_RowWidth = 0;

    /// @brief   Inverse transform for converting world position to tile coord
    glm::mat4 m_InvMat = glm::mat4(1);

    /// @brief   Parent's old transform mat, for comparing. Invert as rarely as possible
    glm::mat4 m_Mat = glm::mat4(1);



//-----------------------------------------------------------------------------
//              Helpers
//-----------------------------------------------------------------------------
private:

    /// @brief  Loads tilemap array into TilemapSprite of the parent for rendering.
    void loadTilemapIntoSprite();

    /// @brief  Updates inverse transform matrix, if parent's transform has changed.
    void updateMat();


//-----------------------------------------------------------------------------
//              Reading
//-----------------------------------------------------------------------------
private:

    /// @brief  Read in the text this Tilemap displays
    /// @param  stream  The json to read from.
    void readTilemap( nlohmann::ordered_json const& data );

    /// @brief the map of read methods for this Component
    static ReadMethodMap< Tilemap > const s_ReadMethods;

    /// @brief gets the map of read methods for this Component
    /// @return the map of read methods for this Component
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override
    {
        return (ReadMethodMap< ISerializable > const&)s_ReadMethods;
    }

};
