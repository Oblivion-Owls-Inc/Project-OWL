/// @file       PlayBar.cpp
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      UI Bar that handles playing, pausing, and autosaves of the scene
/// @version    0.1
/// @date       2024-02-27
/// 
/// @copyright  Copyright (c) 2024 Digipen Instutute of Technology


#include "pch.h" // precompiled header has to be included first
#include "PlayBar.h"

#include "DebugSystem.h"
#include "SceneSystem.h"

//-----------------------------------------------------------------------------
// public: constructor
//-----------------------------------------------------------------------------


    /// @brief  default constructor
    PlayBar::PlayBar()
    {}


//-----------------------------------------------------------------------------
// public: methods
//-----------------------------------------------------------------------------


    /// @brief  displays the PlayBar
    void PlayBar::Display()
    {
        ImGui::Begin( "PlayBar", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar );

        ImGui::Indent( 20.0f );

        // stop button
        bool disabled = m_AtStartOfScene;
        if ( disabled )
        {
            ImGui::BeginDisabled();
        }
        if ( ImGui::SmallButton( "|<" ) )
        {
            stop();
        }
        if ( disabled )
        {
            ImGui::EndDisabled();
        }

        ImGui::SameLine();

        // restart button
        disabled = m_AtStartOfScene;
        if ( disabled )
        {
            ImGui::BeginDisabled();
        }
        if ( ImGui::SmallButton( "<" ) )
        {
            restart();
        }
        if ( disabled )
        {
            ImGui::EndDisabled();
        }

        ImGui::SameLine();

        // pause button
        disabled = Debug().IsEditorRunning() == false;
        if ( disabled )
        {
            ImGui::BeginDisabled();
        }
        if ( ImGui::SmallButton( "||" ) )
        {
            pause();
        }
        if ( disabled )
        {
            ImGui::EndDisabled();
        }

        ImGui::SameLine();

        // play button
        disabled = Debug().IsEditorRunning();
        if ( disabled )
        {
            ImGui::BeginDisabled();
        }
        if ( ImGui::SmallButton( ">" ) )
        {
            play();
        }
        if ( disabled )
        {
            ImGui::EndDisabled();
        }


        ImGui::End();
    }


    /// @brief  should be called when a scene is loaded from the inspector
    void PlayBar::OnInspectorLoadScene()
    {
        if ( Debug().IsEditorRunning() == false )
        {
            m_AtStartOfScene = true;
        }
    }

//-----------------------------------------------------------------------------
// private: methods
//-----------------------------------------------------------------------------


    /// @brief  unpauses the scene. If at start of scene, also autosaves
    void PlayBar::play()
    {
        if ( m_AtStartOfScene )
        {
            Scenes()->Autosave();
        }

        Debug().SetNonEditorSystemsEnabled( true );
        m_AtStartOfScene = false;
    }


    /// @brief  pauses the scene in its current state
    void PlayBar::pause()
    {
        Debug().SetNonEditorSystemsEnabled( false );
    }


    /// @brief  reloads the scene from an autosave
    void PlayBar::restart()
    {
        if ( Debug().IsEditorRunning() == false )
        {
            m_AtStartOfScene = true;
        }

        Scenes()->LoadAutosave();
    }


    /// @brief  pauses and restarts the scene
    void PlayBar::stop()
    {
        pause();
        restart();
    }

    
//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------

    
//-----------------------------------------------------------------------------
// public: reading / writing
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
