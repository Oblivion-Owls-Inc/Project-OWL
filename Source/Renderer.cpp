/// @file     Renderer.cpp
/// @author   Eli Tsereteli (ilya.tsereteli@digipen.edu)
/// 
/// @brief    System responsible for drawing sprites and basic shapes.
#include "Renderer.h"
#include "glew.h"
#include "glm/gtc/matrix_transform.hpp"
#include <string>
#include <fstream>      // reading shader code
#include <iostream>     // cout (temporary)     TODO: replace with Jax'es debug thing when it's ready

glm::mat4 screen2clip;

// TEMPORARY   =============================================================== //
#include "PlatformSystem.h"                                                    //
#include "glfw3.h"                                                             //
#include "Mesh.h"                                                              //
Mesh mesh;                                                                     //
#define _platform PlatformSystem::getInstance()                                //
float angle = 0.0f;                                                            //
glm::vec2 pos1, pos2;                                                          //
bool pressed = false;                                                          //
// =========================================================================== //


Renderer* Renderer::instance = nullptr; // Init the instance pointer

/// @brief    Constructor: inits shader pointers to null.
Renderer::Renderer() : _colorShader(nullptr), _textureShader(nullptr) {}


void Renderer::OnInit()
{
    _colorShader = new Shader("shaders/vshader.vert", "shaders/color.frag");
    _textureShader = new Shader("shaders/vshader.vert", "shaders/texture.frag");
    
    // TEMPORARY   ============================================================= //
    mesh.load_square();                                                          //
                                                                                 //
    glm::vec2 s = _platform->GetWindowDimensions();                              //
    screen2clip = glm::translate(glm::mat4(1), glm::vec3(-1,1,0)) *              //
                glm::scale(glm::mat4(1), glm::vec3(2.0f/s.x, -2.0f/s.y, 0.0));   //
                                                                                 //
    // ========================================================================= //
}

void Renderer::OnExit()
{
    delete _colorShader;
    delete _textureShader;
}

void Renderer::DrawRect(const glm::vec2& position, const glm::vec2& scale,
                        float angle, const glm::vec4& color)
{
    glm::mat4 I(1);
    glm::mat4 s = glm::scale(I, {scale.x, scale.y, 0});
    glm::mat4 r = glm::rotate(I, angle, {0,0,1});
    glm::mat4 t = glm::translate(I, {position.x, position.y, 0});
    glm::mat4 transform = screen2clip * t * r * s;

    _colorShader->use();
    glUniformMatrix4fv(_colorShader->GetUniformID("transform"), 1, false, &transform[0][0]);
    glUniform4fv(_colorShader->GetUniformID("color"), 1, &color[0]);
    mesh.draw();
}

void Renderer::DrawLine(const glm::vec2& P1, const glm::vec2& P2, float thickness, const glm::vec4& color)
{
    glm::vec2 midpoint = (P1 + P2) * 0.5f;
    glm::vec2 direction = P2 - P1;
    float angle = glm::atan(direction.y, direction.x);
    float length = glm::sqrt(glm::dot(direction, direction));

    DrawRect(midpoint, { length, thickness }, angle, color);
}

// TEMPORARY   ===================================================================== //
void Renderer::OnUpdate(float dt)                                                    //
{                                                                                    //
    GLFWwindow* w = _platform->GetWindowHandle();                                    //
    if (pressed)                                                                     //
    {                                                                                //
        if (glfwGetMouseButton(w, GLFW_MOUSE_BUTTON_2))                              //
        {                                                                            //
            double x, y;                                                             //
            glfwGetCursorPos(w, &x, &y);                                             //
            pos2 = { x, y };                                                         //
            DrawLine(pos1, pos2);                                                    //
        }                                                                            //
        else                                                                         //
            pressed = false;                                                         //
    }                                                                                //
    else if (glfwGetMouseButton(w, GLFW_MOUSE_BUTTON_2))                             //
    {                                                                                //
        double x, y;                                                                 //
        glfwGetCursorPos(w, &x, &y);                                                 //
        pos1 = { x,y };                                                              //
        pressed = true;                                                              //
    }                                                                                //
}                                                                                    //
// ================================================================================= // 



/// @brief    Gets the instance of Renderer
/// @return   Renderer pointer: new or existing instance of this system
Renderer* Renderer::getInstance()
{
    if (instance == nullptr)
        instance = new Renderer();
    
    return instance;
}

