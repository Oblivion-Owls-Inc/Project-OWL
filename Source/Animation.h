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
	
	const unsigned GetIndex() const;
	void SetIndex(unsigned newIndex);

	const unsigned GetCount() const;
	void SetCount(unsigned newCount);

	const float GetDelay() const;
	void SetDelay(float newDelay);

	const float GetDuration() const;
	void SetDuration(float newDuration);

	const bool GetRunning() const;
	void SetRunning(bool newRunning);

	const bool GetLooping() const;
	void SetLooping(bool newLooping);

	const bool GetDone() const;
	void SetDone(bool newDone);

	const AnimationAsset* GetAsset() const;
	void SetAsset(AnimationAsset* newAsset);

	void animationPlay();

	


private:

	virtual void OnCollisionEvent() override {}

	virtual Component* Behavior::Clone() const override;
	virtual void OnUpdate(float dt) override;
	virtual void OnFixedUpdate() override {}

	static ReadMethodMap< Animation > const readMethods;
	virtual ReadMethodMap< Component > const& GetReadMethods() override;

	void AdvanceFrame();
	Animation(Animation const&);
	AnimationAsset* m_asset;
	unsigned m_FrameIndex;
	unsigned m_FrameStart;
	unsigned m_FrameCount;
	float m_FrameDelay;
	float m_FrameDuration;
	bool m_IsRunning;
	bool m_IsLooping;
	bool m_IsDone;
};
