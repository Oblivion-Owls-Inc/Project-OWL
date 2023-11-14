#version 430 core

out vec4 pixel_color;

in vec2 v_UV;
in float v_opacity;

uniform sampler2D TextureSlot;
uniform float opacity = 1.0f;
uniform vec4 tint = vec4(0,0,0,0);

void main()
{
    pixel_color = texture(TextureSlot, v_UV);
    pixel_color.w *= opacity * v_opacity;
    pixel_color += tint * pixel_color.w;
}
