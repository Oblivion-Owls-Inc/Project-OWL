/// @file     Shader.h
/// @author   Eli Tsereteli (ilya.tsereteli@digipen.edu)
/// 
/// @brief    Shader class declaration/interface.
#pragma once
#include <map>

class Shader
{
private:
    unsigned int shaderID;
    std::map<const char*, unsigned int> uniformIDs;

public:
    /// @brief                      Constructor: compiles new shader from given vertex+fragment shaders' source code.
    /// @param vertex_filepath      Vertex Shader file
    /// @param fragment_filepath    Fragment Shader file
    Shader(const char* vertex_filepath, const char* fragment_filepath);

    /// @brief      Cleans up memory (deletes shader program)
    ~Shader();

    /// @brief      Gets the ID of this shader program (its reference on GPU).
    unsigned int GetID();

    /// @brief                 Gets the ID/location of a uniform variable declared inside the shader.
    /// @param uniform_name    Exact name of the declared uniform
    unsigned int GetUniformID(const char* uniform_name);

    /// @brief      Sets this shader program as active.
    void use();
};
