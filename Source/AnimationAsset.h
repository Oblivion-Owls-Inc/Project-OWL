/*****************************************************************//**
 * \file   AnimationAsset.h
 * \brief  animation Asset class
 * 
 * \author Tyler Birdsall (tyler.birdsall@digipen.edu)
 * \date   September 2023
 *********************************************************************/

#pragma once

#include "Sprite.h"
#include "Behavior.h"
#include "Animation.h"

class AnimationAsset
{
public:

	/// @brief	Default constructor
	AnimationAsset();

	/// @brief	Gets the start frame of the animation
	/// @return	Start frame of the animation
	const unsigned GetStart() const;
	/// @brief	Sets a new start frame
	/// @param	New start frame to set
	void SetStart(unsigned newStart);

	/// @brief	Gets the end frame of the animation
	/// @return End frame of the animation
	const unsigned GetEnd() const;

	/// @brief	Sets a new end frame
	/// @param	New end frame to set
	void SetEnd(unsigned newEnd);

	/// @brief	Gets the duration of the animation
	/// @return	Duration of the animation
	const float GetDuration() const;

	/// @brief	Sets a new duration
	/// @param	New duration to set
	void SetDuration(float newDuration);

	/// @brief	Gets if the animation is looping
	/// @return	Looping status of the animation
	const bool GetLooping() const;

	/// @brief	Sets a new looping status
	/// @param	New looping status to set
	void SetLooping(bool newLopping);

private:
	
	unsigned m_FrameStart;
	unsigned m_FrameEnd;
	float m_Duration;
	bool m_Looping;
};
