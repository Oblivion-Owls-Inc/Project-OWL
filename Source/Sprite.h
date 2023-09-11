/// @file     Sprite.cpp
/// @author   Eli Tsereteli (ilya.tsereteli@digipen.edu)
/// 
/// @brief    Sprite component - stores mesh and texture that together draw an image on screen. Supports spritesheets.
#pragma once
#include "Component.h"
#include "glm/glm.hpp"  // vec4

// fwd refs
class Mesh;
class Texture;

class Sprite : public Component
{
    int _rows, _columns;
    int _layer;
    int _frame = 0;
    float _heightMult = 1.0f;
    bool _visible = true;

    Mesh* _mesh = nullptr;
    Texture* _texture = nullptr;
    glm::vec4 _color;

public:
    Sprite(const char* image_file, int columns = 1, int rows = 1, int layer = 2);    // with texture
    Sprite(bool init_square = false, glm::vec4 color = { 0,0,0,1 }, int layer = 2); // just mesh

    /// @brief              Copy constructor: shallow copy. Flyweight mesh and texture (eventually). Do not use rn.
    Sprite(Sprite const& other);

    /// @brief              Destructor: frees texture and mesh... for now. Resource library should take care of it.
    ~Sprite();
    // TODO: copy ctor

    /// @brief          Draws the mesh with texture (if one is present), or color.
    void draw();

    /// @brief          Sets current frame of the spritesheet.
    /// @param frame    New frame index
    void setFrame(int frame);

    /// @brief          Sets the rendering layer : 0 - 4. 0 is front, 4 is back.
    /// @param layer    Rendering layer to move this sprite to.
    void setLayer(int layer);
    int getLayer() const;

    /// @brief       Number to multiply width by, to get proportional height based on original image
    /// @return      float: original image's (height/width)
    float getHeightMultiplier() const;

    /// @brief      Returns the 'visible' flag, which RenderSystem uses to determine whether to draw this sprite.
    /// @return     true/false:  is it supposed to be visible
    bool isVisible() const;

    // Inherited via Component
    virtual Component* Clone() const override;
};