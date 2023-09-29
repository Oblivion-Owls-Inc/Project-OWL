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
public:
    
    /// @brief      Temporary. Allow switching to a different texture, while
    ///             keeping track of original one. If called without argument,
    ///             resets texture to original.
    /// @param t    texture pointer - remember to free this one manually. 
    void SetTexture(Texture* t = nullptr);


    /// @brief              Default constructor: does nothing
    TextSprite();

    ~TextSprite();

    /// @brief              Constructor: unlike base Sprite constructor, this one 
    ///                     requires texture and columns/rows, since it's specifically
    ///                     for text.
    ///                     NOTE: Spritesheet should begin with space (ASCII 32), 
    ///                     and have the rest of the characters in proper ASCII order.
    /// 
    /// @param image_file   Path to the image file to load (should be spritesheet)
    /// @param columns      Columns of the spritesheet
    /// @param rows         Rows of the spritesheet
    /// @param stride_mult  (optional) Multiplier to adjust stride (spacing)
    /// @param layer        (optional) Rendering layer: 0-4. 0 is back, 4 is front.
    TextSprite(const char* image_file, int columns, int rows, float stride_mult = 1.0f, int layer = 2);

    /// @brief      Feed text into this function just like with std::cout.
    /// @return     Reference to string stream to feed text into.
    std::ostringstream & Text();

    /// @brief      Sets the width of a single row (amount of columns)
    void SetRowWidth(int columns);

    /// @brief      Draws currently stored text using parent's transform.
    virtual void Draw() override;



    //-------------------------------------------------------------------------
    //          data
    //-------------------------------------------------------------------------
private:
    std::ostringstream m_Stream;     /// @brief     Stream that accepts new text
    unsigned int m_InstBufferID = 0; /// @brief     ID of buffer that stores instance data
    glm::vec2 m_StrideMult = {1,1};  /// @brief     Horizontal/vertical stride. 1.0 is full tile width
    int m_RowWidth = 43;             /// @brief     Amount of tiles per row
    Texture* m_Original;             /// @brief     (temp) to keep track of original texture
};
