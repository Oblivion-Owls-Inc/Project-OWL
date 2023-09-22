/*****************************************************************//**
 * \file   Animation.h
 * \brief  animation component class
 * 
 * \author Tyler Birdsall (tyler.birdsall@digipen.edu)
 * \date   September 2023
 *********************************************************************/

#pragma once

#include "Sprite.h"
#include "Behavior.h"
#include "AnimationAsset.h"

class Animation : public Behavior
{
public:
	Animation();
	
	const unsigned getIndex() const;
	void setIndex(unsigned newIndex);

	const unsigned getCount() const;
	void setCount(unsigned newCount);

	const float getDelay() const;
	void setDelay(float newDelay);

	const float getDuration() const;
	void setDuration(float newDuration);

	const bool getRunning() const;
	void setRunning(bool newRunning);

	const bool getLooping() const;
	void setLooping(bool newLooping);

	const bool getDone() const;
	void setDone(bool newDone);

	const AnimationAsset* getAsset() const;
	void setAsset(AnimationAsset* newAsset);

	void animationPlay();

	
	virtual void CollisionEvent(Entity* other) {}

private:

	virtual Component* Behavior::Clone() const override;
	virtual void OnUpdate(float dt) override;
	virtual void OnFixedUpdate() override {}
	void AdvanceFrame();
	Animation(Animation const&);
	AnimationAsset* asset;
	unsigned frameIndex;
	unsigned frameStart;
	unsigned frameCount;
	float frameDelay;
	float frameDuration;
	bool isRunning;
	bool isLooping;
	bool isDone;
};
