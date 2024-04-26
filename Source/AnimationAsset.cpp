/// @file       AnimationAsset.cpp
/// @author     Tyler Birdsall (tyler.birdsall@digipen.edu)
/// @brief      Asset that contains information about a spritesheet animation
/// @version    0.1
/// @date       September 2023
/// 
/// @copyright  Copyright (c) 2024 Digipen Institute of Technology

#include "pch.h" // precompiled header has to be included first

#include "AnimationAsset.h"




//-----------------------------------------------------------------------------
// public: constructor
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// public: inspection
//-----------------------------------------------------------------------------


    /// @brief  inspects this AnimationAsset
    /// @return whether the AnimationAsset was changed
    bool AnimationAsset::Inspect()
    {
        bool changed = false;

        changed |= ImGui::DragInt( "start frame", &m_Start, 0.05f, 0, INT_MAX );

        changed |= ImGui::DragInt( "end frame", &m_End, 0.05f, 1, INT_MAX );
        
        changed |= ImGui::DragFloat( "frame duration", &m_FrameDuration, 0.01f, 0.0f, INFINITY );

        float fps = 1.0f / m_FrameDuration;
        if ( ImGui::DragFloat( "fps", &fps, 0.1f, 0.0f, INFINITY ) )
        {
            m_FrameDuration = 1.0f / fps;
            changed = true;
        }

        return changed;
    }


//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------

    /// @brief  reads the start frame index of this Animation
    /// @param  stream  the json data to read from
    void AnimationAsset::readStart( nlohmann::ordered_json const& data )
    {
        m_Start = Stream::Read<int>(data);
    }

    /// @brief  reads the end frame index of this Animation
    /// @param  stream  the json data to read from
    void AnimationAsset::readEnd( nlohmann::ordered_json const& data )
    {
        m_End = Stream::Read<int>(data);
    }

    /// @brief  reads the frame duration of this Animation
    /// @param  stream  the json data to read from
    void AnimationAsset::readFrameDuration( nlohmann::ordered_json const& data )
    {
        m_FrameDuration = Stream::Read<float>(data);
    }

    /// @brief  reads the frame rate of this Animation
    /// @param  stream  the json data to read from
    void AnimationAsset::readFrameRate( nlohmann::ordered_json const& data )
    {
        m_FrameDuration = 1.0f / Stream::Read<float>(data);
    }


    /// @brief  map of the SceneSystem read methods
    ReadMethodMap< AnimationAsset > const AnimationAsset::s_ReadMethods = {
        { "Start"        , &readStart         },
        { "End"          , &readEnd           },
        { "FrameDuration", &readFrameDuration },
        { "FrameRate"    , &readFrameRate     }
    };


//-----------------------------------------------------------------------------
// public: writing
//-----------------------------------------------------------------------------


    /// @brief  Write all data for the AnimationAsset to a JSON file.
    /// @return The file containing all the AnimationAsset data.
    nlohmann::ordered_json AnimationAsset::Write() const
    {
        nlohmann::ordered_json data;

        data[ "Start" ] = Stream::Write( m_Start );
        data[ "End" ] = Stream::Write( m_End );
        data[ "FrameDuration" ] = Stream::Write( m_FrameDuration );

        return data;
    }


//-----------------------------------------------------------------------------
