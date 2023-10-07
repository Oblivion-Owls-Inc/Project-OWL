/// @file       Curve.cpp
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      class for interpolating between sequences of values
/// @version    0.1
/// @date       2023-10-06
/// 
/// @copyright  Copyright (c) 2023 Digipen Institute of Technology

#pragma once

#define CURVE_C

#ifndef CURVE_H
#include "Curve.h"
#endif

#include "basics.h"

#include <glm/glm.hpp>
#include <vector>
#include <algorithm>
//-----------------------------------------------------------------------------
// Class ControlPoint
//-----------------------------------------------------------------------------

    //-----------------------------------------------------------------------------
    // public: constructor / destructor
    //-----------------------------------------------------------------------------

        /// @brief  constructor
        /// @tparam ValueType   the type of value that this Curve interpolates between
        template < typename ValueType >
        ControlPoint< ValueType >::ControlPoint() :
            M_Time( 0.0f ),
            M_Value( 0 ),
            M_Derivative( 0 )
        {}

        /// @brief  value constructor
        /// @tparam ValueType   the type of value that this Curve interpolates between
        /// @param  value       the value to assing to this ControlPoint
        template < typename ValueType >
        ControlPoint< ValueType >::ControlPoint( ValueType value ) :
            M_Time( 0.0f ),
            M_Value( value ),
            M_Derivative( 0 )
        {}

        //-----------------------------------------------------------------------------
        // private: reading
        //-----------------------------------------------------------------------------

        /// @brief  reads the time of this ControlPoint
        /// @tparam ValueType   the type of value that this Curve interpolates between
        /// @param  stream      the json data to read from
        template < typename ValueType >
        void ControlPoint< ValueType >::readTime( Stream stream )
        {
            M_Time = stream.Read<float>();
        }

        /// @brief  reads the value of this ControlPoint
        /// @tparam ValueType   the type of value that this Curve interpolates between
        /// @param  stream      the json data to read from
        template < typename ValueType >
        void ControlPoint< ValueType >::readValue( Stream stream )
        {
            M_Value = stream.Read< ValueType >();
        }

        /// @brief  reads the derivative of this ControlPoint
        /// @tparam ValueType   the type of value that this Curve interpolates between
        /// @param  stream      the json data to read from
        template < typename ValueType >
        void ControlPoint< ValueType >::readDerivative( Stream stream )
        {
            M_Derivative = stream.Read< ValueType >();
        }

        /// @brief  map of the SceneSystem read methods
        /// @tparam ValueType   the type of value that this Curve interpolates between
        template < typename ValueType >
        ReadMethodMap< ControlPoint < ValueType > > const ControlPoint< ValueType >::s_ReadMethods = {
            { "Time"      , &readTime       },
            { "Value"     , &readValue      },
            { "Derivative", &readDerivative }
        };

    //-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// public: constructor / destructor
//-----------------------------------------------------------------------------

    /// @brief  constructor
    /// @tparam ValueType   the type of value that this Curve interpolates between
    template < typename ValueType >
    Curve< ValueType >::Curve() :
        m_ControlPoints( { ControlPoint<ValueType>() } ),
        m_CubicCoefficients(),
        m_InterpolationType( InterpolationType::linear ),
        m_IsDirty( true )
    {}

    /// @brief value constructor
    /// @tparam ValueType   the type of value that this Curve interpolates between
    /// @param  value       the value to assign to the initial ControlPoint
    template < typename ValueType >
    Curve< ValueType >::Curve( ValueType value ) :
        m_ControlPoints( { ControlPoint<ValueType>( value ) } ),
        m_CubicCoefficients(),
        m_InterpolationType( InterpolationType::linear ),
        m_IsDirty( true )
    {}

//-----------------------------------------------------------------------------
// public: methods
//-----------------------------------------------------------------------------

    /// @brief  gets the value of this Curve at the specified time
    /// @tparam ValueType   the type of value that this Curve interpolates between
    /// @param  time        the time to sample this Curve at
    /// @return the value of this Curve at the specified time
    template < typename ValueType >
    ValueType Curve< ValueType >::GetValueAtTime( float time ) const
    {
        if ( m_IsDirty )
        {
            const_cast< Curve< ValueType >* >(this)->calculate();
        }

        // handle looping cases
        if ( m_IsLooping )
        {
            time -= (int)(time / GetTotalTime()) * GetTotalTime();
        }
        else if ( time < 0 )
        {
            return m_ControlPoints[0].M_Value;
        }
        else if ( time >= GetTotalTime() )
        {
            return m_ControlPoints[ m_ControlPoints.size() - 1 ].M_Value;
        }

        for ( int i = 0; m_ControlPoints.size() - 1; ++i )
        {
            if ( m_ControlPoints[i + 1].M_Time >= time )
            {
                return interpolate( i, time );
            }
        }

        std::ostringstream errorMessage;
        errorMessage <<
            "Error: No suitable control point found for time " << time <<
            " while sampling curve";
        throw std::runtime_error( errorMessage.str() );
    }

