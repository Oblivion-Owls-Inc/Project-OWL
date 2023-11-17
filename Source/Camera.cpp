/// @file       Camera.hcpp
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu), Eli Tsereteli (ilya.tsereteli@digipen.edu)
/// @brief      Camera component for rendering
/// @version    0.1
/// @date       2023-10-22
/// 
/// @copyright  Copyright (c) 2023 Digipen Institute of Technology

#pragma once

#include "Camera.h"
#include "Transform.h"

#include "CameraSystem.h"
#include "PlatformSystem.h"

#include "Entity.h"

#include <imgui.h>


//-------------------------------------------------------------------------
// public: constructor
//-------------------------------------------------------------------------

    /// @brief  constructor
    Camera::Camera() :
        Component( typeid( Camera ) )
    {}

//-------------------------------------------------------------------------
// public: accessors
//-------------------------------------------------------------------------


    /// @brief  gets the width of the Camera
    /// @return the width of the camera
    float Camera::GetWidth() const
    {
        return m_Scale;
    }

    /// @brief  sets the width of the camera
    /// @param  width   the width of the camera
    /// @note   also overrides height based on viewport aspect ratio
    void Camera::SetWidth( float width )
    {
        m_Scale = width;

        calculateCameraToClip();
        m_WorldToClipDirty = true;
        m_ClipToWorldDirty = true;
    }


    /// @brief  gets the height of the Camera
    /// @return the height of the camera
    float Camera::GetHeight() const
    {
        glm::vec2 windowSize = PlatformSystem::GetInstance()->GetWindowDimensions();

        return m_Scale * windowSize.y / windowSize.x;
    }

    /// @brief  sets the height of the camera
    /// @param  height  the height of the camera
    /// @note   also overrides width based on viewport aspect ratio
    void Camera::SetHeight( float height )
    {
        glm::vec2 windowSize = PlatformSystem::GetInstance()->GetWindowDimensions();

        m_Scale = height * windowSize.x / windowSize.y;

        calculateCameraToClip();
        m_WorldToClipDirty = true;
        m_ClipToWorldDirty = true;
    }


    /// @brief  gets the world space to clip space matrix
    /// @return the world space to clip space matrix
    glm::mat4 const& Camera::GetWorldToClip() const
    {
        if ( m_WorldToClipDirty )
        {
            const_cast< Camera* >( this )->calculateWorldToClip();
        }

        return m_WorldToClip;
    }

    /// @brief  gets the clip space to world space matrix
    /// @return the clip space to world space matrix
    glm::mat4 const& Camera::GetClipToWorld() const
    {
        if ( m_ClipToWorldDirty )
        {
            const_cast< Camera* >( this )->calculateClipToWorld();
        }

        return m_ClipToWorld;
    }


//-------------------------------------------------------------------------
// public: methods
//-------------------------------------------------------------------------
    
    /// @brief  sets this Camera as the active Camera in the scene
    void Camera::SetActive()
    {
        Cameras()->SetActiveCamera( this );
    }

//-------------------------------------------------------------------------
// private: virtual override methods
//-------------------------------------------------------------------------
    
    /// @brief  called once when entering the scene
    void Camera::OnInit()
    {
        m_Transform = GetEntity()->GetComponent< Transform >();

        m_Transform->AddOnTransformChangedCallback( GetId(), std::bind( &Camera::onTransformChangedCallback, this ) );

        if ( m_IsActive )
        {
            SetActive();
        }
    }

    /// @brief  called once when leaving the scene
    void Camera::OnExit()
    {
        m_Transform->RemoveOnTransformChangedCallback( GetId() );

        if ( m_IsActive )
        {
            Cameras()->SetActiveCamera( nullptr );
        }
    }


    /// @brief  shows the Inspector window
    void Camera::Inspector()
    {
        // Edit width
        float width = GetWidth();
        if ( ImGui::DragFloat( "Width", &width, 0.05f ) )
        {
            SetWidth( width );
        }

        // Edit height
        float height = GetHeight();
        if ( ImGui::DragFloat( "Height", &height, 0.05f ) )
        {
            SetHeight( height );
        }
    }


