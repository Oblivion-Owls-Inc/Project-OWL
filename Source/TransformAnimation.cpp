/// @file       TransformAnimation.cpp
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      Asset that animates a Transform with Curves
/// @version    0.1
/// @date       2023-10-06
/// 
/// @copyright  Copyright (c) 2023 Digipen Institute of Technology

#include "TransformAnimation.h"

//-----------------------------------------------------------------------------
// public: constructor / destructor
//-----------------------------------------------------------------------------

    /// @brief default constructor
    TransformAnimation::TransformAnimation() :
        m_ScaleCurve( glm::vec2(1.0f, 1.0f) ),
        m_OffsetCurve( glm::vec2(0.0f, 0.0f) ),
        m_RotationCurve( glm::vec1(0.0f) ),
        m_SkewCurve( glm::vec2(0.0f, 0.0f) )
    {}

//-----------------------------------------------------------------------------
// public: methods
//-----------------------------------------------------------------------------

    /// @brief  samples this TransformAnimation at the specified time
    /// @param  time    the time to sample this TransformAnimation at
    /// @return a matrix of the effect transform at the specified time
    glm::mat4 TransformAnimation::SampleAtTime( float time ) const
    {
        glm::mat4 transformation =
            rotationMatrix( m_RotationCurve.GetValueAtTime( time ) ) *
            skewMatrix( m_SkewCurve.GetValueAtTime( time ) ) *
            scaleMatrix( m_ScaleCurve.GetValueAtTime( time ) );

        glm::vec2 offset = m_OffsetCurve.GetValueAtTime( time );
        transformation[3] = { offset.x, offset.y, 0.0f, 1.0f };

        return transformation;
    }


    /// @brief  gets the amount of time this Effect takes to complete
    /// @return the amount of time this Effect takes to complete
    float TransformAnimation::GetTotalTime() const {
        return std::max(
            std::max( m_ScaleCurve.GetTotalTime(), m_OffsetCurve.GetTotalTime() ),
            std::max( m_RotationCurve.GetTotalTime(), m_SkewCurve.GetTotalTime() )
        );
    }

    /// @brief  Used by the DebugSystem to display information about this TransformAnimation
    void TransformAnimation::Inspect()
    {
        if ( ImGui::TreeNode( "Scale Curve" ) )
        {
            m_ScaleCurve.Inspect();

            ImGui::TreePop();
        }

        if ( ImGui::TreeNode( "Offset Curve" ) )
        {
            m_OffsetCurve.Inspect();

            ImGui::TreePop();
        }

        if ( ImGui::TreeNode( "Rotation Curve" ) )
        {
            m_RotationCurve.Inspect();

            ImGui::TreePop();
        }

        if ( ImGui::TreeNode( "Skew Curve" ) )
        {
            m_SkewCurve.Inspect();

            ImGui::TreePop();
        }
    }

//-----------------------------------------------------------------------------
// private: methods
//-----------------------------------------------------------------------------

    /// @brief  Creates a scale matrix
    /// @param  scale   the factor to scale by
    /// @return (glm::mat2) the scale matrix
    glm::mat2 TransformAnimation::scaleMatrix( glm::vec2 scale )
    {
        return {
            { scale.x, 0 },
            { 0, scale.y }
        };
    }

    /// @brief  creates a rotation matrix
    /// @param  rotation    the angle to rotate by
    /// @return (glm::mat2) the rotation matrix
    glm::mat2 TransformAnimation::rotationMatrix( glm::vec1 rotation )
    {
        float cos = std::cosf( rotation[0] );
        float sin = std::sinf( rotation[0] );
        return {
            {  cos, sin },
            { -sin, cos }
        };
    }

    /// @brief  creates a skew matrix
    /// @param  skew    the factor to skew by
    /// @return (glm::mat2) the skew matrix
    glm::mat2 TransformAnimation::skewMatrix( glm::vec2 skew )
    {
        return {
            { 1, skew.y },
            { skew.x, 1 }
        };
    }

//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------

    /// @brief  reads the ScaleCurve
    /// @param  stream the json data to read from
    void TransformAnimation::readScaleCurve( Stream stream )
    {
        stream.Read( &m_ScaleCurve );
    }

    /// @brief  reads the OffsetCurve
    /// @param  stream the json data to read from
    void TransformAnimation::readOffsetCurve( Stream stream )
    {
        stream.Read( &m_OffsetCurve );
    }

    /// @brief  reads the RotationCurve
    /// @param  stream the json data to read from
    void TransformAnimation::readRotationCurve( Stream stream )
    {
        stream.Read( &m_RotationCurve );
    }

    /// @brief  reads the SkewCurve
    /// @param  stream the json data to read from
    void TransformAnimation::readSkewCurve( Stream stream )
    {
        stream.Read( &m_SkewCurve );
    }

    /// @brief  map of the SceneSystem read methods
    ReadMethodMap< TransformAnimation > const TransformAnimation::s_ReadMethods = {
        { "ScaleCurve"   , &readScaleCurve    },
        { "OffsetCurve"  , &readOffsetCurve   },
        { "RotationCurve", &readRotationCurve },
        { "SkewCurve"    , &readSkewCurve     }
    };

//-----------------------------------------------------------------------------