//-----------------------------------------------------------------------------
// private: methods
//-----------------------------------------------------------------------------

    /// @brief  calculates the curve, preparing it to be sampled
    /// @tparam ValueType   the type of value that this Curve interpolates between
    template < typename ValueType >
    void Curve< ValueType >::calculate()
    {
        std::sort( m_ControlPoints.begin(), m_ControlPoints.end(), []( ControlPoint<ValueType> const& a, ControlPoint<ValueType> const& b ) -> bool {
            return a.M_Time < b.M_Time;
        });

        if ( m_InterpolationType == InterpolationType::cubic )
        {
            calculateCubicCoefficients();
        }
        else
        {
            m_CubicCoefficients.clear();
        }
    }

    /// @brief  samples the value of the curve at the specied time after the specified control point
    /// @tparam ValueType   the type of value that this Curve interpolates between
    /// @param  index       the index of the control point to sample from
    /// @param  time        the time to sample the curve at
    /// @return (ValueType) the value of the curve at the specified time
    template < typename ValueType >
    ValueType Curve< ValueType >::interpolate( int index, float time ) const
    {
        switch ( m_InterpolationType )
        {
            case InterpolationType::none:
                return m_ControlPoints[index].M_Value;
            case InterpolationType::linear:
                time = (time - m_ControlPoints[index].M_Time) / (m_ControlPoints[index + 1].M_Time - m_ControlPoints[index].M_Time);
                return lerp( m_ControlPoints[index].M_Value, m_ControlPoints[index + 1].M_Value, time );
            case InterpolationType::cubic:
                time = (time - m_ControlPoints[index].M_Time) / (m_ControlPoints[index + 1].M_Time - m_ControlPoints[index].M_Time);
                Coefficients coef = m_CubicCoefficients[index];
                return (coef.a * time * time * time) + (coef.b * time * time) + (coef.c * time) + coef.d;
            default:
                throw std::runtime_error( "Error: invalid interpolation type" );
        }
    }

    /// @brief  calculates the coefficients for the cubic polynomials
    /// @tparam ValueType   the type of value that this Curve interpolates between
    template < typename ValueType >
    void Curve< ValueType >::calculateCubicCoefficients()
    {
        m_CubicCoefficients.clear();
        for ( int i = 0; i < m_ControlPoints.size() - 1; ++i )
        {
            ValueType const& p0 = m_ControlPoints[i].M_Value;
            ValueType const& p1 = m_ControlPoints[i + 1].M_Value;
            float duration = m_ControlPoints[i + 1].M_Time - m_ControlPoints[i].M_Time;
            ValueType d0 = m_ControlPoints[i].M_Derivative * duration;
            ValueType d1 = m_ControlPoints[i + 1].M_Derivative * duration;
            m_CubicCoefficients.push_back( {
                2.0f * p0 - 2.0f * p1 + d0 + d1,
                -3.0f * p0 + 3.0f * p1 - 2.0f * d0 - d1,
                d0,
                p0
            } );
        }
    }

//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------

    /// @brief  reads the interpolation type of this Curve
    /// @tparam ValueType   the type of value that this Curve interpolates between
    /// @param  stream      the json data to read from
    template < typename ValueType >
    void Curve< ValueType >::readInterpolationType( Stream stream )
    {
        m_InterpolationType = (InterpolationType)stream.Read<int>();
    }

    /// @brief  reads whether this Curve loops
    /// @tparam ValueType   the type of value that this Curve interpolates between
    /// @param  stream  the json data to read from
    template < typename ValueType >
    void Curve< ValueType >::readIsLooping( Stream stream )
    {
        m_IsLooping = stream.Read<bool>();
    }

    /// @brief  reads the control points of this Curve
    /// @tparam ValueType   the type of value that this Curve interpolates between
    /// @param  stream  the json data to read from
    template < typename ValueType >
    void Curve< ValueType >::readControlPoints( Stream stream )
    {
        for ( auto& pointData : stream.GetArray() )
        {
            ControlPoint<ValueType> point;
            Stream( pointData ).Read( &point );
            AddControlPoint( point );
        }
    }

    /// @brief  map of the SceneSystem read methods
    /// @tparam ValueType   the type of value that this Curve interpolates between
    template < typename ValueType >
    ReadMethodMap< Curve< ValueType > > const Curve< ValueType >::s_ReadMethods = {
        { "InterpolationType", &readInterpolationType },
        { "IsLooping"        , &readIsLooping         },
        { "ControlPoints"    , &readControlPoints     }
    };

//-----------------------------------------------------------------------------
