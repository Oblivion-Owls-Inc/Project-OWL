#version 430 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 UV;
layout(location = 2) in float opacity;
layout(location = 3) in mat4 mvp;

out vec2 v_UV;
out float v_opacity;

uniform vec2 UV_offset = vec2(0,0);

void main()
{
    gl_Position = mvp * position;  // this mvp is per-particle
    v_opacity = opacity;

    v_UV = UV + UV_offset;
}
