/// @file     Sprite.cpp
/// @author   Eli Tsereteli (ilya.tsereteli@digipen.edu)
/// 
/// @brief    Implementation of Sprite component.
#include "Sprite.h"
#include "RenderSystem.h"
#include "Mesh.h"
#include "Texture.h"
#include "Transform.h"
#include "Entity.h"
#include <algorithm>    // min/max

#include <iostream>     // error msg (temporary)

/// @brief          Creates new Sprite using copy constructor.
/// @return         pointer to copied Sprite component.
Component* Sprite::Clone() const { return new Sprite(*this); }

/// @brief              Copy constructor: shallow copy. Flyweight mesh and texture (eventually). Do not use rn.
Sprite::Sprite(Sprite const& other) : Component(typeid(Sprite))
{
    *this = other;  // DO NOT MAKE CLONES until resource library is implemented for meshes/textures.
                    // (to avoid double deletion of resources.)
}


/// @brief              Textured sprite constructor. Accepts image file, and (optional) rows and 
///                     columns if it's a spritesheet. Also adds this sprite's pointer to RenderSystem.
/// @param image_file   Path to the image file to load (single image or spritesheet)
/// @param columns      (optional) Columns of the spritesheet
/// @param rows         (optional) Rows of the spritesheet
/// @param layer        (optional) Rendering layer: 0-4. 0 is front, 4 is back.
Sprite::Sprite(const char* image_file, int columns, int rows, int layer) : 
        Component(typeid(Sprite)), 
        _rows(rows), _columns(columns),
        _color(0,0,0,1), _layer(std::max(0, std::min(layer, 4)))
{
    Renderer()->AddSprite(this);

    if (image_file)
    {
        _mesh = new Mesh(true, rows, columns);    // TODO: obtain it from mesh library
        _texture = new Texture(image_file);
        glm::vec2 size = _texture->getImageDimensions();
        glm::vec2 uvsize = _mesh->get_uvSize();

        _heightMult = (size.y / size.x) * (uvsize.y / uvsize.x);
    }

}


/// @brief              Plain square sprite constructor. Accepts boolean, which needs to be true for
///                     the square to be generated, or false to create uninitialized sprite. Also adds 
///                     this sprite's pointer to RenderSystem.
/// @param init_square  true/false - initialize the square or nah?
/// @param color        (optional) Color to initialize the square to
/// @param layer        (optional) Rendering layer: 0-4. 0 is front, 4 is back.
Sprite::Sprite(bool init_square, glm::vec4 color, int layer) :
        Component(typeid(Sprite)),
        _rows(1), _columns(1),
        _color(color), _layer(std::max(0, std::min(layer, 4)))
{
    Renderer()->AddSprite(this);

    if (init_square)
        _mesh = new Mesh(true, 1, 1);
}

/// @param      Clears out memory, removes this sprite's pointer from RenderSystem.
Sprite::~Sprite()
{
    Renderer()->RemoveSprite(this);

    // TODO: remove this after data library is implemented
    delete _mesh;
    delete _texture;
}

/// @brief          Sets current frame of the spritesheet.
/// @param frame    New frame index
void Sprite::setFrame(int frame)
{
    if (frame < _rows * _columns)
        _frame = frame;
    else
        std::cout << "SPRITE ERROR: frame index out of range. Max frame of this sprite is " 
                  << (_rows * _columns - 1) << std::endl;
}

/// @brief          Sets the rendering layer : 0 - 4. 0 is front, 4 is back.
/// @param layer    Rendering layer to move this sprite to.
void Sprite::setLayer(int layer) {   _layer = std::max(0, std::min(layer, 4));   }


/// @brief          Returns the number (for transform) to multiply width by, in order to maintain scale
///                 proportions of the original image. (accounts for uv size of frames as well)
/// @return         float: multiplier for obtaining height:  width * mult = height
float Sprite::getHeightMultiplier() const
{
    if (_texture)
        return _heightMult;
    else
        return 1.0f;
}


/// @brief          Draws the mesh with texture (if one is present), or color.
void Sprite::draw()
{
    // Render texture if one is present, or color if none
    if (_texture)
    {
        Renderer()->TextureMode();
        _texture->bind();

        // set the UV offset based on current frame
        int row = _frame / _columns;
        int col = _frame % _columns;
        glm::vec2 UV = _mesh->get_uvSize();
        Renderer()->SetUV(UV.x * col, UV.y * row);
    }
    else
    {
        Renderer()->ColorMode();
        Renderer()->SetColor(_color);
    }

    if (Parent() && Parent()->HasComponent(typeid(Transform)))      // what a mess
        Renderer()->SetTransformMat( *((Transform*)Parent()->HasComponent(typeid(Transform)))->getMatrix() );

    _mesh->draw();
}

/// @brief      Returns the 'visible' flag, which RenderSystem uses to determine whether to draw this sprite.
/// @return     true/false:  is it supposed to be visible 
bool Sprite::isVisible() const { return _visible; }
