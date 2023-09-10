/// @file     Renderer.h
/// @author   Eli Tsereteli (ilya.tsereteli@digipen.edu)
///
/// @brief    System responsible for drawing sprites and basic shapes.
#pragma once
#include "System.h"
#include "Shader.h"
#include "glm/glm.hpp"

#define _renderer Renderer::getInstance()

class Renderer : public System
{
public:
    void DrawRect(const glm::vec2& position, const glm::vec2& scale = {100,100}, 
                  float angle = 0.0f, const glm::vec4& color = {0.3,0.8,0.3,1.0});

    void DrawLine(const glm::vec2& P1, const glm::vec2& P2, float thickness = 8,
                  const glm::vec4& color = { 0,0,0.2,1 });


    /// @brief      Gets the instance of Renderer
    /// @return     Renderer pointer: new or existing instance of this system
    static Renderer* getInstance();

    // Prevent copying
    Renderer(Renderer& other) = delete;
    void operator=(const Renderer&) = delete;


private:
    // ============================= DATA ============================== //

    Shader *_colorShader;       /// @brief      Simple color shader
    Shader* _textureShader;     /// @brief      Simple texture shader
    static Renderer* instance;  /// @brief      The singleton instance of Renderer 
    
    // ================================================================= //


    /// @brief      Private constructor - only used by getInstance()
    Renderer();

    // Inherited virtuals
    virtual void OnInit() override;
    virtual void OnExit() override;
    virtual void OnUpdate(float dt) override;

    // Unused virtuals
    virtual void OnFixedUpdate() override {}
    virtual void OnSceneLoad() override {}
    virtual void OnSceneInit() override {}
    virtual void OnSceneExit() override {}
    virtual void Load( rapidjson::Value const& ) override {}
};

