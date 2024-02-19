#version 430 core
layout(local_size_x = 128, local_size_y = 1, local_size_z = 1) in;

// iirc as long as the sizes of structs are multiples of 8, they'll be aligned properly in the buffer.
// so add ints/floats in pairs. use "placeholder" if not sure what elese is needed.

// raw particle data
struct Particle
{
    vec2 pos, vel, acc, vec2padding; 
    float size, rotation,
          dirAcc,  drag,   // dirAcc - magnitude of accelerating in the initial direction
          lifetime, time,
          fadeIn, fadeOut,
          sizePerSec, align;
};

// how to initialize particles (including randomness and change over time)
struct InitData
{
    vec2 offset, pos_spread;
    vec2 acceleration; float direction,  speed;
    float size, rotation,  dir_spread, speed_spread; 
    float size_spread, rotation_spread,  fadeInDuration, fadeOutDuration;
    float lifetime, startAhead, dirAcc, sizePerSec;
    int bufferSize, align1;
};

// Particle data buffer
layout(std430, binding = 0) buffer DataSSBO { Particle particles[]; };

// Opacities buffer. Having it separate form matrices saves space (no padding)
layout(std430, binding = 1) buffer OpacitiesSSBO { float opacities[]; };

// Transform matrices buffer
layout(std430, binding = 2) buffer MatrixSSBO { mat4 transforms[]; };

// Uniform block - represents array of particle groups to emit during this frame
layout(std430, binding = 3) buffer InitDataUBO { InitData init[]; };


uniform int initCount;  // amount of emissions to initialize

uniform mat4 proj;      // projection matrix
uniform float t;        // time
uniform float dt;       // deltaTime
uniform ivec2 range;    // init start and end
uniform int initIndex;  // which init data to use this time
uniform int oldest;     // oldest particle index
uniform vec2 parentPos; // emit from parent entity's position. scale and rotation should be independent, I think...
                        //  TODO:? flag to stay within entity's local space? (as in, move with it instead of independently?)

// positive unit random (0 to 1)
float prand(float seed) { return fract( sin((float(gl_GlobalInvocationID.x) + 242.9 + seed) * fract(t)) * 43758.5453); }

// unit random (-1 to 1)
float urand(float seed) { return 2 * prand(seed) - 1.0; }

// returns "progress" (0 to 1) between start and end of 'time'
float lerpOpacity(float start, float end, float time) { return clamp((time - start) / (end-start), 0.0, 1.0); }

void main()
{
    // it's one-dimensional. Only need the x.
    const uint idx = gl_GlobalInvocationID.x;

    // If this particle's ID is within range, initialize it
    if (range.x <= idx && idx < range.y)
    {
        // pre-randomize
        vec2 pos_spread = vec2( init[initIndex].pos_spread.x * urand(1.0), init[initIndex].pos_spread.y * urand(2.0) );
        float direction = init[initIndex].direction + init[initIndex].dir_spread * urand(3.0);
        float speed = init[initIndex].speed + init[initIndex].speed_spread * urand(4.0);

        // 'zinit' will be 0 if range covers the entire buffer.
        // (I zero-init everything by setting range to the whole buffer size)
        float zinit = step(range.y - range.x, init[initIndex].bufferSize-1);

        // initialize   (to 0 or to initial values)
        particles[idx].vel = zinit *  (vec2( cos(direction), sin(direction) ) * speed);
        particles[idx].pos = zinit *  (parentPos + init[initIndex].offset + pos_spread +
                                       init[initIndex].startAhead * particles[idx].vel);
        particles[idx].size = zinit *  (init[initIndex].size - 
                                        init[initIndex].size_spread * prand(5.0));
        particles[idx].rotation = zinit *  (init[initIndex].rotation + 
                                            init[initIndex].rotation_spread * urand(6.0));
        particles[idx].acc = zinit * (init[initIndex].acceleration +
                                      init[initIndex].dirAcc * particles[idx].vel);
        particles[idx].lifetime = zinit * init[initIndex].lifetime;
        particles[idx].fadeIn =   zinit * init[initIndex].fadeInDuration;
        particles[idx].fadeOut =  zinit * particles[idx].lifetime - init[initIndex].fadeOutDuration;
        particles[idx].time = 0.0;
        particles[idx].sizePerSec = zinit * init[initIndex].sizePerSec;

        particles[idx].drag = 0.0; // TODO:?
    }

    // apply the uh.. derivatives
    particles[idx].vel *= (1.0 - particles[idx].drag*dt);
    particles[idx].vel += particles[idx].acc * dt;
    particles[idx].pos += particles[idx].vel * dt;
    particles[idx].size += step(0, particles[idx].size) * particles[idx].sizePerSec * dt;

    // "destroy" (just set size to 0) when time runs out
    particles[idx].time += dt;
    particles[idx].size *= step(particles[idx].time, particles[idx].lifetime);

    // pre-calculate mvp matrix here, to make life easier for vertex shader
    mat4 S = mat4( mat2(particles[idx].size) );
    float rcos = cos(particles[idx].rotation);
    float rsin = sin(particles[idx].rotation);
    mat4 R = mat4( mat2(rcos, rsin, -rsin, rcos) );
    mat4 T = mat4(1.0);  T[3] = vec4(particles[idx].pos, 0, 1);

    // arrange them from oldest to newest for rendering. inverse deque, wooo
    int renderIndex = (int(idx) + (init[initIndex].bufferSize - oldest)) % init[initIndex].bufferSize;
    transforms[renderIndex] = proj * T * R * S;

    // fade in and out
    opacities[renderIndex] = lerpOpacity(0,                      particles[idx].fadeIn,   particles[idx].time) *
                          (1-lerpOpacity(particles[idx].fadeOut, particles[idx].lifetime, particles[idx].time));
}

