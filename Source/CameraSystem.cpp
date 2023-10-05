/// @file     CameraSystem.cpp
/// @author   Eli Tsereteli (ilya.tsereteli@digipen.edu)
///
/// @brief    System that handles the camera and coordinate systems / spaces.
#include "CameraSystem.h"
#include "PlatformSystem.h"
#include "glm/gtc/matrix_transform.hpp"


/// @brief      Performs initial matrix calculations.
void CameraSystem::OnInit() {
    recalcMatrices();
}


/// @brief      Re-calculates matrices if there's been changes to camera state.
/// @param dt
void CameraSystem::OnUpdate(float dt)
{
    if (m_Dirty)
        recalcMatrices();

    // TODO: optimizations, maybe? Multiple dirty flags, only recalculate
    //       matrices that are affected by latest changes? Or maybe it dont matter

    (void) dt;
}


//-----------------------------------------------------------------------------
//          setters and getters
//-----------------------------------------------------------------------------

/// @brief            Sets all camera transform properties at once.
/// @param position
/// @param zoom       zoom factor: 1.0 is standard, higher = further away.
/// @param rotation
void CameraSystem::Set(glm::vec2 position, float zoom, float rotation)
{
    m_CamPos = position;
    m_CamZoom = zoom;
    m_CamAngle = rotation;
    m_Dirty = true;
}

/// @brief            Sets the camera position.
/// @param position
void CameraSystem::SetPosition(glm::vec2 position)
{
    m_CamPos = position;
    m_Dirty = true;
}

/// @brief            Sets the camera rotation.
/// @param rotation
void CameraSystem::SetRotation(float rotation)
{
    m_CamAngle = rotation;
    m_Dirty = true;
}

/// @brief            Sets the camera zoom factor.
/// @param zoom       1.0 is the default zoom. Higher number = zoom out.
void CameraSystem::SetZoom(float zoom)
{
    m_CamZoom = zoom;
    m_Dirty = true;
}


/// @return     UI space to clip space matrix
glm::mat4 const& CameraSystem::GetMat_UItoClip() const { return m_UI2Clip; }

/// @return     World space to clip space matrix
glm::mat4 const& CameraSystem::GetMat_WorldToClip() const { return m_World2Clip; }

/// @return     Screen to UI space matrix
glm::mat4 const& CameraSystem::GetMat_ScreenToUI() const { return m_Screen2UI; }

/// @return     Screen to world space matrix
glm::mat4 const& CameraSystem::GetMat_ScreenToWorld() const { return m_Screen2World; }

/// @return     UI space to world space matrix
glm::mat4 const& CameraSystem::GetMat_UItoWorld() const { return m_UI2World; }

/// @return     World space to UI space matrix
glm::mat4 const& CameraSystem::GetMat_WorldToUI() const { return m_World2UI; }


// helper
/// @brief    Calculates all the matrices using current camera data
void CameraSystem::recalcMatrices()
{
    glm::mat4 I(1);
    glm::vec2 screen = PlatformSystem::GetInstance()->GetWindowDimensions();
    float screen_ratio = screen.y / screen.x;

    // screen to UI: re-scale based on screen width/height, flip, move to center
    glm::vec3 scale = glm::vec3( 10.0f/screen.x,  -10.0f * screen_ratio/screen.y,  0.0 );
    glm::vec3 shift = glm::vec3(-5, 5 * screen_ratio, 0);
    m_Screen2UI = glm::translate(I, shift) * glm::scale(glm::mat4(1), scale);

    // UI to world: it's just the camera transform.
    glm::mat4 s = glm::scale(I, {m_CamZoom, m_CamZoom, 1});
    glm::mat4 r = glm::rotate(I, m_CamAngle, {0,0,1});
    glm::mat4 t = glm::translate(I, {m_CamPos.x, m_CamPos.y, 0});
    m_UI2World = t * r * s;
    // and its inverse
    m_World2UI = glm::inverse(m_UI2World);
    
    // UI to clip (-1 to 1)
    m_UI2Clip = glm::scale(glm::mat4(1), {1.0f/5, 1.0f/(screen_ratio * 5), 0});

    // these 2 are just combinations (go through UI)
    m_Screen2World = m_UI2World * m_Screen2UI;
    m_World2Clip = m_UI2Clip * m_World2UI;

    m_Dirty = false;
}

//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------

    /// @brief map of the CameraSystem read methods
    ReadMethodMap< CameraSystem > const CameraSystem::s_ReadMethods = {};

    /// @brief  gets this System's read methods
    /// @return this System's read methods
    ReadMethodMap< System > const& CameraSystem::GetReadMethods() const
    {
        return (ReadMethodMap< System > const&)s_ReadMethods;
    }

//-----------------------------------------------------------------------------
// singleton stuff
//-----------------------------------------------------------------------------
CameraSystem* CameraSystem::instance = nullptr;

CameraSystem::CameraSystem() :
    System( "CameraSystem" )
{}

CameraSystem* CameraSystem::GetInstance()
{
    if (instance == nullptr)
    {
        instance = new CameraSystem();
    }
    return instance;
}

