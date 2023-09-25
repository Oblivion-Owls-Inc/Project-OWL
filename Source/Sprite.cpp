/// @file     Sprite.cpp
/// @author   Eli Tsereteli (ilya.tsereteli@digipen.edu)
/// 
/// @brief    Implementation of Sprite component.
#include "Sprite.h"
#include "RenderSystem.h"
#include "Mesh.h"
#include "Texture.h"
#include "Transform.h"  // matrix
#include "Entity.h"     // parent
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
/// @param layer        (optional) Rendering layer: 0-4. 0 is back, 4 is front.
/*Sprite::Sprite(const char* image_file, int columns, int rows, int layer) :
        Component(typeid(Sprite)), 
        _rows(rows), _columns(columns),
        _color(0,0,0,1), _layer(std::max(0, std::min(layer, 4)))
{
    Renderer()->AddSprite(this, layer);

    if (image_file)
    {
        _mesh = new Mesh(true, rows, columns);    // TODO: obtain it from mesh library
        _texture = new Texture(image_file);

        // calculate height multiplier
        glm::vec2 size = _texture->getImageDimensions();
        glm::vec2 uvsize = _mesh->get_uvSize();
        _heightMult = (size.y / size.x) * (uvsize.y / uvsize.x);
    }

}*/


/// @brief              Plain square sprite constructor. Accepts boolean, which needs to be true for
///                     the square to be generated, or false to create uninitialized sprite. Also adds 
///                     this sprite's pointer to RenderSystem.
/// @param init_square  true/false - initialize the square or nah?
/// @param color        (optional) Color to initialize the square to
/// @param layer        (optional) Rendering layer: 0-4. 0 is back, 4 is front.
/*Sprite::Sprite(bool init_square, glm::vec4 color, int layer) :
        Component(typeid(Sprite)),
        _rows(1), _columns(1),
        _color(color), _layer(std::max(0, std::min(layer, 4)))
{
    Renderer()->AddSprite(this, _layer);

    if (init_square)
        _mesh = new Mesh(true, 1, 1);
}*/

/// @brief Default sprite component constructor.
Sprite::Sprite()
    : Component(typeid(Sprite))
    , _rows(0)
    , _columns(0)
    , _layer(0)
    , _color( 0, 0, 0, 1 )
    , m_filename("")
    , m_IsTextured(false)
{}

/// @param      Clears out memory, removes this sprite's pointer from RenderSystem.
Sprite::~Sprite()
{
    Renderer()->RemoveSprite(this, _layer);

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

/// @brief          Sets the rendering layer : 0 - 4. 0 is back, 4 is front.
/// @param layer    Rendering layer to move this sprite to.
void Sprite::setLayer(int layer) 
{   
    Renderer()->RemoveSprite(this, _layer);
    _layer = std::max(0, std::min(layer, 4));
    Renderer()->AddSprite(this, _layer);
}

/// @brief          Sets the opacity.
/// param opacity   I'm not explaining this.
void Sprite::setOpacity(float opacity) { _opacity = opacity; }

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

    Renderer()->SetOpacity(_opacity);

    _mesh->draw();
}

//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------

/// @brief Takes all the read in data and makes a sprite.
void Sprite::ReadSprite()
{
    Renderer()->AddSprite(this, _layer);

    // If a filepath has been reda in.
    if (!m_filename.empty() && m_IsTextured == true)
    {
        _mesh = new Mesh(true, _rows, _columns);    // TODO: obtain it from mesh library
        _texture = new Texture(m_filename.c_str());

        // calculate height multiplier
        glm::vec2 size = _texture->getImageDimensions();
        glm::vec2 uvsize = _mesh->get_uvSize();
        _heightMult = (size.y / size.x) * (uvsize.y / uvsize.x);
    }
    else
    {
        _mesh = new Mesh(true, _rows, _columns);
    }
}

 /// @brief Read in the number of rows for a sprite.
 /// @param stream the json to read from.
void Sprite::ReadRows( Stream stream )
{
    _rows = stream.Read<int>();
}

/// @brief        Read in the colour for a sprite.
/// @param stream The json to read from.
void Sprite::ReadColor( Stream stream )
{
    _color = stream.Read<glm::vec4>();
}

/// @brief        Read in the layer for a sprite.
/// @param stream The json to read from.
void Sprite::ReadLayer( Stream stream )
{
    int layer = stream.Read<int>();
    _layer = std::max( 0, std::min( layer, 4 ) );
}

/// @brief        Read in the file name for a sprite.
/// @param stream The json to read from.
void Sprite::ReadName( Stream stream )
{
    m_filename = stream.Read<std::string>();
}

/// @brief Read in the number of columns for a sprite.
/// @param stream the json to read from.
void Sprite::ReadColumns( Stream stream )
{
    _columns = stream.Read<int>();
}

/// @brief        Does the sprite have a texture?
/// @param stream The json to read from.
void Sprite::ReadIsTextured(Stream stream)
{
    m_IsTextured = stream.Read<bool>();
    ReadSprite();
}

/// @brief the map of read methods for this Component
ReadMethodMap< Sprite > const Sprite::readMethods = {
    { "columns"    , &ReadColumns    },
    { "rows"       , &ReadRows       },
    { "layer"      , &ReadLayer      },
    { "color"      , &ReadColor      },
    { "name"       , &ReadName       },
    { "isTextured" , &ReadIsTextured },
};

/// @brief gets the map of read methods for this Component
/// @return the map of read methods for this Component
ReadMethodMap< Component > const& Sprite::getReadMethods()
{
    return (ReadMethodMap< Component> const&)readMethods;
}