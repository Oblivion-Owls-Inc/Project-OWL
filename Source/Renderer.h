/// @file     Renderer.h
/// @author   Eli Tsereteli (ilya.tsereteli@digipen.edu)
///
/// @brief    Example System meant to be copy-pasted when creating new Systems
#pragma once
#include "System.h"


class Renderer : public System
{
private:
    class Shader
    {
        unsigned int _shaderID;
        unsigned int u_transform;

    public:
        Shader(const char* vertex_filepath, const char* fragment_filepath);
        ~Shader();
    };

    // ------------------------------ DATA ------------------------------ //

    /// @brief      Simple color shader
    Shader *_colorShader;

    /// @brief      The singleton instance of Renderer  
    static Renderer* instance;


    // ----------------------------- METHODS ----------------------------//

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



public:

    /// @brief      Gets the instance of Renderer
    /// @return     Renderer pointer: new or existing instance of this system
    static Renderer* getInstance();

    // Prevent copying
    Renderer(Renderer& other) = delete;
    void operator=(const Renderer&) = delete;
};

