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

#include "Transform.h"
#include "Entity.h"

#include "DebugSystem.h"
#include "InputSystem.h"

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

        m_Transform = GetEntity()->GetComponent< Transform >();
    }

    /// @brief  called once when exiting the scene
    void EditorCameraController::OnExit()
    {
        Behaviors< EditorCameraController >()->RemoveComponent( this );
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
            glm::vec2 offset = m_MouseTargetPos - mousePos;

            m_Transform->SetTranslation( m_Transform->GetTranslation() + offset );
        }
    }


//-----------------------------------------------------------------------------
// private: methods
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// public: inspection
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// public: reading / writing
//-----------------------------------------------------------------------------

    
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
        Behavior( other )
    {}


//-----------------------------------------------------------------------------
