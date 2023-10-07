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

#include "imgui.h"
#include "implot.h"

//-----------------------------------------------------------------------------
// Class ControlPoint
//-----------------------------------------------------------------------------

    //-----------------------------------------------------------------------------
    // public: constructor / destructor
    //-----------------------------------------------------------------------------

        /// @brief  constructor
        /// @tparam dimensionality  the number of dimensions this curve goes through
        template< int dimensionality >
        ControlPoint< dimensionality >::ControlPoint() :
            M_Value( 0 ),
            M_Derivative( 0 )
        {
            M_Derivative[dimensionality] = 1.0f;
        }

        /// @brief  value constructor
        /// @tparam dimensionality  the number of dimensions this curve goes through
        /// @param  value       the value to assing to this ControlPoint
        template< int dimensionality >
        ControlPoint< dimensionality >::ControlPoint( glm::vec< dimensionality, float > const& value ) :
            M_Value ( 0 ),
            M_Derivative( 0 )
        {
            for ( int i = 0; i < dimensionality; ++i )
            {
                M_Value[i] = value[i];
            }
            M_Derivative[dimensionality] = 1.0f;
        }

    //-----------------------------------------------------------------------------
    // private: reading
    //-----------------------------------------------------------------------------

        /// @brief  reads the time of this ControlPoint
        /// @tparam dimensionality  the number of dimensions this curve goes through
        /// @param  stream      the json data to read from
        template< int dimensionality >
        void ControlPoint< dimensionality >::readTime( Stream stream )
        {
            M_Value[ dimensionality ] = stream.Read<float>();
        }

        /// @brief  reads the value of this ControlPoint
        /// @tparam dimensionality  the number of dimensions this curve goes through
        /// @param  stream      the json data to read from
        template< int dimensionality >
        void ControlPoint< dimensionality >::readValue( Stream stream )
        {
            glm::vec< dimensionality, float > value = stream.Read< glm::vec< dimensionality, float > >();
            for ( int i = 0; i < dimensionality; ++i )
            {
                M_Value[i] = value[i];
            }
        }

        /// @brief  reads the derivative of this ControlPoint
        /// @tparam dimensionality  the number of dimensions this curve goes through
        /// @param  stream      the json data to read from
        template< int dimensionality >
        void ControlPoint< dimensionality >::readDerivative( Stream stream )
        {
            M_Derivative = stream.Read< glm::vec< dimensionality + 1, float > >();
            if ( M_Derivative[ dimensionality ] == 0.0f )
            {
                M_Derivative[ dimensionality ] = 1.0f;
            }
        }

        /// @brief  map of the SceneSystem read methods
        /// @tparam dimensionality  the number of dimensions this curve goes through
        template< int dimensionality >
        ReadMethodMap< ControlPoint < dimensionality > > const ControlPoint< dimensionality >::s_ReadMethods = {
            { "Time"      , &readTime       },
            { "Value"     , &readValue      },
            { "Derivative", &readDerivative }
        };

    //-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// public: constructor / destructor
//-----------------------------------------------------------------------------

    /// @brief  constructor
    /// @tparam dimensionality  the number of dimensions this curve goes through
    template< int dimensionality >
    Curve< dimensionality >::Curve() :
        m_ControlPoints( { ControlPoint< dimensionality >() } ),
        m_CubicCoefficients(),
        m_InterpolationType( InterpolationType::linear ),
        m_IsDirty( true )
    {}

    /// @brief value constructor
    /// @tparam dimensionality  the number of dimensions this curve goes through
    /// @param  value       the value to assign to the initial ControlPoint
    template< int dimensionality >
    Curve< dimensionality >::Curve( glm::vec< dimensionality, float > value ) :
        m_ControlPoints( { ControlPoint< dimensionality >( value ) } ),
        m_CubicCoefficients(),
        m_InterpolationType( InterpolationType::linear ),
        m_IsDirty( true )
    {}

