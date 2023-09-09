#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec2 UV;

out vec2 v_UV;
out vec4 v_color;

// init to identity matrix by default
uniform mat4 transform = mat4(1,0,0,0, 
                              0,1,0,0, 
                              0,0,1,0, 
                              0,0,0,1); // (it's transposed btw)

void main()
{
    gl_Position = transform * position;

    v_UV = UV;
    v_color = color;
}
