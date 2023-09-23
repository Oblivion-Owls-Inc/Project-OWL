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

	AnimationAsset();

	const unsigned getStart() const;
	void setStart(unsigned newStart);

	const unsigned getEnd() const;
	void setEnd(unsigned newEnd);

	const float getDuration() const;
	void setDuration(float newDuration);

	const bool getLooping() const;
	void setLooping(bool newLopping);

private:
	
	unsigned frameStart;
	unsigned frameEnd;
	float duration;
	bool looping;
};
