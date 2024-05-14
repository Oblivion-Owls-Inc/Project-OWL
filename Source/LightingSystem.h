/*****************************************************************//**
 * \file       LightingSystem.h
 * \brief      Lighting system interface.
 * 
 * \author     Eli Tsereteli (ilya.tsereteli@digipen.edu)
 * \date       Jan 2024
 * \copyright  Copyright (c) 2024 Digipen Instutute of Technology
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
        virtual LightingSprite* Clone() const override { return nullptr; }
    };


//-----------------------------------------------------------------------------
//              Public methods
//-----------------------------------------------------------------------------
public:

    /// @return     Enabled state of the lighting system
    __inline bool GetLightingEnabled() const { return m_Enabled; }

    /// @brief      Sets the enabled state to provided boolean
    __inline void SetLightingEnabled(bool enabled)
    {
        m_Enabled = enabled;
        m_Sprite->SetOpacity( m_Enabled ? 1.0f : 0.0f );
    }

    /// @brief      Sets current shadow layer to given integer
    __inline void SetShadowLayer(int layer) { m_Sprite->SetLayer(layer); }

    void DrawLights() const;


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
    bool m_Enabled = false;               /// @brief   For debugging. Can disable all lights.
    glm::mat4 m_S2W = {}, m_W2S = {};     /// @brief   Screen-to-world matrix (and inverse)
    unsigned int m_UBOpos = -1, 
                 m_UBOrad = -1, 
                 m_UBOstr = -1;
    

//-----------------------------------------------------------------------------
//              Helpers
//-----------------------------------------------------------------------------
//private:


//-----------------------------------------------------------------------------
//              Reading / Writing
//-----------------------------------------------------------------------------
private:

    /// @brief       Reads whether lighting system should be enabled on launch
    /// @param data  json data
    void readEnabled(nlohmann::ordered_json const& data);

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

public:

    /// @return   The singleton instance
    static LightingSystem * GetInstance();

    // Prevent copying
    LightingSystem(LightingSystem& other) = delete;
    void operator=(const LightingSystem&) = delete;
};


/// @brief      Convenient function for getting LightingSystem instance.
__inline LightingSystem* Lights() { return LightingSystem::GetInstance(); }
