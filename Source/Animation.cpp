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
	return frameCount;
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

void Animation::AdvanceFrame()
{
	frameIndex += 1;
	if (frameIndex >= frameCount)
	{
		if (isLooping)
		{
			frameIndex = 0;
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

/* if (animation != NULL)
	{
		animation->setDone(FALSE);
		if (animation->getRunning())
		{
			animation->setDelay(animation->getDelay() - dt);
			if (animation->getDelay() <= 0)
			{
				advanceFrame(animation);
				//animation->frameDelay += animation->frameDuration;
			}
		}
	}*/

/* static void advanceFrame(Animation* animation)
{
	if (animation != NULL)
	{
		animation->setIndex(animation->getIndex() + 1);
		if (animation->getIndex() >= animation->getCount())
		{
			if (animation->getLooping())
			{
				animation->setIndex(0);
			}
			else
			{
				animation->setIndex(animation->getCount() - 1);
				animation->setRunning(FALSE);
			}
			animation->setDone(TRUE);
		}
		//else
		//{
			if (animation->getRunning())
			{
				SpriteSetFrame(EntityGetSprite(animation->Parent()), animation->getIndex());
				animation->setDelay(animation->getDelay() + animation->getDuration());
			}
			else
			{
				animation->setDelay(0);
			}
		//}
	}
}*/

/* void AnimationPlay(Animation* animation, int frameCount, float frameDuration, bool isLooping)
{
	if (animation != NULL)
	{
		animation->setIndex(0);
		animation->setCount(frameCount);
		animation->setDuration(frameDuration);
		animation->setDelay(frameDuration);
		animation->setLooping(isLooping);
		animation->setRunning(TRUE);
		animation->setDone(FALSE);
		SpriteSetFrame(EntityGetSprite(animation->Parent()), animation->getIndex());
	}
}*/
