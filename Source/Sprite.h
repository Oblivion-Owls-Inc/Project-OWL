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
    ~Sprite();
    // TODO: copy ctor

    void draw();

    void setFrame(int frame);

    void setLayer(int layer);
    int getLayer() const;

    /// @brief       Number to multiply width by, to get proportional height based on original image
    /// @return      float: original image's (height/width)
    float getHeightMultiplier() const;

    bool isVisible() const;

    // Inherited via Component
    virtual Component* Clone() const override;
};