#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 UV;

out vec2 v_UV;

// init to identity matrix by default
uniform mat4 mvp = mat4(1,0,0,0, 
                        0,1,0,0, 
                        0,0,1,0, 
                        0,0,0,1); // (it's transposed btw)

uniform vec2 UV_offset = vec2(0,0);

void main()
{
    gl_Position = mvp * position;

    v_UV = UV + UV_offset;
}
