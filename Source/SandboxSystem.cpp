/// @file       SandboxSystem.cpp
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      Sandbox system meant to mimic CS-230 scenes
/// @version    0.1
/// @date       2023-09-05
/// 
/// @copyright  Copyright (c) 2023 Digipen Institute of Technology

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

    /// @brief  Gets called whenever a new Scene is loaded
    void SandboxSystem::OnSceneLoad()
    {
        sound = new Sound( "Data/Sounds/test.mp3", false );

    }

    /// @brief  Gets called whenever a scene is initialized
    void SandboxSystem::OnSceneInit()
    {
        update = true;
        entity = new Entity();
        entity->Add(new AudioPlayer());
        entity->GetComponent<AudioPlayer>()->SetSound(sound);

        entity2= new Entity();
        entity2->Add( new Sprite("Elementals_leaf_ranger_288x128_SpriteSheet.png", 22,17));
        entity2->Add( new RigidBody());
        entity2->Add( new Transform());
        entity2->Add( new MovementAI());
        float spriteSize = entity2->GetComponent<Sprite>()->GetHeightMultiplier();
        entity2->GetComponent<Transform>()->SetTranslation(glm::vec3(300.0f, 300.0f, 0.0f));
        entity2->GetComponent<Transform>()->SetScale(glm::vec3(600.0f, (-600.0f * spriteSize), 0.0f));
    }

    /// @brief  Gets called once every simulation frame. Use this function for anything that affects the simulation.
    void SandboxSystem::OnFixedUpdate()
    {
        if (!update)
            return;
        if ( Input()->GetKeyTriggered( GLFW_KEY_SPACE ) )
        {
            entity->GetComponent<AudioPlayer>()->Play();
        }
        // Create an instance of DebugConsole
        DebugConsole output(*DebugSystem::GetInstance());

        // Append the message and the formatted value
        glm::vec3 translation = *entity2->GetComponent<Transform>()->GetTranslation();
        output << "Position: (" << translation.x << ", " << translation.y << ", " << translation.z << ")" << "\n";
    }

    /// @brief  Gets called once every graphics frame. Do not use this function for anything that affects the simulation.
    /// @param  dt  the elapsed time in seconds since the previous frame
    void SandboxSystem::OnUpdate( float dt )
    {
        if (!update)
            return;
    }

    /// @brief  Gets called whenever a scene is exited
    void SandboxSystem::OnSceneExit()
    {
        delete entity;
        delete sound;
        delete entity2;
    }

//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------

    /// @brief  map of the SandboxSystem read methods
    ReadMethodMap< SandboxSystem > const SandboxSystem::s_ReadMethods = {};

    /// @brief  gets this System's read methods
    /// @return this System's read methods
    ReadMethodMap< System > const& SandboxSystem::GetReadMethods() const
    {
        return (ReadMethodMap< System > const&)s_ReadMethods;
    }

//-----------------------------------------------------------------------------
// singleton stuff
//-----------------------------------------------------------------------------

    /// @brief  Constructs the SandboxSystem
    SandboxSystem::SandboxSystem() {}

    /// @brief  The singleton instance of SandboxSystem
    SandboxSystem * SandboxSystem::s_Instance = nullptr;

    /// @brief  gets the instance of SandboxSystem
    /// @return the instance of the SandboxSystem
    SandboxSystem * SandboxSystem::GetInstance()
    {
        if ( s_Instance == nullptr )
        {
            s_Instance = new SandboxSystem();
        }
        return s_Instance;
    }

//-----------------------------------------------------------------------------
