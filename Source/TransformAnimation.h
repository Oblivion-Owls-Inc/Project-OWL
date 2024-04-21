/// @file       TransformAnimation.h
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      Asset that animates a Transform with Curves
/// @version    0.1
/// @date       2023-10-06
/// 
/// @copyright  Copyright (c) 2024 Digipen Institute of Technology

#pragma once

#include "ISerializable.h"

#include "Curve.h"

/// @brief  Asset that animates a Transform with Curves
class TransformAnimation : public ISerializable
{
//-----------------------------------------------------------------------------
public: // constructor / destructor / Inspector
//-----------------------------------------------------------------------------

    /// @brief default constructor
    TransformAnimation();

    /// @brief Destroys this TransformAnimation
    ~TransformAnimation() = default;

//-----------------------------------------------------------------------------
public: // methods
//-----------------------------------------------------------------------------

    /// @brief  samples this TransformAnimation at the specified time
    /// @param  time    the time to sample this TransformAnimation at
    /// @return a matrix of the effect transform at the specified time
    glm::mat4 SampleAtTime( float time ) const;

    /// @brief  gets the amount of time this Effect takes to complete
    /// @return the amount of time this Effect takes to complete
    float GetTotalTime() const;

    /// @brief Used by the DebugSystem to display information about this TransformAnimation
    void Inspect();

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
private: // reading
//-----------------------------------------------------------------------------

    /// @brief  reads the ScaleCurve
    /// @param  stream the json data to read from
    void readScaleCurve( nlohmann::ordered_json const& data );

    /// @brief  reads the OffsetCurve
    /// @param  stream the json data to read from
    void readOffsetCurve( nlohmann::ordered_json const& data );

    /// @brief  reads the RotationCurve
    /// @param  stream the json data to read from
    void readRotationCurve( nlohmann::ordered_json const& data );

    /// @brief  reads the SkewCurve
    /// @param  stream the json data to read from
    void readSkewCurve( nlohmann::ordered_json const& data );

    /// @brief  map of the SceneSystem read methods
    static ReadMethodMap< TransformAnimation > const s_ReadMethods;


 public:

     /// @brief  gets the TransformAnimation read method map
     /// @return the read method map
     virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override
     {
         return (ReadMethodMap< ISerializable > const&)s_ReadMethods;
     }

     /// @brief  Write all TransformAnimation data to a JSON file.
     /// @return The JSON file containing the TransformAnimation data.
     virtual nlohmann::ordered_json Write() const override;


//-----------------------------------------------------------------------------
};
