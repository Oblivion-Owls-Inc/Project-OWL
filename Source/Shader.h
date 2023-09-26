/// @file     Shader.h
/// @author   Eli Tsereteli (ilya.tsereteli@digipen.edu)
/// 
/// @brief    Shader class declaration/interface.
#pragma once
#include <map>  // to store uniform ID's


/// @brief          Compiles a shader program, and stores information for managing it.
class Shader
{
public:
    /// @brief                      Constructor: compiles new shader from given 
    ///                             vertex+fragment shaders' source code.
    /// 
    /// @param vertex_filepath      Vertex Shader file
    /// @param fragment_filepath    Fragment Shader file
    Shader(const char* vertex_filepath, const char* fragment_filepath);

    /// @brief      Cleans up memory (deletes shader program)
    ~Shader();

    /// @return     ID of this shader program
    unsigned int GetID();

    /// @brief                 Gets the ID (location) of a uniform variable 
    ///                        declared inside the shader.
    /// 
    /// @param uniform_name    Exact name of the declared uniform
    /// @return                uniform ID
    unsigned int GetUniformID(const char* uniform_name);

    /// @brief      Sets this shader program as active.
    void use();



    //-------------------------------------------------------------------------
    //          data
    //-------------------------------------------------------------------------
private:
    /// @brief      ID of the shader program
    unsigned int m_ShaderID = 0;

    /// @brief      local storage of uniform IDs for faster retreival
    std::map<const char*, unsigned int> m_UniformIDs;
};
