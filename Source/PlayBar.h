/// @file       PlayBar.h
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      UI Bar that handles playing, pausing, and autosaves of the scene
/// @version    0.1
/// @date       2024-02-27
/// 
/// @copyright  Copyright (c) 2024 Digipen Instutute of Technology


#pragma once

#include "ISerializable.h"


/// @brief  UI Bar that handles playing, pausing, and autosaves of the scene
class PlayBar : public ISerializable
{
//-----------------------------------------------------------------------------
public: // constructor
//-----------------------------------------------------------------------------


    /// @brief  default constructor
    PlayBar();


//-----------------------------------------------------------------------------
public: // methods
//-----------------------------------------------------------------------------


    /// @brief  displays the PlayBar
    void Display();


    /// @brief  should be called when a scene is loaded from the inspector
    void OnInspectorLoadScene();


//-----------------------------------------------------------------------------
private: // members
//-----------------------------------------------------------------------------


    /// @brief  whether currently at the start of the scene
    bool m_AtStartOfScene = true;


//-----------------------------------------------------------------------------
private: // methods
//-----------------------------------------------------------------------------


    /// @brief  unpauses the scene. If at start of scene, also autosaves
    void play();


    /// @brief  pauses the scene in its current state
    void pause();


    /// @brief  reloads the scene from an autosave
    void restart();


    /// @brief  pauses and restarts the scene
    void stop();

    
//-----------------------------------------------------------------------------
private: // copying
//-----------------------------------------------------------------------------


    // disable copying
    PlayBar( PlayBar const& ) = delete;
    void operator =( PlayBar const& ) = delete;


//-----------------------------------------------------------------------------
};

