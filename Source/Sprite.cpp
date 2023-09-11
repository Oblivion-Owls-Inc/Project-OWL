#include "Sprite.h"
#include "RenderSystem.h"
#include "Mesh.h"
#include "Texture.h"
#include "Transform.h"
#include "Entity.h"
#include <algorithm>    // min/max

#include <iostream>     // error msg (temporary)

Component* Sprite::Clone() const { return new Sprite(*this); }


// Texture / spritesheet
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


// Plain mesh
Sprite::Sprite(bool init_square, glm::vec4 color, int layer) :
        Component(typeid(Sprite)),
        _rows(1), _columns(1),
        _color(color), _layer(std::max(0, std::min(layer, 4)))
{
    Renderer()->AddSprite(this);

    if (init_square)
        _mesh = new Mesh(true, 1, 1);
}

Sprite::~Sprite()
{
    Renderer()->RemoveSprite(this);

    delete _mesh;
    delete _texture;
}


void Sprite::setFrame(int frame)
{
    if (frame < _rows * _columns)
        _frame = frame;
    else
        std::cout << "SPRITE ERROR: frame index out of range. Max frame of this sprite is " 
                  << (_rows * _columns - 1) << std::endl;
}

void Sprite::setLayer(int layer) {   _layer = std::max(0, std::min(layer, 4));   }

float Sprite::getHeightMultiplier() const
{
    if (_texture)
        return _heightMult;
    else
        return 1.0f;
}


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


int Sprite::getLayer() const { return _layer; }

bool Sprite::isVisible() const { return _visible; }
