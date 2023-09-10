#version 330 core

uniform vec4 color = vec4(0,0,0,1);

out vec4 out_color;
void main()
{
    out_color = color;
}
