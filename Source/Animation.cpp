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

Animation::Animation() : 
	Behavior( typeid( Animation ) ),
	frameIndex(0),
	frameCount(0),
	frameDelay(0.0),
	frameDuration(0.0),
	isRunning(false),
	isLooping(false),
	isDone(false)
{}

Animation::Animation(const Animation& other) : 
	Behavior( typeid( Animation ) )
{
	frameIndex = other.frameIndex;
	frameCount = other.frameCount;
	frameDelay = other.frameDelay;
	frameDuration = other.frameDuration;
	isRunning = other.isRunning;
	isLooping = other.isLooping;
	isDone = other.isDone;
}

const unsigned Animation::getIndex() const
{
	return frameIndex;
}

void Animation::setIndex(unsigned newIndex)
{
	frameIndex = newIndex;
}

const unsigned Animation::getCount() const
{
	return frameCount;
}

void Animation::setCount(unsigned newCount)
{
	frameCount = newCount;
}

const float Animation::getDelay() const
{
	return frameDelay;
}

void Animation::setDelay(float newDelay)
{
	frameDelay = newDelay;
}

const float Animation::getDuration() const
{
	return frameDuration;
}

void Animation::setDuration(float newDuration)
{
	frameDuration = newDuration;
}

const bool Animation::getRunning() const
{
	return isRunning;
}

void Animation::setRunning(bool newRunning)
{
	isRunning = newRunning;
}

const bool Animation::getLooping() const
{
	return isLooping;
}

void Animation::setLooping(bool newLooping)
{
	isLooping = newLooping;
}

const bool Animation::getDone() const
{
	return isDone;
}

void Animation::setDone(bool newDone)
{
	isDone = newDone;
}

const AnimationAsset* Animation::getAsset() const
{
	return asset;
}

void Animation::setAsset(AnimationAsset* newAsset)
{
	asset = newAsset;
}

void Animation::animationPlay()
{
	frameStart = asset->getStart();
	frameIndex = asset->getStart();
	frameCount = asset->getEnd();
	frameDuration = asset->getDuration();
	frameDelay = asset->getDuration();
	isLooping = asset->getLooping();
	isRunning = true;
	isDone = false;
	Parent()->GetComponent<Sprite>()->setFrame(frameIndex);
}

void Animation::AdvanceFrame()
{
	frameIndex += 1;
	if (frameIndex >= frameCount)
	{
		if (isLooping)
		{
			frameIndex = frameStart;
		}
		else
		{
			frameIndex = (frameCount - 1);
			isRunning = false;
		}
		isDone = true;
	}
	
	if (isRunning)
	{
		Parent()->GetComponent<Sprite>()->setFrame(frameIndex);
		frameDelay = (frameDelay + frameDuration);
	}
	else
	{
		frameDelay = 0;
	}
}

void Animation::OnUpdate(float dt)
{
	isDone = false;
	if (isRunning)
	{
		setDelay(frameDelay - dt);
		if (frameDelay <= 0)
		{
			AdvanceFrame();
		}
	}
}

Component* Animation::Clone() const
{
	return new Animation(*this);
}

/// @brief the map of read methods for RigidBodys
ReadMethodMap< Animation > const Animation::readMethods = {
	
};

/// @brief gets the map of read methods for this Component
/// @return the map of read methods for this Component
ReadMethodMap< Component > const& Animation::getReadMethods()
{
	return (ReadMethodMap< Component > const&)readMethods;
}

