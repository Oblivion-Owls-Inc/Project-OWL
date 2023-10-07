/// @file     TilemapSprite.h
/// @author   Eli Tsereteli (ilya.tsereteli@digipen.edu)
/// 
/// @brief    A version of Sprite specifically for rendering tiles.
#pragma once
#include "Sprite.h"
#include <vector>
#include <sstream>  // accept new text


/// @brief      A version of Sprite for rendering tilemaps using GPU instancing.
class TilemapSprite : public Sprite
{
public:

    /// @brief              Default constructor
    TilemapSprite();


    /// @brief              Textured constructor
    /// 
    /// @param texture      Texture (spritesheet) to use for tiling
    /// @param stride_mult  (optional) Multiplier to adjust stride (spacing)
    /// @param layer        (optional) Rendering layer: 0-4. 0 is back, 4 is front.
    /// @param type         (for derived) Component type
    TilemapSprite(Texture* texture, float stride_mult = 1.0f, int layer = 2,
                  std::type_index type = typeid(TilemapSprite));;



//-----------------------------------------------------------------------------
//          Public methods
//-----------------------------------------------------------------------------
public:

    /// @brief          Loads the tile array from a raw char array.
    /// @param tiles    tile IDs  (spritesheet frames)
    /// @param size     array size
    void LoadTileArray(const char* tiles, int size);


    /// @brief          Loads the tile array from a vector of ints.
    /// @param tiles    tile IDs  (spritesheet frames)
    void LoadTileArray(std::vector<int> tiles);


    /// @brief          Sets the width of a single row (amount of columns)
    /// @param columns  (tiles per row)
    __inline void SetRowWidth(int width) { m_RowWidth = width; }

    /// @return         Tiles per row 
    __inline int GetRowWidth() const { return m_RowWidth; }


    /// @brief          Sets the stride multiplier. Default stride is the full
    ///                 width/height of single tile. (Example: To space out the 
    ///                 tiles, set multipliers to over 1.0)
    /// @param mults    x=horizontal, y=vertical
    __inline void SetStrideMultiplier(glm::vec2 mults) { m_StrideMult = mults; }


    /// @brief          Retreives the stride multiplier.
    /// @return         x=horizontal, y=vertical
    __inline glm::vec2 GetStrideMultiplier() const { return m_StrideMult; }

//-----------------------------------------------------------------------------
//          Virtual overrides
//-----------------------------------------------------------------------------
private:
    // from Component
    
    /// @brief  called when entering a scene
    virtual void OnInit() override;

    /// @brief  called when exiting a scene
    virtual void OnExit() override;


public:
    // from Sprite

    /// @brief     Draws tilemap using currently loaded array.
    virtual void Draw() override;



//-----------------------------------------------------------------------------
//              Data
//-----------------------------------------------------------------------------
private:

    unsigned int m_InstBufferID = 0; /// @brief     ID of buffer that stores instance data (tile IDs)
    unsigned int m_VAO = 0;          /// @brief     VAO that uses this specific buffer
    glm::vec2 m_StrideMult ={1,1};   /// @brief     Horizontal/vertical stride. 1.0 is full tile width
    int m_RowWidth = 20;             /// @brief     Amount of tiles per row
    int m_TileCount = 0;             /// @brief     How many tiles to draw



//-----------------------------------------------------------------------------
//              Helpers
//-----------------------------------------------------------------------------
private:

    /// @brief    Initializes instancing buffer and its VAO
    void initInstancingStuff();



//-----------------------------------------------------------------------------
//              Reading
//-----------------------------------------------------------------------------
private:

    /// @brief            Read in the stride multiplier
    /// @param  stream    The json to read from.
    void readStrideMultiplier(Stream stream);

    /// @brief            Read in the amount of tiles per row
    /// @param  stream    The json to read from.
    void readRowWidth(Stream stream);

    void afterLoad(Stream stream);

    /// @brief            The map of read methods for this Component
    static ReadMethodMap< TilemapSprite > const s_ReadMethods;

    /// @brief            Gets the map of read methods for this Component
    /// @return           The map of read methods for this Component
    virtual ReadMethodMap< Component > const& GetReadMethods() const override;

};
