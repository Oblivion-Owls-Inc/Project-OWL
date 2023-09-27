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

AnimationAsset::AnimationAsset()
{
	m_FrameStart = 0;
	m_FrameEnd = 0;
	m_Duration = 0.0f;
	m_Looping = false;
}

const unsigned AnimationAsset::GetStart() const
{
	return m_FrameStart;
}

void AnimationAsset::SetStart(unsigned newStart)
{
	m_FrameStart = newStart;
}

const unsigned AnimationAsset::GetEnd() const
{
	return m_FrameEnd;
}

void AnimationAsset::SetEnd(unsigned newEnd)
{
	m_FrameEnd = newEnd;
}

const float AnimationAsset::GetDuration() const
{
	return m_Duration;
}

void AnimationAsset::SetDuration(float newDuration)
{
	m_Duration = newDuration;
}

const bool AnimationAsset::GetLooping() const
{
	return	m_Looping;
}

void AnimationAsset::SetLooping(bool newLooping)
{
	m_Looping = newLooping;
}

