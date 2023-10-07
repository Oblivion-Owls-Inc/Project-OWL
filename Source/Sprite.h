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
//-----------------------------------------------------------------------------
public: // constructor / destructors
//-----------------------------------------------------------------------------

    /// @brief  default constructor
    Sprite();

    /// @brief  Textured constructor
    /// @param  texture the texture for this Sprite to use
    /// @param  layer   (optional) Rendering layer: 0-4. 0 is back, 4 is front.
    /// @param  type    for use ONLY when deriving with this Constructor
    Sprite( Texture const* texture, int layer = 2, std::type_index type = typeid( Sprite ) );

    /// @brief  Plain square constructor
    /// @param  init_square true/false - initialize the square or nah?
    /// @param  color       (optional) Color to initialize the square to
    /// @param  layer       (optional) Rendering layer: 0-4. 0 is back, 4 is front.
    Sprite( bool init_square, glm::vec4 const& color = { 0, 0, 0, 1 }, int layer = 2 );

    /// @brief  Destructor: frees texture and mesh... for now. Resource library should take care of it.
    virtual ~Sprite();

//-----------------------------------------------------------------------------
protected: // constructor
//-----------------------------------------------------------------------------

    /// @brief  inherited constructor
    Sprite( std::type_index type );

//-----------------------------------------------------------------------------
public: // copying
//-----------------------------------------------------------------------------
    
    /// @brief  creates a new Sprite component which is a duplicate of this one
    /// @return Copy of this component.
    virtual Component* Clone() const override;

//-----------------------------------------------------------------------------
private: // copying
//-----------------------------------------------------------------------------

    /// @brief  Copy constructor
    Sprite(Sprite const& other);

//-----------------------------------------------------------------------------
public: // methods
//-----------------------------------------------------------------------------

    /// @brief  Draws the mesh with texture (if one is present), or color.
    virtual void Draw();

//-----------------------------------------------------------------------------
public: // accessors
//-----------------------------------------------------------------------------

    /// @brief  gets the frame index
    /// @return the frame index
    __inline int GetFrameIndex() const { return m_FrameIndex; }

    /// @brief  Sets current frame index of the spritesheet.
    /// @param  frameIndex  New frame index
    __inline void SetFrameIndex( int frameIndex ) { m_FrameIndex = frameIndex; }

    /// @brief  gets the layer
    /// @return the layer
    __inline int GetLayer() const { return m_Layer; }

    /// @brief  Sets the rendering layer : 0 - 4.  0 is back, 4 is front.
    /// @param  layer   Rendering layer to move this sprite to.
    __inline void SetLayer( int layer ) { m_Layer = layer; }

    /// @brief  gets the opacity
    /// @return the opacity
    __inline float GetOpacity() const { return m_Opacity; }

    /// @brief  Sets the opacity.
    /// @param  opacity I'm not explaining this.
    __inline void SetOpacity( float opacity ) { m_Opacity = opacity; }

    /// @brief  gets the color
    /// @return the color
    __inline glm::vec4 const& GetColor() const { return m_Color; }

    /// @brief  sets the color
    /// @param  color   the color to apply
    __inline void SetColor( glm::vec4 const& color ) { m_Color = color; }

    /// @brief  gets the Texture this Sprite is using
    /// @return the Texture this Sprite is using
    __inline Texture const* GetTexture() const { return m_Texture; }

    /// @brief  sets the Texture this Sprite is using
    /// @param  texture the Texture to set this Sprite to use
    __inline void SetTexture( Texture const* texture ) { m_Texture = texture; }

    /// @brief  gets the Mesh this Sprite is using
    /// @return the Mesh this Sprite is using
    __inline Mesh const* GetMesh() const { return m_Mesh; }

    /// @brief  sets the Mesh this Sprite is using
    /// @param  mesh    the Mesh to set this Sprite to use
    __inline void SetMesh( Mesh const* mesh ) { m_Mesh = mesh; }

//-----------------------------------------------------------------------------
protected: // virtual override methods
//-----------------------------------------------------------------------------
    
    /// @brief  called when entering the scene
    virtual void OnInit() override;

    /// @brief  called when exiting the scene
    virtual void OnExit() override;

    virtual void Inspector() override;

//-----------------------------------------------------------------------------
protected: // member variables
//-----------------------------------------------------------------------------

    int m_Layer;
    int m_FrameIndex;
    float m_Opacity;

    glm::vec4 m_Color;

    bool m_IsTextured;

    Mesh const* m_Mesh;
    Texture const* m_Texture;

//-----------------------------------------------------------------------------
protected: // methods
//-----------------------------------------------------------------------------

    /// @brief  Calculates UV offset based on current frameIndex
    /// @return the UV offset
    glm::vec2 calcUVoffset() const;

//-----------------------------------------------------------------------------
protected: // reading
//-----------------------------------------------------------------------------

    /// @brief  reads the Texture of this Sprite
    /// @param  stream  the json data to read from
    void readTexture( Stream stream );

    /// @brief  reads the frame index of this Sprite
    /// @param  stream  the json data to read from
    void readFrameIndex( Stream stream );

    /// @brief  Read in the color for a sprite.
    /// @param  stream  The json to read from.
    void readColor(Stream stream);

    /// @brief  Read in the opacity for a sprite.
    /// @param  stream  The json to read from.
    void readOpacity(Stream stream);

    /// @brief  Read in the layer for a sprite.
    /// @param  stream  The json to read from.
    void readLayer(Stream stream);
    
//-----------------------------------------------------------------------------
private: // reading
//-----------------------------------------------------------------------------

    /// @brief the map of read methods for this Component
    static ReadMethodMap< Sprite > const s_ReadMethods;

    /// @brief gets the map of read methods for this Component
    /// @return the map of read methods for this Component
    virtual ReadMethodMap< Component > const& GetReadMethods() const override;
};