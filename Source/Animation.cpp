/*****************************************************************//**
 * \file   Animation.cpp
 * \brief  animation component implimentation
 * 
 * \author Tyler Birdsall (tyler.birdsall@digipen.edu)
 * \date   September 2023
 *********************************************************************/

#include "Animation.h"
#include "Sprite.h"
#include "BehaviorSystem.h"
#include "Entity.h"

/// @brief	Defualt constructor
Animation::Animation() : 
	Behavior( typeid( Animation ) ),
	m_FrameIndex(0),
	m_FrameCount(0),
	m_FrameDelay(0.0),
	m_FrameDuration(0.0),
	m_IsRunning(false),
	m_IsLooping(false),
	m_IsDone(false)
	
{
	BehaviorSystem<Animation>::GetInstance()->AddBehavior(this);
}

Animation::~Animation()
{
	BehaviorSystem<Animation>::GetInstance()->RemoveBehavior(this);
}

/// @brief	Copy constructor
/// @param	Other animation to copy
Animation::Animation(const Animation& other) : 
	Behavior( typeid( Animation ) )
{
	m_FrameIndex = other.m_FrameIndex;
	m_FrameCount = other.m_FrameCount;
	m_FrameDelay = other.m_FrameDelay;
	m_FrameDuration = other.m_FrameDuration;
	m_IsRunning = other.m_IsRunning;
	m_IsLooping = other.m_IsLooping;
	m_IsDone = other.m_IsDone;
}

/// @brief	Gets the frame index of the animation
/// @return	Frame index of the animation
const unsigned Animation::GetIndex() const
{
	return m_FrameIndex;
}

/// @brief	Sets a new frame index
/// @param	New frame index to set
void Animation::SetIndex(unsigned newIndex)
{
	m_FrameIndex = newIndex;
}

/// @brief	Gets the frame count of the animation
/// @return	Frame count of the animation
const unsigned Animation::GetCount() const
{
	return m_FrameCount;
}

/// @brief	Sets a new frame count
/// @param	New frame count to set
void Animation::SetCount(unsigned newCount)
{
	m_FrameCount = newCount;
}

/// @brief	Gets the frame delay of the animation
/// @return	Frame delay of the animation
const float Animation::GetDelay() const
{
	return m_FrameDelay;
}

/// @brief	Sets a new frame delay
/// @param	New frame delay to set
void Animation::SetDelay(float newDelay)
{
	m_FrameDelay = newDelay;
}

/// @brief	Gets the frame duration of the animation
/// @return	Frame duration of the animation
const float Animation::GetDuration() const
{
	return m_FrameDuration;
}

/// @brief	Sets a new frame duration
/// @param	New frame duration to set
void Animation::SetDuration(float newDuration)
{
	m_FrameDuration = newDuration;
}

/// @brief	Gets the running status of the animation
/// @return	Running status of the animation
const bool Animation::GetRunning() const
{
	return m_IsRunning;
}


/// @brief	Sets a new running status
/// @param	New running status to set
void Animation::SetRunning(bool newRunning)
{
	m_IsRunning = newRunning;
}

/// @brief	Gets the looping status of the animation
/// @return	Looping status of the animation
const bool Animation::GetLooping() const
{
	return m_IsLooping;
}

/// @brief	Sets a new looping status
/// @param	New looping status to set
void Animation::SetLooping(bool newLooping)
{
	m_IsLooping = newLooping;
}

/// @brief	Gets if the animation is done
/// @return	done of the animation
const bool Animation::GetDone() const
{
	return m_IsDone;
}

/// @brief	Sets a new done status
/// @param	New done status to set
void Animation::SetDone(bool newDone)
{
	m_IsDone = newDone;
}

/// @brief	Gets the current animation asset
/// @return	The asset currently connected
const AnimationAsset* Animation::GetAsset() const
{
	return m_asset;
}

/// @brief	Sets a new animation asset
/// @brief	this and animation play can change
/// @brief	the current animation without
///	@brief	having to swap the sprite sheet
/// @brief	or other information
/// @param	New animation asset to set
void Animation::SetAsset(AnimationAsset* newAsset)
{
	m_asset = newAsset;
}

