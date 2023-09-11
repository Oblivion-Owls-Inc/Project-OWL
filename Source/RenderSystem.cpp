/// @file     RenderSystem.cpp
/// @author   Eli Tsereteli (ilya.tsereteli@digipen.edu)
/// 
/// @brief    System responsible for drawing sprites and basic shapes.
#include "glew.h"
#include "RenderSystem.h"
#include "Sprite.h"

// TODO: screen2clip should be built in a different system
#include "glm/gtc/matrix_transform.hpp"
#include "PlatformSystem.h" 
#include "Entity.h"
#include "Mesh.h"
#include "Transform.h"
static glm::mat4 screen2clip;
static Mesh mesh;   // also, this should be stored in a different system... probably

// TEST ============================================================================================================
static Entity* testEnt;
static float frametime = 0.0f;
static void make_Ent()
{
    Sprite* s = new Sprite("Elementals_leaf_ranger_288x128_SpriteSheet.png", 22, 17);
    Transform* t = new Transform;
    t->setScale({800, -800 * s->getHeightMultiplier(), 0}); // for screen space, y is flipped, so flip the image.
    t->setTranslation({ 100,400,0 });
    testEnt = new Entity; testEnt->Add(s); testEnt->Add(t);
}
//      ============================================================================================================


RenderSystem* RenderSystem::instance = nullptr; // Init the instance pointer


/// @brief    Constructor: inits shader pointers to null.
RenderSystem::RenderSystem() : _colorShader(nullptr), _textureShader(nullptr) {}


/// @brief      Initializes color and texture shaders for sprites
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

    make_Ent();  // TEST -----------------------------------------------------------------------------------------
}

void RenderSystem::OnUpdate(float dt)
{
    for (Sprite* sprite : _sprites)
        if (sprite->isVisible())
            sprite->draw();
    
    // TEST ======================================================================================================
    frametime += dt*10;
    if (frametime < 44.0f && frametime >= 12.0f)
        frametime = 0.0f;
    if (frametime >= 65.0f)
        frametime = 0.0f;

    ((Sprite*)testEnt->HasComponent(typeid(Sprite)))->setFrame((int)frametime);
    //      ======================================================================================================
}


/// @brief      Cleans up memory
void RenderSystem::OnExit()
{
    delete _colorShader;
    delete _textureShader;

    delete testEnt;      // TEST ---------------------------------------------------------------------------------
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


void RenderSystem::ColorMode() { _colorShader->use(); _activeShader = _colorShader; }
void RenderSystem::TextureMode() { _textureShader->use(); _activeShader = _textureShader; }

void RenderSystem::SetColor(glm::vec4 const& color)
{
    glUniform4fv(_colorShader->GetUniformID("color"), 1, &color[0]);
}

void RenderSystem::SetUV(float u, float v)
{
    glUniform2f(_textureShader->GetUniformID("UV_offset"), u, v);
}

void RenderSystem::SetTransformMat(glm::mat4 const& mat) const
{
    // Assume mat is screen transform     TODO: Transform should prolly have a flag that says which space it's in.
    glm::mat4 mvp = screen2clip * mat;

    glUniformMatrix4fv(_activeShader->GetUniformID("mvp"), 1, 0, &mvp[0][0]);
}


void RenderSystem::AddSprite(Sprite* sprite) { _sprites.emplace(sprite); }
void RenderSystem::RemoveSprite(Sprite* sprite) { _sprites.erase(sprite); }



/// @brief    Gets the instance of RenderSystem
/// @return   RenderSystem pointer: new or existing instance of this system
RenderSystem* RenderSystem::getInstance()
{
    if (instance == nullptr)
        instance = new RenderSystem();
    
    return instance;
}

