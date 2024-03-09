/// @file     CameraSystem.cpp
/// @author   Eli Tsereteli (ilya.tsereteli@digipen.edu)
///
/// @brief    System that handles the camera and coordinate systems / spaces.
/// 

#include "pch.h" // precompiled header has to be included first
#include "CameraSystem.h"
#include "PlatformSystem.h"
#include "DebugSystem.h"

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

        if ( camera == nullptr )
        {
            return;
        }

        camera->SetIsActive( true );

        // recalculate height from width, in case aspect ratio changed
        m_ActiveCamera->SetWidth( m_ActiveCamera->GetWidth() );
    }


    /// @return     UI space to clip space matrix
    glm::mat4 const& CameraSystem::GetMat_UiToClip() const
    {
        return m_UiToClip;
    }

    /// @return     World space to clip space matrix
    glm::mat4 const& CameraSystem::GetMat_WorldToClip() const
    {
        if ( m_ActiveCamera == nullptr )
        {
            static glm::mat4 const identity = glm::mat4( 1.0f );
            return identity;
        }
        return m_ActiveCamera->GetWorldToClip();
    }

    /// @return     Screen to UI space matrix
    glm::mat4 const& CameraSystem::GetMat_ScreenToUi() const
    {
        return m_ScreenToUi;
    }

    /// @return     Screen to world space matrix
    glm::mat4 CameraSystem::GetMat_ScreenToWorld() const
    {
        if ( m_ActiveCamera == nullptr )
        {
            return glm::mat4( 1.0f );
        }

        return m_ActiveCamera->GetClipToWorld() * m_ScreenToClip;
    }


//-------------------------------------------------------------------------
// private: virtual override methods
//-------------------------------------------------------------------------

    /// @brief  called once when the engine starts
    void CameraSystem::OnInit()
    {
        calculateMatrices();
        Platform()->AddOnWindowResizeCallback( GetId(), std::bind( &CameraSystem::onWindowResizeCallback, this, std::placeholders::_1 ) );
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
        m_ScreenToClip[ 2 ][ 2 ] = 1.0f;
        m_ScreenToClip[ 3 ] = glm::vec4( offset, 0, 1 );

        // screen to UI: re-scale based on screen width/height, flip, move to center
        scale = glm::vec2( 10.0f, -10.0f) / screen.x;
        offset = glm::vec2( -5, 5 * screen_ratio );
        m_ScreenToUi[ 0 ][ 0 ] = scale.x;
        m_ScreenToUi[ 1 ][ 1 ] = scale.y;
        m_ScreenToUi[ 2 ][ 2 ] = 1.0f;
        m_ScreenToUi[ 3 ] = glm::vec4( offset, 0, 1 );
    
        // UI to clip (-1 to 1)
        m_UiToClip = glm::scale( glm::mat4( 1 ), glm::vec3( 1.0f / 5, 1.0f / (screen_ratio * 5), 1 ) );
    }


    /// @brief  callback to call whenever the window resizes
    /// @param  size    the new size of the window
    void CameraSystem::onWindowResizeCallback( glm::ivec2 const& size )
    {
        calculateMatrices();
        // recalculate the active camera's height from it's width
        if ( m_ActiveCamera != nullptr )
        {
            m_ActiveCamera->SetWidth( m_ActiveCamera->GetWidth() );
        }
    }


//-----------------------------------------------------------------------------
// singleton stuff
//-----------------------------------------------------------------------------

    CameraSystem::CameraSystem() :
        System( "CameraSystem" )
    {
    }

    CameraSystem* CameraSystem::GetInstance()
    {
        static std::unique_ptr<CameraSystem> s_Instance = nullptr;

        if (s_Instance == nullptr)
        {
            s_Instance.reset(new CameraSystem());
        }
        return s_Instance.get();
    }

//-----------------------------------------------------------------------------
