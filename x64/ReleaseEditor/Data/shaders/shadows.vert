// Simplified for rendering shadows. Cutting out all but the essentials.
// Compute shader already takes care of vertex coordinates, and there's no UV's.
#version 430 core

layout(location = 0) in vec4 position;

void main()
{
    gl_Position = position * 2;
}
