#version 430 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 UV;
layout(location = 2) in mat4 mvp;

out vec2 v_UV;

void main()
{
    gl_Position = mvp * position;

    v_UV = UV;
}
