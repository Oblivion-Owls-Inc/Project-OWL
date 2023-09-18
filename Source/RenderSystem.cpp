/// @file     RenderSystem.cpp
/// @author   Eli Tsereteli (ilya.tsereteli@digipen.edu)
/// 
/// @brief    System responsible for drawing sprites and basic shapes.
#include "glew.h"
#include "RenderSystem.h"
#include "Sprite.h"
#include "PlatformSystem.h"     // for building screen2clip matrix
#include "glm/gtc/matrix_transform.hpp"

// TODO: remove this once we have resource library
#include "Mesh.h"
static Mesh mesh;

// Singleton stuff
RenderSystem* RenderSystem::instance = nullptr; // Init the instance pointer
RenderSystem::RenderSystem() {}


/// @brief      Initializes color and texture shaders for sprites, as well as screen2clip matrix
void RenderSystem::OnInit()
{
    _colorShader = new Shader("Data/shaders/vshader.vert", "Data/shaders/color.frag");
    _textureShader = new Shader("Data/shaders/vshader.vert", "Data/shaders/texture.frag");

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // TODO: screen2clip should be built elsewhere
    glm::vec2 s = PlatformSystem::getInstance()->GetWindowDimensions();
    screen2clip = glm::translate(glm::mat4(1), glm::vec3(-1, 1, 0)) *
        glm::scale(glm::mat4(1), glm::vec3(2.0f / s.x, -2.0f / s.y, 0.0));

    mesh.load_square(); // for shapes (TODO: figure out a better system for this)
}


/// @brief      Draws all sprites layer by layer.
/// @param dt   Time since last frame
void RenderSystem::OnUpdate(float dt)
{
    for (std::set<Sprite*>& layer : _sprites)
        for (Sprite* sprite : layer)
            sprite->draw();

    (void)dt;
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
    glm::mat4 transform = t * r * s;
    
    // Set the matrix and color, draw the mesh
    ColorMode();
    SetTransformMat(transform);

    SetColor(color);
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

/// @brief      Switch to color shader.
void RenderSystem::ColorMode() { _colorShader->use(); _activeShader = _colorShader; }
/// @brief      Switch to texture shader.
void RenderSystem::TextureMode() { _textureShader->use(); _activeShader = _textureShader; }
/// @brief      Set color for the color shader
void RenderSystem::SetColor(glm::vec4 const& color)
{
    glUniform4fv(_colorShader->GetUniformID("color"), 1, &color[0]);
}
/// @brief      Set UV offset for the texture shader
void RenderSystem::SetUV(float u, float v)
{
    glUniform2f(_textureShader->GetUniformID("UV_offset"), u, v);
}
/// @brief      Set transformation matrix for either color or texture shader
void RenderSystem::SetTransformMat(glm::mat4 const& mat) const
{
    // Assume mat is screen transform     TODO: Transform should prolly have a flag that says which space it's in.
    glm::mat4 mvp = screen2clip * mat;

    glUniformMatrix4fv(_activeShader->GetUniformID("mvp"), 1, 0, &mvp[0][0]);
}

void RenderSystem::SetOpacity(float opacity) const
{
    glUniform1f(_activeShader->GetUniformID("opacity"), opacity);
}

/// @brief          Add sprite so it can be rendered during update. To be used by Sprite constructor.
/// @param sprite   Sprite pointer to add and keep track of
void RenderSystem::AddSprite(Sprite* sprite, int layer) { _sprites[layer].emplace(sprite); }

/// @brief          Remove sprite from the list to stop rendering it on update. To be used by Sprite destructor.
/// @param sprite   Sprite pointer to remove
void RenderSystem::RemoveSprite(Sprite* sprite, int layer) { _sprites[layer].erase(sprite); }


/// @brief the Read Methods used in this System
std::map< std::string, ReadMethod< RenderSystem > > const RenderSystem::ReadMethods;

/// @brief Gets the read methods of this System
/// @return the map of read methods of this System
std::map< std::string, ReadMethod< System > > const& RenderSystem::GetReadMethods()
{
    return (std::map< std::string, ReadMethod< System > > const&)ReadMethods;
}


/// @brief    Gets the instance of RenderSystem
/// @return   RenderSystem pointer: new or existing instance of this system
RenderSystem* RenderSystem::getInstance()
{
    if (instance == nullptr)
        instance = new RenderSystem();
    
    return instance;
}

