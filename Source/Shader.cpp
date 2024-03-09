/// @file     Shader.cpp
/// @author   Eli Tsereteli (ilya.tsereteli@digipen.edu)
/// 
/// @brief    Shader class implementation.



#include "pch.h" // precompiled header has to be included first
#include "glew.h"
#include "Shader.h"
#include "DebugSystem.h"


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
        Debug() << "SHADER ERROR: Can't open or read shader file: " << filepath << std::endl;
        return 0;
    }
    const char* ccode = code.c_str();

    // Create new shader, load source code into it and compile it
    unsigned int id = glCreateShader(GL_type_SHADER);
    glShaderSource(id, 1, &ccode, nullptr); // 1 shader, from this source code, which is nul-terminated.
    glCompileShader(id);

    return id;
}


/// @brief                      Constructor: compiles new shader from given vertex+fragment 
///                             shaders' source code.
/// 
/// @param vertex_filepath      Vertex Shader file
/// @param fragment_filepath    Fragment Shader file
Shader::Shader(const char* vertex_filepath, const char* fragment_filepath)
{
    char shaderInfo[ 1024 ];
    GLsizei logSize = 0;

    // Compile vertex and fragment parts
    unsigned int vertID = CompileShader(vertex_filepath, GL_VERTEX_SHADER);
    unsigned int fragID = CompileShader(fragment_filepath, GL_FRAGMENT_SHADER);

    glGetShaderInfoLog( vertID, 1024, &logSize, shaderInfo );
    if ( logSize > 0 )
    {
        Debug() << " === shader \"" << vertex_filepath << "\" info log ===\n" << shaderInfo << std::endl;
    }

    glGetShaderInfoLog( fragID, 1024, &logSize, shaderInfo );
    if ( logSize > 0 )
    {
        Debug() << " === shader \"" << fragment_filepath << "\" info log ===\n" << shaderInfo << std::endl;
    }

    // Link them together into one shader program.
    if (vertID && fragID)
    {
        m_ShaderID = glCreateProgram();
        glAttachShader(m_ShaderID, vertID);
        glAttachShader(m_ShaderID, fragID);
        glLinkProgram(m_ShaderID);
        glValidateProgram(m_ShaderID);
    }

    // Separate shader objects are no longer needed, get rid of them.
    if (vertID)     glDeleteShader(vertID);
    if (fragID)     glDeleteShader(fragID);

    glGetProgramInfoLog( m_ShaderID, 1024, &logSize, shaderInfo );
    if ( logSize > 0 )
    {
        Debug() << " === program info log ===\n" << shaderInfo << std::endl;
    }
}


/// @brief                      Constructor: compiles new shader from given 
///                             compute shader's source code.
/// 
/// @param compute_filepath     Compute Shader file
Shader::Shader(const char* compute_filepath)
{
    unsigned int cmpID = CompileShader(compute_filepath, GL_COMPUTE_SHADER);
    if (!cmpID)
        return;

    m_ShaderID = glCreateProgram();
    glAttachShader(m_ShaderID, cmpID);
    glLinkProgram(m_ShaderID);
    glValidateProgram(m_ShaderID);

    glDeleteShader(cmpID);
}


/// @brief      Cleans up memory (deletes shader program)
Shader::~Shader()
{
    glUseProgram(0);

    if (m_ShaderID)
        glDeleteProgram(m_ShaderID);
}

/// @brief      Sets this shader as active.
void Shader::use() { glUseProgram(m_ShaderID); }


/// @brief      Gets this shader's ID  
unsigned int Shader::GetID() { return m_ShaderID; }


/// @brief      Gets the location of a uniform declared in the shader. Obtains it from the shader
///             on first request, then stores it locally for faster subsequent retrieval.
unsigned int Shader::GetUniformID(const char* uniform_name) 
{
    // Check if key exists. If it does, return what's there.
    std::map<const  char*, unsigned int>::iterator itr = m_UniformIDs.find(uniform_name);
    if (itr != m_UniformIDs.end())
        return itr->second;
    else
    {
        unsigned int u = glGetUniformLocation(m_ShaderID, uniform_name);
        // (returns -1 for undeclared uniforms)
        
        if (u != -1)
        {
            m_UniformIDs[uniform_name] = u;
        }
        else
        {
            Debug() << "SHADER ERROR: uniform \"" << uniform_name << "\" does not exist." << std::endl;
        }
        
        return u;
    }
}