//-----------------------------------------------------------------------------
// public: methods
//-----------------------------------------------------------------------------

    /// @brief  gets the value of this Curve at the specified time
    /// @tparam dimensionality  the number of dimensions this curve goes through
    /// @param  time        the time to sample this Curve at
    /// @return the value of this Curve at the specified time
    template< int dimensionality >
    glm::vec< dimensionality, float > Curve< dimensionality >::GetValueAtTime( float time ) const
    {
        if ( m_IsDirty )
        {
            // recalculate the curve if necessary
            const_cast< Curve< dimensionality >* >(this)->calculate();
        }

        // handle looping cases
        if ( m_IsLooping )
        {
            // wrap time
            time = std::fmod( time, GetTotalTime() );
        }
        else if ( time < 0 )
        {
            // clamp to beginning
            return m_ControlPoints[0].M_Value;
        }
        else if ( time >= GetTotalTime() )
        {
            // clamp to end
            return m_ControlPoints[ m_ControlPoints.size() - 1 ].M_Value;
        }
        

        // sample the curve
        for ( int i = 0; m_ControlPoints.size() - 1; ++i )
        {
            if ( m_ControlPoints[ i + 1 ].GetTime() >= time )
            {
                return interpolate( i, time );
            }
        }

        // if no valid control point was found, throw an error
        std::ostringstream errorMessage;
        errorMessage <<
            "Error: No suitable control point found for time " << time <<
            " while sampling curve";
        throw std::runtime_error( errorMessage.str() );
    }

    /// @brief  displays a 1d in the Inspector
    /// @tparam dimensionality  the number of dimensions this curve goes through
    template< int dimensionality >
    void Curve< dimensionality >::Inspector()
    {
        ImGui::BulletText("Click and drag each point.");

        static ImPlotDragToolFlags flags = ImPlotDragToolFlags_None;
        static ImPlotAxisFlags ax_flags = ImPlotAxisFlags_None;
        if ( ImPlot::BeginPlot( "Curve", ImVec2( -1, 0 ), ImPlotFlags_CanvasOnly ) )
        {
            ImPlot::SetupAxes( 0, 0, ax_flags, ax_flags );
            ImPlot::SetupAxesLimits( 0, 1, 0, 1 );
            static ImPlotPoint P[] = {
                ImPlotPoint( .05f, .05f ),
                ImPlotPoint( 0.2, 0.4 ),
                ImPlotPoint( 0.8, 0.6 ),
                ImPlotPoint( .95f, .95f )
            };

            ImPlot::DragPoint( 0, &P[ 0 ].x, &P[ 0 ].y, ImVec4( 0, 0.9f, 0, 1 ), 4, flags );
            ImPlot::DragPoint( 1, &P[ 1 ].x, &P[ 1 ].y, ImVec4( 1, 0.5f, 1, 1 ), 4, flags );
            ImPlot::DragPoint( 2, &P[ 2 ].x, &P[ 2 ].y, ImVec4( 0, 0.5f, 1, 1 ), 4, flags );
            ImPlot::DragPoint( 3, &P[ 3 ].x, &P[ 3 ].y, ImVec4( 0, 0.9f, 0, 1 ), 4, flags );

            static ImPlotPoint B[100];
            for (int i = 0; i < 100; ++i)
            {
                double t  = i / 99.0;
                double u  = 1 - t;
                double w1 = u*u*u;
                double w2 = 3*u*u*t;
                double w3 = 3*u*t*t;
                double w4 = t*t*t;
                B[i] = ImPlotPoint(
                    w1 * P[ 0 ].x + w2 * P[ 1 ].x + w3 * P[ 2 ].x + w4 * P[ 3 ].x,
                    w1 * P[ 0 ].y + w2 * P[ 1 ].y + w3 * P[ 2 ].y + w4 * P[ 3 ].y
                );
            }


            ImPlot::SetNextLineStyle( ImVec4( 1, 0.5f, 1, 1 ) );
            ImPlot::PlotLine( "##h1", &P[ 0 ].x, &P[ 0 ].y, 2, 0, 0, sizeof( ImPlotPoint ) );

            ImPlot::SetNextLineStyle( ImVec4( 0, 0.5f, 1, 1 ) );
            ImPlot::PlotLine( "##h2", &P[ 2 ].x, &P[ 2 ].y, 2, 0, 0, sizeof( ImPlotPoint ) );

            ImPlot::SetNextLineStyle( ImVec4( 0, 0.9f, 0, 1 ), 2 );
            ImPlot::PlotLine( "##bez", &B[ 0 ].x, &B[ 0 ].y, 100, 0, 0, sizeof( ImPlotPoint ) );

            ImPlot::EndPlot();
        }
    }



