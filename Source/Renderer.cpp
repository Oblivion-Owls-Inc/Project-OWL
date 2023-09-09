/// @file     Renderer.cpp
/// @author   Name (first.last@digipen.edu)
/// 
/// @brief    Example System meant to be copy-pasted when creating new Systems
#include "Renderer.h"
#include "glew.h"
#include "glm/glm.hpp"
#include <string>
#include <fstream>      // reading shader code
#include <iostream>     // cout (temporary)     TODO: replace with Jax'es debug thing

#include "PlatformSystem.h"
#include "glfw3.h"

// Init the instance pointer
Renderer* Renderer::instance = nullptr;

// TODO: temporary for testing - will go into Mesh
static unsigned int _vaoID, _bufferID, _vertexCount;
struct Vertex
{
    glm::vec2 pos;
    glm::vec4 color;
    glm::vec2 UV;
};
static void simplemesh()
{
    // Vertex array object (contains the rest)
    glGenVertexArrays(1, &_vaoID);
    glBindVertexArray(_vaoID);

    // Main buffer: create it, bind it to proper target, define its layout.
    glGenBuffers(1, &_bufferID);
    glBindBuffer(GL_ARRAY_BUFFER, _bufferID);

    // Define the layout (attributes): vertex position + color + UV.
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);                              // index 0: position (2 floats)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color)); // index 1: color (4 floats)
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, UV));    // index 2: UV (2 floats)
    glEnableVertexAttribArray(2);

    // No index buffer, I'll just use trianglestrip.

    // Default mesh: centered black unit square.    
    Vertex vertices[] = { {{-0.5f,  0.5f}, {1,0,0,1}, {0.0f, 0.0f}},
                          {{ 0.5f,  0.5f}, {0,1,0,1}, {1.0f, 0.0f}},
                          {{-0.5f, -0.5f}, {0,0,1,1}, {0.0f, 1.0f}},
                          {{ 0.5f, -0.5f}, {1,1,1,1}, {1.0f, 1.0f}} };

    glBindBuffer(GL_ARRAY_BUFFER, _bufferID);
    glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Vertex), vertices, GL_STATIC_DRAW);
    _vertexCount = 4;
}


/// @brief    Constructs the Renderer
Renderer::Renderer() : _colorShader(nullptr) {}

void Renderer::OnInit()
{
    _colorShader = new Shader("shaders/vshader.vert", "shaders/color.frag");

    // TODO: temporary for testing              <-------------------------
    simplemesh();
}

// TODO: temporary for testing                  <-------------------------
void Renderer::OnUpdate(float dt)
{
    if (glfwGetKey(PlatformSystem::getInstance()->GetWindowHandle(), GLFW_KEY_SPACE))
    {
        glBindVertexArray(_vaoID);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, _vertexCount);        
    }

}

void Renderer::OnExit()
{
    delete _colorShader;

    // TODO: temporary for testing              <-------------------------
    glDeleteBuffers(1, &_bufferID);
    glDeleteVertexArrays(1, &_vaoID);
}


/// @brief    Gets the instance of Renderer
/// @return   Renderer pointer: new or existing instance of this system
Renderer* Renderer::getInstance()
{
    if (instance == nullptr)
    {
        instance = new Renderer();
    }
    return instance;
}
// MAY INCLUDE A DUPLICATE OF THE ABOVE FUNCTION WITH CONSTRUCTOR ARGUMENTS HERE



// ===========================    SHADER STUFF     ============================ //


/// @brief     (helper) Reads shader source code from a file.
/// 
/// @param filepath     Shader file to read.
/// 
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


/// @brief      Constructor: compiles new shader program from given source code files.
/// 
/// @param vertex_filepath      Vertex shader file
/// @param fragment_filepath    Fragment shader file
Renderer::Shader::Shader(const char* vertex_filepath, const char* fragment_filepath) : _shaderID(0)
{
    // Compile vertex and fragment parts
    unsigned int vertID = CompileShader(vertex_filepath, GL_VERTEX_SHADER);
    unsigned int fragID = CompileShader(fragment_filepath, GL_FRAGMENT_SHADER);

    // Link them together into one shader program.
    if (vertID && fragID)
    {
        _shaderID = glCreateProgram();
        glAttachShader(_shaderID, vertID);
        glAttachShader(_shaderID, fragID);
        glLinkProgram(_shaderID);
        glValidateProgram(_shaderID);
    }

    // Separate shader objects are no longer needed, get rid of them.
    if (vertID)     glDeleteShader(vertID);
    if (fragID)     glDeleteShader(fragID);

    glUseProgram(_shaderID);
    u_transform = glGetUniformLocation(_shaderID, "transform");

    // TODO: temporary
    glm::mat4 m(1);
    glUniformMatrix4fv(u_transform, 1, false, (float*)&m);
}


Renderer::Shader::~Shader()
{
    glUseProgram(0);

    if (_shaderID)    glDeleteProgram(_shaderID);
}