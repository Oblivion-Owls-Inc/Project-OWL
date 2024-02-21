#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 UV;

out vec2 v_UV;

// init to identity matrix by default
uniform mat4 mvp = mat4(1,0,0,0, 
                        0,1,0,0, 
                        0,0,1,0, 
                        0,0,0,1); // (it's transposed btw)

/// @brief  the size of the UVs of this mesh
uniform vec2 uvSize = vec2( 1.0, 1.0 );

void main()
{
    gl_Position = mvp * position;
    v_UV = UV / uvSize; // normalize the UVs to between 0 and 1 (yes this introduces redundant calculations, but this way fits in our existing framework)
}
