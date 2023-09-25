/// @file XinoScene.h
/// @version 0.1
/// @date 2023-09-05
/// 
/// @copyright Copyright (c) 2023

#pragma once

#include "System.h"

/// @brief Sandbox system meant to mimic CS-230 scenes
class XinoScene : public System
{
private: // virtual override methods

    /// @brief Gets called whenever a new Scene is loaded
    virtual void OnSceneLoad() override;


    /// @brief Gets called whenever a scene is initialized
    virtual void OnSceneInit() override;

    /// @brief Gets called once every simulation frame. Use this function for anything that affects the simulation.
    virtual void OnFixedUpdate() override;

    /// @brief Gets called once every graphics frame. Do not use this function for anything that affects the simulation.
    /// @param dt the elapsed time in seconds since the previous frame
    virtual void OnUpdate( float dt ) override;

    /// @brief Gets called whenever a scene is exited
    virtual void OnSceneExit() override;

//-----------------------------------------------------------------------------
private: // reading
//-----------------------------------------------------------------------------

    /// @brief map of the XinoScene read methods
    static ReadMethodMap< XinoScene > const s_ReadMethods;

    /// @brief  gets this System's read methods
    /// @return this System's read methods
    virtual ReadMethodMap< System > const& GetReadMethods() const override;

private: // singleton stuff

    /// @brief Constructs the XinoScene
    XinoScene();


    /// @brief The singleton instance of XinoScene
    static XinoScene * instance;

public: // singleton stuff

    /// @brief gets the instance of XinoScene
    /// @return the instance of the XinoScene
    static XinoScene * GetInstance();

    // Prevent copying
    XinoScene(XinoScene& other) = delete;
    void operator=(const XinoScene&) = delete;
};

