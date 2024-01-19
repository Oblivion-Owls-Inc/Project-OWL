// Draws spotlight.
// Well, not really. It actually draws the darkness around it.
#version 430 core

out vec4 pixel_color;

uniform float ambient = 0.0;
uniform float light_strength = 0.8;
uniform float light_radius = 150.0;  // in screen space
uniform vec2 light_pos;              // in screen space

void main()
{
    // light-to-pixel vector
    vec2 l2p = gl_FragCoord.xy - light_pos;

    // calculate light strength at this pixel
    float spot = (light_radius * light_radius) 
                        / dot(l2p, l2p);
    spot *= light_strength;

    // use it as transparency
    pixel_color = vec4( 0,0,0, clamp(1.0 - spot, 0.0, 1.0) );
}
