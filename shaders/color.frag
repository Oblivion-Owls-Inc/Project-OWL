#version 330 core

in vec4 v_color;      // input color from vertex shader (interpolated)
out vec4 out_color;   // output pixel color. For some reason, this lint does not recognize gl_FragColor

uniform vec4 u_tint;

void main()
{
    out_color = v_color;
    out_color += vec4(u_tint.xyz, 0);
    out_color.w *= u_tint.w;
}
