/// @file       AnimationAsset.h
/// @author     Tyler Birdsall (tyler.birdsall@digipen.edu)
/// @brief      Asset that contains information about a spritesheet animation
/// @version    0.1
/// @date       September 2023
/// 
/// @copyright  Copyright (c) 2023 Digipen Institute of Technology

#pragma once

#include "ISerializable.h"

class AnimationAsset : public ISerializable
{
//-----------------------------------------------------------------------------
public: // constructor
//-----------------------------------------------------------------------------

	/// @brief	constructor
	AnimationAsset();

    /// @brief  destructor
    ~AnimationAsset() = default;

//-----------------------------------------------------------------------------
public: // accessors
//-----------------------------------------------------------------------------

	/// @brief	gets the start frame of the animation
	/// @return	start frame of the animation
	unsigned GetStart() const { return m_Start; }
	/// @brief	sets a new start frame
	/// @param	start   start frame to set
	void SetStart( unsigned start ) { m_Start = start; }

	/// @brief	gets the end frame of the animation
	/// @return end frame of the animation
	unsigned GetEnd() const { return m_End; }
	/// @brief	sets a new end frame
	/// @param	end end frame to set
	void SetEnd( unsigned end ) { m_End = end; }

	/// @brief	gets the duration of each frame of animation
	/// @return	duration of each frame
	float GetFrameDuration() const { return m_FrameDuration; }
	/// @brief	sets a new frame duration
	/// @param	frameDuration   frame duration to set
	void SetFrameDuration( float frameDuration ) { m_FrameDuration = frameDuration; }

	/// @brief	Gets if the animation is looping
	/// @return	Looping status of the animation
	bool GetIsLooping() const { return m_IsLooping; }
	/// @brief	Sets a new looping status
	/// @param	New looping status to set
	void SetIsLooping( bool isLooping ) { m_IsLooping = isLooping; }

//-----------------------------------------------------------------------------
public: // methods
//-----------------------------------------------------------------------------

    // TODO: move this into a base class
    void Inspect() {}
    
//-----------------------------------------------------------------------------
private: // private variables
//-----------------------------------------------------------------------------	

	/// @brief  the start frame index of the animation
	unsigned m_Start;

	/// @brief  the end frame index of the animation
	unsigned m_End;

	/// @brief  how long each frame lasts
	float m_FrameDuration;

	/// @brief  whether this animation loops
	bool m_IsLooping;

//-----------------------------------------------------------------------------
private: // reading
//-----------------------------------------------------------------------------

    /// @brief  reads the start frame index of this Animation
    /// @param  stream  the json data to read from
    void readStart( Stream stream );

    /// @brief  reads the end frame index of this Animation
    /// @param  stream  the json data to read from
    void readEnd( Stream stream );

    /// @brief  reads the frame duration of this Animation
    /// @param  stream  the json data to read from
    void readFrameDuration( Stream stream );

    /// @brief  reads the frame rate of this Animation
    /// @param  stream  the json data to read from
    void readFrameRate( Stream stream );

    /// @brief  reads the whether this Animation is looping
    /// @param  stream  the json data to read from
    void readIsLooping( Stream stream );

    /// @brief  map of the SceneSystem read methods
    static ReadMethodMap< AnimationAsset > const s_ReadMethods;

    /// @brief  gets the AnimationAsset read method map
    /// @return the read method map
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override
    {
        return (ReadMethodMap< ISerializable > const&)s_ReadMethods;
    }

//-----------------------------------------------------------------------------	
};
