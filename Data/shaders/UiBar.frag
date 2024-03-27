/// @file       UiBar.frag
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      fragment shader for a configurable ui bar
/// @date       2024-01-22
/// 
/// @copyright  Copyright (c) 2023 Digipen Institute of Technology


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

/// @breif  the offset from the left edge of the sprite that the bar starts and ends
uniform vec2  borderPositions;

/// @breif  the total size of the bar, including the border
uniform vec2  size;

/// @breif  the number of sections in the bar
uniform int   numSections;

/// @breif  the opacity of the bar
uniform float opacity;

/// @breif  the vertical offset of the center of rotation
uniform float rotationPosition;


/// @brief  checks if a pixel is part of the border
/// @param  uv  the position of the pixel to check
/// @return whether the pixel is part of the border
bool isBorderPixel( vec2 uv )
{
    return (
        uv.x <= borderPositions.x ||
        uv.x >  borderPositions.y
    );
}


/// @brief  translates a pixel position to a bar position
/// @param  pixelPos    the pixel position to translate
/// @return the bar position of the pixel
vec2 pixelPosToBarPos( vec2 pixelPos )
{
    vec2  barOrigin = vec2( borderPositions.x * size.x, rotationPosition * size.y );
    
    float barLength = size.x * (borderPositions.y - borderPositions.x);

    return (pixelPos - barOrigin) / barLength;
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
    
    pixel_color = texture( TextureSlot, v_UV );
    pixel_color.w *= opacity;

    // // if the pixel is part of the border, sample the texture
    // if ( isBorderPixel( v_UV ) )
    // {
    //     return;
    // }

    // use the color of the first section the pixel is a part of
    pos = pixelPosToBarPos( pos );
    for ( int i = 0; i < numSections - 1; ++i )
    {
        if ( isWithinSection( pos, i ) )
        {
            pixel_color *= sectionColors[ i ];
            return;
        }
    }

    // if the pixel didn't find a section, use the last section
    pixel_color *= sectionColors[ numSections - 1 ];
}
