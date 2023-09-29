/// @file XinoScene.cpp
/// @version 0.1
/// @date 2023-09-05
/// 
/// @copyright Copyright (c) 2023

#include "glm/glm.hpp"
#include "XinoScene.h"

#include "EntitySystem.h"
#include "AudioPlayer.h"
#include "BehaviorSystem.h"
#include "RenderSystem.h"
#include "TextSprite.h"
#include "CircleCollider.h"
#include "LineCollider.h"
#include "MovementAI.h"
#include "InputSystem.h"
#include "DebugSystem.h"
#include "PlayerController.h"
#include "Texture.h"
#include "Animation.h"

static void drawText(float dt);

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
static vec3 velocity = { 1.0f, 1.0f, 0.0f };

//-----------------------------------------------------------------------------
// virtual override methods
//-----------------------------------------------------------------------------

/// @brief Gets called whenever a new Scene is loaded
void XinoScene::OnSceneLoad()
{
    sound = new Sound( "Data/Sounds/test.mp3", false );
}

/// @brief Gets called whenever a scene is initialized
void XinoScene::OnSceneInit()
{
    /// Audio Player
    update = true;
    entity = new Entity();
    entity->Add(new AudioPlayer());
    entity->GetComponent<AudioPlayer>()->SetSound(sound);

        /// Bottom Wall
        Wall = new Entity();
        Wall->SetName("Wall");
        Wall->Add(new Sprite("Temp_Assets/Wall.png", 1, 1, 0));
        Wall->Add(new Transform());
        Wall->Add(new LineCollider());
        LineCollider* collider = (LineCollider*)Wall->GetComponent<Collider>();
        collider ->AddLineSegment(glm::vec2(0.0f, 0.0f), glm::vec2(800.0f, 0.0f));
        Wall->GetComponent<Transform>()->SetTranslation(glm::vec3(400.0f, 500.0f, 0.0f));
        Wall->GetComponent<Transform>()->SetScale(glm::vec3(800.0f, 50.0f, 0.0f));
        
    /// Ball 1
    entity2= new Entity();
    entity2->SetName("Ball1");
    entity2->Add( new Sprite("Temp_Assets/Balls/Ball5.png", 1,1, 1));
    entity2->Add( new RigidBody()); 
    entity2->Add(new CircleCollider());
    entity2->Add( new Transform());
    entity2->Add( new MovementAI());
    CircleCollider * collider2 = (CircleCollider*)entity2->GetComponent<Collider>();
    float spriteSize = entity2->GetComponent<Sprite>()->GetHeightMultiplier();
    entity2->GetComponent<Transform>()->SetTranslation(glm::vec3(0.0f, 0.0f, 0.0f));
    entity2->GetComponent<Transform>()->SetScale(glm::vec3(1.0f, spriteSize, 0.0f));
    entity2->GetComponent<RigidBody>()->SetVelocity(&velocity);
    collider2->SetRadius(spriteSize / 2); /// 48 is the width of the sprite

    /// Ball 2
    entity3 = new Entity();
    entity3->SetName("Ball2");
    entity3->Add(new Sprite("Temp_Assets/Balls/Ball2.png", 1, 1, 1));
    entity3->Add(new RigidBody());
    entity3->Add(new CircleCollider());
    entity3->Add(new Transform());
    entity3->Add(new MovementAI());
    CircleCollider* collider3 = (CircleCollider*)entity3->GetComponent<Collider>();
    spriteSize = entity2->GetComponent<Sprite>()->GetHeightMultiplier();
    entity3->GetComponent<Transform>()->SetTranslation(glm::vec3(2.0f, 0.0f, 0.0f));
    entity3->GetComponent<Transform>()->SetScale(glm::vec3(1.0f, (spriteSize), 0.0f));
    collider3->SetRadius(spriteSize / 2); /// 48 is the width of the sprite

    ///Ball3
    entity4 = new Entity();
    entity4->SetName("Ball3");
    entity4->Add(new Sprite("Temp_Assets/Balls/Ball.png", 1, 1, 1));
    entity4->Add(new RigidBody());
    entity4->Add(new CircleCollider());
    entity4->Add(new Transform());
    entity4->Add(new MovementAI());
    CircleCollider* collider4 = (CircleCollider*)entity4->GetComponent<Collider>();
    spriteSize = entity2->GetComponent<Sprite>()->GetHeightMultiplier();
    entity4->GetComponent<Transform>()->SetTranslation(glm::vec3(4.0f, 0.0f, 0.0f));
    entity4->GetComponent<Transform>()->SetScale(glm::vec3(1.0f, (spriteSize), 0.0f));
    collider4->SetRadius(spriteSize / 2); /// 48 is the width of the sprite
}

