#version 430 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 UV;
layout(location = 2) in float tileID;

out vec2 v_UV;

// uniforms needed:
// 1. UV size
// 2. stride vector - pre-projected
// 3. amount of columns in the sprite
uniform vec2 UVsize = vec2(0.0,0.0);
uniform vec2 stridex = vec2(0.0,0.0);
uniform vec2 stridey = vec2(0.0,0.0);
uniform int columns = 1;     // amt of columns in the spritesheet
uniform int rowwidth = 10;   // row width of the tilemap

// fun fact: unused uniforms are excluded when compiling. Unless they are initialized.

// init to identity matrix by default
uniform mat4 mvp;

void main()
{
    //           discard negative | tilemap position | individual tile offset 
    gl_Position = step(0, tileID) * ( mvp * position + vec4(stridex,0,0) * (gl_InstanceID%rowwidth) + 
                                                       vec4(stridey,0,0) * (gl_InstanceID/rowwidth) );

    // calculate UV's for current letter instance
    int frame = int(tileID);
    // why tf does GLSL not receive ints thru buffer correctly??? 
    // (or maybe I'm just fucking it up..)
    int row = frame / columns;
    int col = frame % columns;
    vec2 UV_offset = vec2(UVsize.x * col, UVsize.y * row);

    v_UV = UV + UV_offset;
}
