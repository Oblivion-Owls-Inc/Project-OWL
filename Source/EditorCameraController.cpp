/// @file       EditorCameraController.cpp
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      controls the camera while in the editor
/// @version    0.1
/// @date       2024-02-09
/// 
/// @copyright  Copyright (c) 2024 Digipen Institute of Technology

#pragma once

#include "EditorCameraController.h"
#include "BehaviorSystem.h"

#include "ComponentReference.t.h"
#include "Transform.h"
#include "Camera.h"

#include "DebugSystem.h"
#include "InputSystem.h"
#include "PlatformSystem.h"

//-----------------------------------------------------------------------------
// public: constructor / Destructor
//-----------------------------------------------------------------------------


    /// @brief  default constructor
    EditorCameraController::EditorCameraController() :
        Behavior( typeid( EditorCameraController ) )
    {}


//-----------------------------------------------------------------------------
// public: methods
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// public: accessors
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// public: virtual override methods
//-----------------------------------------------------------------------------


    /// @brief  called once when entering the scene
    void EditorCameraController::OnInit()
    {
        Behaviors< EditorCameraController >()->AddComponent( this );

        m_Transform.Init( GetEntity() );
        m_Camera   .Init( GetEntity() );
    }

    /// @brief  called once when exiting the scene
    void EditorCameraController::OnExit()
    {
        Behaviors< EditorCameraController >()->RemoveComponent( this );

        m_Transform.Exit();
        m_Camera   .Exit();
    }


    /// @brief  called once every graphics frame
    /// @param  dt  the duration of the frame
    void EditorCameraController::OnUpdate( float dt )
    {
        if ( m_Transform == nullptr )
        {
            Debug() << "WARNING: EditorCameraController doesn't have an attached Transform" << std::endl;
            return;
        }

        glm::vec2 mousePos = Input()->GetMousePosWorld();
        if ( Input()->GetMouseTriggered( GLFW_MOUSE_BUTTON_3 ) )
        {
            m_MouseTargetPos = mousePos;
        }
        else if ( Input()->GetMouseDown( GLFW_MOUSE_BUTTON_3 ) )
        {
            moveToAlignMouse( mousePos );
        }

        if ( m_Camera != nullptr )
        {
            if ( Input()->GetMouseDeltaScroll() != 0.0f && Platform()->isPosWithinWindow( Input()->GetMousePosScreen() ) )
            {
                m_MouseTargetPos = mousePos;

                m_Camera->SetWidth( m_Camera->GetWidth() * std::pow( 2.0f, m_Sensativity * Input()->GetMouseDeltaScroll() ) );

                moveToAlignMouse( Input()->GetMousePosWorld() );
            }
        }
    }


//-----------------------------------------------------------------------------
// private: methods
//-----------------------------------------------------------------------------


    /// @brief  moves the Transform to align the mouse target pos
    /// @param  mousePos    the position of the mouse
    void EditorCameraController::moveToAlignMouse( glm::vec2 const& mousePos )
    {
        glm::vec2 offset = m_MouseTargetPos - mousePos;
        m_Transform->SetTranslation( m_Transform->GetTranslation() + offset );
    }


//-----------------------------------------------------------------------------
// public: inspection
//-----------------------------------------------------------------------------


    /// @brief  shows the inspector for EditorCameraController
    void EditorCameraController::Inspector()
    {
        ImGui::DragFloat( "scroll sensativity", &m_Sensativity, 0.05f );
    }


//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------


    /// @brief  reads how sensitive is the scroll wheel zooming
    /// @param  data    the JSON data to read from
    void EditorCameraController::readSensativity( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_Sensativity, data );
    }


//-----------------------------------------------------------------------------
// public: reading / writing
//-----------------------------------------------------------------------------

    
    /// @brief  gets the map of read methods for this EditorCameraController
    /// @return the map of read methods for this EditorCameraController
    ReadMethodMap< ISerializable > const& EditorCameraController::GetReadMethods() const
    {
        static ReadMethodMap< EditorCameraController > const readMethods = {
            { "Sensativity", &EditorCameraController::readSensativity }
        };

        return (ReadMethodMap< ISerializable > const&)readMethods;
    }


    /// @brief  Writes all EditorCameraController data to a JSON file.
    /// @return The JSON file containing the data.
    nlohmann::ordered_json EditorCameraController::Write() const
    {
        nlohmann::ordered_json json;

        json[ "Sensativity" ] = Stream::Write( m_Sensativity );

        return json;
    }


//-----------------------------------------------------------------------------
// public: copying
//-----------------------------------------------------------------------------


    /// @brief  clones this EditorCameraController
    /// @return the newly created clone of this EditorCameraController
    EditorCameraController* EditorCameraController::Clone() const
    {
        return new EditorCameraController( *this );
    }


//-----------------------------------------------------------------------------
// private: copying
//-----------------------------------------------------------------------------


    /// @brief  copy-constructor for the EditorCameraController
    /// @param  other   the other EditorCameraController to copy
    EditorCameraController::EditorCameraController( EditorCameraController const& other ) :
        Behavior( other ),
        m_Sensativity( other.m_Sensativity )
    {}


//-----------------------------------------------------------------------------
