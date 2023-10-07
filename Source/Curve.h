/// @file       Curve.h
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      class for interpolating between sequences of values
/// @version    0.1
/// @date       2023-10-06
/// 
/// @copyright  Copyright (c) 2023 Digipen Institute of Technology

#pragma once

#define CURVE_H

#include "Stream.h"

#include <glm/glm.hpp>
#include <vector>

/// @brief  control points used by this Curve
template< typename ValueType >
class ControlPoint
{
//-----------------------------------------------------------------------------
public: // constructor / destructor
//-----------------------------------------------------------------------------

    /// @brief  constructor
    ControlPoint();

    /// @brief  value constructor
    /// @param  value   the value to assign to this ControlPoint
    ControlPoint( ValueType value );

    /// @brief  destructor
    ~ControlPoint() = default;

//-----------------------------------------------------------------------------
public: // member variables
//-----------------------------------------------------------------------------

    /// @brief  the time value this ControlPoint is located at
    float M_Time;

    /// @brief  the value at this ControlPoint
    ValueType M_Value;

    /// @brief  the value at this ControlPoint
    ValueType M_Derivative;

//-----------------------------------------------------------------------------
public: // reading
//-----------------------------------------------------------------------------

    /// @brief  gets the Curve read method map
    /// @return the read method map
    static __inline ReadMethodMap< ControlPoint > const& GetReadMethods() { return s_ReadMethods; }

    void AfterRead() {}

//-----------------------------------------------------------------------------
private: // reading
//-----------------------------------------------------------------------------

    /// @brief  reads the time of this ControlPoint
    /// @param  stream  the json data to read from
    void readTime( Stream stream );

    /// @brief  reads the value of this ControlPoint
    /// @param  stream  the json data to read from
    void readValue( Stream stream );

    /// @brief  reads the derivative of this ControlPoint
    /// @param  stream  the json data to read from
    void readDerivative( Stream stream );

    /// @brief  map of the SceneSystem read methods
    static ReadMethodMap< ControlPoint > const s_ReadMethods;

//-----------------------------------------------------------------------------
};





template < typename ValueType >
class Curve
{

//-----------------------------------------------------------------------------
public: // constructor / destructor
//-----------------------------------------------------------------------------

    /// @brief constructor
    Curve();

    /// @brief value constructor
    /// @param  value   the value to assign to the initial ControlPoint
    Curve( ValueType value );

    /// @brief destructor
    ~Curve() = default;

//-----------------------------------------------------------------------------
public: // methods
//-----------------------------------------------------------------------------

    /// @brief  gets the value of this Curve at the specified time
    /// @param  time    the time to sample this Curve at
    /// @return the value of this Curve at the specified time
    ValueType GetValueAtTime( float time ) const;

    /// @brief  marks this Curve as dirty to be recalculated
    __inline void MarkDirty() { m_IsDirty = true; }

//-----------------------------------------------------------------------------
public: // types
//-----------------------------------------------------------------------------


    /// @brief  the type of interpolation this Curve uses
    enum class InterpolationType
    {
        none = 0,
        linear = 1,
        cubic = 2
    };

//-----------------------------------------------------------------------------
public: // accessors
//-----------------------------------------------------------------------------


    /// @brief  gets how many control points are in this Curve
    /// @return the nuber of control points in this Curve
    __inline int GetControlPointCount() const { return m_ControlPoints.size(); }

    /// @brief  gets the total amount of time the Curve takes up
    /// @return the total time of the Curve
    __inline float GetTotalTime() const { return m_ControlPoints[ m_ControlPoints.size() - 1 ].M_Time; }

    /// @brief  gets a reference to the ControlPoint at the specified index
    /// @param  index   the index to get the control point at
    /// @return a reference to the ControlPoint
    __inline ControlPoint<ValueType> const& operator []( int index ) const { return m_ControlPoints[ index ]; }

    /// @brief  gets a reference to the ControlPoint at the specified index
    /// @param  index   the index to get the control point at
    /// @return a reference to the ControlPoint
    __inline ControlPoint<ValueType>& operator []( int index ) { return m_ControlPoints[ index ]; MarkDirty(); }

    /// @brief  adds a control point to the Curve
    /// @param  controlPoint    the control point to add
    __inline void AddControlPoint( ControlPoint<ValueType> const& controlPoint ) { m_ControlPoints.push_back( controlPoint ); MarkDirty(); }

    /// @brief  gets this Curve's interpolation type
    /// @return this Curve's interpolation type
    __inline InterpolationType GetInterpolationType() const { return m_InterpolationType; }

    /// @brief  sets this Curve's interpolation type
    /// @param  interpolationType   the interpolation type to set this Curve to
    __inline void SetInterpolationType( InterpolationType interpolationType ) { m_InterpolationType = interpolationType; MarkDirty(); }

    /// @brief  gets whether this curve loops
    /// @return whether this curve loops
    __inline bool GetIsLooping() const { return m_IsLooping; }

    /// @brief  sets whether this curve loops
    /// @param  isLooping   whether this curve is looping
    __inline void SetIsLooping( bool isLooping ) { m_IsLooping = isLooping; MarkDirty(); }

//-----------------------------------------------------------------------------
private: // types
//-----------------------------------------------------------------------------

    struct Coefficients
    {
        ValueType a, b, c, d;
    };

//-----------------------------------------------------------------------------
private: // methods
//-----------------------------------------------------------------------------
    
    /// @brief  calculates the curve, preparing it to be sampled
    void calculate();

    /// @brief  samples the value of the curve at the specied time after the specified control point
    /// @param  index   the index of the control point to sample from
    /// @param  time    the time to sample the curve at
    /// @return (ValueType) the value of the curve at the specified time
    ValueType interpolate( int index, float time ) const;

    /// @brief  calculates the coefficients for the cubic polynomials
    void calculateCubicCoefficients();

//-----------------------------------------------------------------------------
private: // member variables
//-----------------------------------------------------------------------------

    /// @brief  the control points in this Curve
    std::vector< ControlPoint<ValueType> > m_ControlPoints;

    /// @brief  the coefficients of the cubic polynomials (if interpolation is cubic)
    std::vector< Coefficients > m_CubicCoefficients;

    /// @brief  the interpolation type of this Curve
    InterpolationType m_InterpolationType;

    /// @brief  whether or not the curve loops out of range
    bool m_IsLooping;

    /// @brief  whether the Curve needs to be recalculated
    bool m_IsDirty;

//-----------------------------------------------------------------------------
public: // reading
//-----------------------------------------------------------------------------

    /// @brief  gets the Curve read method map
    /// @return the read method map
    static __inline ReadMethodMap< Curve > const& GetReadMethods() { return s_ReadMethods; }

//-----------------------------------------------------------------------------
private: // reading
//-----------------------------------------------------------------------------

    /// @brief  reads the interpolation type of this Curve
    /// @param  stream  the json data to read from
    void readInterpolationType( Stream stream );

    /// @brief  reads whether this Curve loops
    /// @param  stream  the json data to read from
    void readIsLooping( Stream stream );

    /// @brief  reads the control points of this Curve
    /// @param  stream  the json data to read from
    void readControlPoints( Stream stream );

    /// @brief  map of the SceneSystem read methods
    static ReadMethodMap< Curve > const s_ReadMethods;

//-----------------------------------------------------------------------------
};

#ifndef CURVE_C
#include "Curve.cpp"
#endif