/// @brief Gets called once every simulation frame. Use this function for anything that affects the simulation.
void XinoScene::OnFixedUpdate()
{
    if (!update)
        return;
    if ( Input()->GetKeyTriggered( GLFW_KEY_SPACE ) )
    {
        entity->GetComponent<AudioPlayer>()->Play();
    }
    // Create an instance of DebugConsole
    DebugConsole output( *DebugSystem::GetInstance() );

    // Append the message and the formatted value
    glm::vec3 translation = *entity2->GetComponent<Transform>()->GetTranslation();
    //output << "Position: (" << translation.x << ", " << translation.y << ", " << translation.z << ")" << "\n";
}

/// @brief Gets called once every graphics frame. Do not use this function for anything that affects the simulation.
/// @param dt the elapsed time in seconds since the previous frame
void XinoScene::OnUpdate( float dt )
{
    if (!update)
        return;

    drawText(dt);
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


static void drawText(float dt)
{
    static Entity textEnt, tileEnt;
    static char text_buffer[64] = {};
    static char tile_buffer[21] = {};
    static float time = 0.0f;  // for color change
    static bool textMode = true;
    static Texture tiles("temp_Assets/Tileset_16x8.png");
    time += dt;

    // init
    if (!textEnt.GetComponent<Sprite>())
    {
        textEnt.Add(new TextSprite("temp_Assets/Roboto_Mono_white_16x8.png", 16, 8, 0.6f));
        textEnt.Add(new Transform);
        textEnt.GetComponent<Transform>()->SetTranslation({-4.9, 2, 0});
        textEnt.GetComponent<Transform>()->SetScale({0.35, 0.35, 0});
        strcpy_s(text_buffer, 63, "The Quick Brown Fox Jumps Over The Lazy Dog");

        tileEnt.Add(new TextSprite("temp_Assets/Roboto_Mono_white_16x8.png", 16, 8));
        tileEnt.Add(new Transform);
        tileEnt.GetComponent<Transform>()->SetTranslation({-4, 1, 0});
        tileEnt.GetComponent<Transform>()->SetScale({0.6, 0.6, 0});
        ((TextSprite*) tileEnt.GetComponent<Sprite>())->SetRowWidth(5);
    }

    ImGui::Begin("Text Input");
    ImGui::InputText("Tiles", tile_buffer, 21);
    ImGui::InputText("Actuial text", text_buffer, 63);
    if (ImGui::Button(textMode ? "Text Mode" : "Tile Mode"))
    {
        textMode = !textMode;
        ((TextSprite*) tileEnt.GetComponent<Sprite>())->SetTexture(textMode ? nullptr : &tiles);
    }
    ImGui::End();
    ((TextSprite*) textEnt.GetComponent<Sprite>())->Text() << text_buffer;
    ((TextSprite*) textEnt.GetComponent<Sprite>())->SetColor({glm::abs(glm::sin(time*2)),glm::abs(glm::cos(time)),0,1});
    ((TextSprite*) tileEnt.GetComponent<Sprite>())->Text() << tile_buffer;
}


//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------

    /// @brief map of the XinoScene read methods
    ReadMethodMap< XinoScene > const XinoScene::s_ReadMethods = {};

    /// @brief  gets this System's read methods
    /// @return this System's read methods
    ReadMethodMap< System > const& XinoScene::GetReadMethods() const
    {
        return (ReadMethodMap< System > const&)s_ReadMethods;
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
XinoScene * XinoScene::GetInstance()
{
    if ( instance == nullptr )
    {
        instance = new XinoScene();
    }
    return instance;
}

//-----------------------------------------------------------------------------
