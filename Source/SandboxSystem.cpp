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
#include "BehaviorSystem.h"
#include "RenderSystem.h"
#include "Sprite.h"
#include "MovementAI.h"
#include "InputSystem.h"
#include "DebugSystem.h"

//-----------------------------------------------------------------------------
// variables
//-----------------------------------------------------------------------------

    static Entity* entity;
    static Entity* entity2;
    static bool update = false;
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
        update = true;
        entity = new Entity();
        entity->Add(new AudioPlayer());
        entity->GetComponent<AudioPlayer>()->setSound(sound);

        entity2= new Entity();
        //entity2->Add( new Sprite("Elementals_leaf_ranger_288x128_SpriteSheet.png", 22,17));
        entity2->Add( new RigidBody());
        entity2->Add( new Transform());
        entity2->Add( new MovementAI());
        float spriteSize = entity2->GetComponent<Sprite>()->GetHeightMultiplier();
        entity2->GetComponent<Transform>()->setTranslation(glm::vec3(300.0f, 300.0f, 0.0f));
        entity2->GetComponent<Transform>()->setScale(glm::vec3(600.0f, (-600.0f * spriteSize), 0.0f));
    }

    /// @brief Gets called once every simulation frame. Use this function for anything that affects the simulation.
    void SandboxSystem::OnFixedUpdate()
    {
        if (!update)
            return;
        if ( Input()->getKeyTriggered( GLFW_KEY_SPACE ) )
        {
            entity->GetComponent<AudioPlayer>()->Play();
        }
        // Create an instance of DebugConsole
        DebugConsole output(*DebugSystem::getInstance());

        // Append the message and the formatted value
        glm::vec3 translation = *entity2->GetComponent<Transform>()->getTranslation();
        output << "Position: (" << translation.x << ", " << translation.y << ", " << translation.z << ")" << "\n";
    }

    /// @brief Gets called once every graphics frame. Do not use this function for anything that affects the simulation.
    /// @param dt the elapsed time in seconds since the previous frame
    void SandboxSystem::OnUpdate( float dt )
    {
        if (!update)
            return;
    }

    /// @brief Gets called whenever a scene is exited
    void SandboxSystem::OnSceneExit()
    {
        delete entity;
        delete sound;
        delete entity2;
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
