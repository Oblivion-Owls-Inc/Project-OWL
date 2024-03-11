/// @file     Tilemap.h
/// @author   Eli Tsereteli (ilya.tsereteli@digipen.edu)
/// 
/// @brief    Tilemap component - loads and manages a tilemap array.

#pragma once

#define TILEMAP_H

#include "pch.h" 
#include "Component.h"

#include "ComponentReference.h"
#include "Transform.h"


/// @brief  untemplatized base Tilemap class
class TilemapBase : public Component
{
//-----------------------------------------------------------------------------
protected: // constructor
//-----------------------------------------------------------------------------

    /// @brief    Default constructor
    TilemapBase( std::type_index type ) :
        Component( type )
    {}

    /// @brief        Copy constructor
    /// @param other  TilemapBase to copy
    TilemapBase( TilemapBase const& other ) :
        Component( other )
    {}

//-----------------------------------------------------------------------------    
};


/// @brief        Loads and manages a tilemap array.
template < typename TileType >
class Tilemap : public TilemapBase
{
//-----------------------------------------------------------------------------
public: // constructors
//-----------------------------------------------------------------------------


    /// @brief    Default constructor
    Tilemap();

    /// @return   A copy of this component
    virtual Tilemap* Clone() const override;


//-----------------------------------------------------------------------------
private: // constructors
//-----------------------------------------------------------------------------


    /// @brief        Copy constructor
    /// @param other  Tilemap to copy
    Tilemap( Tilemap const& other );

//-----------------------------------------------------------------------------
public: // types
//-----------------------------------------------------------------------------


    /// @brief  OnTilemapChanged callback type
    /// @param  tilemap         the tilemap this callback was called from
    /// @param  tilepos         the position of the tile that was changed - will be (-1, -1) if whole tilemap changed
    /// @param  previousValue   the value of the tile before it was changed - garbage if whole tilemap changed
    using OnTilemapChangedCallback = std::function< void (
        Tilemap< TileType >* tilemap,
        glm::ivec2 const& tilePos,
        TileType const& previousValue
    ) >;


//-----------------------------------------------------------------------------
public: // methods
//-----------------------------------------------------------------------------


    /// @brief          Retrieve entire tilemap
    /// @return         array (vector) of tile IDs
    std::vector<TileType> const& GetTilemap() const { return m_Tiles; }

    /// @brief          Sets the whole tilemap to a given array (vector)
    /// @param  tiles   vector of tile IDs
    void SetTilemap(std::vector<TileType> const& tiles);


    /// @brief          Gets the index of the tile at given coordinate.
    /// @param coord    tile 2D coordinate (within the tilemap)
    /// @return         index of the tile
    TileType GetTile(glm::ivec2 coord) const { return m_Tiles[coord.y*m_Dimensions.x + coord.x]; }

    /// @brief          Sets the tile at given coordinate to given index.
    /// @param coord    Tile 2D coordinate (within the tilemap)
    /// @param tileID   Index to change the tile to
    void SetTile(glm::ivec2 coord, TileType const& tile);


    /// @brief          Gets tilemap coordinate of the tile at given world position.
    ///                 NOTE: this assumes the (0,0) tile is the top-left.
    /// @param pos      Position in world space
    /// @return         Tilemap coordinate of the tile. If the provided location
    ///                 is outside the tilemap, returns (-1,-1)
    glm::ivec2 WorldPosToTileCoord( glm::vec2 pos ) const;


    /// @brief          Gets world position of the given tilemap coordinates.
    ///                 NOTE: this assumes the (0,0) tile is the top-left.
    /// @param  coord   Tilemap coordinate
    /// @return         World position of the tile at given tilemap coordinate
    glm::vec2 TileCoordToWorldPos( glm::ivec2 coord ) const;


    /// @brief  Adds a function to the list of callbacks. The given function will get called whenever the tilemap is updated.
    /// @param  ownerId     the ID of the owner of the callback
    /// @param  callback    the callback function
    void AddOnTilemapChangedCallback( unsigned ownerId, OnTilemapChangedCallback callback );

