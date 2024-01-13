/*****************************************************************//**
 * \file   LightingSystem.h
 * \brief  Lighting system interface.
 * 
 * \author Eli Tsereteli
 * \date   November 2023
 *********************************************************************/
#pragma once
#include "System.h"
#include "Sprite.h"
#include "Light.h"

/// @brief   Let there be light.
class LightingSystem : public System
{
public:

    // A sprite that is only used by the lighting system to render all lights.
    class LightingSprite : public Sprite
    {
    public:
        LightingSprite();
        virtual void Draw() override;

        // no copying
        LightingSprite(LightingSprite const& other) = delete;
        virtual Component* Clone() const override { return nullptr; }
    };


//-----------------------------------------------------------------------------
//              Public methods
//-----------------------------------------------------------------------------
public:
    /// @brief        Adds a new light source to keep track of.
    /// @param light  Light component
    /// @return       Index of the added light
    int AddLightSource(Light* light);

    /// @brief        Removes light source from the system.
    /// @param index  Index of the light
    void RemoveLightSource(int index);

    __inline unsigned int GetTexArrayID() const { return m_TextureArrayID; }

    __inline int GetLightCount() const { return (int)m_LightSources.size(); }

//-----------------------------------------------------------------------------
//              Virtual overrides
//-----------------------------------------------------------------------------
private:

    /// @brief  Called when system starts:
    virtual void OnInit() override;

    /// @brief  Called every frame: 
    virtual void OnUpdate(float dt) override;

    /// @brief  Called when system exits
    virtual void OnExit() override;

    /// @brief  Called when entering a scene
    virtual void OnSceneInit() override;

    /// @brief  Called when exiting a scene
    virtual void OnSceneExit() override;

    /// @brief  ImGui
    virtual void DebugWindow() override;


//-----------------------------------------------------------------------------
//              Data
//-----------------------------------------------------------------------------
private:
    LightingSprite* m_Sprite = nullptr;   /// @brief   Does the rendering.
    std::vector<Light*> m_LightSources;   /// @brief   All light sources in scene
    unsigned int m_TextureArrayID = 0;    /// @brief   Texture array for light sources
    GLuint m_FBO = -1;                    /// @brief   Framebuffer for rendering lights to texture 
    bool m_Enabled = true;                /// @brief   For debugging. Can disable all lights.
    glm::mat4 m_S2W = {}, m_W2S = {};     /// @brief   Screen-to-world matrix (and inverse)


//-----------------------------------------------------------------------------
//              Helpers
//-----------------------------------------------------------------------------
private:
    /// @brief  Reallocates texture array - for when the amount of lights 
    ///         or the screen resolution changes.
    void reallocTexArray();

//-----------------------------------------------------------------------------
//              Singleton stuff
//-----------------------------------------------------------------------------
private:

    /// @brief  Private constructor
    LightingSystem();

    /// @brief  The singleton instance of LightingSystem
    static LightingSystem * s_Instance;

public:

    /// @return   The singleton instance
    static LightingSystem * GetInstance();

    // Prevent copying
    LightingSystem(LightingSystem& other) = delete;
    void operator=(const LightingSystem&) = delete;
};


/// @brief      Convenient function for getting LightingSystem instance.
__inline LightingSystem* Lights() { return LightingSystem::GetInstance(); }
