/// @file     Renderer.cpp
/// @author   Name (first.last@digipen.edu)
/// 
/// @brief    Example System meant to be copy-pasted when creating new Systems
#include "Renderer.h"
#include "glew.h"
#include "glm/glm.hpp"
#include <string>
#include <fstream>      // reading shader code
#include <iostream>     // cout (temporary)     TODO: replace with Jax'es debug thing when it's ready

// TEMPORARY (test)  ----------------------------------------------------------
#include "PlatformSystem.h"
#include "glfw3.h"
#include "Mesh.h"
Mesh mesh;
// ----------------------------------------------------------------------------


Renderer* Renderer::instance = nullptr; // Init the instance pointer

/// @brief    Constructor: inits shader pointers to null.
Renderer::Renderer() : _colorShader(nullptr) {}


void Renderer::OnInit()
{
    _colorShader = new Shader("shaders/vshader.vert", "shaders/color.frag");

    // TEMPORARY
    mesh.load_square();
}

void Renderer::OnExit()
{
    delete _colorShader;
}


// TEMPORARY (test)   ----------------------------------------------------
void Renderer::OnUpdate(float dt)
{
    if (glfwGetKey(PlatformSystem::getInstance()->GetWindowHandle(), GLFW_KEY_SPACE))
    {
        _colorShader->use();
        mesh.draw();
    }
}
// -----------------------------------------------------------------------


Shader *Renderer::GetShader() { return _colorShader; }


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

