#pragma once

class Shader
{
private:
    unsigned int shaderID;
    unsigned int u_transform;   // TODO: map to support more uniforms (store their ID's)
    // (yes, map needs to be stored in individual Shader object)

public:

    Shader(const char* vertex_filepath, const char* fragment_filepath);
    ~Shader();

    unsigned int GetID();
    unsigned int GetUniformID();  // TODO: accept const char* to retreive uniform by name
    void use();
};
