/// @file     CameraSystem.h
/// @author   Eli Tsereteli (ilya.tsereteli@digipen.edu)
///
/// @brief    System that handles the camera and coordinate systems / spaces.
#pragma once
#include "System.h"
#include "glm/glm.hpp"


/// @brief      Handles camera and coordinate systems / spaces.
class CameraSystem : public System
{
public:
    //-------------------------------------------------------------------------
    //          setters and getters
    //-------------------------------------------------------------------------

    /// @brief            Sets all camera transform properties at once.
    /// @param position
    /// @param zoom       zoom factor: 1.0 is standard, higher = further away.
    /// @param rotation
    void Set(glm::vec2 position, float zoom, float rotation);

    /// @brief            Sets the camera position.
    /// @param position
    void SetPosition(glm::vec2 position);

    /// @brief            Sets the camera rotation.
    /// @param rotation
    void SetRotation(float rotation);

    /// @brief            Sets the camera zoom factor.
    /// @param zoom       1.0 is the default zoom. Higher number = zoom out.
    void SetZoom(float zoom);


    /// @return     UI space to clip space matrix
    glm::mat4 const& GetMat_UItoClip() const;

    /// @return     World space to clip space matrix
    glm::mat4 const& GetMat_WorldToClip() const;

    /// @return     Screen to UI space matrix
    glm::mat4 const& GetMat_ScreenToUI() const;

    /// @return     Screen to world space matrix
    glm::mat4 const& GetMat_ScreenToWorld() const;

    /// @return     World space to UI space matrix
    glm::mat4 const& GetMat_WorldToUI() const;

    /// @return     UI space to world space matrix
    glm::mat4 const& GetMat_UItoWorld() const;


private:
    //-------------------------------------------------------------------------
    //          inherited virtuals
    //-------------------------------------------------------------------------

    // overrides
    virtual void OnInit() override;
    virtual void OnUpdate(float dt) override;


    //-------------------------------------------------------------------------
    //          data
    //-------------------------------------------------------------------------
private:
    /// @brief  Matrices for converting mouse position to UI/world coordinates
    glm::mat4 m_Screen2UI, m_Screen2World;

    /// @brief  Matrices for converting between world and UI coordinates
    glm::mat4 m_World2UI, m_UI2World;

    /// @brief  Matrices for converting UI or world to clip space (for rendering)
    glm::mat4 m_World2Clip, m_UI2Clip;

    glm::vec2 m_CamPos ={};  /// @brief     Current camera position
    float m_CamZoom = 1.0f;  /// @brief     Camera zoom factor
    float m_CamAngle = 0.0f; /// @brief     Camera rotation
    bool m_Dirty = true;     /// @brief     Flag: do matrices need to be recalculated?



    //-------------------------------------------------------------------------
    //          helpers
    //-------------------------------------------------------------------------
private:
    /// @brief  Calculates all the matrices using current camera data
    void recalcMatrices();


//-----------------------------------------------------------------------------
private: // reading
//-----------------------------------------------------------------------------

    /// @brief map of the CameraSystem read methods
    static ReadMethodMap< CameraSystem > const s_ReadMethods;

    /// @brief  gets this System's read methods
    /// @return this System's read methods
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override
    {
        return (ReadMethodMap< ISerializable > const&)s_ReadMethods;
    }

    //-------------------------------------------------------------------------
    //          singleton stuff
    //-------------------------------------------------------------------------
private:
    /// @brief    The singleton instance of CameraSystem 
    static CameraSystem* instance;
    /// @brief    Private constructor
    CameraSystem();

public:
    /// @brief    Gets the instance of CameraSystem
    /// @return   CameraSystem pointer: new or existing instance of this system
    static CameraSystem* GetInstance();

    // Prevent copying
    CameraSystem(CameraSystem& other) = delete;
    void operator=(const CameraSystem&) = delete;
};

/// @brief      Convenient function for getting CameraSystem instance. 
__inline CameraSystem* Camera() { return CameraSystem::GetInstance(); }
