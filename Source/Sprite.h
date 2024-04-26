/// @file     Sprite.cpp
/// @author   Eli Tsereteli (ilya.tsereteli@digipen.edu)
/// 
/// @brief    Sprite component - stores mesh and texture that together draw an image on screen. Supports spritesheets.
/// @copyright  Copyright (c) 2024 Digipen Institute of Technology

#pragma once




/// @brief      Stores mesh + texture, along with other data needed to draw a basic 2D sprite.
class Sprite : public Component
{
//-----------------------------------------------------------------------------
public: // constructor / destructors
//-----------------------------------------------------------------------------

    /// @brief              Default constructor
    Sprite();

    /// @brief              Textured constructor.
    /// @param  texture     the texture for this Sprite to use
    /// @param  layer       (optional) Rendering layer: 0-4. 0 is back, 4 is front.
    /// @param  type        for use ONLY when deriving with this Constructor
    Sprite( AssetReference< Texture > const& texture, int layer = 2, std::type_index type = typeid( Sprite ) );


    /// @brief              Virtual destructor: just to let derived types free stuff
    virtual ~Sprite() {}

//-----------------------------------------------------------------------------
protected: // constructor
//-----------------------------------------------------------------------------

    /// @brief              Inherited constructor
    /// @param type         typeid(DerivedClass)
    Sprite( std::type_index type );

//-----------------------------------------------------------------------------
public: // copying
//-----------------------------------------------------------------------------
    
    /// @brief  creates a new Sprite component which is a duplicate of this one
    /// @return Copy of this component.
    virtual Sprite* Clone() const override;

//-----------------------------------------------------------------------------
protected: // copying
//-----------------------------------------------------------------------------

    /// @brief  Copy constructor
    Sprite(Sprite const& other);

//-----------------------------------------------------------------------------
public: // methods
//-----------------------------------------------------------------------------

    /// @brief  Draws the mesh with texture (if one is present), or color.
    virtual void Draw();


    /// @brief  checks if a a point in local space overlaps this Sprite
    /// @param  point   the point to check if overlaps this Sprite
    /// @return whether this Sprite overlaps the point
    virtual bool OverlapsLocalPoint( glm::vec2 const& point ) const;


//-----------------------------------------------------------------------------
public: // accessors
//-----------------------------------------------------------------------------

    /// @brief  gets the frame index
    /// @return the frame index
    int GetFrameIndex() const;

    /// @brief  Sets current frame index of the spritesheet.
    /// @param  frameIndex  New frame index
    void SetFrameIndex( int frameIndex );

    /// @brief  gets the layer
    /// @return the layer
    int GetLayer() const;

    /// @brief  Sets the rendering layer : 0 - 4.  0 is back, 4 is front.
    /// @param  layer   Rendering layer to move this sprite to.
    void SetLayer( int layer );

    /// @brief  gets the opacity
    /// @return the opacity
    float GetOpacity() const;

    /// @brief  Sets the opacity.
    /// @param  opacity I'm not explaining this.
    void SetOpacity( float opacity );

    /// @brief  gets the color
    /// @return the color
    glm::vec4 const& GetColor() const;

    /// @brief  sets the color
    /// @param  color   the color to apply
    void SetColor( glm::vec4 const& color );

    /// @brief  gets the Texture this Sprite is using
    /// @return the Texture this Sprite is using
    AssetReference< Texture > const& GetTexture() const;

    /// @brief  sets the Texture this Sprite is using
    /// @param  texture the Texture to set this Sprite to use
    void SetTexture( AssetReference< Texture > const& texture );

    /// @brief  gets the Transform component attached to this Sprite
    /// @return the Transform component attached to this Sprite
    Transform* GetTransform();


//-----------------------------------------------------------------------------
protected: // virtual override methods
//-----------------------------------------------------------------------------
    
    /// @brief  called when entering a scene
    virtual void OnInit() override;

    /// @brief  called when exiting a scene
    virtual void OnExit() override;

    virtual void Inspector() override;


//-----------------------------------------------------------------------------
protected: // member variables
//-----------------------------------------------------------------------------

    int m_Layer;
    int m_FrameIndex;
    float m_Opacity;

    glm::vec4 m_Color;

    /// @brief  the texture that this Sprite uses
    AssetReference< Texture > m_Texture;

    /// @brief  the Transform attached to this Sprite
    ComponentReference< Transform > m_Transform;


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
    void readTexture( nlohmann::ordered_json const& data );

    /// @brief  reads the frame index of this Sprite
    /// @param  stream  the json data to read from
    void readFrameIndex( nlohmann::ordered_json const& data );

    /// @brief  Read in the color for a sprite.
    /// @param  stream  The json to read from.
    void readColor( nlohmann::ordered_json const& data );

    /// @brief  Read in the opacity for a sprite.
    /// @param  stream  The json to read from.
    void readOpacity( nlohmann::ordered_json const& data );

    /// @brief  Read in the layer for a sprite.
    /// @param  stream  The json to read from.
    void readLayer( nlohmann::ordered_json const& data );

public:

    /// @brief Write all Sprite component data to a JSON file.
    /// @return The JSON file containing the Sprite component data.
    virtual nlohmann::ordered_json Write() const override;
    
//-----------------------------------------------------------------------------
private: // reading
//-----------------------------------------------------------------------------

    /// @brief the map of read methods for this Component
    static ReadMethodMap< Sprite > const s_ReadMethods;

    /// @brief gets the map of read methods for this Component
    /// @return the map of read methods for this Component
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override
    {
        return (ReadMethodMap< ISerializable > const&)s_ReadMethods;
    }

//-----------------------------------------------------------------------------
};