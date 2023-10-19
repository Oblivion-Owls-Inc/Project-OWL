/// @file     Tilemap.h
/// @author   Eli Tsereteli (ilya.tsereteli@digipen.edu)
/// 
/// @brief    Tilemap component - loads and manages a tilemap array.
#pragma once
#define TILEMAP_H

#include "Behavior.h"
#include "Transform.h"
#include <functional>   // callbacks
#include <map>


/// @brief        Loads and manages a tilemap array.
template < typename TileType >
class Tilemap : public Behavior
{
public:

    /// @brief    Default constructor
    Tilemap();

    /// @return   A copy of this component
    virtual Component * Clone() const override;

    /// @brief        Copy constructor
    /// @param other  Tilemap to copy
    Tilemap(Tilemap const& other);



//-----------------------------------------------------------------------------
//          Public methods
//-----------------------------------------------------------------------------
public:

    /// @brief          Retrieve entire tilemap
    /// @return         array (vector) of tile IDs
    __inline std::vector<TileType> const& GetTilemap() const { return m_Tilemap; }

    /// @brief          Sets the whole tilemap to a given array (vector)
    /// @param  tiles   vector of tile IDs
    void SetTilemap(std::vector<TileType> const& tiles);


    /// @brief          Gets the index of the tile at given coordinate.
    /// @param coord    tile 2D coordinate (within the tilemap)
    /// @return         index of the tile
    __inline TileType GetTile(glm::ivec2 coord) const { return m_Tilemap[coord.y*m_RowWidth + coord.x]; }

    /// @brief          Sets the tile at given coordinate to given index.
    /// @param coord    Tile 2D coordinate (within the tilemap)
    /// @param tileID   Index to change the tile to
    void SetTile(glm::ivec2 coord, TileType const& tile);


    /// @brief          Gets tilemap coordinate of the tile at given world position.
    ///                 NOTE: this assumes the (0,0) tile is the top-left.
    /// @param pos      Position in world space
    /// @return         Tilemap coordinate of the tile. If the provided location
    ///                 is outside the tilemap, returns (-1,-1)
    glm::ivec2 WorldPosToTileCoord(glm::vec2 pos);


    /// @brief          Gets world position of the given tilemap coordinates.
    ///                 NOTE: this assumes the (0,0) tile is the top-left.
    /// @param coord    Tilemap coordinate
    /// @return         World position of the tile at given tilemap coordinate
    glm::vec2 TileCoordToWorldPos(glm::ivec2 coord);


    /// @brief          Adds a function to the list of callbacks. The given
    ///                 function will get called whenever the tilemap is updated.
    /// @param objPtr   Pointer to object (this), used to keep track of function.
    /// @param function Callback function
    void AddOnTilemapChangedCallback( int componentID, std::function<void()> function );

    /// @brief          Removes a function from the list of callbacks
    /// @param objPtr   Pointer to object (this)
    void RemoveOnTilemapChangedCallback(int componentID);


    /// @brief          Sets the tile scale. Default scale (1,1) is the full
    ///                 width/height of single tile.
    /// @param mults    x=horizontal, y=vertical
    __inline void SetTileScale(glm::vec2 mults) { m_TileScale = mults; m_Modified = true; }


    /// @brief          Retreives the tile scale.
    /// @return         x=horizontal, y=vertical
    __inline glm::vec2 GetTileScale() const { return m_TileScale; }


    /// @brief          Sets the width of a single row (amount of columns)
    /// @param columns  (tiles per row)
    __inline void SetRowWidth(int width) { m_RowWidth = width; m_Modified = true; }


    /// @return         Tiles per row 
    __inline int GetTilemapWidth() const { return m_RowWidth; }

    /// @return         Tiles per column
    __inline int GetTilemapHeight() const { return (int)m_Tilemap.size() / m_RowWidth; }


    /// @brief  gets the Tilemap to word matrix
    /// @return the matrix
    glm::mat4 const& GetTilemapToWorldMatrix() const { return m_Mat; }

    /// @brief  gets the World to Tilemap matrix
    /// @return the matrix
    glm::mat4 const& GetWorldToTilemapMatrix() const { return m_InvMat; }


//-----------------------------------------------------------------------------
//          Virtual overrides
//-----------------------------------------------------------------------------
private:

    /// @brief  called after component is added & read
    virtual void OnInit() override;

    /// @brief  called every frame
    virtual void OnUpdate( float dt );

    /// @brief  called when component is removed
    virtual void OnExit() override;

    virtual void OnFixedUpdate() {};

    /// @brief Used by the Debug System to display information about this Component
    virtual void Inspector();

//-----------------------------------------------------------------------------
//              Data
//-----------------------------------------------------------------------------
private:

    /// @brief   Tilemap array
    std::vector<TileType> m_Tilemap;

    /// @brief   Width of each row (amount of columns)
    int m_RowWidth = 10;

    /// @brief   Scale of tiles (on top of transform) - to adjust spacing
    glm::vec2 m_TileScale = {1,1};

    /// @brief   Matrix for converting tile coord to world pos
    glm::mat4 m_Mat = glm::mat4(1);

    /// @brief   Matrix for converting world pos to tile coord
    glm::mat4 m_InvMat = glm::mat4(1);

    /// @brief   Parent's transform (cached)
    Transform* m_PTransform = nullptr;

    /// @brief   Callback functions - they get called whenever tilemap changes.
    std::map< int, std::function<void()> > m_Callbacks;

    /// @brief   Whether tilemap has been modified during this frame
    bool m_Modified = false;


//-----------------------------------------------------------------------------
//              Helpers
//-----------------------------------------------------------------------------
private:

    /// @brief  Updates inverse transform matrix, if parent's transform has changed.
    void updateMat();


//-----------------------------------------------------------------------------
//              Reading
//-----------------------------------------------------------------------------
private:

    /// @brief          Read in the tile data
    /// @param stream   The json to read from.
    void readTilemap( nlohmann::ordered_json const& data);

    /// @brief          Read in the row width of the tilemap
    /// @param stream   The json to read from
    void readRowWidth( nlohmann::ordered_json const& data );

    /// @brief            Read in the stride multiplier
    /// @param  stream    The json to read from.
    void readTileScale( nlohmann::ordered_json const& data );

    /// @brief the map of read methods for this Component
    static ReadMethodMap< Tilemap > const s_ReadMethods;

    /// @brief gets the map of read methods for this Component
    /// @return the map of read methods for this Component
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override
    {
        return (ReadMethodMap< ISerializable > const&)s_ReadMethods;
    }

public:

    /// @brief Write all Tilemap component data to a JSON file.
    /// @return The JSON file containing the Tilemap component data.
    virtual nlohmann::ordered_json Write() const override;

};


#ifndef TILEMAP_C
#include "Tilemap.cpp"
#endif
