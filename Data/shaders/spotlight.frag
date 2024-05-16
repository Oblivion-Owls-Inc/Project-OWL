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
    // Start with completely dark pixel
    float total_light_intensity = 1.0;

    for (int i = 0; i < light_count; ++i)
    {
        // Light-to-pixel vector
        vec2 l2p = gl_FragCoord.xy - positions[i].xy;

        // Calculate light strength at this pixel
        float r = radii[i]*strengths[i];
        float spot = (r * r) / (r * r + dot(l2p, l2p));

        // Combine light intensity multiplicatively
        total_light_intensity *= 1.0 - spot;
    }

    // Use the combined intensity to determine the transparency of the darkness overlay
    pixel_color = vec4(0, 0, 0, total_light_intensity);
}


