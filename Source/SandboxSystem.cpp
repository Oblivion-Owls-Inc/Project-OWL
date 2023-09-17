/// @file SandboxSystem.cpp
/// @author Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief Sandbox system meant to mimic CS-230 scenes
/// @version 0.1
/// @date 2023-09-05
/// 
/// @copyright Copyright (c) 2023

#include "SandboxSystem.h"

#include "Entity.h"
#include "AudioPlayer.h"
#include "InputSystem.h"

//-----------------------------------------------------------------------------
// variables
//-----------------------------------------------------------------------------

    static Entity* entity;

    static Sound* sound;

//-----------------------------------------------------------------------------
// virtual override methods
//-----------------------------------------------------------------------------

    /// @brief Gets called whenever a new Scene is loaded
    void SandboxSystem::OnSceneLoad()
    {
        std::cout << "test" << std::endl;
        sound = new Sound( "Data/Sounds/test.mp3", false );
    }

    /// @brief Gets called whenever a scene is initialized
    void SandboxSystem::OnSceneInit()
    {
        entity = new Entity();
        entity->Add(new AudioPlayer());
        entity->GetComponent<AudioPlayer>()->setSound(sound);
    }

    /// @brief Gets called once every simulation frame. Use this function for anything that affects the simulation.
    void SandboxSystem::OnFixedUpdate()
    {
        if ( Input()->getKeyTriggered( GLFW_KEY_SPACE ) )
        {
            entity->GetComponent<AudioPlayer>()->Play();
        }
    }

    /// @brief Gets called once every graphics frame. Do not use this function for anything that affects the simulation.
    /// @param dt the elapsed time in seconds since the previous frame
    void SandboxSystem::OnUpdate( float dt )
    {

    }

    /// @brief Gets called whenever a scene is exited
    void SandboxSystem::OnSceneExit()
    {
        delete entity;
        delete sound;
    }

//-----------------------------------------------------------------------------
// singleton stuff
//-----------------------------------------------------------------------------

/// @brief Constructs the SandboxSystem
SandboxSystem::SandboxSystem() {}

/// @brief The singleton instance of SandboxSystem
SandboxSystem * SandboxSystem::instance = nullptr;

/// @brief gets the instance of SandboxSystem
/// @return the instance of the SandboxSystem
SandboxSystem * SandboxSystem::getInstance()
{
    if ( instance == nullptr )
    {
        instance = new SandboxSystem();
    }
    return instance;
}

//-----------------------------------------------------------------------------
