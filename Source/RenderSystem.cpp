/// @file     RenderSystem.cpp
/// @author   Eli Tsereteli (ilya.tsereteli@digipen.edu)
/// 
/// @brief    System responsible for drawing sprites and basic shapes.
#include "glew.h"
#include "RenderSystem.h"
#include "Sprite.h"
#include "PlatformSystem.h"     // screen size
#include "Entity.h"
#include "Transform.h"
#include <vector>
#include <algorithm>
#include "Mesh.h"
#include "AssetLibrarySystem.h"
#include "Texture.h"

#include "InputSystem.h"
#include "Engine.h"

static std::vector<Entity*> shapes; // this is inefficient.
// efficiency will improve once we have resource library.


/// @brief      Initializes color and texture shaders for sprites
void RenderSystem::OnInit()
{
    m_DefaultMesh = new Mesh();

    // These 2 will be used to render basic colored and textured sprites.
    m_Shaders["texture"] = new Shader("Data/shaders/vshader.vert", "Data/shaders/texture.frag");

    // Enable transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // set background color
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    // enable debug output
    glEnable              ( GL_DEBUG_OUTPUT );
    glDebugMessageCallback( errorCallback, 0 );

    // init FBO and texture for the screen buffer
    glGenFramebuffers(1, &m_ScreenBufferFBO);
    reallocScreenBufferTexture();
    Platform()->AddOnWindowResizeCallback(GetId(), std::bind(&RenderSystem::reallocScreenBufferTexture, this));
}


/// @brief      Draws all sprites layer by layer.
/// @param dt   Time since last frame
void RenderSystem::OnUpdate(float dt)
{
    std::stable_sort( m_Sprites.begin(), m_Sprites.end(), []( Sprite const* a, Sprite const* b ) -> bool {
        return a->GetLayer() < b->GetLayer();
    });

    // draw to off-screen texture instead of main buffer
    if (m_DrawToBuffer)
        glBindFramebuffer(GL_FRAMEBUFFER, m_ScreenBufferFBO);

    glClear(GL_COLOR_BUFFER_BIT);

    for ( Sprite* sprite : m_Sprites)
    {
        sprite->Draw();
    }

    // draw debug shapes
    for ( Entity* entity : shapes )
    {
        entity->GetComponent< Sprite >()->Draw();
        delete entity;
    }
    shapes.clear();

    // switch back to main buffer (for ImGui stuff to draw normally)
    if (m_DrawToBuffer)
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    glBindVertexArray(0);

    (void)dt;
}


/// @brief      Cleans up memory
void RenderSystem::OnExit()
{
    for (auto &shader : m_Shaders)
        delete shader.second;

    delete m_DefaultMesh;

    glDeleteBuffers(1, &m_ScreenBufferFBO);
    glDeleteTextures(1, &m_ScreenBufferTexID);
}


/// @brief              Draws a rectangle.
/// 
/// @param position     Position
/// @param scale        (optional) Scale
/// @param angle        (optional) Angle
/// @param color        (optional) Color
/// @param alpha        (optional) transparency
    /// @param isDiegetic   (optional) whether the shape is diegetic
void RenderSystem::DrawRect(
    glm::vec2 const& position, glm::vec2 const& scale, float angle,
    glm::vec4 const& color, float alpha,
    bool isDiegetic
)
{
    static Texture debugRectTexture = Texture( "Data/Textures/Debug/Rectangle.png" );

    DrawTexture( &debugRectTexture, position, scale, angle, color, alpha, isDiegetic );
}

/// @brief              Draws a line between 2 points.
/// @param P1           Point 1
/// @param P2           Point 2
/// @param thickness    (optional) How thicc the line is
/// @param color        (optional) Color of the line
/// @param alpha        (optional) transparency
/// @param isDiegetic   (optional) whether the shape is diegetic
void RenderSystem::DrawLine(
    glm::vec2 const& P1, glm::vec2 const& P2, float thickness,
    glm::vec4 const& color, float alpha,
    bool isDiegetic
)
{
    // Position a rectangle between the 2 points, angle it and stretch it
    glm::vec2 midpoint = (P1 + P2) * 0.5f;
    glm::vec2 direction = P2 - P1;
    float angle = glm::atan( direction.y, direction.x );
    float length = glm::length( direction );

    DrawRect( midpoint, { length, thickness }, angle, color, alpha, isDiegetic );
}

/// @brief              Draws a circle.
/// @param position     Position
/// @param radius       (optional) Radius
/// @param color        (optional) Color
/// @param alpha        (optional) transparency
/// @param isDiegetic   (optional) whether the shape is diegetic
void RenderSystem::DrawCircle(
    glm::vec2 const& position, float radius,
    glm::vec4 const& color, float alpha,
    bool isDiegetic
)
{
    static Texture debugCircleTexture = Texture( "Data/Textures/Debug/Circle.png" );

    DrawTexture( &debugCircleTexture, position, glm::vec2( radius * 2.0f ), 0.0f, color, alpha, isDiegetic );
}

