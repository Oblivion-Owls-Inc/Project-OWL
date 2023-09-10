#pragma once
#include <map>

class Shader
{
private:
    unsigned int shaderID;
    std::map<const char*, unsigned int> uniformIDs;

public:

    Shader(const char* vertex_filepath, const char* fragment_filepath);
    ~Shader();

    unsigned int GetID();
    unsigned int GetUniformID(const char* uniform_name);
    void use();
};
