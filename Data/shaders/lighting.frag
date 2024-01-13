#version 430 core

out vec4 pixel_color;

in vec2 v_UV;

uniform sampler2DArray TextureArray;
uniform float opacity = 1.0f;
uniform int light_count = 0;

void main()
{
    pixel_color = vec4(0,0,0,1);

    for (int i=0; i<light_count; ++i)
        pixel_color *= texture( TextureArray, vec3(v_UV, i) );

    pixel_color.w *= opacity;
}