/// @brief              Draws a texture
/// @param texture      the texture to draw
/// @param position     Position
/// @param scale        (optional) Scale
/// @param angle        (optional) Angle
/// @param color        (optional) Color
/// @param alpha        (optional) transparency
/// @param isDiegetic   (optional) whether the shape is diegetic
void RenderSystem::DrawTexture(
    Texture const* texture,
    glm::vec2 const& position, glm::vec2 const& scale, float angle,
    glm::vec4 const& color, float alpha,
    bool isDiegetic,
    int frameIndex
)
{
    shapes.push_back( new Entity() );

    Transform* transform = new Transform();
    transform->SetTranslation( position );
    transform->SetScale( scale );
    transform->SetRotation(angle);
    transform->SetIsDiegetic( isDiegetic );
    shapes.back()->AddComponent( transform );

    AssetReference< Texture > textureReference;
    textureReference = texture;
    Sprite* sprite = new Sprite( textureReference );
    sprite->SetColor( color );
    sprite->SetOpacity( alpha );
    sprite->SetFrameIndex( frameIndex );
    shapes.back()->AddComponent( sprite );
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
    auto it = std::find( m_Sprites.begin(), m_Sprites.end(), sprite);
    if ( it != m_Sprites.end() )
    {
        m_Sprites.erase( it );
    }
}


/// @brief          Adds a shader to keep track of, so it can be freed automatically upon shutdown.
/// param name      Name to reference shader with
/// param shader    Pointer to the new shader
void RenderSystem::AddShader(const char* name, Shader* shader) { m_Shaders[name] = shader; }


/// @brief          Helper method for finding shaders by name. If shader isn't found, prints error and returns null.
/// @param name     Name of the shader as a basic c-style string
/// @return         Pointer to the shader, or nullptr if shader isn't found
Shader* RenderSystem::FindShader(const char* name)
{
    std::map<const  char*, Shader*>::iterator itr = m_Shaders.find(name);
    if (itr != m_Shaders.end())
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
        m_ActiveShader = s;
    }

    return s;
}


/// @brief          Returns pointer to a stored shader
/// @param name     Name of the shader to return
/// @return         Pointer to shader
Shader* RenderSystem::GetShader(const char* name) { return FindShader(name); }


/// @brief  gets the topmost Sprite the mouse is over
/// @return the topmost Sprite the mouse is over
Sprite* RenderSystem::GetMouseOverSprite()
{
    
    // store a cached return value that we can reuse if this function gets called multiple times in the same frame
    static int lastGottenFrame = -1;
    static Sprite* cachedSprite = nullptr;
    
    if ( lastGottenFrame == GameEngine()->GetFrameCount() )
    {
        return cachedSprite;
    }
    
    lastGottenFrame = GameEngine()->GetFrameCount();

    glm::vec2 mousePosUi = Input()->GetMousePosUI();
    glm::vec2 mousePosWorld = Input()->GetMousePosWorld();

    // iterate from back to front, as the back of the array gets drawn on top
    for ( auto it = m_Sprites.rbegin(); it != m_Sprites.rend(); ++it )
    {
        Sprite* sprite = *it;

        if ( sprite->GetTransform() == nullptr )
        {
            continue;
        }

        if ( sprite->OverlapsLocalPoint( sprite->GetTransform()->GetIsDiegetic() ? mousePosWorld : mousePosUi ) )
        {
            cachedSprite = sprite;
            return cachedSprite;
        }
    }

    cachedSprite = nullptr;
    return cachedSprite;
}


/// @brief  openGL error message callback
/// @param  source      the source of the message
/// @param  type        the type of message
/// @param  id          the id of the message
/// @param  severity    the severity of the message
/// @param  length      the length of the message
/// @param  message     the message
/// @param  userParam   additional user-supplied data
void GLAPIENTRY RenderSystem::errorCallback(
    GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    const GLchar* message,
    const void* userParam
)
{
    // if ( type == GL_DEBUG_TYPE_ERROR )
    // {
        Debug() << "OpenGL Error:\n" <<
            " - source  : " << source <<
            " - type    : " << type <<
            " - id      : " << id <<
            " - severity: " << severity <<
            " - message : " << message << std::endl;
    // }
}


/// @brief   Reallocates the texture for screen buffer.
//           (needs to happen when resizing the screen)
void RenderSystem::reallocScreenBufferTexture()
{
    // delete old
    if (m_ScreenBufferTexID != -1)
        glDeleteTextures(1, &m_ScreenBufferTexID);

    // create new (same as screen size)
    glm::ivec2 dims = Platform()->GetWindowDimensions();

    // (create, bind, allocate)
    glGenTextures(1, &m_ScreenBufferTexID);
    glBindTexture(GL_TEXTURE_2D, m_ScreenBufferTexID);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, dims.x, dims.y);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // attach (or re-attach) it to the FBO
    glBindFramebuffer(GL_FRAMEBUFFER, m_ScreenBufferFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ScreenBufferTexID, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    // (this should also be adjusted when changing screen size)
    glViewport(0, 0, dims.x, dims.y);
}


//-----------------------------------------------------------------------------
// singleton stuff
//-----------------------------------------------------------------------------

RenderSystem::RenderSystem() :
    System( "RenderSystem" )
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
