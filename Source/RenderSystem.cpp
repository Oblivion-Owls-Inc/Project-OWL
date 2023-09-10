/// @file     RenderSystem.cpp
/// @author   Eli Tsereteli (ilya.tsereteli@digipen.edu)
/// 
/// @brief    System responsible for drawing sprites and basic shapes.
#include "glew.h"
#include "RenderSystem.h"
#include "Mesh.h"

// TODO: screen2clip should be built in a different system
#include "glm/gtc/matrix_transform.hpp"
#include "PlatformSystem.h" 
static glm::mat4 screen2clip;
static Mesh mesh;   // also, this should be stored in a different system... probably


RenderSystem* RenderSystem::instance = nullptr; // Init the instance pointer


/// @brief    Constructor: inits shader pointers to null.
RenderSystem::RenderSystem() : _colorShader(nullptr), _textureShader(nullptr) {}


/// @brief      Initializes color and texture shaders for sprites
void RenderSystem::OnInit()
{
    _colorShader = new Shader("shaders/vshader.vert", "shaders/color.frag");
    _textureShader = new Shader("shaders/vshader.vert", "shaders/texture.frag");

    // TODO: screen2clip should be built elsewhere
    glm::vec2 s = PlatformSystem::getInstance()->GetWindowDimensions();
    screen2clip = glm::translate(glm::mat4(1), glm::vec3(-1, 1, 0)) *
        glm::scale(glm::mat4(1), glm::vec3(2.0f / s.x, -2.0f / s.y, 0.0));
}

/// @brief      Cleans up memory
void RenderSystem::OnExit()
{
    delete _colorShader;
    delete _textureShader;
}

/// @brief              Draws a rectangle.
/// @param position     Position
/// @param scale        (optional) Scale
/// @param angle        (optional) Angle
/// @param color        (optional) Color
void RenderSystem::DrawRect(const glm::vec2& position, const glm::vec2& scale,
                        float angle, const glm::vec4& color)
{
    // Build a matrix
    glm::mat4 I(1);
    glm::mat4 s = glm::scale(I, {scale.x, scale.y, 0});
    glm::mat4 r = glm::rotate(I, angle, {0,0,1});
    glm::mat4 t = glm::translate(I, {position.x, position.y, 0});
    glm::mat4 transform = screen2clip * t * r * s;

    // Set the matrix and color, draw the mesh
    _colorShader->use();
    glUniformMatrix4fv(_colorShader->GetUniformID("transform"), 1, false, &transform[0][0]);
    glUniform4fv(_colorShader->GetUniformID("color"), 1, &color[0]);
    mesh.draw();
}


/// @brief              Draws a line between 2 points.
/// @param P1           Point 1
/// @param P2           Point 2
/// @param thickness    (optional) How thicc the line is
/// @param color        (optional) Color of the line
void RenderSystem::DrawLine(const glm::vec2& P1, const glm::vec2& P2, float thickness, const glm::vec4& color)
{
    // Position a rectangle between the 2 points, angle it and stretch it
    glm::vec2 midpoint = (P1 + P2) * 0.5f;
    glm::vec2 direction = P2 - P1;
    float angle = glm::atan(direction.y, direction.x);
    float length = glm::sqrt(glm::dot(direction, direction));

    DrawRect(midpoint, { length, thickness }, angle, color);
}


/// @brief    Gets the instance of RenderSystem
/// @return   RenderSystem pointer: new or existing instance of this system
RenderSystem* RenderSystem::getInstance()
{
    if (instance == nullptr)
        instance = new RenderSystem();
    
    return instance;
}

