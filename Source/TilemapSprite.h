/// @file     TilemapSprite.h
/// @author   Eli Tsereteli (ilya.tsereteli@digipen.edu)
/// 
/// @brief    A version of Sprite specifically for rendering tiles.
#pragma once

#include "pch.h" 
#include "Sprite.h"
#include "ComponentReference.h"
#include "Tilemap.h"

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
    TilemapSprite( AssetReference< Texture > const& texture, float stride_mult = 1.0f, int layer = 2,
                  std::type_index type = typeid(TilemapSprite));;


    ~TilemapSprite();


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


    /// @brief  checks if a a point in local space overlaps this Sprite
    /// @return whether this Sprite overlaps the point
    virtual bool OverlapsLocalPoint( glm::vec2 const& ) const override { return false; }

//-----------------------------------------------------------------------------
//              Data
//-----------------------------------------------------------------------------
private:

    unsigned int m_InstBufferID = 0; /// @brief   ID of buffer that stores instance data (tile IDs)
    unsigned int m_VAO = 0;          /// @brief   VAO that uses this specific buffer
    bool m_TilemapChanged = true;    /// @brief   Whether new tiles need to be loaded in

    /// @brief  Cached parent tilemap
    ComponentReference< Tilemap< int > > m_Tilemap;


//-----------------------------------------------------------------------------
//              Helpers
//-----------------------------------------------------------------------------
private:

    /// @brief    Initializes instancing buffer and its VAO
    void initInstancingStuff();


    /// @brief          Sets TilemapChanged flag to true. Used as callbeck for
    ///                 Tilemap component. The flag is reset when Draw() is called.
    void onTilemapChanged( Tilemap< int >*, glm::ivec2 const&, int const& ) { m_TilemapChanged = true; }


//-----------------------------------------------------------------------------
public: // copying
//-----------------------------------------------------------------------------


    /// @brief  creates a copy of this TilemapSprite
    /// @return the newly created copy of this TilemapSprite
    virtual TilemapSprite* Clone() const override;

    
//-----------------------------------------------------------------------------
private: // copying
//-----------------------------------------------------------------------------


    /// @brief  copy constructor
    /// @param  other   the TilemapSprite to copy
    TilemapSprite( TilemapSprite const& other );


//-----------------------------------------------------------------------------
};