    /// @brief          Removes a function from the list of callbacks
    /// @param  ownerId the ID of the owner of the callback
    void RemoveOnTilemapChangedCallback( unsigned ownerId );


    /// @brief          Sets the tile scale. Default scale (1,1) is the full width/height of single tile.
    /// @param mults    x=horizontal, y=vertical
    void SetTileScale( glm::vec2 mults ) { m_TileScale = mults; callOnTilemapChangedCallbacks(); }

    /// @brief          Retreives the tile scale.
    /// @return         x=horizontal, y=vertical
    glm::vec2 GetTileScale() const { return m_TileScale; }


    /// @brief  sets the size of the tilemap in tiles
    /// @param  dimensions  the size of the tilemap
    void SetDimensions( glm::ivec2 const& dimensions ) { m_Dimensions = dimensions; m_Tiles.resize( dimensions.x * dimensions.y ); }

    /// @return size of the tilemap in tiles
    glm::ivec2 GetDimensions() const { return m_Dimensions; }


    /// @brief  gets the Tilemap to word matrix
    /// @return the matrix
    glm::mat4 const& GetTilemapToWorldMatrix() const { return m_Mat; }

    /// @brief  gets the World to Tilemap matrix
    /// @return the matrix
    glm::mat4 const& GetWorldToTilemapMatrix() const { return m_InvMat; }


    /// @brief  gets whether a tile is within this Tilemap
    /// @param  tilePos the tile position to check
    /// @return whether the tile position is within this Tilemap
    bool IsPositionWithinBounds( glm::ivec2 const& tilePos ) const;


//-----------------------------------------------------------------------------
private: // virtual overrides
//-----------------------------------------------------------------------------


    /// @brief  called after component is added & read
    virtual void OnInit() override;

    /// @brief  called after component is removed
    virtual void OnExit() override;


    /// @brief Used by the Debug System to display information about this Component
    virtual void Inspector() override;


//-----------------------------------------------------------------------------
private: // data
//-----------------------------------------------------------------------------


    /// @brief   Tilemap array
    std::vector<TileType> m_Tiles;

    /// @brief   Size of the Tilemap in tiles
    glm::ivec2 m_Dimensions = { 10, 0 };

    /// @brief   Scale of tiles (on top of transform) - to adjust spacing
    glm::vec2 m_TileScale = {1,1};

    /// @brief   Matrix for converting tile coord to world pos
    glm::mat4 m_Mat = glm::mat4(1);

    /// @brief   Matrix for converting world pos to tile coord
    glm::mat4 m_InvMat = glm::mat4(1);

    /// @brief   Parent's transform (cached)
    ComponentReference< Transform > m_Transform;

    /// @brief   Callback functions - they get called whenever tilemap changes.
    std::map< unsigned, OnTilemapChangedCallback > m_OnTilemapChangedCallbacks;


//-----------------------------------------------------------------------------
private: // methods
//-----------------------------------------------------------------------------


    /// @brief  calls all the OnTilemapChanged callbacks attached to this Tilemap
    /// @param  tilepos         the position of the tile that was changed - will be (-1, -1) if whole tilemap changed
    /// @param  previousValue   the value of the tile before it was changed - garbage if whole tilemap changed
    void callOnTilemapChangedCallbacks( glm::ivec2 const& tilePos = { -1, -1 }, TileType const& previousValue = TileType() );


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
    void readDimensions( nlohmann::ordered_json const& data );

    /// @brief            Read in the stride multiplier
    /// @param  stream    The json to read from.
    void readTileScale( nlohmann::ordered_json const& data );

    /// @brief the map of read methods for this Component
    static ReadMethodMap< Tilemap > const s_ReadMethods;

public:

    /// @brief gets the map of read methods for this Component
    /// @return the map of read methods for this Component
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override
    {
        return (ReadMethodMap< ISerializable > const&)s_ReadMethods;
    }

    /// @brief Write all Tilemap component data to a JSON file.
    /// @return The JSON file containing the Tilemap component data.
    virtual nlohmann::ordered_json Write() const override;

};

/// @brief  specialization of int tilemap inspector
template<>
void Tilemap<int>::Inspector();

#ifndef TILEMAP_C
#include "Tilemap.t.cpp"
#endif
