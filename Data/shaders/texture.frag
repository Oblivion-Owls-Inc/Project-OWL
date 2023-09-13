#version 430 core

out vec4 pixel_color;

in vec2 v_UV;

uniform sampler2D TextureSlot; // set by glActiveTexture() inside Texture::bind()
uniform vec2 UV_offset = vec2(0,0);

void main()
{
    pixel_color = texture(TextureSlot, v_UV + UV_offset);
}
