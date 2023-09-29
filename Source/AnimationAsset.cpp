/*****************************************************************//**
 * \file   Animation.cpp
 * \brief  animation component implimentation
 * 
 * \author Tyler Birdsall (tyler.birdsall@digipen.edu)
 * \date   September 2023
 *********************************************************************/

#include "Animation.h"
#include "Sprite.h"
#include "Behavior.h"
#include "Entity.h"
#include "AnimationAsset.h"

 /// @brief	Default constructor
AnimationAsset::AnimationAsset()
{
	m_FrameStart = 0;
	m_FrameEnd = 0;
	m_Duration = 0.0f;
	m_Looping = false;
}

/// @brief	Gets the start frame of the animation
/// @return	Start frame of the animation
const unsigned AnimationAsset::GetStart() const
{
	return m_FrameStart;
}

/// @brief	Sets a new start frame
/// @param	New start frame to set
void AnimationAsset::SetStart(unsigned newStart)
{
	m_FrameStart = newStart;
}

/// @brief	Gets the end frame of the animation
/// @return End frame of the animation
const unsigned AnimationAsset::GetEnd() const
{
	return m_FrameEnd;
}

/// @brief	Sets a new end frame
/// @param	New end frame to set
void AnimationAsset::SetEnd(unsigned newEnd)
{
	m_FrameEnd = newEnd;
}

/// @brief	Gets the duration of the animation
/// @return	Duration of the animation
const float AnimationAsset::GetDuration() const
{
	return m_Duration;
}

/// @brief	Sets a new duration
/// @param	New duration to set
void AnimationAsset::SetDuration(float newDuration)
{
	m_Duration = newDuration;
}

/// @brief	Gets if the animation is looping
/// @return	Looping status of the animation
const bool AnimationAsset::GetLooping() const
{
	return	m_Looping;
}

/// @brief	Sets a new looping status
/// @param	New looping status to set
void AnimationAsset::SetLooping(bool newLooping)
{
	m_Looping = newLooping;
}

