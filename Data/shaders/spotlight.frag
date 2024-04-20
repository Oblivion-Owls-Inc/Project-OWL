// Draws spotlight.
// Well, not really. It actually draws the darkness around it.
#version 430 core

out vec4 pixel_color;

layout(std430, binding = 13) buffer light_positions { vec4 positions[]; };
layout(std430, binding = 14) buffer light_radii { float radii[]; };
layout(std430, binding = 15) buffer light_strengths { float strengths[]; };

//uniform float ambient = 0.0;
uniform int light_count = 0;

void main()
{
    pixel_color = vec4(0,0,0,1);

    for (int i=0; i<light_count; ++i)
    {
        // light-to-pixel vector
        vec2 l2p = gl_FragCoord.xy - positions[i].xy;

        // calculate light strength at this pixel
        float spot = (radii[i] * radii[i]) 
                            / ( radii[i] * radii[i] + dot(l2p, l2p));
        spot *= strengths[i];

        // use it as transparency
        pixel_color.w = min( 1.0-clamp(spot, 0.0, 1.0), pixel_color.w);
    }
}
