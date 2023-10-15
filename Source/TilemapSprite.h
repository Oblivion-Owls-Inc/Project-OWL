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

    unsigned int m_InstBufferID = 0; /// @brief   ID of buffer that stores instance data (tile IDs)
    unsigned int m_VAO = 0;          /// @brief   VAO that uses this specific buffer
    int m_TileCount = 0;             /// @brief   How many tiles to draw
    bool m_TilemapChanged = true;    /// @brief   Whether new tiles need to be loaded in
    
    // keep local copies of these to keep drawing without Tilemap's updates
    int m_RowWidth = 10;             /// @brief   Tiles per row (amount of columns)
    glm::vec2 m_StrideMult = {1,1};  /// @brief   Stride multiplier(s): 1 is full tile width/height


//-----------------------------------------------------------------------------
//              Helpers
//-----------------------------------------------------------------------------
private:

    /// @brief    Initializes instancing buffer and its VAO
    void initInstancingStuff();


    /// @brief          Sets TilemapChanged flag to true. Used as callbeck for
    ///                 Tilemap component. The flag is reset when Draw() is called.
    __inline void onTilemapChanged() { m_TilemapChanged = true; }

};
