/// @file     CameraSystem.h
/// @author   Eli Tsereteli (ilya.tsereteli@digipen.edu)
///
/// @brief    System that handles the camera and coordinate systems / spaces.
#pragma once
#include "System.h"
#include "glm/glm.hpp"

#include "Camera.h"


/// @brief      Handles camera and coordinate systems / spaces.
class CameraSystem : public System
{
//-------------------------------------------------------------------------
public: // accessors
//-------------------------------------------------------------------------


    /// @brief  sets the active Camera
    /// @return the active camera
    Camera* GetActiveCamera();

    /// @brief  gets the active Camera
    /// @param  camera  the active Camera to set to
    void SetActiveCamera( Camera* camera );


    /// @return UI space to clip space matrix
    glm::mat4 const& GetMat_UiToClip() const;

    /// @return World space to clip space matrix
    glm::mat4 const& GetMat_WorldToClip() const;

    /// @return Screen to UI space matrix
    glm::mat4 const& GetMat_ScreenToUi() const;

    /// @return Screen to world space matrix
    glm::mat4 GetMat_ScreenToWorld() const;


//-------------------------------------------------------------------------
private: // virtual override methods
//-------------------------------------------------------------------------
    

    /// @brief  called once when the engine starts
    virtual void OnInit() override;


//-------------------------------------------------------------------------
private: // member variables
//-------------------------------------------------------------------------


    /// @brief  Matrix for converting screen position to clip space
    glm::mat4 m_ScreenToClip = glm::mat4( 1 );

    /// @brief  Matrix for converting mouse position to UI coordinates
    glm::mat4 m_ScreenToUi = glm::mat4( 1 );

    /// @brief  Matrix for converting UI to clip space (for rendering)
    glm::mat4 m_UiToClip = glm::mat4( 1 );


    /// @brief  the current active camera in the scene
    Camera* m_ActiveCamera = nullptr;


//-------------------------------------------------------------------------
private: // methods
//-------------------------------------------------------------------------


    /// @brief  Calculates all the CameraSystem Matrices
    void calculateMatrices();


    /// @brief  callback to call whenever the window resizes
    /// @param  size    the new size of the window
    void onWindowResizeCallback( glm::ivec2 const& size );


//-------------------------------------------------------------------------
private: // singleton stuff
//-------------------------------------------------------------------------

    /// @brief    The singleton instance of CameraSystem 
    static CameraSystem* instance;
    /// @brief    Private constructor
    CameraSystem();

//-------------------------------------------------------------------------
public: // singleton stuff
//-------------------------------------------------------------------------

    /// @brief    Gets the instance of CameraSystem
    /// @return   CameraSystem pointer: new or existing instance of this system
    static CameraSystem* GetInstance();

    // Prevent copying
    CameraSystem(CameraSystem& other) = delete;
    void operator=(const CameraSystem&) = delete;

//-------------------------------------------------------------------------
};

/// @brief      Convenient function for getting CameraSystem instance. 
__inline CameraSystem* Cameras() { return CameraSystem::GetInstance(); }
