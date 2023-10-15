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
#include "MovementAI.h"
#include "InputSystem.h"
#include "DebugSystem.h"
#include "EntitySystem.h"
#include "SceneSystem.h"

#include "Pathfinder.h"
#include "AssetLibrarySystem.h"
#include "RigidBody.h"


static void spawnEnemy(glm::vec2 mousepos);
static void pathfindDemo(float dt);

//-----------------------------------------------------------------------------
// variables
//-----------------------------------------------------------------------------

static bool update = false;

static Entity* tiles;
static std::vector<Entity*> enemies;

//-----------------------------------------------------------------------------
// virtual override methods
//-----------------------------------------------------------------------------

/// @brief  Gets called whenever a scene is initialized
void SandboxSystem::OnSceneInit()
{
    update = true;


}

/// @brief  Gets called once every simulation frame. Use this function for anything that affects the simulation.
void SandboxSystem::OnFixedUpdate()
{
    if (!update)
        return;


}

/// @brief  Gets called once every graphics frame. Do not use this function for anything that affects the simulation.
/// @param  dt  the elapsed time in seconds since the previous frame
void SandboxSystem::OnUpdate( float dt )
{
    if (!update)
        return;

    tiles = Entities()->GetEntity("Tiles");
    if (!tiles)
        return;

    pathfindDemo(dt);
}


/// @brief  Gets called whenever a scene is exited
void SandboxSystem::OnSceneExit()
{
    
}



static void pathfindDemo(float dt)
{
    // init stuff
    static Tilemap<int>* t = tiles->GetComponent<Tilemap<int>>();
    static Pathfinder* pf = tiles->GetComponent<Pathfinder>();
    static glm::ivec2 dest = {-1,-1};
    if (dest.x == -1)
    {
        dest = t->WorldPosToTileCoord(pf->GetDestination());
        t->SetTile(dest, 2);
    }


    glm::vec2 mousepos = Input()->GetMousePosWorld();
    glm::ivec2 coord = t->WorldPosToTileCoord(mousepos); // (tile column+row)

    // Right click: delete tile
    if (Input()->GetMouseDown(GLFW_MOUSE_BUTTON_2) && coord.x != -1 && t->GetTile(coord) != 0)
        t->SetTile(coord, 0);

    // D: set new destination
    if (Input()->GetKeyTriggered(GLFW_KEY_D))
    {
        pf->SetDestination( mousepos );
        t->SetTile(dest, 0);
        dest = coord;
        t->SetTile(dest, 2);
    }

    // S: spawn enemy
    if (Input()->GetKeyTriggered(GLFW_KEY_S))
        spawnEnemy(mousepos);

    // Space (hold): enemies move to destination
    if (Input()->GetKeyDown(GLFW_KEY_SPACE))
    {
        for (auto& enemy : enemies)
        {
            glm::vec2 pos = enemy->GetComponent<Transform>()->GetTranslation();
            RigidBody* rb = enemy->GetComponent<RigidBody>();

            // accelerate along path
            glm::vec3 moveDir = glm::vec3(pf->GetDirectionAt(pos),0);
            rb->SetAcceleration(moveDir*12.0f);

            // air friction or something
            float af = 5.5f;
            glm::vec2 vel = rb->GetVelocity();
            rb->SetVelocity(vel * (1.0f - af*dt) );
        }
    }

    // they stop when space is released
    if (Input()->GetKeyReleased(GLFW_KEY_SPACE))
        for (auto& enemy : enemies)
        {
            RigidBody* rb = enemy->GetComponent<RigidBody>();
            rb->SetAcceleration({});
            rb->SetVelocity({});
        }
}



static void spawnEnemy(glm::vec2 mousepos)
{
    static const Entity* enemy = AssetLibrary<Entity>()->GetAsset("Enemy");
    static int i = 0;
    if (!enemy || i>10)
        return;

    Entity* enemycopy = new Entity;
    *enemycopy = *enemy;
    enemycopy->GetComponent<Transform>()->SetTranslation(mousepos);
    enemycopy->SetName("AAAAAAAAAAAAAA" + i++);  // each will have shorter name
    Entities()->AddEntity(enemycopy);
    enemies.push_back(enemycopy);
}



//-----------------------------------------------------------------------------
// singleton stuff
//-----------------------------------------------------------------------------

    /// @brief  Constructs the SandboxSystem
    SandboxSystem::SandboxSystem() : System( "SandboxSystem" ) {}

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
