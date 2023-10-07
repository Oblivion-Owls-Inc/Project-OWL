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

#include "Tilemap.h"

//-----------------------------------------------------------------------------
// variables
//-----------------------------------------------------------------------------

static bool update = false;

//-----------------------------------------------------------------------------
// virtual override methods
//-----------------------------------------------------------------------------

/// @brief  Gets called whenever a new Scene is loaded
void SandboxSystem::OnSceneLoad()
{}

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

    static Tilemap* t = nullptr;
    if (!t)
        t = Entities()->GetEntity("Tiles")->GetComponent<Tilemap>();

    glm::ivec2 coord = t->WorldPosToTileCoord(Input()->GetMousePosWorld());
    ImGui::Begin("Sandbox");
    ImGui::InputInt2("coord", &coord.x);
    ImGui::End();

    if (Input()->GetMouseTriggered(GLFW_MOUSE_BUTTON_1) && coord.x != -1)
        t->SetTile(coord, (int)'*'-32);
}

/// @brief  Gets called whenever a scene is exited
void SandboxSystem::OnSceneExit()
{
        
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
