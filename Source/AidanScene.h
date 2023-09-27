///--------------------------------------------------------------------------//
/// @file   AidanScene.h
/// @brief  A stub file for easy scene creation
/// 
/// @author Aidan Straker (aidan.straker)
/// @date   September 2023
///
/// @copyright © 2023 DigiPen (USA) Corporation.
///--------------------------------------------------------------------------//
#pragma once
#include "System.h"
class AidanScene : public System
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
    virtual void OnUpdate(float dt) override;

    /// @brief Gets called whenever a scene is exited
    virtual void OnSceneExit() override;

private: // unused virtual overrides

    /// @brief Gets called once when this System is added to the Engine
    virtual void OnInit() override {}


    /// @brief Gets called once before the Engine closes
    virtual void OnExit() override {}


//-----------------------------------------------------------------------------
private: // reading
//-----------------------------------------------------------------------------

           /// @brief map of the StubScene read methods
    static ReadMethodMap< AidanScene > const s_ReadMethods;

    /// @brief  gets this System's read methods
    /// @return this System's read methods
    virtual ReadMethodMap< System > const& GetReadMethods() const override;

private: // singleton stuff

    /// @brief Constructs the AidanScene
    AidanScene();


    /// @brief The singleton instance of AidanScene
    static AidanScene* instance;

public: // singleton stuff

    /// @brief gets the instance of AidanScene
    /// @return the instance of the AidanScene
    static AidanScene* GetInstance();

    // Prevent copying
    AidanScene(AidanScene& other) = delete;
    void operator=(const AidanScene&) = delete;
};

