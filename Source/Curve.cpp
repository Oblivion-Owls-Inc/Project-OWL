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
        {}

        /// @brief  value constructor
        /// @tparam dimensionality  the number of dimensions this curve goes through
        /// @param  value       the value to assing to this ControlPoint
        /// @param  time        the time to assign to this ControlPoint
        /// @param  derivative  the derivative to assign to this ControlPoint
        template< int dimensionality >
        ControlPoint< dimensionality >::ControlPoint( glm::vec< dimensionality, float > const& value, float time, glm::vec< dimensionality, float > const& derivative ) :
            M_Derivative( derivative )
        {
            for ( int i = 0; i < dimensionality; ++i )
            {
                M_Value[ i ] = value[ i ];
            }
            M_Value[ dimensionality ] = time;
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
            M_Derivative = stream.Read< glm::vec< dimensionality, float > >();
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
        m_IsDirty( true ),
        m_Id( GetUniqueId() )
    {}

    /// @brief value constructor
    /// @tparam dimensionality  the number of dimensions this curve goes through
    /// @param  value       the value to assign to the initial ControlPoint
    template< int dimensionality >
    Curve< dimensionality >::Curve( glm::vec< dimensionality, float > value ) :
        m_ControlPoints( { ControlPoint< dimensionality >( value ) } ),
        m_CubicCoefficients(),
        m_InterpolationType( InterpolationType::linear ),
        m_IsDirty( true ),
        m_Id( GetUniqueId() )
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

        if ( m_ControlPoints.size() == 1 )
        {
            return m_ControlPoints[0].M_Value;
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
    void Curve< dimensionality >::Inspect()
    {
        // interpolation type dropdown
        static char const* interpolationTypes[ 3 ] = { "None", "Linear", "Cubic" };
        if ( ImGui::BeginCombo( ("Interpolation Type##" + std::to_string( m_Id )).c_str(), interpolationTypes[ (int)m_InterpolationType] ) )
        {
            for ( int i = 0; i < 3; ++i )
            {
                bool isSelected = ( (int)m_InterpolationType == i );
                if ( ImGui::Selectable( interpolationTypes[ i ], isSelected ) )
                {
                    m_InterpolationType = (InterpolationType)i; // Change the selected scene
                    calculate();
                }

                if ( isSelected )
                {
                    ImGui::SetItemDefaultFocus(); // Automatically focus on the selected item
                }
            }

            ImGui::EndCombo();
        }

        ImGui::Checkbox( "Is Looping", &m_IsLooping );

        // rendering info
        static float const pointRadius = 4.0f;
        static ImVec4 const pointColor = { 0.0f, 1.0f, 1.0f, 1.0f };
        static ImVec4 const tangentColor = { 1.0f, 1.0f, 0.0f, 1.0f };
        static ImVec4 const selectedColor = { 1.0f, 0.0f, 0.0f, 1.0f };
        static int const sampleCount = 64;

        // the currently selected controlPoint index for each Curve
        static std::map< unsigned, unsigned > selectedIndices;
        if ( selectedIndices[ m_Id ] >= m_ControlPoints.size() )
        {
            selectedIndices[ m_Id ] = 0;
        }

        // create a plot for each axis
        for ( int axis = 0; axis < dimensionality; ++axis )
        {

            // get the axis name
            std::string axisName;
            switch ( axis )
            {
                case 0: axisName = "X Axis"; break;
                case 1: axisName = "Y Axis"; break;
                case 2: axisName = "Z Axis"; break;
                case 3: axisName = "W Axis"; break;
                default:
                    axisName = "Axis [" + std::to_string(axis) + ']';
                    break;
            }
            axisName += "##" + std::to_string( m_Id );

            // put the plot in a dropdown
            if ( !ImGui::TreeNode( axisName.c_str() ) )
            {
                continue;
            }

            // calculate buffer zone outside of curve
            glm::vec< dimensionality, float > buffer = (m_MaxPointValue - m_MinPointValue) * 0.1f;
            float timeBuffer = GetTotalTime() * 0.1f;

            // start the plot
            if ( !ImPlot::BeginPlot( (axisName + "plot").c_str(), ImVec2(-1, 0), ImPlotFlags_CanvasOnly))
            {
                ImGui::TreePop();
                continue;
            }

            // set up the axes
            ImPlot::SetupAxes( 0, 0, ImPlotAxisFlags_None, ImPlotAxisFlags_None );
            // ImPlot::SetupAxesLimits(
            //     m_MinPointValue[ axis ] - buffer[ axis ], m_MaxPointValue[axis] + buffer[axis],
            //     -timeBuffer, GetTotalTime() + timeBuffer
            // );

            std::string guiLabel;

            // loop through each control point
            for ( int i = 0; i < m_ControlPoints.size(); ++i )
            {
                // get values
                glm::vec< dimensionality + 1, float >& value = m_ControlPoints[ i ].M_Value;
                glm::vec< dimensionality, float >& derivative = m_ControlPoints[ i ].M_Derivative;


                // display the draggable point
                ImPlotPoint points[2];
                points[0] = ImPlotPoint( value[ dimensionality ], value[ axis ] );
                if ( ImPlot::DragPoint( 2 * i, &points[0].x, &points[0].y, (selectedIndices[ m_Id ] == i) ? selectedColor : pointColor, pointRadius + (selectedIndices[ m_Id ] == i) * pointRadius, ImPlotDragToolFlags_None ) )
                {
                    selectedIndices[ m_Id ] = i;
                }

                // save the adjusted values
                value[ axis ] = (float)points[0].y;
                value[ dimensionality ] = (float)points[0].x;

                // draw tangents
                if ( m_InterpolationType == InterpolationType::cubic )
                {
                    // draggable point
                    points[1] = ImPlotPoint( points[0].x + timeBuffer, points[0].y + derivative[axis] * timeBuffer );
                    if ( ImPlot::DragPoint( 2 * i + 1, &points[1].x, &points[1].y, tangentColor, pointRadius, ImPlotDragToolFlags_None ) )
                    {
                        selectedIndices[ m_Id ] = i;
                    }

                    // draw the tangent line
                    ImPlot::SetNextLineStyle( tangentColor );
                    guiLabel = "##tangent" + std::to_string( m_Id ) + '_' + std::to_string( axis ) + '_' + std::to_string(i);
                    ImPlot::PlotLine( guiLabel.c_str(), &points[0].x, &points[0].y, 2, 0, 0, sizeof(ImPlotPoint));

                    derivative[ axis ] = (float)( points[1].y - points[0].y ) / (float)( points[1].x - points[0].x );
                }

                // don't draw any curve after the last point
                if ( i >= m_ControlPoints.size() - 1 )
                {
                    break;
                }

                // sample along the curve
                float duration = m_ControlPoints[ i + 1 ].GetTime() - value[ dimensionality ];
                std::vector< ImPlotPoint > samples;
                samples.resize( sampleCount );
                for ( int j = 0; j < sampleCount; ++j )
                {
                    float t = j * duration / ( sampleCount - 1 ) + value[ dimensionality ];
                    float sampledValue = interpolate( i, t )[ axis ];
                    samples[ j ] = ImPlotPoint( t, sampledValue );
                }

                // draw the curve
                ImPlot::SetNextLineStyle( ImVec4( 1.0f, 1.0f, 1.0f, 1.0f ) );
                guiLabel = "##curve" + std::to_string( m_Id ) + '_' + std::to_string( axis ) + '_' + std::to_string(i);
                ImPlot::PlotLine( guiLabel.c_str(), &samples[0].x, &samples[0].y, sampleCount, 0, 0, sizeof(ImPlotPoint));

            }

            ImPlot::EndPlot();

            ImGui::TreePop();
        }

        ImGui::DragInt( ( "Point##" + std::to_string( m_Id ) ).c_str(), (int*)&selectedIndices[ m_Id ], 0.05f, 0, m_ControlPoints.size() - 1, nullptr, (m_ControlPoints.size() <= 1) ? ImGuiSliderFlags_NoInput : ImGuiSliderFlags_None );

        ImGui::DragFloat( ( "Time##" + std::to_string( m_Id ) ).c_str(), &m_ControlPoints[ selectedIndices[ m_Id ] ].M_Value[ dimensionality ], 0.01f );
        ImGui::DragScalarN( ( "Value##" + std::to_string( m_Id ) ).c_str(), ImGuiDataType_Float, &m_ControlPoints[ selectedIndices[ m_Id ] ].M_Value[0], dimensionality, 0.01f );

        if ( m_InterpolationType == InterpolationType::cubic )
        {
            ImGui::DragScalarN( ( "Derivative##" + std::to_string( m_Id ) ).c_str(), ImGuiDataType_Float, &m_ControlPoints[ selectedIndices[ m_Id ] ].M_Derivative[0], dimensionality, 0.01f );
        }

        if ( ImGui::Button( ( "Add Point##" + std::to_string( m_Id ) ).c_str() ) )
        {
            ControlPoint< dimensionality > cp;
            if ( selectedIndices[ m_Id ] == m_ControlPoints.size() - 1 )
            {
                cp.SetTime( GetTotalTime() + 1.0f );
                AddControlPoint( cp );
            }
            else
            {
                
                cp.SetTime( 0.5f * ( m_ControlPoints[ selectedIndices[ m_Id ] + 1 ].GetTime() + m_ControlPoints[ selectedIndices[ m_Id ] ].GetTime() ) );
                glm::vec< dimensionality, float > sample = interpolate( selectedIndices[ m_Id ], cp.GetTime() );
                
                for ( int i = 0; i < dimensionality; ++i )
                {
                    cp.M_Value[ i ] = sample[ i ];
                }

                if ( m_InterpolationType == InterpolationType::cubic )
                {
                    Coefficients coefficients = m_CubicCoefficients[ selectedIndices[ m_Id ] ];
                    glm::vec< dimensionality, float > c = m_ControlPoints[ selectedIndices[ m_Id ] ].M_Derivative;
                    float duration =  m_ControlPoints[ selectedIndices[ m_Id ] + 1 ].GetTime() - m_ControlPoints[ selectedIndices[ m_Id ] ].GetTime();
                    cp.M_Derivative = (3.0f / 4.0f * coefficients.a + coefficients.b + c) / duration;
                }

                AddControlPoint( cp );
            }

            ++selectedIndices[ m_Id ];
        }

        if ( ImGui::Button( ( "Remove Point##" + std::to_string( m_Id ) ).c_str() ) )
        {
            if ( m_ControlPoints.size() <= 1 )
            {
                m_ControlPoints[ 0 ] = ControlPoint< dimensionality >();
            }
            else
            {
                RemoveControlPoint( selectedIndices[ m_Id ] );
                --selectedIndices[ m_Id ];
            }
        }

        calculate();
        
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

        // calculate minimum and maximum values
        m_MinPointValue = m_ControlPoints[0].M_Value;
        m_MaxPointValue = m_ControlPoints[0].M_Value;
        for ( int i = 1; i < m_ControlPoints.size(); ++i )
        {
            for ( int j = 0; j < dimensionality; ++j )
            {
                m_MinPointValue[ j ] = std::min( m_MinPointValue[ j ], m_ControlPoints[ i ].M_Value[ j ] );
                m_MaxPointValue[ j ] = std::max( m_MaxPointValue[ j ], m_ControlPoints[ i ].M_Value[ j ] );
            }
        }

        // calculate the cubic coefficients if necessary
        m_CubicCoefficients.clear();
        if ( m_InterpolationType == InterpolationType::cubic )
        {
            calculateCubicCoefficients();
        }

        m_IsDirty = false;
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
            {
                // interpolate between ControlPoints using precomputed cubic polynomial coefficients
                float duration = m_ControlPoints[ index + 1 ].GetTime() - m_ControlPoints[ index ].GetTime();
                time = (time - m_ControlPoints[ index ].GetTime())
                    / duration;
                Coefficients coefficients = m_CubicCoefficients[ index ];
                glm::vec< dimensionality, float > c = m_ControlPoints[ index ].M_Derivative * duration;
                glm::vec< dimensionality, float > d = m_ControlPoints[ index ].M_Value;
                return (coefficients.a * time * time * time) + (coefficients.b * time * time) + (c * time) + d;
            }

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
                -3.0f * p0 + 3.0f * p1 - 2.0f * d0 - d1
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
        m_ControlPoints.clear();
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
