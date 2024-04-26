/// @file       TilemapTextureConnector.h
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      Component that uses a parent 'source' tilemap to update a child tilemap with connected textures
/// @version    0.1
/// @date       2023-10-20
/// 
/// @copyright  Copyright (c) 2024 Digipen Institute of Technology

#pragma once







/// @brief  Component that uses a parent 'source' tilemap to update a child tilemap with connected textures
class TilemapTextureConnector : public Component
{
//-----------------------------------------------------------------------------
public: // types
//-----------------------------------------------------------------------------

    
//-----------------------------------------------------------------------------
private: // types
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
public: // accessors
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
public: //  methods
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
public: // virtual override methods
//-----------------------------------------------------------------------------


    /// @brief  called once when entering the scene
    virtual void OnInit() override;


    /// @brief  called once when exiting the scene
    virtual void OnExit() override;


    /// @brief  called whenever the hierarchy this Entity is a part of changes
    /// @param  previousParent  the previous parent of this Entity
    virtual void OnHierarchyChange( Entity* previousParent ) override;


//-----------------------------------------------------------------------------
private: // members
//-----------------------------------------------------------------------------


    /// @brief  which tiles connect to which other tiles
    /// @details each index of the vector represents the ID of a tile
    /// @details the set at that index contains which tiles that tile connects with
    std::vector< std::set< int > > m_ConnectionRules = {};


    /// @brief  the offset from the start of the tilesheet of the first usable texture
    int m_FirstTileOffset = 0;

    /// @brief  how many textures each tile type has
    int m_TexturesPerTile = 24;


    /// @brief  the texture offsets to use for the top left tile
    int m_TopLeftTextures[ 8 ] = { 0 };

    /// @brief  the texture offsets to use for the top right tile
    int m_TopRightTextures[ 8 ] = { 0 };

    /// @brief  the texture offsets to use for the bottom left tile
    int m_BottomLeftTextures[ 8 ] = { 0 };

    /// @brief  the texture offsets to use for the bottom right tile
    int m_BottomRightTextures[ 8 ] = { 0 };


    /// @brief  the 'source' tilemap attached to the parent
    ComponentReference< Tilemap< int > > m_ParentTilemap;


    /// @brief  the tilemap attached to this Entity that the TilemapTextureConnector writes to
    ComponentReference< Tilemap< int > > m_Tilemap;


//-----------------------------------------------------------------------------
private: // helper methods
//-----------------------------------------------------------------------------


    /// @brief  called whenever the source tilemap is modified
    /// @param  tilemap         the tilemap this callback was called from
    /// @param  tilepos         the position of the tile that was changed - will be (-1, -1) if whole tilemap changed
    /// @param  previousValue   the value of the tile before it was changed - garbage if whole tilemap changed
    void onTilemapChangedCallback(
        Tilemap< int >* tilemap,
        glm::ivec2 const& tilePos,
        int const& previousValue
    );


    /// @brief  updates the textures of the whole tilemap
    void updateWholeTilemap();


    /// @brief  updates the textures of all tiles around the specified tile
    /// @param  centerTile  the center tile of the neighborhood to update
    void updateNeighborhood( glm::ivec2 const& centerTile );

    /// @brief  updates the textures of the specified tile
    /// @param  tilePos the position of the (source) tile to update
    void updateTile( glm::ivec2 const& tilePos );


    /// @brief  updates the top left corner of the specified tile
    /// @param  tilePos the position of the (source) tile to update
    /// @param  tileId  the ID of the type of tile being updated
    void updateTopLeftCorner( glm::ivec2 const& tilePos, int tileId );

    /// @brief  updates the top right corner of the specified tile
    /// @param  tilePos the position of the (source) tile to update
    /// @param  tileId  the ID of the type of tile being updated
    void updateTopRightCorner( glm::ivec2 const& tilePos, int tileId );

    /// @brief  updates the bottom left corner of the specified tile
    /// @param  tilePos the position of the (source) tile to update
    /// @param  tileId  the ID of the type of tile being updated
    void updateBottomLeftCorner( glm::ivec2 const& tilePos, int tileId );

    /// @brief  updates the bottom right corner of the specified tile
    /// @param  tilePos the position of the (source) tile to update
    /// @param  tileId  the ID of the type of tile being updated
    void updateBottomRightCorner( glm::ivec2 const& tilePos, int tileId );


    /// @brief  gets the texture index of a corner
    /// @param  tilePos     the position of the (source) tile to update
    /// @param  tileId      the ID of the type of tile being updated
    /// @param  offsets     the tile position offsets to build the connection flags with
    /// @param  textures    the texture index offsets to display
    /// @return the texture index for this corner
    int getCornerTexture( glm::ivec2 const& tilePos, int tileId, glm::ivec2 const offsets[], int const textures[] ) const;


//-----------------------------------------------------------------------------
public: // inspection
//-----------------------------------------------------------------------------

    
    /// @brief  displays this TilemapTextureConnector in the Inspector
    virtual void Inspector() override;


//-----------------------------------------------------------------------------
private: // reading
//-----------------------------------------------------------------------------

    
    /// @brief  reads which tiles connect to which other tiles
    /// @param  data    the json data to read from
    void readConnectionRules( nlohmann::ordered_json const& data );


    /// @brief  the offset from the start of the tilesheet of the first usable texture
    /// @param  data    the json data to read from
    void readFirstTileOffset( nlohmann::ordered_json const& data );

    /// @brief  how many textures each tile type has
    /// @param  data    the json data to read from
    void readTexturesPerTile( nlohmann::ordered_json const& data );


    /// @brief  the texture offsets to use for the top left tile
    /// @param  data    the json data to read from
    void readTopLeftTextures( nlohmann::ordered_json const& data );

    /// @brief  the texture offsets to use for the top right tile
    /// @param  data    the json data to read from
    void readTopRightTextures( nlohmann::ordered_json const& data );

    /// @brief  the texture offsets to use for the bottom left tile
    /// @param  data    the json data to read from
    void readBottomLeftTextures( nlohmann::ordered_json const& data );

    /// @brief  the texture offsets to use for the bottom right tile
    /// @param  data    the json data to read from
    void readBottomRightTextures( nlohmann::ordered_json const& data );
    


//-----------------------------------------------------------------------------
public: // reading / writing
//-----------------------------------------------------------------------------


    /// @brief  gets the map of read methods for this TilemapTextureConnector
    /// @return the map of read methods for this TilemapTextureConnector
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override;


    /// @brief  Write all TilemapTextureConnector data to a JSON file.
    /// @return The JSON file containing the TilemapTextureConnector data.
    virtual nlohmann::ordered_json Write() const override;


//-----------------------------------------------------------------------------
public: // constructor / Destructor
//-----------------------------------------------------------------------------


    /// @brief  constructor
    TilemapTextureConnector();


//-----------------------------------------------------------------------------
public: // copying
//-----------------------------------------------------------------------------


    /// @brief  clones this TilemapTextureConnector
    /// @return the newly created clone of this TilemapTextureConnector
    virtual TilemapTextureConnector* Clone() const override;


//-----------------------------------------------------------------------------
private: // copying
//-----------------------------------------------------------------------------


    /// @brief  copy-constructor for the TilemapTextureConnector
    /// @param  other   the other TilemapTextureConnector to copy
    TilemapTextureConnector( TilemapTextureConnector const& other );

    // diable = operator
    void operator =( TilemapTextureConnector const& ) = delete;


//-----------------------------------------------------------------------------
};
