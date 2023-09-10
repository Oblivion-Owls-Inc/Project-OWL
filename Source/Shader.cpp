/// @file     Shader.cpp
/// @author   Eli Tsereteli (ilya.tsereteli@digipen.edu)
/// 
/// @brief    Shader class implementation.
#include <iostream>
#include <fstream>
#include "glew.h"
#include "Shader.h"


/// @brief              (helper) Reads shader source code from a file.
/// @param filepath     Shader file to read.
/// @return             ID of the compiled shader. Returns 0 if failed.
static unsigned int CompileShader(const char* filepath, unsigned int GL_type_SHADER)
{
#define LINE_LENGTH 200     // I don't think I'll ever write GLSL lines longer than that...

    std::string code;

    // open the file, read its contents (source code)
    std::ifstream file(filepath);
    if (!file.fail())
    {
        char buffer[LINE_LENGTH];
        while (!file.eof())
        {
            file.getline(buffer, LINE_LENGTH);
            code += buffer;
            code += '\n';
        }
    }
    // fun fact: ifstream's destructor closes the file for you.

    // If code got read, get its c-style string version
    if (code.empty())
    {
        std::cout << "SHADER ERROR: Can't open or read shader file: " << filepath << std::endl;
        return 0;
    }
    const char* ccode = code.c_str();

    // Create new shader, load source code into it and compile it
    unsigned int id = glCreateShader(GL_type_SHADER);
    glShaderSource(id, 1, &ccode, nullptr); // 1 shader, from this source code, which is nul-terminated.
    glCompileShader(id);

    return id;
}


/// @brief                      Constructor: compiles new shader program from given source code files.
/// @param vertex_filepath      Vertex shader file
/// @param fragment_filepath    Fragment shader file
Shader::Shader(const char* vertex_filepath, const char* fragment_filepath) : shaderID(0)
{
    // Compile vertex and fragment parts
    unsigned int vertID = CompileShader(vertex_filepath, GL_VERTEX_SHADER);
    unsigned int fragID = CompileShader(fragment_filepath, GL_FRAGMENT_SHADER);

    // Link them together into one shader program.
    if (vertID && fragID)
    {
        shaderID = glCreateProgram();
        glAttachShader(shaderID, vertID);
        glAttachShader(shaderID, fragID);
        glLinkProgram(shaderID);
        glValidateProgram(shaderID);
    }

    // Separate shader objects are no longer needed, get rid of them.
    if (vertID)     glDeleteShader(vertID);
    if (fragID)     glDeleteShader(fragID);
}

/// @brief      Cleans up memory (deletes shader program)
Shader::~Shader()
{
    glUseProgram(0);

    if (shaderID)
        glDeleteProgram(shaderID);
}

/// @brief      Sets this shader as active.
void Shader::use() { glUseProgram(shaderID); }

/// @brief      Gets this shader's ID  
unsigned int Shader::GetID() { return shaderID; }

/// @brief      Gets the location of a uniform declared in the shader. Obtains it from the shader
///             on first request, then stores it for faster subsequent retrieval.
unsigned int Shader::GetUniformID(const char* uniform_name) 
{
    try
    {
        return uniformIDs.at(uniform_name);
    }
    catch (std::out_of_range &exception)
    {
        (void)exception;  // don't need it

        unsigned int u = glGetUniformLocation(shaderID, uniform_name);
        // (returns -1 for undeclared uniforms)
        
        if (u != -1)    uniformIDs[uniform_name] = u;
        else            std::cout << "SHADER ERROR: this uniform does not exist." << std::endl;
        
        return u;
    }
}