//-----------------------------------------------------------------------------
// private: methods
//-----------------------------------------------------------------------------

    /// @brief  calculates the curve, preparing it to be sampled
    /// @tparam dimensionality  the number of dimensions this curve goes through
    template< int dimensionality >
    void Curve< dimensionality >::calculate()
    {
        // ensure that the ControlPoints are in time order
        std::sort( m_ControlPoints.begin(), m_ControlPoints.end() );

        m_CubicCoefficients.clear();

        // calculate the cubic coefficients if necessary
        if ( m_InterpolationType == InterpolationType::cubic )
        {
            calculateCubicCoefficients();
        }
    }

    /// @brief  samples the value of the curve at the specied time after the specified control point
    /// @tparam dimensionality  the number of dimensions this curve goes through
    /// @param  index       the index of the control point to sample from
    /// @param  time        the time to sample the curve at
    /// @return (glm::vec< dimensionality, float >) the value of the curve at the specified time
    template< int dimensionality >
    glm::vec< dimensionality, float > Curve< dimensionality >::interpolate( int index, float time ) const
    {
        switch ( m_InterpolationType )
        {
            case InterpolationType::none:
                // return most recent ControlPoint befote this time
                return m_ControlPoints[ index ].M_Value;

            case InterpolationType::linear:
                // interpolate linearly between this ControlPoint and the next
                time = (time - m_ControlPoints[ index ].GetTime())
                    / (m_ControlPoints[ index + 1 ].GetTime() - m_ControlPoints[ index ].GetTime());
                return lerp( m_ControlPoints[ index ].M_Value, m_ControlPoints[ index + 1 ].M_Value, time );

            case InterpolationType::cubic:
                // interpolate between ControlPoints using precomputed cubic polynomial coefficients
                time = (time - m_ControlPoints[ index ].GetTime())
                    / (m_ControlPoints[ index + 1 ].GetTime() - m_ControlPoints[ index ].GetTime());
                Coefficients coefficients = m_CubicCoefficients[ index ];
                return (coefficients.a * time * time * time) + (coefficients.b * time * time) + (coefficients.c * time) + coefficients.d;

            default:
                throw std::runtime_error( "Error: invalid Curve interpolation type" );
        }
    }

    /// @brief  calculates the coefficients for the cubic polynomials
    /// @tparam dimensionality  the number of dimensions this curve goes through
    template< int dimensionality >
    void Curve< dimensionality >::calculateCubicCoefficients()
    {
        for ( int i = 0; i < m_ControlPoints.size() - 1; ++i ) // last ControlPoint doesn't need coefficients
        {
            glm::vec< dimensionality, float > const& p0 = m_ControlPoints[i].M_Value;
            glm::vec< dimensionality, float > const& p1 = m_ControlPoints[i + 1].M_Value;

            // scale derivative to accommodate for durations other than 1.0
            float duration = m_ControlPoints[i + 1].GetTime() - m_ControlPoints[i].GetTime();
            glm::vec< dimensionality, float > d0 = m_ControlPoints[i].M_Derivative * duration;
            glm::vec< dimensionality, float > d1 = m_ControlPoints[i + 1].M_Derivative * duration;

            // calculate cubic polynomial coefficients such that f(0) = p0, f(1) = p1, f'(0) = d0, and f'(1) = d1
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
    /// @tparam dimensionality  the number of dimensions this curve goes through
    /// @param  stream      the json data to read from
    template< int dimensionality >
    void Curve< dimensionality >::readInterpolationType( Stream stream )
    {
        m_InterpolationType = (InterpolationType)stream.Read<int>();
    }

    /// @brief  reads whether this Curve loops
    /// @tparam dimensionality  the number of dimensions this curve goes through
    /// @param  stream  the json data to read from
    template< int dimensionality >
    void Curve< dimensionality >::readIsLooping( Stream stream )
    {
        m_IsLooping = stream.Read<bool>();
    }

    /// @brief  reads the control points of this Curve
    /// @tparam dimensionality  the number of dimensions this curve goes through
    /// @param  stream  the json data to read from
    template< int dimensionality >
    void Curve< dimensionality >::readControlPoints( Stream stream )
    {
        for ( auto& pointData : stream.GetArray() )
        {
            ControlPoint< dimensionality > point;
            Stream( pointData ).Read( &point );
            AddControlPoint( point );
        }
    }

    /// @brief  map of the SceneSystem read methods
    /// @tparam dimensionality  the number of dimensions this curve goes through
    template< int dimensionality >
    ReadMethodMap< Curve< dimensionality > > const Curve< dimensionality >::s_ReadMethods = {
        { "InterpolationType", &readInterpolationType },
        { "IsLooping"        , &readIsLooping         },
        { "ControlPoints"    , &readControlPoints     }
    };

//-----------------------------------------------------------------------------
