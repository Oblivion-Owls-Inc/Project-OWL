/// @file       EditorCameraController.h
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      controls the camera while in the editor
/// @version    0.1
/// @date       2024-02-09
/// 
/// @copyright  Copyright (c) 2024 Digipen Institute of Technology

#pragma once





class Transform;
class Camera;


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


    /// @brief  how sensitive is the scroll wheel zooming
    float m_Sensativity = 1.0f;


    /// @brief  the Transform component attached to this Entity
    ComponentReference< Transform > m_Transform;

    /// @brief  the Camera Component attached to this Entity
    ComponentReference< Camera > m_Camera;


    /// @brief  the target world position of the mouse
    glm::vec2 m_MouseTargetPos = { 0.0f, 0.0f };


//-----------------------------------------------------------------------------
private: // methods
//-----------------------------------------------------------------------------


    /// @brief  moves the Transform to align the mouse target pos
    /// @param  mousePos    the position of the mouse
    void moveToAlignMouse( glm::vec2 const& mousePos );


//-----------------------------------------------------------------------------
public: // inspection
//-----------------------------------------------------------------------------


    /// @brief  shows the inspector for EditorCameraController
    virtual void Inspector() override;


//-----------------------------------------------------------------------------
private: // reading
//-----------------------------------------------------------------------------


    /// @brief  reads how sensitive is the scroll wheel zooming
    /// @param  data    the JSON data to read from
    void readSensativity( nlohmann::ordered_json const& data );


//-----------------------------------------------------------------------------
public: // reading / writing
//-----------------------------------------------------------------------------

    
    /// @brief  gets the map of read methods for this EditorCameraController
    /// @return the map of read methods for this EditorCameraController
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override;


    /// @brief  writes this EditorCameraController to JSON
    /// @return the JSON data of this EditorCameraController
    virtual nlohmann::ordered_json Write() const override;


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
