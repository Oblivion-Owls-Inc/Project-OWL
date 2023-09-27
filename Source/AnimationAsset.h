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

	const unsigned GetStart() const;
	void SetStart(unsigned newStart);

	const unsigned GetEnd() const;
	void SetEnd(unsigned newEnd);

	const float GetDuration() const;
	void SetDuration(float newDuration);

	const bool GetLooping() const;
	void SetLooping(bool newLopping);

private:
	
	unsigned m_FrameStart;
	unsigned m_FrameEnd;
	float m_Duration;
	bool m_Looping;
};
