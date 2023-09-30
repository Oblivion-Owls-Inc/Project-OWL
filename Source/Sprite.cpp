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

/// @brief          Default constructor - does nothing
Sprite::Sprite( std::type_index type ) :
    Component( type )
{}

Sprite::Sprite() :
    Component( typeid( Sprite ) )
{}

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


/// @brief              Textured sprite constructor. Accepts image file, and 
///                     (optional) rows and columns if it's a spritesheet. 
///                     Also adds this sprite's pointer to RenderSystem.
/// 
/// @param image_file   Path to the image file to load (single image or spritesheet)
/// @param columns      (optional) Columns of the spritesheet
/// @param rows         (optional) Rows of the spritesheet
/// @param layer        (optional) Rendering layer: 0-4. 0 is back, 4 is front.
Sprite::Sprite(const char* image_file, int columns, int rows, int layer) : 
    Component(typeid(Sprite)), 
    m_Rows(rows), m_Columns(columns),
    m_Layer(std::max(0, std::min(layer, 4)))
{

    if (image_file)
    {
        m_Mesh = new Mesh(true, rows, columns);    // TODO: obtain it from mesh library
        m_Texture = new Texture(image_file);
        calcHeightMult();
    }
}


/// @brief              Plain square sprite constructor. Accepts boolean, which
///                     needs to be true for the square to be generated, or 
///                     false to create uninitialized sprite. Also adds 
///                     this sprite's pointer to RenderSystem.
/// 
/// @param init_square  true/false - initialize the square or nah?
/// @param color        (optional) Color to initialize the square to
/// @param layer        (optional) Rendering layer: 0-4. 0 is back, 4 is front.
Sprite::Sprite(bool init_square, glm::vec4 color, int layer) :
    Component(typeid(Sprite)),
    m_Rows(1), m_Columns(1),
    m_Color(color), m_Layer(std::max(0, std::min(layer, 4)))
{

    if (init_square)
        m_Mesh = new Mesh(true, 1, 1);
}

/// @brief      Clears out memory, removes this sprite's pointer from RenderSystem.
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

/// @brief          Sets the color.
/// @param color    colr as a vec4
void Sprite::SetColor(glm::vec4 const& color) { m_Color = color; }


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
    if (GetParent())
    {
        Transform* t = GetParent()->GetComponent<Transform>();
        if (t)
        {
            mat = *t->GetMatrix();

            // world or UI space
            if (t->GetIsDiegetic())
                mat = Camera()->GetMat_WorldToClip() * mat;
            else
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
    Renderer()->AddSprite(this, m_Layer);

    // If a filepath has been reda in.
    if (!m_Filename.empty() && m_IsTextured == true)
    {
        m_Mesh = new Mesh(true, m_Rows, m_Columns);    // TODO: obtain it from mesh library
        m_Texture = new Texture(m_Filename.c_str());

        // calculate height multiplier
        glm::vec2 size = m_Texture->GetImageDimensions();
        glm::vec2 uvsize = m_Mesh->GetUVsize();
        m_HeightMult = (size.y / size.x) * (uvsize.y / uvsize.x);
    }
    else
    {
        m_Mesh = new Mesh(true, m_Rows, m_Columns);
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
    m_Color = stream.Read<glm::vec4>();
}

/// @brief        Read in the layer for a sprite.
/// @param stream The json to read from.
void Sprite::ReadLayer( Stream stream )
{
    int layer = stream.Read<int>();
    m_Layer = std::max( 0, std::min( layer, 4 ) );
}

/// @brief        Read in the file name for a sprite.
/// @param stream The json to read from.
void Sprite::ReadName( Stream stream )
{
    m_Filename = stream.Read<std::string>();
    m_IsTextured = true;
}

/// @brief Read in the number of columns for a sprite.
/// @param stream the json to read from.
void Sprite::ReadColumns( Stream stream )
{
    m_Columns = stream.Read<int>();
}

/// @brief the map of read methods for this Component
ReadMethodMap< Sprite > const Sprite::s_ReadMethods = {
    { "columns"    , &ReadColumns    },
    { "rows"       , &ReadRows       },
    { "layer"      , &ReadLayer      },
    { "color"      , &ReadColor      },
    { "name"       , &ReadName       },
    { "AFTERLOAD"  , &ReadSprite     }
};

/// @brief gets the map of read methods for this Component
/// @return the map of read methods for this Component
ReadMethodMap< Component > const& Sprite::GetReadMethods() const
{
    return (ReadMethodMap< Component> const&)s_ReadMethods;
}