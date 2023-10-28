#version 430 core
layout(local_size_x = 1024, local_size_y = 1, local_size_z = 1) in;

struct Particle
{
    vec2 pos; float size,    rotation;        // 4
    vec2 vel; float size_d,  rotation_d;      // 4
    vec2 acc; float drag;                     // 3
    float lifetime, fade_startTime, fade_rate; // 3
};

// how to initialize particles (including, how to randomize them)
struct InitData
{
    ivec2 range;
    vec2 position;
    float direction,  speed,        size,         rotation;
    vec2 pos_spread;
    float dir_spread, speed_spread, size_spread,  rotation_spread;
    float fade_startTime, fade_rate;
};

// Particle data buffer
layout(std430, binding = 0) buffer DataSSBO
{
    Particle particles[];
};

// Transform matrices buffer
layout(std430, binding = 1) buffer MatrixSSBO
{
    mat4 transforms[];
};

// Uniform block - represents array of particle groups to emit during this frame
layout(std430, binding = 2) buffer InitDataUBO
{
    InitData init[];
};

uniform int initCount;  // amount of emissions to initialize

uniform mat4 proj;  // projection matrix
uniform float t;    // time
uniform float dt;   // deltaTime

float rand(float seed) { return 2*fract( sin((float(gl_GlobalInvocationID.x) + 242.9 + seed) * fract(t)) * 43758.5453) - 1.0; }

void main()
{
    // it's onde-dimensional. Only need the x.
    const uint idx = gl_GlobalInvocationID.x;

    // If this particle's ID is within range, initialize it
    for (int i=0; i<initCount; ++i)
    {
        if (init[i].range.x <= idx && idx < init[i].range.y)
        {
            vec2 pos_spread = vec2( init[i].pos_spread.x * rand(1.0), init[i].pos_spread.y * rand(2.0) );
            float direction = init[i].direction + init[i].dir_spread * rand(3.0);
            float speed = init[i].speed + init[i].speed_spread * rand(4.0);

            particles[idx].pos = init[i].position + pos_spread;
            particles[idx].vel = vec2( cos(direction), sin(direction) ) * speed;
            particles[idx].size = init[i].size + init[i].size_spread * rand(5.0);
            particles[idx].rotation = init[i].rotation + init[i].rotation_spread * rand(6.0);
            particles[idx].fade_startTime = init[i].fade_startTime;

            particles[idx].lifetime = 0.0;
        }
    }

    // apply the uh.. derivatives
    particles[idx].vel *= (1.0 - particles[idx].drag*dt);
    particles[idx].vel += particles[idx].acc * dt;
    particles[idx].pos += particles[idx].vel * dt;

    particles[idx].size += particles[idx].size_d * dt;
    particles[idx].rotation += particles[idx].rotation_d * dt;

    // "destroy" when time runs out
    particles[idx].lifetime += dt;
    particles[idx].size *= step(particles[idx].lifetime, particles[idx].fade_startTime);

    // pre-calculate mvp matrix here, make life easier for vertex shader
    mat4 S = mat4( mat2(particles[idx].size) );
    float rcos = cos(particles[idx].rotation);
    float rsin = sin(particles[idx].rotation);
    mat4 R = mat4( mat2(rcos, rsin, -rsin, rcos) );
    mat4 T = mat4(1.0);  T[3] = vec4(particles[idx].pos, 0, 1);
    transforms[idx] = proj * T * R * S;
}
