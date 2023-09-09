/// @file     Renderer.h
/// @author   Eli Tsereteli (ilya.tsereteli@digipen.edu)
///
/// @brief    Example System meant to be copy-pasted when creating new Systems
#pragma once
#include "System.h"
#include "Shader.h"

// Renderer will compile and store the shaders, and allow to set them as active.
// Managing uniforms can be done anywhere.


class Renderer : public System
{
public:
    


    // Sets active shader and returns its ID
    Shader* GetShader();   // TODO: accepts const char* (shaders stored in a map)

    /// @brief      Gets the instance of Renderer
    /// @return     Renderer pointer: new or existing instance of this system
    static Renderer* getInstance();

    // Prevent copying
    Renderer(Renderer& other) = delete;
    void operator=(const Renderer&) = delete;

private:
    // ----------------------------- DATA ------------------------------ //

    // TODO: map instead of single shader
    Shader *_colorShader;       /// @brief      Simple color shader
    static Renderer* instance;  /// @brief      The singleton instance of Renderer 
    
    // ----------------------------------------------------------------- //


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
};

