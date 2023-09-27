/// @file     Sprite.cpp
/// @author   Eli Tsereteli (ilya.tsereteli@digipen.edu)
/// 
/// @brief    Implementation of Sprite component.
#include "Sprite.h"
#include "RenderSystem.h"
#include "Mesh.h"
#include "Texture.h"
#include "Transform.h"      // matrix
#include "Entity.h"         // parent
#include "CameraSystem.h"   // projection matrix
#include <algorithm>        // min/max
#include <iostream>         // error msg

/// @brief          Creates new Sprite using copy constructor.
/// @return         pointer to copied Sprite component.
Component* Sprite::Clone() const { return new Sprite(*this); }

/// @brief          Copy constructor: shallow copy. Flyweight mesh and texture 
///                 (eventually, once we have data library). Do not use rn.
Sprite::Sprite(Sprite const& other) : Component(typeid(Sprite))
{
    *this = other;  // DO NOT MAKE CLONES until resource library is implemented for meshes/textures.
    // (to avoid double deletion of resources.)
}

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
    // TODO: move this once components have init/exit
    Renderer()->RemoveSprite(this, m_Layer);

    // TODO: remove this after data library is implemented
    delete m_Mesh;
    delete m_Texture;
}

/// @brief          Sets current frame of the spritesheet.
/// @param frame    New frame index
void Sprite::SetFrame(int frame)
{
    if (frame < m_Rows * m_Columns)
        m_Frame = frame;
    else
        std::cout << "SPRITE ERROR: frame index out of range. Max frame of this sprite is " 
        << (m_Rows * m_Columns - 1) << std::endl;
}

/// @brief          Sets the rendering layer : 0 - 4. 0 is back, 4 is front.
/// @param layer    Rendering layer to move this sprite to.
void Sprite::SetLayer(int layer) 
{   
    Renderer()->RemoveSprite(this, m_Layer);
    m_Layer = std::max(0, std::min(layer, 4));
    Renderer()->AddSprite(this, m_Layer);
}

/// @brief          Sets the opacity.
/// param opacity   I'm not explaining this.
void Sprite::SetOpacity(float opacity) { m_Opacity = opacity; }

/// @brief          Returns the number (for transform) to multiply width by, in order to maintain scale
///                 proportions of the original image. (accounts for uv size of frames as well)
/// @return         multiplier for obtaining height:  height = width * mult
float Sprite::GetHeightMultiplier() const { return m_HeightMult; }



/// @brief          Draws the mesh with texture (if one is present), or color.
void Sprite::Draw()
{
    Shader* sh;

    // Render texture if one is present, or color if not
    if (m_Texture)
    {
        sh = Renderer()->SetActiveShader("texture");
        m_Texture->Bind();
        glm::vec2 uv_offset = calcUVoffset();
        glUniform2f(sh->GetUniformID("UV_offset"), uv_offset.x, uv_offset.y);
    }
    else
    {
        sh = Renderer()->SetActiveShader("color");
        glUniform4fv(sh->GetUniformID("color"), 1, &m_Color.x);
    }

    // Stuff they both have in common: transform and opacity
    glm::mat4 mat(1);   // it can still draw without parent and transform
    if (Parent())
    {
        Transform* t = Parent()->GetComponent<Transform>();
        if (t)
        {
            mat = *t->GetMatrix();

            // world or UI space
#if 0
            if (t->getIsDiegetic())
                mat = Camera()->GetMat_WorldToClip() * mat;
            else
#endif
                mat = Camera()->GetMat_UItoClip() * mat;
        }
    }

    glUniformMatrix4fv(sh->GetUniformID("mvp"), 1, 0, &mat[0][0]);
    glUniform1f(sh->GetUniformID("opacity"), m_Opacity);

    // Render it with triangle strip mode
    glBindVertexArray(m_Mesh->GetVAO());
    glDrawArrays(GL_TRIANGLE_STRIP, 0, m_Mesh->GetVertCount());
    glBindVertexArray(0);
}



//-----------------------------------------------------------------------------
//          helpers
//-----------------------------------------------------------------------------

/// @brief          Calculates UV offset based on current frame
glm::vec2 Sprite::calcUVoffset()
{
    int row = m_Frame / m_Columns;
    int col = m_Frame % m_Columns;
    glm::vec2 UV = m_Mesh->GetUVsize();
    UV.x *= col;    UV.y *= row;
    return UV;
}


/// @brief      Pre-calculates height multiplier based on texture dimensions
void Sprite::calcHeightMult()
{
    if (!m_Texture || !m_Mesh)
        return;

    glm::vec2 size = m_Texture->GetImageDimensions();
    glm::vec2 uvsize = m_Mesh->GetUVsize();
    m_HeightMult = (size.y / size.x) * (uvsize.y / uvsize.x);
}



//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------

/// @brief Takes all the read in data and makes a sprite.
void Sprite::ReadSprite( Stream )
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
    m_Rows = stream.Read<int>();
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
    m_Columns = stream.Read<int>();
}

/// @brief        Does the sprite have a texture?
/// @param stream The json to read from.
void Sprite::ReadIsTextured(Stream stream)
{
    m_IsTextured = stream.Read<bool>();
}

/// @brief the map of read methods for this Component
ReadMethodMap< Sprite > const Sprite::readMethods = {
    { "columns"    , &ReadColumns    },
    { "rows"       , &ReadRows       },
    { "layer"      , &ReadLayer      },
    { "color"      , &ReadColor      },
    { "name"       , &ReadName       },
    { "isTextured" , &ReadIsTextured },
    { "AFTERLOAD"  , &ReadSprite     }
};

/// @brief gets the map of read methods for this Component
/// @return the map of read methods for this Component
ReadMethodMap< Component > const& Sprite::GetReadMethods()
{
    return (ReadMethodMap< Component> const&)readMethods;
}