/// @file       AnimationAsset.cpp
/// @author     Tyler Birdsall (tyler.birdsall@digipen.edu)
/// @brief      Asset that contains information about a spritesheet animation
/// @version    0.1
/// @date       September 2023
/// 
/// @copyright  Copyright (c) 2023 Digipen Institute of Technology

#include "AnimationAsset.h"

//-----------------------------------------------------------------------------
// public: constructor
//-----------------------------------------------------------------------------

     /// @brief	Default constructor
    AnimationAsset::AnimationAsset() :
	    m_Start( 0 ),
	    m_End( 1 ),
	    m_FrameDuration( 1.0f / 12.0f ),
	    m_IsLooping( false )
    {}

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

    /// @brief  reads the whether this Animation is looping
    /// @param  stream  the json data to read from
    void AnimationAsset::readIsLooping( nlohmann::ordered_json const& data )
    {
        m_IsLooping = Stream::Read<bool>(data);
    }

    /// @brief  map of the SceneSystem read methods
    ReadMethodMap< AnimationAsset > const AnimationAsset::s_ReadMethods = {
        { "Start"        , &readStart         },
        { "End"          , &readEnd           },
        { "FrameDuration", &readFrameDuration },
        { "FrameRate"    , &readFrameRate     },
        { "IsLooping"    , &readIsLooping     }
    };

//-----------------------------------------------------------------------------
