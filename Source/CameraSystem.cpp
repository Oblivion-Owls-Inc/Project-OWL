/// @file     CameraSystem.cpp
/// @author   Eli Tsereteli (ilya.tsereteli@digipen.edu)
///
/// @brief    System that handles the camera and coordinate systems / spaces.
#include "CameraSystem.h"
#include "PlatformSystem.h"
#include "glm/gtc/matrix_transform.hpp"


//-------------------------------------------------------------------------
// public: accessors
//-------------------------------------------------------------------------

    /// @brief  sets the active Camera
    /// @return the active camera
    Camera* CameraSystem::GetActiveCamera()
    {
        return m_ActiveCamera;
    }
    /// @brief  gets the active Camera
    /// @param  camera  the active Camera to set to
    void CameraSystem::SetActiveCamera( Camera* camera )
    {
        if ( m_ActiveCamera != nullptr )
        {
            m_ActiveCamera->SetIsActive( false );
        }
        m_ActiveCamera = camera;
        camera->SetIsActive( true );
    }


    /// @return     UI space to clip space matrix
    glm::mat4 const& CameraSystem::GetMat_UItoClip() const { return m_UiToClip; }

    /// @return     World space to clip space matrix
    glm::mat4 const& CameraSystem::GetMat_WorldToClip() const { return m_ActiveCamera->GetWorldToClip(); }

    /// @return     Screen to UI space matrix
    glm::mat4 const& CameraSystem::GetMat_ScreenToUI() const { return m_ScreenToUi; }

    /// @return     Screen to world space matrix
    glm::mat4 CameraSystem::GetMat_ScreenToWorld() const
    {
        return m_ActiveCamera->GetClipToWorld() * m_ScreenToClip;
    }


//-------------------------------------------------------------------------
// private: virtual override methods
//-------------------------------------------------------------------------

    /// @brief  called once when the engine starts
    void CameraSystem::OnInit()
    {
        calculateMatrices();
    }

//-------------------------------------------------------------------------
// private: methods
//-------------------------------------------------------------------------

    /// @brief  Calculates all the CameraSystem Matrices
    void CameraSystem::calculateMatrices()
    {
        glm::mat4 I(1);
        glm::vec2 screen = PlatformSystem::GetInstance()->GetWindowDimensions();
        float screen_ratio = screen.y / screen.x;

        // screen to clip
        glm::vec2 scale = glm::vec2( 2, -2 ) / screen;
        glm::vec2 offset = glm::vec2( -1, 1 );
        m_ScreenToClip = glm::mat4( 0 );
        m_ScreenToClip[ 0 ][ 0 ] = scale.x;
        m_ScreenToClip[ 1 ][ 1 ] = scale.y;
        m_ScreenToClip[ 3 ] = glm::vec4( offset, 0, 1 );

        // screen to UI: re-scale based on screen width/height, flip, move to center
        scale = glm::vec2( 10.0f, -10.0f) / screen.x;
        offset = glm::vec2( -5, 5 * screen_ratio );
        m_ScreenToUi[ 0 ][ 0 ] = scale.x;
        m_ScreenToUi[ 1 ][ 1 ] = scale.y;
        m_ScreenToUi[ 3 ] = glm::vec4( offset, 0, 1 );
    
        // UI to clip (-1 to 1)
        m_UiToClip = glm::scale( glm::mat4( 1 ), glm::vec3( 1.0f / 5, 1.0f / (screen_ratio * 5), 0 ) );
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

//-----------------------------------------------------------------------------
