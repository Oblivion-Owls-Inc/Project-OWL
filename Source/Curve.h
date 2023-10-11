/// @file       Curve.h
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      class for interpolating between sequences of values
/// @version    0.1
/// @date       2023-10-06
/// 
/// @copyright  Copyright (c) 2023 Digipen Institute of Technology

#pragma once

#define CURVE_H

#include "ISerializable.h"

#include <glm/glm.hpp>
#include <vector>

/// @brief  control points used by this Curve
template< int dimensionality >
class ControlPoint : public ISerializable
{
//-----------------------------------------------------------------------------
public: // constructor / destructor
//-----------------------------------------------------------------------------

    /// @brief  constructor
    ControlPoint();

    /// @brief  value constructor
    /// @param  value       the value to assign to this ControlPoint
    /// @param  time        the time to assign to this ControlPoint
    /// @param  derivative  the derivative to assign to this ControlPoint
    ControlPoint( glm::vec< dimensionality, float > const& value, float time = 0.0f, glm::vec< dimensionality, float > const& derivative = glm::vec< dimensionality, float >( 0 ) );

    /// @brief  destructor
    ~ControlPoint() = default;

//-----------------------------------------------------------------------------
public: // accessors
//-----------------------------------------------------------------------------
    
    /// @brief  gets the time of this ControlPoint
    /// @return the time of this ControlPoint
    float GetTime() const { return M_Value[ dimensionality ]; }

    /// @brief  sets the time of this ControlPoint
    /// @return the time of this ControlPoint
    void SetTime( float time ) { M_Value[ dimensionality ] = time; }

//-----------------------------------------------------------------------------
public: // comparison operator
//-----------------------------------------------------------------------------

    /// @brief  checks if one ControlPoint is before another (for sorting)
    /// @param  other   the ControlPoint to compare against
    /// @return whether this ControlPoint is before the other
    bool operator <( ControlPoint const& other ) const { return GetTime() < other.GetTime(); }
    
//-----------------------------------------------------------------------------
public: // member variables
//-----------------------------------------------------------------------------

    /// @brief  the value at this ControlPoint
    glm::vec< dimensionality + 1, float > M_Value;

    /// @brief  the value at this ControlPoint
    glm::vec< dimensionality, float > M_Derivative;

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
    static ReadMethodMap< ControlPoint< dimensionality > > const s_ReadMethods;

    /// @brief  gets the Curve read method map
    /// @return the read method map
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override
    {
        return (ReadMethodMap< ISerializable > const&)s_ReadMethods;
    }

//-----------------------------------------------------------------------------
};


template< int dimensionality >
class Curve : public ISerializable
{
//-----------------------------------------------------------------------------
public: // constructor / destructor
//-----------------------------------------------------------------------------

    /// @brief constructor
    Curve();

    /// @brief value constructor
    /// @param  value   the value to assign to the initial ControlPoint
    Curve( glm::vec< dimensionality, float > value );

    /// @brief destructor
    ~Curve() = default;

//-----------------------------------------------------------------------------
public: // methods
//-----------------------------------------------------------------------------

    /// @brief  gets the value of this Curve at the specified time
    /// @param  time    the time to sample this Curve at
    /// @return the value of this Curve at the specified time
    glm::vec< dimensionality, float > GetValueAtTime( float time ) const;

    /// @brief  marks this Curve as dirty to be recalculated
    void MarkDirty() { m_IsDirty = true; }

    /// @brief  displays this curve in the Inspector
    void Inspect();

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
    __inline int GetControlPointCount() const { return m_ControlPoints.dimensionality(); }


    /// @brief  gets the total amount of time the Curve takes up
    /// @return the total time of the Curve
    __inline float GetTotalTime() const { return m_ControlPoints[ m_ControlPoints.size() - 1 ].GetTime(); }


    /// @brief  gets the smallest values of each axis among all ControlPoints
    /// @return the smallest values of each axis among all ControlPoints
    __inline glm::vec< dimensionality, float > const& GetMinPointValue() const { if ( m_IsDirty ) const_cast< Curve<dimensionality>* >(this)->calculate; return m_MinPointValue; }

    /// @brief  gets the largest values of each axis among all ControlPoints
    /// @return the largest values of each axis among all ControlPoints
    __inline glm::vec< dimensionality, float > const& GetMaxPointValue() const { if ( m_IsDirty ) const_cast< Curve<dimensionality>* >(this)->calculate; return m_MaxPointValue; }


    /// @brief  gets a reference to the ControlPoint at the specified index
    /// @param  index   the index to get the control point at
    /// @return a reference to the ControlPoint
    __inline ControlPoint< dimensionality > const& operator []( int index ) const { if ( m_IsDirty ) const_cast< Curve<dimensionality>* >(this)->calculate; return m_ControlPoints[ index ]; }

    /// @brief  gets a reference to the ControlPoint at the specified index
    /// @param  index   the index to get the control point at
    /// @return a reference to the ControlPoint
    /// @note   this method DOES NOT automatically clean up the Curve
    __inline ControlPoint< dimensionality >& operator []( int index ) { return m_ControlPoints[ index ]; MarkDirty(); }


    /// @brief  adds a control point to the Curve
    /// @param  controlPoint    the control point to add
    __inline void AddControlPoint( ControlPoint< dimensionality > const& controlPoint ) { m_ControlPoints.push_back( controlPoint ); MarkDirty(); }

    /// @brief  removes a control point to the Curve
    /// @param  index   the index to remove a ControlPoint from
    __inline void RemoveControlPoint( int index ) { m_ControlPoints.erase( m_ControlPoints.begin() + index ); MarkDirty(); }


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

    /// @brief  cubic polynomial coefficients for cubic interpolation
    struct Coefficients
    {
        glm::vec< dimensionality, float > a, b;
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
    glm::vec< dimensionality, float > interpolate( int index, float time ) const;

    /// @brief  calculates the coefficients for the cubic polynomials
    void calculateCubicCoefficients();


//-----------------------------------------------------------------------------
private: // member variables
//-----------------------------------------------------------------------------

    /// @brief  the control points in this Curve
    std::vector< ControlPoint< dimensionality > > m_ControlPoints;

    /// @brief  the coefficients of the cubic polynomials (if interpolation is cubic)
    std::vector< Coefficients > m_CubicCoefficients;

    /// @brief  the interpolation type of this Curve
    InterpolationType m_InterpolationType;

    /// @brief  whether or not the curve loops out of range
    bool m_IsLooping;

    /// @brief  whether the Curve needs to be recalculated
    bool m_IsDirty;

    /// @brief  the smallest value among all points
    glm::vec< dimensionality, float > m_MinPointValue;

    /// @brief  the largest value among all points
    glm::vec< dimensionality, float > m_MaxPointValue;

    /// @brief  unique id for this curve
    unsigned m_Id;

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

    /// @brief  gets the Curve read method map
    /// @return the read method map
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override
    {
        return (ReadMethodMap< ISerializable > const&)s_ReadMethods;
    }

//-----------------------------------------------------------------------------
};

#ifndef CURVE_C
#include "Curve.cpp"
#endif
