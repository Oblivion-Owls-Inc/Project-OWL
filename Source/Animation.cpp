/*****************************************************************//**
 * \file   Animation.cpp
 * \brief  animation component implimentation
 * 
 * \author Tyler Birdsall (tyler.birdsall@digipen.edu)
 * \date   September 2023
 *********************************************************************/

#include "Animation.h"

#include "BehaviorSystem.h"
#include "AssetLibrarySystem.h"

#include "Sprite.h"
#include "Entity.h"

//-----------------------------------------------------------------------------
// public: constructor
//-----------------------------------------------------------------------------

    /// @brief	Defualt constructor
    Animation::Animation() : 
	    Behavior( typeid( Animation ) ),
	    m_FrameIndex( 0 ),
	    m_FrameDelay( 0.0 ),
	    m_IsRunning( false ),
        m_OnAnimationCompleteCallbacks(),
	    m_Asset( nullptr ),
        m_Sprite( nullptr )
    {}

//-----------------------------------------------------------------------------
// public: methods
//-----------------------------------------------------------------------------

    /// @brief  starts playing an Animation
    /// @param  assetName   the AnimationAsset to play
    void Animation::Play( std::string const& assetName )
    {
        m_Asset = AssetLibrary< AnimationAsset >()->GetAsset( assetName );
        Play();
    }

    /// @brief  starts playing an Animation
    /// @param  asset   the AnimationAsset to play
    void Animation::Play( AnimationAsset const* asset )
    {
        m_Asset = asset;
        Play();
    }

    /// @brief  starts playing this Animation
    void Animation::Play()
    {
        m_FrameIndex = m_Asset->GetStart();
        m_FrameDelay = m_Asset->GetFrameDuration();
        m_IsRunning = true;
        
        m_Sprite->SetFrameIndex( m_FrameIndex );
    }


    /// @brief  how much longer until the current animation is done playing
    /// @return the amount of remaining time
    float Animation::GetRemainingTime() const
    {
        return m_FrameDelay + (m_Asset->GetEnd() - m_FrameIndex - 1) * m_Asset->GetFrameDuration();
    }


    /// @brief  adds a callback function to be called when the animation completes
    /// @param  callback    the function to be called when the animation completes
    /// @return a handle to the created callback
    /// @note   YOU MUST CLEAR THE CALLBACK IF YOU ARE DONE WITH IT,
    /// @note   the callback will be called every time the animation completes (but not each time it loops)
    unsigned Animation::AddOnAnimationCompleteCallback( std::function< void() > callback )
    {
        unsigned handle = GetUniqueId();
        m_OnAnimationCompleteCallbacks.emplace( handle, std::move( callback ) );
        return handle;
    }

    /// @brief  removes a callback function to be called when the animation completes
    /// @param  callbackHandle  the handle of the callback to remove
    void Animation::RemoveOnAnimationCompleteCallback( unsigned callbackHandle )
    {
        m_OnAnimationCompleteCallbacks.erase( callbackHandle );
    }


//-----------------------------------------------------------------------------
// public: accessors
//-----------------------------------------------------------------------------


    /// @brief	Gets the frame index of the animation
    /// @param  relative    if true, return the index relative to the start of the current animation
    /// @return	Frame index of the animation
    unsigned Animation::GetIndex( bool relative ) const
    {
	    return m_FrameIndex - (relative ? m_Asset->GetStart() : 0);
    }

    /// @brief	Sets a new frame index
    /// @param  relative    if true, set the index relative to the start of the current animation
    /// @param	index       frame index to set
    void Animation::SetIndex( unsigned index, bool relative )
    {
	    m_FrameIndex = index + (relative ? m_Asset->GetStart() : 0);
    }

    /// @brief	Gets the frame delay of the animation
    /// @return	Frame delay of the animation
    float Animation::GetDelay() const
    {
	    return m_FrameDelay;
    }

    /// @brief	Sets a new frame delay
    /// @param	New frame delay to set
    void Animation::SetDelay(float newDelay)
    {
	    m_FrameDelay = newDelay;
    }

    /// @brief	Gets the running status of the animation
    /// @return	Running status of the animation
    bool Animation::GetRunning() const
    {
	    return m_IsRunning;
    }
    /// @brief	Sets a new running status
    /// @param	New running status to set
    void Animation::SetRunning(bool newRunning)
    {
	    m_IsRunning = newRunning;
    }

    /// @brief	Gets the current animation asset
    /// @return	The asset currently connected
    AnimationAsset const* Animation::GetAsset() const
    {
	    return m_Asset;
    }
    /// @brief	Sets a new animation asset
    /// @brief	this and animation play can change
    /// @brief	the current animation without
    ///	@brief	having to swap the sprite sheet
    /// @brief	or other information
    /// @param	New animation asset to set
    void Animation::SetAsset( AnimationAsset const* newAsset )
    {
	    m_Asset = newAsset;
    }

