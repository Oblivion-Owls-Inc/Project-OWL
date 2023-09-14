#version 330 core

uniform vec4 color = vec4(0,0,0,1);
uniform float opacity = 1.0f;

out vec4 out_color;
void main()
{
    out_color = color;
    out_color.w *= opacity;
}
