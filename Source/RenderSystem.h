/// @file     RenderSystem.h
/// @author   Eli Tsereteli (ilya.tsereteli@digipen.edu)
///
/// @brief    System responsible for drawing sprites and basic shapes.
#pragma once
#include "System.h"
#include "Shader.h"
#include "glm/glm.hpp"  // vec4 for color, mat4 for transform and scree2clip
#include <set>          // for sprite references

class Sprite;   // fwd reference

class RenderSystem : public System
{
public:
    /// @brief              Draws a rectangle.
    /// @param position     Position
    /// @param scale        (optional) Scale
    /// @param angle        (optional) Angle
    /// @param color        (optional) Color
    void DrawRect(const glm::vec2& position, const glm::vec2& scale = {100,100}, 
                  float angle = 0.0f, const glm::vec4& color = {0.3,0.8,0.3,1.0});

    /// @brief              Draws a line between 2 points.
    /// @param P1           Point 1
    /// @param P2           Point 2
    /// @param thickness    (optional) How thicc the line is
    /// @param color        (optional) Color of the line
    void DrawLine(const glm::vec2& P1, const glm::vec2& P2, float thickness = 8,
                  const glm::vec4& color = { 0,0,0.2,1 });

    /// @brief      Switch to color shader.
    void ColorMode();
    /// @brief      Switch to texture shader.
    void TextureMode();
    /// @brief      Set color for the color shader
    void SetColor(glm::vec4 const& color);
    /// @brief      Set UV offset for the texture shader
    void SetUV(float u, float v);
    /// @brief      Set transformation matrix for currently active shader
    void SetTransformMat(glm::mat4 const& mat) const;

    /// @brief          Add sprite so it can be rendered during update. To be used by Sprite constructor.
    /// @param sprite   Sprite pointer to add and keep track of
    void AddSprite(Sprite* sprite);
    
    /// @brief          Remove sprite from the list to stop rendering it on update. To be used by Sprite destructor.
    /// @param sprite   Sprite pointer to remove
    void RemoveSprite(Sprite* sprite);


    /// @brief      Gets the instance of RenderSystem
    /// @return     RenderSystem pointer: new or existing instance of this system
    static RenderSystem* getInstance();

    // Prevent copying
    RenderSystem(RenderSystem& other) = delete;
    void operator=(const RenderSystem&) = delete;


private:
    // ============================= DATA ============================== //

    Shader* _colorShader = nullptr;  /// @brief      Simple color shader
    Shader* _textureShader = nullptr;/// @brief      Simple texture shader
    Shader* _activeShader = nullptr; /// @brief      Keep track of currently bound shader
    std::set<Sprite*> _sprites;      /// @brief      Sprite references
    glm::mat4 screen2clip = {};      /// @brief      Screen space to clip space projection matrix
    static RenderSystem* instance;   /// @brief      The singleton instance of RenderSystem
    
    // ================================================================= //


    /// @brief      Private constructor - only used by getInstance()
    RenderSystem();

    // Inherited virtuals
    virtual void OnInit() override;
    virtual void OnExit() override;
    virtual void OnUpdate(float dt) override;

    // Unused virtuals
    virtual void OnFixedUpdate() override {}
    virtual void OnSceneLoad() override {}
    virtual void OnSceneInit() override {}
    virtual void OnSceneExit() override {}
    virtual void Load( rapidjson::Value const& configData ) override {}
};

__inline RenderSystem* Renderer() { return RenderSystem::getInstance(); }