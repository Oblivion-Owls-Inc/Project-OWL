#version 430 core

out vec4 pixel_color;

in vec2 v_UV;

uniform sampler2D u_TextureSlot;
uniform vec2 u_UV_offset = vec2(0,0);

void main()
{
    pixel_color = texture(u_TextureSlot, v_UV + u_UV_offset);
}
