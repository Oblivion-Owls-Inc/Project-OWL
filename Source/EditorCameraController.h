/// @file       EditorCameraController.h
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      controls the camera while in the editor
/// @version    0.1
/// @date       2024-02-09
/// 
/// @copyright  Copyright (c) 2024 Digipen Institute of Technology

#pragma once

#include "Behavior.h"


class Transform;


/// @brief  controls the camera while in the editor
class EditorCameraController : public Behavior
{
//-----------------------------------------------------------------------------
public: // constructor / Destructor
//-----------------------------------------------------------------------------


    /// @brief  default constructor
    EditorCameraController();


//-----------------------------------------------------------------------------
public: // methods
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
public: // accessors
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
public: // virtual override methods
//-----------------------------------------------------------------------------


    /// @brief  called once when entering the scene
    virtual void OnInit() override;

    /// @brief  called once when exiting the scene
    virtual void OnExit() override;


    /// @brief  called once every graphics frame
    /// @param  dt  the duration of the frame
    virtual void OnUpdate( float dt ) override;


//-----------------------------------------------------------------------------
private: // members
//-----------------------------------------------------------------------------


    /// @brief  the Transform component attached to this Entity
    Transform* m_Transform = nullptr;


    /// @brief  the target world position of the mouse
    glm::vec2 m_MouseTargetPos = { 0.0f, 0.0f };


//-----------------------------------------------------------------------------
private: // methods
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
public: // inspection
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
private: // reading
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
public: // reading / writing
//-----------------------------------------------------------------------------

    
//-----------------------------------------------------------------------------
public: // copying
//-----------------------------------------------------------------------------


    /// @brief  clones this EditorCameraController
    /// @return the newly created clone of this EditorCameraController
    virtual EditorCameraController* Clone() const override;


//-----------------------------------------------------------------------------
private: // copying
//-----------------------------------------------------------------------------


    /// @brief  copy-constructor for the EditorCameraController
    /// @param  other   the other EditorCameraController to copy
    EditorCameraController( EditorCameraController const& other );


    // diable = operator
    void operator =( EditorCameraController const& ) = delete;


//-----------------------------------------------------------------------------
};