//-------------------------------------------------------------------------
// private: methods
//-------------------------------------------------------------------------

    /// @brief  called whenever this Camera's Transform changes
    void Camera::onTransformChangedCallback()
    {
        m_WorldToCameraDirty = true;
        m_WorldToClipDirty = true;
        m_ClipToWorldDirty = true;
    }

    /// @brief  calculates to WorldToCamera matrix
    void Camera::calculateWorldToCamera()
    {
        glm::mat2 linearPart = m_Transform->GetMatrix();
        linearPart = glm::inverse( linearPart );

        glm::vec2 translation = -(linearPart * m_Transform->GetTranslation());

        m_WorldToCamera = linearPart;
        m_WorldToCamera[ 3 ] = glm::vec4( translation, 0, 1 );

        m_WorldToCameraDirty = false;
    }

    /// @brief  calculates to CameraToClip matrix
    void Camera::calculateCameraToClip()
    {
        m_CameraToClip[ 0 ][ 0 ] = 2.0f / GetWidth();
        m_CameraToClip[ 1 ][ 1 ] = 2.0f / GetHeight();
    }

    /// @brief  calculates to WorldToClip matrix
    void Camera::calculateWorldToClip()
    {
        if ( m_WorldToCameraDirty )
        {
            calculateWorldToCamera();
        }

        m_WorldToClip = m_CameraToClip * m_WorldToCamera;

        m_WorldToClipDirty = false;
    }

    /// @brief  calculates to ClipToWorld matrix
    void Camera::calculateClipToWorld()
    {
        if ( m_WorldToClipDirty )
        {
            calculateWorldToClip();
        }

        glm::mat2 linearPart = m_WorldToClip;
        linearPart = glm::inverse( linearPart );

        glm::vec2 translation = -(linearPart * m_WorldToClip[ 3 ]);

        m_ClipToWorld = linearPart;
        m_ClipToWorld[ 3 ] = glm::vec4( translation, 0, 1 );

        m_ClipToWorldDirty = false;
    }

//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------

    /// @brief  reads the width of this Camera
    /// @param  json    the json data to read from
    void Camera::readWidth( nlohmann::ordered_json const& json )
    {
        SetWidth( Stream::Read< float >( json ) );
    }

    /// @brief  reads the height of this Camera
    /// @param  json    the json data to read from
    void Camera::readHeight( nlohmann::ordered_json const& json )
    {

        SetHeight( Stream::Read< float >( json ) );
    }

    /// @brief  reads the active state of this Camera
    /// @param  json    the json data to read from
    void Camera::readIsActive( nlohmann::ordered_json const& json )
    {
        m_IsActive = Stream::Read< bool >( json );
    }

    /// @brief map of the CameraSystem read methods
    ReadMethodMap< Camera > const Camera::s_ReadMethods = {
        { "Width",    &readWidth    },
        { "Height",   &readHeight   },
        { "IsActive", &readIsActive }
    };

//-----------------------------------------------------------------------------
// public: writing
//-----------------------------------------------------------------------------

    /// @brief  writes this Camera to json
    /// @return the written json data
    nlohmann::ordered_json Camera::Write() const
    {
        nlohmann::ordered_json json;

        json[ "Width" ] = Stream::Write( m_Scale );
        json[ "IsActive" ] = Stream::Write( m_IsActive );

        return json;
    }

//-----------------------------------------------------------------------------
// private: copying
//-----------------------------------------------------------------------------

    /// @brief  copy constructor
    /// @param  other   the Camera to copy
    Camera::Camera( Camera const& other ) :
        Component( other ),
        m_Scale( other.m_Scale )
    {}

//-----------------------------------------------------------------------------
