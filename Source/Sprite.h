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

class Sprite : public Component
{
    int _rows, _columns;
    int _layer;
    int _frame = 0;
    float _heightMult = 1.0f;
    float _opacity = 1.0f;

    Mesh* _mesh = nullptr;
    Texture* _texture = nullptr;
    glm::vec4 _color;

public:
    /// @brief              Textured sprite constructor. Accepts image file, and (optional) rows and 
    ///                     columns if it's a spritesheet. Also adds this sprite's pointer to RenderSystem.
    /// @param image_file   Path to the image file to load (single image or spritesheet)
    /// @param columns      (optional) Columns of the spritesheet
    /// @param rows         (optional) Rows of the spritesheet
    /// @param layer        (optional) Rendering layer: 0-4. 0 is back, 4 is front.
    Sprite(const char* image_file, int columns = 1, int rows = 1, int layer = 2);

    /// @brief              Plain square sprite constructor. Accepts boolean, which needs to be true for
    ///                     the square to be generated, or false to create uninitialized sprite. Also adds 
    ///                     this sprite's pointer to RenderSystem.
    /// @param init_square  true/false - initialize the square or nah?
    /// @param color        (optional) Color to initialize the square to
    /// @param layer        (optional) Rendering layer: 0-4. 0 is back, 4 is front.
    Sprite(bool init_square = false, glm::vec4 color = { 0,0,0,1 }, int layer = 2); // just mesh

    /// @brief              Copy constructor: shallow copy. Flyweight mesh and texture (eventually). Do not use rn.
    Sprite(Sprite const& other);

    /// @brief              Destructor: frees texture and mesh... for now. Resource library should take care of it.
    ~Sprite();

    /// @brief          Draws the mesh with texture (if one is present), or color.
    void draw();

    /// @brief          Sets current frame of the spritesheet.
    /// @param frame    New frame index
    void SetFrame(int frame);

    /// @brief          Sets the rendering layer : 0 - 4.  0 is back, 4 is front.
    /// @param layer    Rendering layer to move this sprite to.
    void SetLayer(int layer);

    /// @brief          Sets the opacity.
    /// param opacity   I'm not explaining this.
    void SetOpacity(float opacity);

    /// @brief       Number to multiply width by, to get proportional height based on original image
    /// @return      float: original image's (height/width)
    float GetHeightMultiplier() const;

    // Inherited via Component
    virtual Component* Clone() const override;

private: // reading

    /// @brief Read in the number of rows for a sprite.
    /// @param stream the json to read from.
    void ReadRows( Stream stream );

    /// @brief Read in the number of columns for a sprite.
    /// @param stream the json to read from.
    void ReadColumns( Stream stream );

    /// @brief the map of read methods for this Component
    static ReadMethodMap< Sprite > const readMethods;

    /// @brief gets the map of read methods for this Component
    /// @return the map of read methods for this Component
    virtual ReadMethodMap< Component > const& GetReadMethods() override;
};