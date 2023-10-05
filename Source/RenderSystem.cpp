/// @file     RenderSystem.cpp
/// @author   Eli Tsereteli (ilya.tsereteli@digipen.edu)
/// 
/// @brief    System responsible for drawing sprites and basic shapes.
#include "glew.h"
#include "RenderSystem.h"
#include "Sprite.h"
#include <iostream>             // error msg
#include "Entity.h"
#include "Transform.h"
#include <vector>
#include <algorithm>

static std::vector<Entity*> shapes; // this is inefficient.
// efficiency will improve once we have resource library.

// TODO: shapes: need a nice transform constructor.

/// @brief      Initializes color and texture shaders for sprites
void RenderSystem::OnInit()
{
    // These 2 will be used to render basic colored and textured sprites.
    _shaders["color"] = new Shader("Data/shaders/vshader.vert", "Data/shaders/color.frag");
    _shaders["texture"] = new Shader("Data/shaders/vshader.vert", "Data/shaders/texture.frag");

    // Enable transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}


/// @brief      Draws all sprites layer by layer.
/// @param dt   Time since last frame
void RenderSystem::OnUpdate(float dt)
{
    std::stable_sort( m_Sprites.begin(), m_Sprites.end(), []( Sprite const* a, Sprite const* b ) -> bool {
        return a->GetLayer() < b->GetLayer();
    });

    for ( Sprite* sprite : m_Sprites)
    {
        sprite->Draw();
    }

    // Once we have resource library, this will just be clear()
    for (Entity* e : shapes)
    {
        delete e;
    }

    shapes.clear();

    (void)dt;
}


/// @brief      Cleans up memory
void RenderSystem::OnExit()
{
    for (auto &shader : _shaders)
        delete shader.second;
}


/// @brief              Draws a rectangle.
/// 
/// @param position     Position
/// @param scale        (optional) Scale
/// @param angle        (optional) Angle
/// @param color        (optional) Color
void RenderSystem::DrawRect(const glm::vec2& position, const glm::vec2& scale,
                            float angle, const glm::vec4& color)
{
    shapes.push_back(new Entity);
    //shapes.back()->Add(new Transform(position, scale, angle));
    shapes.back()->AddComponent(new Sprite(true, color));
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


/// @brief          Add sprite so it can be rendered during update. To be used by Sprite constructor.
/// @param sprite   Sprite pointer to add and keep track of
void RenderSystem::AddSprite( Sprite* sprite )
{
    m_Sprites.push_back(sprite);
}


/// @brief          Remove sprite from the list to stop rendering it on update. To be used by Sprite destructor.
/// @param sprite   Sprite pointer to remove
void RenderSystem::RemoveSprite( Sprite* sprite )
{
    m_Sprites.erase( std::find( m_Sprites.begin(), m_Sprites.end(), sprite) );
}


/// @brief          Adds a shader to keep track of, so it can be freed automatically upon shutdown.
/// param name      Name to reference shader with
/// param shader    Pointer to the new shader
void RenderSystem::AddShader(const char* name, Shader* shader) { _shaders[name] = shader; }


/// @brief          Helper method for finding shaders by name. If shader isn't found, prints error and returns null.
/// @param name     Name of the shader as a basic c-style string
/// @return         Pointer to the shader, or nullptr if shader isn't found
Shader* RenderSystem::FindShader(const char* name)
{
    std::map<const  char*, Shader*>::iterator itr = _shaders.find(name);
    if (itr != _shaders.end())
        return itr->second;

    return nullptr;
}


/// @brief          Sets shader with given name as active, and returns pointer to it.
/// 
/// @param name     Name of the shader
/// @return         Shader pointer if the shader is found, nullptr if it isn't.
Shader* RenderSystem::SetActiveShader(const char* name) 
{
    Shader* s = FindShader(name);
    if (s)
    {
        s->use();
        _activeShader = s;
    }

    return s;
}


/// @brief          Returns pointer to a stored shader
/// @param name     Name of the shader to return
/// @return         Pointer to shader
Shader* RenderSystem::GetShader(const char* name) { return FindShader(name); }


//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------

    /// @brief map of the RenderSystem read methods
    ReadMethodMap< RenderSystem > const RenderSystem::s_ReadMethods = {};

    /// @brief  gets this System's read methods
    /// @return this System's read methods
    ReadMethodMap< System > const& RenderSystem::GetReadMethods() const
    {
        return (ReadMethodMap< System > const&)s_ReadMethods;
    }

//-----------------------------------------------------------------------------
// singleton stuff
//-----------------------------------------------------------------------------

RenderSystem::RenderSystem()
{}

RenderSystem* RenderSystem::instance = nullptr;

/// @brief    Gets the instance of RenderSystem
/// @return   RenderSystem pointer: new or existing instance of this system
RenderSystem* RenderSystem::GetInstance()
{
    if (instance == nullptr)
        instance = new RenderSystem();

    return instance;
}
