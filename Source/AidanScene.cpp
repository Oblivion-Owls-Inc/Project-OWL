///--------------------------------------------------------------------------//
/// @file   AidanScene.cpp
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
#include "AidanScene.h"

/// @brief Gets called whenever a new scene is loaded.
void AidanScene::OnSceneLoad()
{
}

/// @brief Gets called whenever a scene is initialised.
void AidanScene::OnSceneInit()
{
}

/// @brief Gets called once every simulation frame. Use this function for anything that affects the simulation.
void AidanScene::OnFixedUpdate()
{
}

/// @brief Gets called once every graphics frame. Do not use this function for anything that affects the simulation.
/// @param dt the elapsed time in seconds since the previous frame.
void AidanScene::OnUpdate(float dt)
{
}

/// @brief Gets called once before the Engine closes.
void AidanScene::OnSceneExit()
{
}


//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------

	/// @brief map of the XinoScene read methods
ReadMethodMap< AidanScene > const AidanScene::s_ReadMethods = {};

/// @brief  gets this System's read methods
/// @return this System's read methods
ReadMethodMap< System > const& AidanScene::GetReadMethods() const
{
	return (ReadMethodMap< System > const&)s_ReadMethods;
}

//-----------------------------------------------------------------------------
// singleton stuff
//-----------------------------------------------------------------------------

/// @brief Constructs the Stub scene.
AidanScene::AidanScene() {}

// The signleton instance of the AidanScene.
AidanScene* AidanScene::instance = nullptr;

/// @brief  Gets the instance of AidanScene.
/// @return The instance of the AidanScene.
AidanScene* AidanScene::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new AidanScene();
	}
	return instance;
}