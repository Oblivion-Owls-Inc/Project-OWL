/// @file       AnimationAsset.cpp
/// @author     Tyler Birdsall (tyler.birdsall@digipen.edu)
/// @brief      Asset that contains information about a spritesheet animation
/// @version    0.1
/// @date       September 2023
/// 
/// @copyright  Copyright (c) 2023 Digipen Institute of Technology

#include "AnimationAsset.h"

#include "Stream.h"

//-----------------------------------------------------------------------------
// public: constructor
//-----------------------------------------------------------------------------


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
