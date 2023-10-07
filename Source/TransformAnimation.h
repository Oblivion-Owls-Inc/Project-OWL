/// @file       TransformAnimation.h
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      Asset that animates a Transform with Curves
/// @version    0.1
/// @date       2023-10-06
/// 
/// @copyright  Copyright (c) 2023 Digipen Institute of Technology

#pragma once

#include "Stream.h"

#include "Curve.h"

/// @brief  Asset that animates a Transform with Curves
class TransformAnimation
{
//-----------------------------------------------------------------------------
public: // constructor / destructor / Inspector
//-----------------------------------------------------------------------------

    /// @brief default constructor
    TransformAnimation();

    /// @brief Destroys this TransformAnimation
    ~TransformAnimation() = default;

    /// @brief Used by the DebugSystem to display information about this TransformAnimation
    void Inspect() {};

//-----------------------------------------------------------------------------
public: // methods
//-----------------------------------------------------------------------------

    glm::mat4 SampleAtTime( float time ) const;

    
    /// @brief  gets the amount of time this Effect takes to complete
    /// @return the amount of time this Effect takes to complete
    float GetTotalTime() const;

//-----------------------------------------------------------------------------
public: // accessors
//-----------------------------------------------------------------------------

    /// @brief  gets the ScaleCurve
    /// @return the ScaleCurve
    __inline Curve< 2 >& GetScaleCurve() { return m_ScaleCurve; }

    /// @brief  gets the OffsetCurve
    /// @return the OffsetCurve
    __inline Curve< 2 >& GetOffsetCurve() { return m_OffsetCurve; }

    /// @brief  gets the RotationCurve
    /// @return the RotationCurve
    __inline Curve< 1 >& GetRotationCurve() { return m_RotationCurve; }

    /// @brief  gets the SkewCurve
    /// @return the SkewCurve
    __inline Curve< 2 >& GetSkewCurve() { return m_SkewCurve; }

//-----------------------------------------------------------------------------
private: // member variables
//-----------------------------------------------------------------------------

    /// @brief  curve that animates scale
    Curve< 2 > m_ScaleCurve;

    /// @brief  curve that animates offset
    Curve< 2 > m_OffsetCurve;

    /// @brief  curve that animates rotation
    Curve< 1 > m_RotationCurve;

    /// @brief  curve that animates skew
    Curve< 2 > m_SkewCurve;

//-----------------------------------------------------------------------------
private: // methods
//-----------------------------------------------------------------------------
    
    /// @brief  Creates a scale matrix
    /// @param  scale   the factor to scale by
    /// @return (glm::mat2) the scale matrix
    static glm::mat2 scaleMatrix( glm::vec2 scale );

    /// @brief  creates a rotation matrix
    /// @param  rotation    the angle to rotate by
    /// @return (glm::mat2) the rotation matrix
    static glm::mat2 rotationMatrix( glm::vec1 rotation );

    /// @brief  creates a skew matrix
    /// @param  skew    the factor to skew by
    /// @return (glm::mat2) the skew matrix
    static glm::mat2 skewMatrix( glm::vec2 skew );

//-----------------------------------------------------------------------------
public: // reading
//-----------------------------------------------------------------------------

    /// @brief  reads the ScaleCurve
    /// @param  stream the json data to read from
    void readScaleCurve( Stream stream );

    /// @brief  reads the OffsetCurve
    /// @param  stream the json data to read from
    void readOffsetCurve( Stream stream );

    /// @brief  reads the RotationCurve
    /// @param  stream the json data to read from
    void readRotationCurve( Stream stream );

    /// @brief  reads the SkewCurve
    /// @param  stream the json data to read from
    void readSkewCurve( Stream stream );

    /// @brief  gets the TransformAnimation read method map
    /// @return the read method map
    static __inline ReadMethodMap< TransformAnimation > const& GetReadMethods() { return s_ReadMethods; }

    void AfterLoad() {}

//-----------------------------------------------------------------------------
private: // reading
//-----------------------------------------------------------------------------

    /// @brief  map of the SceneSystem read methods
    static ReadMethodMap< TransformAnimation > const s_ReadMethods;

//-----------------------------------------------------------------------------
};
