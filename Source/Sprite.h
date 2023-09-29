/// @file     Sprite.cpp
/// @author   Eli Tsereteli (ilya.tsereteli@digipen.edu)
/// 
/// @brief    Sprite component - stores mesh and texture that together draw an image on screen. Supports spritesheets.
#pragma once
#include "Component.h"
#include "glm/glm.hpp"  // vec4
#include "Stream.h"

// fwd refs
class Mesh;
class Texture;

/// @brief      Stores mesh + texture, along with other data needed to draw a basic 2D sprite.
class Sprite : public Component
{
public:
    /// @brief              Textured sprite constructor. Accepts image file, and 
    ///                     (optional) rows and columns if it's a spritesheet. 
    ///                     Also adds this sprite's pointer to RenderSystem.
    /// 
    /// @param image_file   Path to the image file to load (single image or spritesheet)
    /// @param columns      (optional) Columns of the spritesheet
    /// @param rows         (optional) Rows of the spritesheet
    /// @param layer        (optional) Rendering layer: 0-4. 0 is back, 4 is front.
    Sprite(const char* image_file, int columns = 1, int rows = 1, int layer = 2);


    /// @brief              Plain square sprite constructor. Accepts boolean, which 
    ///                     needs to be true for the square to be generated, or 
    ///                     false to create uninitialized sprite. Also adds 
    ///                     this sprite's pointer to RenderSystem.
    /// 
    /// @param init_square  true/false - initialize the square or nah?
    /// @param color        (optional) Color to initialize the square to
    /// @param layer        (optional) Rendering layer: 0-4. 0 is back, 4 is front.
    Sprite(bool init_square, glm::vec4 color = { 0,0,0,1 }, int layer = 2);


    Sprite();

protected:
    /// @brief              Default constructor - does not init anything
    Sprite(std::type_index type );

private:
    /// @brief              Copy constructor: shallow copy. Flyweight mesh and texture 
    ///                     (eventually, once we have data library). Do not use rn.
    Sprite(Sprite const& other);

public:
    /// @return             Copy of this component.
    virtual Component* Clone() const override;

    /// @brief              Destructor: frees texture and mesh... for now. Resource library should take care of it.
    virtual ~Sprite();

    /// @brief          Draws the mesh with texture (if one is present), or color.
    virtual void Draw();

    /// @brief          Sets current frame of the spritesheet.
    /// @param frame    New frame index
    void SetFrame(int frame);

    /// @brief          Sets the rendering layer : 0 - 4.  0 is back, 4 is front.
    /// @param layer    Rendering layer to move this sprite to.
    void SetLayer(int layer);

    /// @brief          Sets the opacity.
    /// param opacity   I'm not explaining this.
    void SetOpacity(float opacity);

    void SetColor(glm::vec4 const& color);

    /// @brief          Number to multiply width by, to get proportional height
    ///                 based on original image
    /// @return         float: original image's height divided by width
    float GetHeightMultiplier() const;


    //-------------------------------------------------------------------------
    //          data
    //-------------------------------------------------------------------------
protected:
    int m_Rows = 1, m_Columns = 1;
    int m_Layer = 2;
    int m_Frame = 0;
    float m_HeightMult = 1.0f;
    float m_Opacity = 1.0f;
    std::string m_Filename;
    bool m_IsTextured;

    Mesh* m_Mesh = nullptr;
    Texture* m_Texture = nullptr;
    glm::vec4 m_Color = {1,1,1,1};


    //-------------------------------------------------------------------------
    //          helpers
    //-------------------------------------------------------------------------

    /// @brief      Calculates UV offset based on current frame  
    glm::vec2 calcUVoffset();

    /// @brief      Pre-calculates height multiplier based on texture dimensions
    void calcHeightMult();


protected: // reading

    /// @brief        Read in the number of rows for a sprite.
    /// @param stream The json to read from.
    void ReadRows(Stream stream);

    /// @brief        Read in the colour for a sprite.
    /// @param stream The json to read from.
    void ReadColor(Stream stream);

    /// @brief        Read in the layer for a sprite.
    /// @param stream The json to read from.
    void ReadLayer(Stream stream);

    /// @brief        Read in the file name for a sprite.
    /// @param stream The json to read from.
    void ReadName(Stream stream);

    /// @brief Read in the number of columns for a sprite.
    /// @param stream the json to read from.
    void ReadColumns(Stream stream);

    /// @brief Takes all the read in data and makes a sprite.
    void ReadSprite( Stream );

private:
    /// @brief the map of read methods for this Component
    static ReadMethodMap< Sprite > const s_ReadMethods;

    /// @brief gets the map of read methods for this Component
    /// @return the map of read methods for this Component
    virtual ReadMethodMap< Component > const& GetReadMethods() const override;
};