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
#include "CollisionSystem.h"

#include "Pathfinder.h"
#include "Pool.h"
#include "AssetLibrarySystem.h"
#include "RigidBody.h"


static void spawnEnemy(glm::vec2 mousepos);
static void pathfindDemo(float dt);

//-----------------------------------------------------------------------------
// variables
//-----------------------------------------------------------------------------

static bool update = false;

static Entity* tiles;
static Tilemap<int>* t;
static Pathfinder* pf;

static const Entity* enemyArch;
static Entity* turret;
static int eCount; // enemy count
static glm::ivec2 dest;

static glm::vec2 rayOrigin;

//-----------------------------------------------------------------------------
// virtual override methods
//-----------------------------------------------------------------------------

/// @brief  Gets called whenever a scene is initialized
void SandboxSystem::OnSceneInit()
{
    update = true;

    tiles = Entities()->GetEntity("Tiles");
    if (!tiles)
        return;

    t = tiles->GetComponent<Tilemap<int>>();
    pf = tiles->GetComponent<Pathfinder>();
    enemyArch = AssetLibrary<Entity>()->GetAsset("Enemy");
    eCount = 0;
    dest = t->WorldPosToTileCoord(pf->GetDestination());
    t->SetTile(dest, 2);
}

/// @brief  Gets called once every simulation frame. Use this function for anything that affects the simulation.
void SandboxSystem::OnFixedUpdate()
{
    if (!update)
        return;

    if (tiles)
        pathfindDemo(Engine::GetInstance()->GetFixedFrameDuration());

}

/// @brief  Gets called once every graphics frame. Do not use this function for anything that affects the simulation.
/// @param  dt  the elapsed time in seconds since the previous frame
void SandboxSystem::OnUpdate( float dt )
{
    if (!update)
        return;


    // R: set raycast start point
    if ( Input()->GetKeyTriggered( GLFW_KEY_R ) )
    {
        rayOrigin = Input()->GetMousePosWorld();
    }

    // Left-Shift (hold): show raycast
    if ( Input()->GetKeyDown( GLFW_KEY_LEFT_SHIFT ) )
    {
        glm::vec2 direction = glm::normalize( Input()->GetMousePosWorld() - rayOrigin );
        RayCastHit hit = Collisions()->RayCast( rayOrigin, direction );
        Renderer()->DrawLine( rayOrigin, rayOrigin + direction * hit.distance, 0.1f );
    }
}


/// @brief  Gets called whenever a scene is exited
void SandboxSystem::OnSceneExit()
{
    tiles = nullptr;
}



static void pathfindDemo(float dt)
{
    glm::vec2 mousepos = Input()->GetMousePosWorld();
    glm::ivec2 coord = t->WorldPosToTileCoord(mousepos); // (tile column+row)

    // Right click: delete tile
    if (Input()->GetMouseDown(GLFW_MOUSE_BUTTON_2) && coord.x != -1 && t->GetTile(coord) != 0)
        t->SetTile(coord, 0);

    // D: set new destination
    if (Input()->GetKeyTriggered(GLFW_KEY_D))
    {
        if(!turret)
        {
            turret = new Entity;
            *turret = *AssetLibrary<Entity>()->GetAsset("Turret");
            turret->SetName("Turret");
            Entities()->AddEntity(turret);
        }

        turret->GetComponent<Transform>()->SetTranslation(mousepos);
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
        for (auto& enemy : Entities()->GetEntities())
        {
            if(enemy->GetName() != std::string("Enemy"))
				continue;

            Transform* transform = enemy->GetComponent<Transform>();

            if (!transform)
            {
                continue;
            }
                glm::vec2 pos = transform->GetTranslation();
            

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
    {
        for (auto& enemy : Entities()->GetEntities())
        {
            if (enemy->GetName() != std::string("Enemy"))
                continue;

            RigidBody* rb = enemy->GetComponent<RigidBody>();
            rb->SetAcceleration({});
            rb->SetVelocity({});
        }
    }
}



static void spawnEnemy(glm::vec2 mousepos)
{
    Entity* enemycopy = new Entity;
    *enemycopy = *enemyArch;
    enemycopy->GetComponent<Transform>()->SetTranslation(mousepos);
    enemycopy->SetName("Enemy");
    Entities()->AddEntity(enemycopy);
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
