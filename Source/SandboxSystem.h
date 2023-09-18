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


    /// @brief the Read Methods used in this System
    static std::map< std::string, ReadMethod< SandboxSystem > > const ReadMethods;

    /// @brief Gets the read methods of this System
    /// @return the map of read methods of this System
    virtual std::map< std::string, ReadMethod< System > > const& GetReadMethods() override;

public:

    /// @brief      Gets the instance of SandboxSystem
    /// @return     SandboxSystem pointer: new or existing instance of this system
    static SandboxSystem* getInstance();

    // Prevent copying
    SandboxSystem(SandboxSystem& other) = delete;
    void operator=(const SandboxSystem&) = delete;
};

