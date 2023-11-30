///--------------------------------------------------------------------------//
/// @file   StubScene.cpp
/// @brief  Stub file for easier scene creation.
/// 
/// @author Aidan Straker (aidan.straker)
/// @date   September 2023
///
/// @copyright © 2023 DigiPen (USA) Corporation.
///--------------------------------------------------------------------------//

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "StubScene.h"

/// @brief Gets called whenever a new scene is loaded.
void StubScene::OnSceneLoad()
{
}

/// @brief Gets called whenever a scene is initialised.
void StubScene::OnSceneInit()
{
}

/// @brief Gets called once every simulation frame. Use this function for anything that affects the simulation.
void StubScene::OnFixedUpdate()
{
}

/// @brief Gets called once every graphics frame. Do not use this function for anything that affects the simulation.
/// @param dt the elapsed time in seconds since the previous frame.
void StubScene::OnUpdate(float dt)
{
}

/// @brief Gets called once before the Engine closes.
void StubScene::OnSceneExit()
{
}


//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------

    /// @brief map of the XinoScene read methods
ReadMethodMap< StubScene > const StubScene::s_ReadMethods = {};

/// @brief  gets this System's read methods
/// @return this System's read methods
ReadMethodMap< System > const& StubScene::GetReadMethods() const
{
    return (ReadMethodMap< System > const&)s_ReadMethods;
}

//-----------------------------------------------------------------------------
// singleton stuff
//-----------------------------------------------------------------------------

/// @brief Constructs the Stub scene.
StubScene::StubScene() {}

// The signleton instance of the StubScene.
StubScene* StubScene::instance = nullptr;

/// @brief  Gets the instance of StubScene.
/// @return The instance of the StubScene.
StubScene* StubScene::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new StubScene();
	}
	return instance;
}