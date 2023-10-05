/// @file     TextSprite.h
/// @author   Eli Tsereteli (ilya.tsereteli@digipen.edu)
/// 
/// @brief    A version of Sprite specifically for rendering text. Uses GPU
///           instancing to draw multiple letters simultaneously.
#pragma once
#include "Sprite.h"
#include <sstream>  // accept new text

class Shader;   // fwd ref


/// @brief      A version of Sprite specifically for rendering text. Uses GPU
///             instancing to draw multiple letters simultaneously.
class TextSprite : public Sprite
{
//-----------------------------------------------------------------------------
public: // constructor / destructors
//-----------------------------------------------------------------------------

    /// @brief  default constructor
    TextSprite();

    /// @brief  manual constructor
    /// @param  texture             the texture for this TextSprite to use
    /// @param  strideMultiplier    (optional) Multiplier to adjust stride (spacing)
    /// @param  layer               (optional) Rendering layer: 0-4. 0 is back, 4 is front.
    TextSprite( Texture const* texture, float strideMultiplier = 1.0f, int layer = 2 );

    /// @brief  destructor
    ~TextSprite();

//-----------------------------------------------------------------------------
public: // methods
//-----------------------------------------------------------------------------

    /// @brief      Draws currently stored text using parent's transform.
    virtual void Draw() override;

//-----------------------------------------------------------------------------
public: // accessors
//-----------------------------------------------------------------------------

    /// @brief  gets the text of this TextSprite
    /// @return the text of this TextSprite
    __inline std::string const& GetText() const { return m_Text; }

    /// @brief          sets the text of this TextSprite
    /// @param  text    the text of this TextSprite
    __inline void SetText( std::string const& text ) { m_Text = text; } 

    /// @brief  gets the row width
    /// @return the row width
    __inline int GetRowWidth() const { return m_RowWidth; }

    /// @brief  Sets the width of a single row (amount of columns)
    /// @param  columns     the number of columns in each row
    __inline void SetRowWidth( int columns ) { m_RowWidth = columns; } 

    /// @brief  gets the stride multiplier
    /// @return the stride multiplier
    __inline glm::vec2 const& GetStrideMultiplier() const { return m_StrideMultiplier; }

    /// @brief  sets the stride multiplier
    /// @param  strideMult the stride multiplier
    __inline void SetStrideMultiplier( glm::vec2 const& strideMult ) { m_StrideMultiplier = strideMult; } 

//-----------------------------------------------------------------------------
private: // virtual override methods
//-----------------------------------------------------------------------------

    /// @brief  called when entering the scene
    virtual void OnInit() override;

    /// @brief  called when exiting the scene
    virtual void OnExit() override;

//-----------------------------------------------------------------------------
private: // member variables
//-----------------------------------------------------------------------------

    /// @brief  text displayed by the textSprite
    std::string m_Text;
    /// @brief  ID of buffer that stores instance data
    unsigned int m_InstBufferID = 0;
    /// @brief  Horizontal/vertical stride. 1.0 is full tile width
    glm::vec2 m_StrideMultiplier = {1,1};
    /// @brief  Amount of tiles per row
    int m_RowWidth = 43;    

 //-------------------------------------------------------------------------
private: // Reading
 //-------------------------------------------------------------------------

    /// @brief  Read in the text this TextSprite displays
    /// @param  stream  The json to read from.
    void readText(Stream stream);

    /// @brief  Read in the stride multiplier
    /// @param  stream  The json to read from.
    void readStrideMultiplier(Stream stream);

    /// @brief  Read in the amount of tile per row
    /// @param  stream  The json to read from.
    void readRowWidth( Stream stream );

    /// @brief  called after loading
    void afterLoad(Stream);

    /// @brief the map of read methods for this Component
    static ReadMethodMap< TextSprite > const s_ReadMethods;

    /// @brief gets the map of read methods for this Component
    /// @return the map of read methods for this Component
    virtual ReadMethodMap< Component > const& GetReadMethods() const override;

//-------------------------------------------------------------------------
};
