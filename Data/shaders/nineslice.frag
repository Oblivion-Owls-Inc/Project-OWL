#version 430 core

out vec4 pixel_color;

in vec2 v_UV;

// set by glActiveTexture() inside Texture::bind()
uniform sampler2D TextureSlot;

/// @brief  the opacity to display with
uniform float opacity = 1.0f;

/// @brief  the tint to display with
uniform vec4 tint = vec4(0,0,0,0);


/// @brief  the UV offset into the spritesheet of the frame to use
uniform vec2 uvOffset = vec2(0,0);

/// @brief  the size of the UVs of this mesh
uniform vec2 uvSize = vec2( 1.0, 1.0 );


/// @brief  the internal edges of the nineslice in negative directions
uniform vec2 minInternalEdge = vec2( 0.25, 0.25 );

/// @brief  the internal edges of the nineslice in positive directions
uniform vec2 maxInternalEdge = vec2( 0.75, 0.75 );


/// @brief  the widths of the minimum borders
uniform vec2 minBorderWidths = vec2( 0.25, 0.25 );

/// @brief  the widths of the maximum borders
uniform vec2 maxBorderWidths = vec2( 0.25, 0.25 );



/// @brief  linearly maps a value in a range to another range
float map( float destMin, float destMax, float sourceMin, float sourceMax, float t )
{
    return (t - sourceMin) / (sourceMax - sourceMin) * (destMax - destMin) + destMin;
}


/// @brief applies the nineslice deformation to the UVs
void applyNinesliceDeformation( inout vec2 uv )
{
    // separately apply the transformation to the X and Y axes
    for ( int i = 0; i < 2; ++i )
    {
        if ( uv[ i ] <= minBorderWidths[ i ] )
        {
            uv[ i ] = map( 0.0, minInternalEdge[ i ], 0, minBorderWidths[ i ], uv[ i ] );
        }
        else if ( uv[ i ] >= 1.0 - maxBorderWidths[ i ] )
        {
            uv[ i ] = map( maxInternalEdge[ i ], 1.0, 1.0 - maxBorderWidths[ i ], 1.0, uv[ i ] );
        }
        else
        {
            uv[ i ] = map( minInternalEdge[ i ], maxInternalEdge[ i ], minBorderWidths[ i ], 1.0 - maxBorderWidths[ i ], uv[ i ] );
        }
    }
}


/// @brief  converts normalized (between 0 and 1) UVs to uvs for the specified frame
/// @param  uv  the UV between 0 and 1 to convert to frame space
void convertToFrame( inout vec2 uv )
{
    uv = uv * uvSize + uvOffset;
}


void main()
{
    vec2 uv = v_UV;
    applyNinesliceDeformation( uv );
    convertToFrame( uv );

    pixel_color = texture( TextureSlot, uv );
    pixel_color.w *= opacity;
    pixel_color += tint * pixel_color.w;
}
