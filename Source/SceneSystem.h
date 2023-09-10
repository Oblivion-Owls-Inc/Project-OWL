/// @file SceneSystem.h
/// @author Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief handles the process of resetting and changing scenes
/// @version 0.1
/// @date 2023-09-08
/// 
/// @copyright Copyright (c) 2023

#pragma once

#include "System.h"

#include "Engine.h"
#include <string>

/// @brief Example System meant to be copy-pasted when creating new Systems
class SceneSystem : public System
{
private:

    /// @brief The name of the current Scene
    std::string currentSceneName;

    /// @brief The name of the next Scene
    std::string nextSceneName;


    /// @brief sets the next Scene to change to
    /// @param nextSceneName the name of the next scene
    void SetNextScene( std::string nextSceneName );

    /// @brief gets the name of the current scene
    /// @return the name of the current scene
    std::string const& getSceneName() const;


    /// @brief Loads the next Scene
    void LoadScene();

    /// @brief Initializes the current Scene
    void InitScene();

    /// @brief Exits the current Scene
    void ExitScene();

    /// @brief Gets called once when this System is added to the Engine
    virtual void OnInit() override;

    /// @brief Gets called once every simulation frame. Use this function for anything that affects the simulation.    
    virtual void OnFixedUpdate() override;

    /// @brief Gets called once before the Engine closes
    virtual void OnExit() override;


    // unused virtual methods
    virtual void OnUpdate( float dt ) override {}
    virtual void OnSceneLoad() override {}
    virtual void OnSceneInit() override {}
    virtual void OnSceneExit() override {}
    virtual void Load( rapidjson::Value const& ) override {}


    /// @brief Constructs the SceneSystem
    SceneSystem( std::string const& initialSceneName = "" );

    /// @brief The singleton instance of SceneSystem
    static SceneSystem * instance;

public:

    /// @brief gets the instance of SceneSystem
    /// @return the instance of the SceneSystem
    static SceneSystem * getInstance( std::string const& initialSceneName = "" );

    // Prevent copying
    SceneSystem( SceneSystem& other ) = delete;
    void operator=( const SceneSystem& ) = delete;
};

