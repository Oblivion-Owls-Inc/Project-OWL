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
#include "CameraSystem.h"

static void drawText(float dt);
static void planets(float dt);
static void moveCamera();

//-----------------------------------------------------------------------------
// variables
//-----------------------------------------------------------------------------

static Entity* entity;
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
    entity = Entities()->GetEntity("Sound");
}

/// @brief Gets called once every simulation frame. Use this function for anything that affects the simulation.
void XinoScene::OnFixedUpdate()
{
    if (!update)
    {
        return;
    }
    if ( Input()->GetKeyTriggered( GLFW_KEY_SPACE ) )
    {
        entity->GetComponent<AudioPlayer>()->Play();
    }
    // Create an instance of DebugConsole
    DebugConsole output( *DebugSystem::GetInstance() );

    // Append the message and the formatted value
    //output << "Position: (" << translation.x << ", " << translation.y << ", " << translation.z << ")" << "\n";
}

/// @brief Gets called once every graphics frame. Do not use this function for anything that affects the simulation.
/// @param dt the elapsed time in seconds since the previous frame
void XinoScene::OnUpdate( float dt )
{
    if (!update)
        return;

    drawText(dt);
    planets(dt);
    moveCamera();
}

/// @brief Gets called whenever a scene is exited
void XinoScene::OnSceneExit()
{
    delete sound;
}


static void moveCamera()
{
    static glm::vec2 clickedPos, campos = {0,0}, offset = {0,0};

    if (Input()->GetMouseTriggered(GLFW_MOUSE_BUTTON_2))
        clickedPos = Input()->GetMousePosUI();

    if (Input()->GetMouseReleased(GLFW_MOUSE_BUTTON_2))
        campos += offset;

    if (Input()->GetMouseDown(GLFW_MOUSE_BUTTON_2))
        offset = clickedPos - Input()->GetMousePosUI();
    else
        offset = {0,0};

    Camera()->SetPosition(campos + offset);
}


static void planets(float dt)
{
    static Transform* t_planet = nullptr;
    static Sprite* s_planet = nullptr;
    static glm::vec3 pos, scale;
    static float time = 0.0f;
    time += dt;

    if (!t_planet)
    {
        Entity* planet = Entities()->GetEntity("Planet2");
        t_planet = planet->GetComponent<Transform>();
        s_planet = planet->GetComponent<Sprite>();
        pos = t_planet->GetTranslation();
        scale = t_planet->GetScale();
    }

    t_planet->SetTranslation( pos + glm::vec3(2 * glm::sin(time), 0, 0) );
    float ds = 0.3f * glm::cos(time);
    t_planet->SetScale( scale + glm::vec3(ds, ds, 1) );

    if (ds > 0.0f)
        s_planet->SetLayer(3);
    else
        s_planet->SetLayer(1);
}


static void drawText(float dt)
{
    static Entity* textEnt, *tileEnt;
    static char text_buffer[64] = {};
    static char tile_buffer[21] = {};
    static float time = 0.0f;  // for color change
    static bool textMode = true;
    static Texture tiles("Data/Textures/Tileset_16x8.png");
    time += dt;

    textEnt = Entities()->GetEntity("TextEnt");
    tileEnt = Entities()->GetEntity("TileEnt");
    // init
    static bool initialized = false;
    if ( !initialized )
    {  
        strcpy_s( text_buffer, 63, Entities()->GetEntity("TextEnt")->GetComponent<TextSprite>()->GetText().c_str() );
        initialized = true;
    }

    ImGui::Begin("Text Input");
    ImGui::InputText("Tiles", tile_buffer, 21);
    ImGui::InputText("Actuial text", text_buffer, 63);
    if (ImGui::Button(textMode ? "Text Mode" : "Tile Mode"))
    {
        textMode = !textMode;
        tileEnt->GetComponent<TextSprite>()->SetTexture(textMode ? nullptr : &tiles);
    }
    ImGui::End();
    textEnt->GetComponent<TextSprite>()->SetText( text_buffer );
    textEnt->GetComponent<TextSprite>()->SetColor({glm::abs(glm::sin(time*2)),glm::abs(glm::cos(time)),0,1});
    tileEnt->GetComponent<TextSprite>()->SetText( tile_buffer );
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
