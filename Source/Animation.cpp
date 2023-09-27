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
	m_FrameIndex(0),
	m_FrameCount(0),
	m_FrameDelay(0.0),
	m_FrameDuration(0.0),
	m_IsRunning(false),
	m_IsLooping(false),
	m_IsDone(false)
{}

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

const unsigned Animation::GetIndex() const
{
	return m_FrameIndex;
}

void Animation::SetIndex(unsigned newIndex)
{
	m_FrameIndex = newIndex;
}

const unsigned Animation::GetCount() const
{
	return m_FrameCount;
}

void Animation::SetCount(unsigned newCount)
{
	m_FrameCount = newCount;
}

const float Animation::GetDelay() const
{
	return m_FrameDelay;
}

void Animation::SetDelay(float newDelay)
{
	m_FrameDelay = newDelay;
}

const float Animation::GetDuration() const
{
	return m_FrameDuration;
}

void Animation::SetDuration(float newDuration)
{
	m_FrameDuration = newDuration;
}

const bool Animation::GetRunning() const
{
	return m_IsRunning;
}

void Animation::SetRunning(bool newRunning)
{
	m_IsRunning = newRunning;
}

const bool Animation::GetLooping() const
{
	return m_IsLooping;
}

void Animation::SetLooping(bool newLooping)
{
	m_IsLooping = newLooping;
}

const bool Animation::GetDone() const
{
	return m_IsDone;
}

void Animation::SetDone(bool newDone)
{
	m_IsDone = newDone;
}

const AnimationAsset* Animation::GetAsset() const
{
	return m_asset;
}

void Animation::SetAsset(AnimationAsset* newAsset)
{
	m_asset = newAsset;
}

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
	Parent()->GetComponent<Sprite>()->SetFrame(m_FrameIndex);
}

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
		Parent()->GetComponent<Sprite>()->SetFrame(m_FrameIndex);
		m_FrameDelay = (m_FrameDelay + m_FrameDuration);
	}
	else
	{
		m_FrameDelay = 0;
	}
}

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
	frameIndex = stream.Read<unsigned int>();
}

/// @brief		  Read from JSON the frame start.
/// @param stream The JSON to read from.
void Animation::ReadFrameStart(Stream stream)
{
	frameStart = stream.Read<unsigned int>();
}

/// @brief		  Read from JSON the frame count.
/// @param stream The JSON to read from.
void Animation::ReadFrameCount(Stream stream)
{
	frameCount = stream.Read<unsigned int>();
}

/// @brief		  Read from JSON the frame count.
/// @param stream The JSON to read from.
void Animation::ReadFrameDelay(Stream stream)
{
	frameDelay = stream.Read<float>();
}

/// @brief		  Read from JSON the frame duration.
/// @param stream The JSON to read from.
void Animation::ReadFrameDuration(Stream stream)
{
	frameDuration = stream.Read<float>();
}

/// @brief		  Read from JSON is the animation running.
/// @param stream The JSON to read from.
void Animation::ReadIsRunning(Stream stream)
{
	isRunning = stream.Read<bool>();
}

/// @brief		  Read from JSON is the animation looping.
/// @param stream The JSON to read from.
void Animation::ReadIsLooping( Stream stream )
{
	isLooping = stream.Read<bool>();
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

