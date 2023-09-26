/// @file XinoScene.cpp
/// @version 0.1
/// @date 2023-09-05
/// 
/// @copyright Copyright (c) 2023

#include "glm/glm.hpp"
#include "XinoScene.h"

#include "Entity.h"
#include "AudioPlayer.h"
#include "BehaviorSystem.h"
#include "RenderSystem.h"
#include "Sprite.h"
#include "CircleCollider.h"
#include "LineCollider.h"
#include "MovementAI.h"
#include "InputSystem.h"
#include "DebugSystem.h"

//-----------------------------------------------------------------------------
// variables
//-----------------------------------------------------------------------------

    static Entity* entity;
    static Entity* Wall;
    static Entity* entity2;
    static Entity* entity3;
    static Entity* entity4;
    static Entity* entity5;
    static Entity* entity6;
    static bool update = false;
    static Sound* sound;

//-----------------------------------------------------------------------------
// virtual override methods
//-----------------------------------------------------------------------------

    /// @brief Gets called whenever a new Scene is loaded
    void XinoScene::OnSceneLoad()
    {
        std::cout << "test" << std::endl;
        sound = new Sound( "Data/Sounds/test.mp3", false );

    }

    /// @brief Gets called whenever a scene is initialized
    void XinoScene::OnSceneInit()
    {
        /// Audio Player
        update = true;
        entity = new Entity();
        entity->Add(new AudioPlayer());
        entity->GetComponent<AudioPlayer>()->setSound(sound);

        /// Ball 1
        entity2= new Entity();
        entity2->SetName("Ball1");
        //entity2->Add( new Sprite("Temp_Assets/Balls/Ball5.png", 1,1, 1));
        entity2->Add( new RigidBody()); 
        entity2->Add(new CircleCollider());
        entity2->Add( new Transform());
        entity2->Add( new MovementAI());
        CircleCollider * collider2 = (CircleCollider*)entity2->GetComponent<Collider>();
        float spriteSize = entity2->GetComponent<Sprite>()->GetHeightMultiplier();
        entity2->GetComponent<Transform>()->setTranslation(glm::vec3(0.0f, -3.0f, 0.0f));
        entity2->GetComponent<Transform>()->setScale(glm::vec3(1.0f, spriteSize, 0.0f));
        collider2->SetRadius(48.00 / 2); /// 48 is the width of the sprite

        /// Ball 2
        entity3 = new Entity();
        entity3->SetName("Ball2");
        //entity3->Add(new Sprite("Temp_Assets/Balls/Ball2.png", 1, 1, 1));
        entity3->Add(new RigidBody());
        entity3->Add(new CircleCollider());
        entity3->Add(new Transform());
        entity3->Add(new MovementAI());
        CircleCollider* collider3 = (CircleCollider*)entity3->GetComponent<Collider>();
        spriteSize = entity2->GetComponent<Sprite>()->GetHeightMultiplier();
        entity3->GetComponent<Transform>()->setTranslation(glm::vec3(500.0f, 300.0f, 0.0f));
        entity3->GetComponent<Transform>()->setScale(glm::vec3(100.0f, (-100.0f * spriteSize), 0.0f));
        collider3->SetRadius(48.00 / 2); /// 48 is the width of the sprite

        ///Ball3
        entity4 = new Entity();
        entity4->SetName("Ball3");
        //entity4->Add(new Sprite("Temp_Assets/Balls/Ball.png", 1, 1, 1));
        entity4->Add(new RigidBody());
        entity4->Add(new CircleCollider());
        entity4->Add(new Transform());
        entity4->Add(new MovementAI());
        CircleCollider* collider4 = (CircleCollider*)entity4->GetComponent<Collider>();
        spriteSize = entity2->GetComponent<Sprite>()->GetHeightMultiplier();
        entity4->GetComponent<Transform>()->setTranslation(glm::vec3(700.0f, 300.0f, 0.0f));
        entity4->GetComponent<Transform>()->setScale(glm::vec3(100.0f, (-100.0f * spriteSize), 0.0f));
        collider4->SetRadius(48.00 / 2); /// 48 is the width of the sprite
    }

    /// @brief Gets called once every simulation frame. Use this function for anything that affects the simulation.
    void XinoScene::OnFixedUpdate()
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
        //output << "Position: (" << translation.x << ", " << translation.y << ", " << translation.z << ")" << "\n";
    }

    /// @brief Gets called once every graphics frame. Do not use this function for anything that affects the simulation.
    /// @param dt the elapsed time in seconds since the previous frame
    void XinoScene::OnUpdate( float dt )
    {
        if (!update)
            return;
    }

    /// @brief Gets called whenever a scene is exited
    void XinoScene::OnSceneExit()
    {
        delete entity;
        delete sound;
        delete entity2;
        delete entity3;
        delete entity4;
        delete Wall;
        //delete entity5;
        //delete entity6;
    }

//-----------------------------------------------------------------------------
// singleton stuff
//-----------------------------------------------------------------------------

/// @brief Constructs the XinoScene
XinoScene::XinoScene() {}

/// @brief The singleton instance of XinoScene
XinoScene * XinoScene::instance = nullptr;

/// @brief gets the instance of XinoScene
/// @return the instance of the XinoScene
XinoScene * XinoScene::getInstance()
{
    if ( instance == nullptr )
    {
        instance = new XinoScene();
    }
    return instance;
}

//-----------------------------------------------------------------------------