//-----------------------------------------------------------------------------
// private: virtual override methods
//-----------------------------------------------------------------------------

    /// @brief  gets called once when entering the scene
    void Animation::OnInit()
    {
        Behaviors<Animation>()->AddBehavior(this);
        m_Sprite = GetParent()->GetComponent<Sprite>();
    }

    /// @brief  gets called once when exiting the scene
    void Animation::OnExit()
    {
        Behaviors<Animation>()->RemoveBehavior(this);
    }

    /// @brief	updates animation
    /// @param	dt
    void Animation::OnUpdate(float dt)
    {
        if ( m_IsRunning == false )
        {
            return;
        }

        m_FrameDelay -= dt;
        if ( m_FrameDelay <= 0 )
        {
            AdvanceFrame();
        }
    }

//-----------------------------------------------------------------------------
// private: methods
//-----------------------------------------------------------------------------

    /// @brief	advances the animations frame when required
    void Animation::AdvanceFrame()
    {
	    m_FrameIndex += 1;
	    if ( m_FrameIndex >= m_Asset->GetEnd() )
	    {
		    if ( m_Asset->GetIsLooping() )
		    {
			    m_FrameIndex = m_Asset->GetStart();
		    }
		    else
		    {
			    m_FrameIndex = m_Asset->GetEnd() - 1;
			    m_IsRunning = false;
                
                for ( auto callback : m_OnAnimationCompleteCallbacks )
                {
                    callback.second();
                }
		    }
	    }
	
	    if ( m_IsRunning )
	    {
		    m_Sprite->SetFrameIndex( m_FrameIndex );
		    m_FrameDelay = m_FrameDelay + m_Asset->GetFrameDuration();
	    }
	    else
	    {
		    m_FrameDelay = 0;
	    }
    }

//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------

    /// @brief		  Read from a JSON the frame index.
    /// @param stream The JSON to read from.
    void Animation::readFrameIndex(Stream stream)
    {
	    m_FrameIndex = stream.Read<unsigned>();
    }

    /// @brief		  Read from JSON the frame count.
    /// @param stream The JSON to read from.
    void Animation::readFrameDelay(Stream stream)
    {
	    m_FrameDelay = stream.Read<float>();
    }

    /// @brief		  Read from JSON is the animation running.
    /// @param stream The JSON to read from.
    void Animation::readIsRunning(Stream stream)
    {
	    m_IsRunning = stream.Read<bool>();
    }

    /// @brief  reads the animation asset this Animation is using
    /// @param  stream  the json data to read from
    void Animation::readAnimation( Stream stream )
    {
        m_Asset = AssetLibrary< AnimationAsset >()->GetAsset( stream.Read<std::string>() );
    }

    /// @brief  map of read methods
    ReadMethodMap< Animation > const Animation::s_ReadMethods = {
	    {"FrameIndex", &readFrameIndex },
	    {"FrameDelay", &readFrameDelay },
	    {"IsRunning" , &readIsRunning  },
        {"Animation" , &readAnimation  }
    };

//-----------------------------------------------------------------------------
// private: copying
//-----------------------------------------------------------------------------

    /// @brief	Clones an animation
    /// @return New animation copy
    Component* Animation::Clone() const
    {
        return new Animation(*this);
    }


    /// @brief	Copy constructor
    /// @param	Other animation to copy
    Animation::Animation(const Animation& other) : 
        Behavior( other ),
        m_FrameIndex( other.m_FrameIndex ),
        m_FrameDelay( other.m_FrameDelay ),
        m_IsRunning( other.m_IsRunning ),
        m_Asset( other.m_Asset ),
        m_OnAnimationCompleteCallbacks(),
        m_Sprite( nullptr )
    {}

//-----------------------------------------------------------------------------
