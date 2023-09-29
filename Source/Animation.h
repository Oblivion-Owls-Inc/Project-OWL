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
#include "stream.h"

class Animation : public Behavior
{
//-----------------------------------------------------------------------------
public: // public functions
//-----------------------------------------------------------------------------
	
	/// @brief	Defualt constructor
	Animation();

	/// @brief	Default Destructor
	~Animation();
	
	/// @brief	Gets the frame index of the animation
	/// @return	Frame index of the animation
	const unsigned GetIndex() const;

	/// @brief	Sets a new frame index
	/// @param	New frame index to set
	void SetIndex(unsigned newIndex);

	/// @brief	Gets the frame count of the animation
	/// @return	Frame count of the animation
	const unsigned GetCount() const;

	/// @brief	Sets a new frame count
	/// @param	New frame count to set
	void SetCount(unsigned newCount);

	/// @brief	Gets the frame delay of the animation
	/// @return	Frame delay of the animation
	const float GetDelay() const;

	/// @brief	Sets a new frame delay
	/// @param	New frame delay to set
	void SetDelay(float newDelay);

	/// @brief	Gets the frame duration of the animation
	/// @return	Frame duration of the animation
	const float GetDuration() const;

	/// @brief	Sets a new frame duration
	/// @param	New frame duration to set
	void SetDuration(float newDuration);

	/// @brief	Gets the running status of the animation
	/// @return	Running status of the animation
	const bool GetRunning() const;

	/// @brief	Sets a new running status
	/// @param	New running status to set
	void SetRunning(bool newRunning);

	/// @brief	Gets the looping status of the animation
	/// @return	Looping status of the animation
	const bool GetLooping() const;

	/// @brief	Sets a new looping status
	/// @param	New looping status to set
	void SetLooping(bool newLooping);

	/// @brief	Gets if the animation is done
	/// @return	done of the animation
	const bool GetDone() const;

	/// @brief	Sets a new done status
	/// @param	New done status to set
	void SetDone(bool newDone);

	/// @brief	Gets the current animation asset
	/// @return	The asset currently connected
	const AnimationAsset* GetAsset() const;

	/// @brief	Sets a new animation asset
	/// @brief	this and animation play can change
	/// @brief	the current animation without
	///	@brief	having to swap the sprite sheet
	/// @brief	or other information
	/// @param	New animation asset to set
	void SetAsset(AnimationAsset* newAsset);

	/// @brief	Sets the animation to play
	/// @brief	this should only be played
	/// @brief	when you want to start,
	/// @brief	this is not an update call
	void AnimationPlay();

	

//-----------------------------------------------------------------------------
private: // overrides
//-----------------------------------------------------------------------------
	
	virtual void OnCollisionEvent() override {}

	/// @brief	Clones an animation
	/// @return New animation copy
	virtual Component* Behavior::Clone() const override;

	/// @brief	updates animation
	/// @param	dt
	virtual void OnUpdate(float dt) override;

	virtual void OnFixedUpdate() override {}

//-----------------------------------------------------------------------------
private: // reading
//-----------------------------------------------------------------------------

	void ReadFrameIndex(Stream stream);
	void ReadFrameStart(Stream stream);
	void ReadFrameCount(Stream stream);
	void ReadFrameDelay(Stream stream);
	void ReadFrameDuration(Stream stream);
	void ReadIsRunning(Stream stream);
	void ReadIsLooping(Stream stream);
	// A map of all the read methods for animation component.
	static ReadMethodMap<Animation> const s_ReadMethods;
	// A function that gets all the read methods for the animation component.
	virtual ReadMethodMap<Component> const& GetReadMethods() const override;

//-----------------------------------------------------------------------------
private: // private functions
//-----------------------------------------------------------------------------

	void AdvanceFrame();
	Animation(Animation const&);
	AnimationAsset* m_asset;


//-----------------------------------------------------------------------------
private: // private variables
//-----------------------------------------------------------------------------
	
	unsigned m_FrameIndex;
	unsigned m_FrameStart;
	unsigned m_FrameCount;
	float m_FrameDelay;
	float m_FrameDuration;
	bool m_IsRunning;
	bool m_IsLooping;
	bool m_IsDone;
};
