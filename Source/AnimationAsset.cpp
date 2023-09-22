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
	frameStart = 0;
	frameEnd = 0;
	duration = 0.0f;
	looping = false;
}

const unsigned AnimationAsset::getStart() const
{
	return frameStart;
}

void AnimationAsset::setStart(unsigned newStart)
{
	frameStart = newStart;
}

const unsigned AnimationAsset::getEnd() const
{
	return frameEnd;
}

void AnimationAsset::setEnd(unsigned newEnd)
{
	frameEnd = newEnd;
}

const float AnimationAsset::getDuration() const
{
	return duration;
}

void AnimationAsset::setDuration(float newDuration)
{
	duration = newDuration;
}

const bool AnimationAsset::getLooping() const
{
	return	looping;
}

void AnimationAsset::setLooping(bool newLooping)
{
	looping = newLooping;
}

