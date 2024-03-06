/// @file       SandboxSystem.cpp
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      Sandbox system meant to mimic CS-230 scenes
/// @version    0.1
/// @date       2023-09-05
/// 
/// @copyright  Copyright (c) 2023 Digipen Institute of Technology

#include "SandboxSystem.h"
#include "InputSystem.h"
#include "SceneSystem.h"   // +AssetLibrary.h
#include "EntitySystem.h"  // +Entity.h


//-----------------------------------------------------------------------------
// variables
//-----------------------------------------------------------------------------

static bool update = false;

//-----------------------------------------------------------------------------
// virtual override methods
//-----------------------------------------------------------------------------

/// @brief  Gets called whenever a scene is initialized
void SandboxSystem::OnSceneInit()
{
    if ( SceneSystem::GetInstance()->GetSceneName() != "Sandbox" )
        return;
    
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


}


/// @brief  Gets called whenever a scene is exited
void SandboxSystem::OnSceneExit()
{

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