/// @brief	Sets the animation to play
/// @brief	this should only be played
/// @brief	when you want to start,
/// @brief	this is not an update call
void Animation::AnimationPlay()
{
	m_FrameStart = m_asset->GetStart();
	m_FrameIndex = m_asset->GetStart();
	m_FrameCount = m_asset->GetEnd();
	m_FrameDuration = m_asset->GetDuration();
	m_FrameDelay = m_asset->GetDuration();
	m_IsLooping = m_asset->GetLooping();
	m_IsRunning = true;
	m_IsDone = false;
	GetParent()->GetComponent<Sprite>()->SetFrameIndex(m_FrameIndex);
}

/// @brief	advances the animations frame when required
void Animation::AdvanceFrame()
{
	m_FrameIndex += 1;
	if (m_FrameIndex >= m_FrameCount)
	{
		if (m_IsLooping)
		{
			m_FrameIndex = m_FrameStart;
		}
		else
		{
			m_FrameIndex = (m_FrameCount - 1);
			m_IsRunning = false;
		}
		m_IsDone = true;
	}
	
	if (m_IsRunning)
	{
		GetParent()->GetComponent<Sprite>()->SetFrameIndex(m_FrameIndex);
		m_FrameDelay = (m_FrameDelay + m_FrameDuration);
	}
	else
	{
		m_FrameDelay = 0;
	}
}

/// @brief	updates animation
/// @param	dt
void Animation::OnUpdate(float dt)
{
	m_IsDone = false;
	if (m_IsRunning)
	{
		SetDelay(m_FrameDelay - dt);
		if (m_FrameDelay <= 0)
		{
			AdvanceFrame();
		}
	}
}

/// @brief	Clones an animation
/// @return New animation copy
Component* Animation::Clone() const
{
	return new Animation(*this);
}

//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------

/// @brief		  Read from a JSON the frame index.
/// @param stream The JSON to read from.
void Animation::ReadFrameIndex(Stream stream)
{
	m_FrameIndex = stream.Read<unsigned int>();
}

/// @brief		  Read from JSON the frame start.
/// @param stream The JSON to read from.
void Animation::ReadFrameStart(Stream stream)
{
	m_FrameStart = stream.Read<unsigned int>();
}

/// @brief		  Read from JSON the frame count.
/// @param stream The JSON to read from.
void Animation::ReadFrameCount(Stream stream)
{
	m_FrameCount = stream.Read<unsigned int>();
}

/// @brief		  Read from JSON the frame count.
/// @param stream The JSON to read from.
void Animation::ReadFrameDelay(Stream stream)
{
	m_FrameDelay = stream.Read<float>();
}

/// @brief		  Read from JSON the frame duration.
/// @param stream The JSON to read from.
void Animation::ReadFrameDuration(Stream stream)
{
	m_FrameDuration = stream.Read<float>();
}

/// @brief		  Read from JSON is the animation running.
/// @param stream The JSON to read from.
void Animation::ReadIsRunning(Stream stream)
{
	m_IsRunning = stream.Read<bool>();
}

/// @brief		  Read from JSON is the animation looping.
/// @param stream The JSON to read from.
void Animation::ReadIsLooping( Stream stream )
{
	m_IsLooping = stream.Read<bool>();
}

// Map of all the read methods for the animation component.
ReadMethodMap< Animation > const Animation::s_ReadMethods = {
	{"frameIndex"	 , &ReadFrameIndex	  },
	{"frameStart"	 , &ReadFrameStart	  },
	{"frameCount"	 , &ReadFrameCount	  },
	{"frameDuration" , &ReadFrameDuration },
	{"isRunning"	 , &ReadIsRunning	  },
	{"isLooping"	 , &ReadIsLooping	  }
	
};

/// @brief Gets the map of read methods for this Component
/// @return the map of read methods for this Component
ReadMethodMap< Component > const& Animation::GetReadMethods() const
{
	return (ReadMethodMap< Component > const&)s_ReadMethods;
}

