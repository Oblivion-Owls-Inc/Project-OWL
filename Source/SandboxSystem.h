/// @file     SandboxSystem.h
/// @author   Anyone
///
/// @brief    Sandbox "scene" for experimenting
#pragma once
#include "System.h"


class SandboxSystem : public System
{
private:
    bool _active = false;

    /// @brief      Calls SceneInit or SceneUpdate when current scene is Sandbox
    virtual void OnUpdate(float dt) override;
    /// @brief      Calls static version if active
    virtual void OnFixedUpdate() override;
    /// @brief      Calls SceneExit if active
    virtual void OnExit() override;

    /// @brief    Private constructor for singleton-ing
    SandboxSystem() {}
    static SandboxSystem* instance;  /// @brief    Singleton instance of SandboxSystem  

    // unused virtuals
    virtual void OnInit() override {}
    virtual void OnSceneLoad() override {}
    virtual void OnSceneInit() override {}
    virtual void OnSceneExit() override {}
    virtual void Load(rapidjson::Value const& configData) override {}


public:

    /// @brief      Gets the instance of SandboxSystem
    /// @return     SandboxSystem pointer: new or existing instance of this system
    static SandboxSystem* getInstance();

    // Prevent copying
    SandboxSystem(SandboxSystem& other) = delete;
    void operator=(const SandboxSystem&) = delete;
};

