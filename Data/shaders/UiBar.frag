#version 430 core


/// @brief  the color of the pixel to display onscreen
out vec4 pixel_color;

/// @brief  the uv coordinates of the fragment
in vec2 v_UV;

/// @brief  border texture
uniform sampler2D TextureSlot; 

/// @brief  the maximum number of sections the bar can support
#define MAX_UI_BAR_SECTIONS 3

/// @brief  the parameters for each section of the bar
uniform vec4  sectionColors[ MAX_UI_BAR_SECTIONS ];
uniform float sectionValues[ MAX_UI_BAR_SECTIONS ];
uniform float sectionSlopes[ MAX_UI_BAR_SECTIONS ];

/// @param  the size of the border of the bar
uniform vec2  negBorderWidth;
uniform vec2  posBorderWidth;

/// @param  the total size of the bar, including the border
uniform vec2  size;

/// @param  the number of sections in the bar
uniform int   numSections;

/// @param the opacity of the bar
uniform float opacity;


/// @brief  checks if a pixel is part of the border
/// @param  pixelPos    the position of the pixel to check
/// @return whether the pixel is part of the border
bool isBorderPixel( vec2 pixelPos )
{
    return (
        pixelPos.x < negBorderWidth.x ||
        pixelPos.y < negBorderWidth.y ||
        size.x - pixelPos.x < posBorderWidth.x ||
        size.y - pixelPos.y < posBorderWidth.y
    );
}


/// @brief  translates a pixel position to a bar position
/// @param  pixelPos    the pixel position to translate
/// @return the bar position of the pixel
vec2 pixelPosToBarPos( vec2 pixelPos )
{
    vec2  barSize = size - posBorderWidth - negBorderWidth;

    vec2  barOrigin = vec2( negBorderWidth.x, negBorderWidth.y + 0.5 * barSize.y );

    return (pixelPos - barOrigin) / barSize.x;
}


/// @brief  checks if a pixel is within a section
/// @param  barPos  the bar position of the pixel
/// @param  section the index of the section to check
/// @return whether the pixel is within the section
bool isWithinSection( vec2 barPos, int section )
{
    return (barPos.x + barPos.y * sectionSlopes[ section ] <= sectionValues[ section ]);
}

/// @brief  fragment main function
void main()
{
    vec2 pos = v_UV * size;

    // if the pixel is part of the border, sample the texture
    if ( isBorderPixel( pos ) )
    {
        pixel_color = texture( TextureSlot, v_UV );
        pixel_color.w *= opacity;
        return;
    }

    // use the color of the first section the pixel is a part of
    pos = pixelPosToBarPos( pos );
    for ( int i = 0; i < MAX_UI_BAR_SECTIONS - 1; ++i )
    {
        if ( i >= numSections - 1 )
        {
            break;
        }

        if ( isWithinSection( pos, i ) )
        {
            pixel_color = sectionColors[ i ];
            pixel_color.w *= opacity;
            return;
        }
    }

    // if the pixel didn't find a section, use the last section
    pixel_color = sectionColors[ numSections - 1 ];
    pixel_color.w *= opacity;
}