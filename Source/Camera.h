/// @file       Camera.h
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu), Eli Tsereteli (ilya.tsereteli@digipen.edu)
/// @brief      Camera component for rendering
/// @version    0.1
/// @date       2023-10-22
/// 
/// @copyright  Copyright (c) 2023 Digipen Institute of Technology

#pragma once

#include "Component.h"

#include <glm/glm.hpp>

#include "ComponentReference.h"
class Transform;


/// @brief      Handles camera and coordinate systems / spaces.
class Camera : public Component
{
//-------------------------------------------------------------------------
public: // constructor
//-------------------------------------------------------------------------


    /// @brief  constructor
    Camera();


//-------------------------------------------------------------------------
public: // accessors
//-------------------------------------------------------------------------


    /// @brief  gets the width of the Camera
    /// @return the width of the camera
    float GetWidth() const;
    /// @brief  sets the width of the camera
    /// @param  width   the width of the camera
    /// @note   also overrides height based on viewport aspect ratio
    void SetWidth( float width);

    /// @brief  gets the height of the Camera
    /// @return the height of the camera
    float GetHeight() const;
    /// @brief  sets the height of the camera
    /// @param  height  the height of the camera
    /// @note   also overrides width based on viewport aspect ratio
    void SetHeight( float height);


    /// @brief  gets the world space to clip space matrix
    /// @return the world space to clip space matrix
    glm::mat4 const& GetWorldToClip() const;

    /// @brief  gets the clip space to world space matrix
    /// @return the clip space to world space matrix
    glm::mat4 const& GetClipToWorld() const;

    /// @brief  sets the active state of this Camera
    /// @param  isActive    whether the Camera is active or not
    /// @note   SHOULD ONLY EVER BE CALLED BY CAMERASYSTEM
    void SetIsActive( bool isActive ) { m_IsActive = isActive; }


//-------------------------------------------------------------------------
public: // methods
//-------------------------------------------------------------------------
    

    /// @brief  sets this Camera as the active Camera in the scene
    void SetActive();


//-------------------------------------------------------------------------
private: // virtual override methods
//-------------------------------------------------------------------------
    

    /// @brief  called once when entering the scene
    virtual void OnInit() override;

    /// @brief  called once when leaving the scene
    virtual void OnExit() override;


//-------------------------------------------------------------------------
private: // member variables
//-------------------------------------------------------------------------


    /// @brief  how big the camera is (height; width is based on height and aspect ratio)
    float m_Scale = 5;

    /// @brief  whether this is the current active Camera
    bool m_IsActive = true;

    /// @brief  world space to camera space transformation matrix
    glm::mat4 m_WorldToCamera = glm::mat4( 1 );
    bool m_WorldToCameraDirty = true;

    /// @brief  camera space to clip space transformation matrix
    glm::mat4 m_CameraToClip = glm::mat4( 1 );

    /// @brief  world space to clip space transformation matrix
    glm::mat4 m_WorldToClip = glm::mat4( 1 );
    bool m_WorldToClipDirty = true;

    /// @brief  world space to clip space transformation matrix
    glm::mat4 m_ClipToWorld = glm::mat4( 1 );
    bool m_ClipToWorldDirty = true;

    /// @brief  the Transform associated with this Camera
    ComponentReference< Transform > m_Transform;



//-------------------------------------------------------------------------
private: // methods
//-------------------------------------------------------------------------


    /// @brief  called whenever this Camera's Transform changes
    void onTransformChangedCallback();


    /// @brief  calculates to WorldToCamera matrix
    void calculateWorldToCamera();

    /// @brief  calculates to CameraToClip matrix
    void calculateCameraToClip();

    /// @brief  calculates to WorldToClip matrix
    void calculateWorldToClip();

    /// @brief  calculates to WorldToClip matrix
    void calculateClipToWorld();

    
//-----------------------------------------------------------------------------
public: // inspection
//-----------------------------------------------------------------------------


    /// @brief  shows the Inspector window
    virtual void Inspector() override;


//-----------------------------------------------------------------------------
private: // reading
//-----------------------------------------------------------------------------
    

    /// @brief  reads the width of this Camera
    /// @param  json    the json data to read from
    void readWidth( nlohmann::ordered_json const& json );

    /// @brief  reads the height of this Camera
    /// @param  json    the json data to read from
    void readHeight( nlohmann::ordered_json const& json );
    
    /// @brief  reads the active state of this Camera
    /// @param  json    the json data to read from
    void readIsActive( nlohmann::ordered_json const& json );


//-----------------------------------------------------------------------------
public: // reading / writing
//-----------------------------------------------------------------------------


    /// @brief  gets this System's read methods
    /// @return this System's read methods
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override;

    /// @brief  writes this Camera to json
    /// @return the written json data
    nlohmann::ordered_json Write() const;


//-----------------------------------------------------------------------------
public: // copying
//-----------------------------------------------------------------------------


    /// @brief  creates a new copy of this Camera
    /// @return the copy of this Camera
    virtual Camera* Clone() const override;


//-----------------------------------------------------------------------------
private: // copying
//-----------------------------------------------------------------------------


    /// @brief  copy constructor
    /// @param  other   the Camera to copy
    Camera( Camera const& other );


    // disable assignment operator
    void operator =( Camera const& ) = delete;


//-----------------------------------------------------------------------------
};
