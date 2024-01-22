/*****************************************************************//**
 * \file   LightingSystem.h
 * \brief  Lighting system interface.
 * 
 * \author Eli Tsereteli
 * \date   Jan 2024
 *********************************************************************/
#pragma once
#include "Entity.h"
#include "ComponentSystem.h"
#include "Sprite.h"
#include "Light.h"

/// @brief   Let there be light.
class LightingSystem : public ComponentSystem<Light>
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

    __inline unsigned int GetTexArrayID() const { return m_TextureArrayID; }

    __inline bool GetLightingEnabled() const { return m_Enabled; }


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
    unsigned int m_TextureArrayID = 0;    /// @brief   Texture array for light sources
    GLuint m_FBO = -1;                    /// @brief   Framebuffer for rendering lights to texture 
    bool m_Enabled = true;                /// @brief   For debugging. Can disable all lights.
    glm::mat4 m_S2W = {}, m_W2S = {};     /// @brief   Screen-to-world matrix (and inverse)
    int m_ShadowLayer = 5;                /// @brief   Rendering layer of the darkness
    unsigned long long m_PrevSize = 0;


//-----------------------------------------------------------------------------
//              Helpers
//-----------------------------------------------------------------------------
private:
    /// @brief  Reallocates texture array - for when the amount of lights 
    ///         or the screen resolution changes.
    void reallocTexArray();



//-----------------------------------------------------------------------------
//              Reading / Writing
//-----------------------------------------------------------------------------
private:

    /// @brief       Reads whether lighting system should be enabled on launch
    /// @param data  json data
    void readEnabled(nlohmann::ordered_json const& data);

    /// @brief       Reads the rendering layer of shadows
    /// @param data  json data
    void readLayer(nlohmann::ordered_json const& data);

    /// @brief map of the PlatformSystem read methods
    static ReadMethodMap< LightingSystem > const s_ReadMethods;


public:

    /// @brief   gets this System's read methods
    /// @return  this System's read methods
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override
    {
        return (ReadMethodMap< ISerializable > const&)s_ReadMethods;
    }


    /// @brief   writes this System config
    /// @return  the writting System config
    virtual nlohmann::ordered_json Write() const override;



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
