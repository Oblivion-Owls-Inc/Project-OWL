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
	AnimationAsset() = default;


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


//-----------------------------------------------------------------------------
public: // methods
//-----------------------------------------------------------------------------


    // TODO: move this into a base class
    void Inspect() {}
    

//-----------------------------------------------------------------------------
private: // private variables
//-----------------------------------------------------------------------------	


	/// @brief  the start frame index of the animation
	unsigned m_Start = 0;

	/// @brief  the end frame index of the animation
	unsigned m_End = 1;

	/// @brief  how long each frame lasts
	float m_FrameDuration = 1.0f / 12.0f;


//-----------------------------------------------------------------------------
private: // reading
//-----------------------------------------------------------------------------


    /// @brief  reads the start frame index of this Animation
    /// @param  data  the json data to read from
    void readStart( nlohmann::ordered_json const& data );

    /// @brief  reads the end frame index of this Animation
    /// @param  data  the json data to read from
    void readEnd( nlohmann::ordered_json const& data );

    /// @brief  reads the frame duration of this Animation
    /// @param  data  the json data to read from
    void readFrameDuration( nlohmann::ordered_json const& data );

    /// @brief  reads the frame rate of this Animation
    /// @param  data  the json data to read from
    void readFrameRate( nlohmann::ordered_json const& data );


    /// @brief  map of the SceneSystem read methods
    static ReadMethodMap< AnimationAsset > const s_ReadMethods;


//-----------------------------------------------------------------------------
public: // reading / writing
//-----------------------------------------------------------------------------


    /// @brief  gets the AnimationAsset read method map
    /// @return the read method map
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override
    {
        return (ReadMethodMap< ISerializable > const&)s_ReadMethods;
    }


	/// @brief  Write all data for the AnimationAsset to a JSON file.
	/// @return The file containing all the AnimationAsset data.
	virtual nlohmann::ordered_json Write() const override;


//-----------------------------------------------------------------------------	
};
