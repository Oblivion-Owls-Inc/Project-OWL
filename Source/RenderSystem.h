/// @file     RenderSystem.h
/// @author   Eli Tsereteli (ilya.tsereteli@digipen.edu)
///
/// @brief    System responsible for drawing sprites and basic shapes.
#pragma once
#include "System.h"
#include "Shader.h"
#include "glm/glm.hpp"
#include <vector>       // for sprite references
#include <map>          // for shader storage

// fwd references
class Sprite;
class Mesh;


/// @brief      Renders all of the Sprite components, keeps track of shaders.
class RenderSystem : public System
{
public:
    /// @brief              Draws a rectangle.
    /// @param position     Position
    /// @param scale        (optional) Scale
    /// @param angle        (optional) Angle
    /// @param color        (optional) Color
    void DrawRect(const glm::vec2& position, const glm::vec2& scale = {100,100}, 
                  float angle = 0.0f, const glm::vec4& color = {0.1,0.6,0.1,1.0});

    /// @brief              Draws a line between 2 points.
    /// @param P1           Point 1
    /// @param P2           Point 2
    /// @param thickness    (optional) How thicc the line is
    /// @param color        (optional) Color of the line
    void DrawLine(const glm::vec2& P1, const glm::vec2& P2, float thickness = 8,
                  const glm::vec4& color = { 0,0.2,0.5,1 });


    /// @brief          Add a sprite so it can be rendered during update.
    /// @param sprite   Sprite pointer to add and keep track of
    void AddSprite( Sprite* sprite );

    /// @brief          Remove sprite from the list to stop rendering it on update.
    /// @param sprite   Sprite pointer to remove
    void RemoveSprite( Sprite* sprite );

    /// @brief          Adds a shader to keep track of, so it can be freed 
    ///                 automatically upon shutdown.
    /// param name      Name to reference shader with
    /// param shader    Pointer to the new shader
    void AddShader(const char* name, Shader* shader);

    /// @brief          Returns pointer to a stored shader
    /// @param name     Name of the shader to return
    /// @return         Pointer to shader
    Shader* GetShader(const char* name);

    /// @brief          Sets shader with given name as active, and returns pointer to it.
    /// @param name     Name of the shader
    /// @return         Shader pointer if the shader is found, nullptr if it isn't.
    Shader* SetActiveShader(const char* name);


    /// @return         Default mesh for simple quad textures
    __inline Mesh const* GetDefaultMesh() const { return m_DefaultMesh; }


    //-------------------------------------------------------------------------
    //          inherited virtuals
    //-------------------------------------------------------------------------
private:

    // Inherited virtuals
    // overrides
    virtual void OnInit() override;
    virtual void OnExit() override;
    virtual void OnUpdate(float dt) override;
    virtual void OnSceneExit() override;

    //-------------------------------------------------------------------------
    //          data
    //-------------------------------------------------------------------------
private:
    std::map<const char*, Shader*> m_Shaders;   /// @brief   Shader storage
    Shader* m_ActiveShader = nullptr;           /// @brief   Currently bound shader
    std::vector<Sprite*> m_Sprites;             /// @brief   Layered sprite references
    Mesh* m_DefaultMesh = nullptr;              /// @brief   Used for 1-frame textures


    //-------------------------------------------------------------------------
    //          helpers
    //-------------------------------------------------------------------------
private:
    /// @brief          Helper method for finding shaders by name
    /// @param name     Name of the shader as a basic c-style string
    /// @return         Pointer to the shader, or nullptr if shader isn't found
    Shader* FindShader(const char* name);



    //-------------------------------------------------------------------------
    //          singleton stuff
    //-------------------------------------------------------------------------
private:
    RenderSystem();
    static RenderSystem* instance;

public:
    static RenderSystem* GetInstance();

    // Prevent copying
    RenderSystem(RenderSystem& other) = delete;
    void operator=(const RenderSystem&) = delete;
};

/// @brief      Convenient function for getting RenderSystem instance.
__inline RenderSystem* Renderer() { return RenderSystem::GetInstance(); }