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

#include "Inspection.h"

//-----------------------------------------------------------------------------
// public: constructor
//-----------------------------------------------------------------------------


    /// @brief	Defualt constructor
    Animation::Animation() : 
	    Behavior( typeid( Animation ) )
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
    /// @param  ownerId     the ID of the owner of the callback
    /// @param  callback    the function to be called when the animation completes
    /// @note   YOU MUST REMOVE THE CALLBACK USING THE CALLBACK HANDLE WHEN YOU ARE DONE WITH IT
    void Animation::AddOnAnimationCompleteCallback( unsigned ownerId, std::function< void() > callback )
    {
        m_OnAnimationCompleteCallbacks.emplace( ownerId, std::move( callback ) );
    }

    /// @brief  removes a callback function to be called when the animation completes
    /// @param  ownerId the ID of the owner of the callback to remove
    void Animation::RemoveOnAnimationCompleteCallback( unsigned ownerId )
    {
        m_OnAnimationCompleteCallbacks.erase( ownerId );
    }


//-----------------------------------------------------------------------------
// public: accessors
//-----------------------------------------------------------------------------


    /// @brief	Gets the frame index of the animation
    /// @param  relative    if true, return the index relative to the start of the current animation
    /// @return	Frame index of the animation
    unsigned Animation::GetFrameIndex( bool relative ) const
    {
	    return m_FrameIndex - (relative ? m_Asset->GetStart() : 0);
    }

    /// @brief	Sets a new frame index
    /// @param  relative    if true, set the index relative to the start of the current animation
    /// @param	index       frame index to set
    void Animation::SetFrameIndex( unsigned index, bool relative )
    {
	    m_FrameIndex = index + (relative ? m_Asset->GetStart() : 0);
        if ( m_Sprite != nullptr )
        {
            m_Sprite->SetFrameIndex( m_FrameIndex );
        }
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
    bool Animation::GetIsRunning() const
    {
	    return m_IsRunning;
    }

    /// @brief	Sets a new running status
    /// @param	New running status to set
    void Animation::SetIsRunning(bool newRunning)
    {
	    m_IsRunning = newRunning;
    }


    /// @brief  gets the remaining loop count of this Animation
    /// @return the loop count of this Animation
    int Animation::GetLoopCount() const
    {
        return m_LoopCount;
    }

    /// @brief  gets the remaining loop count of this Animation
    /// @param  loopCount   the loop count of this Animation
    void Animation::SetLoopCount( int loopCount )
    {
        m_LoopCount = loopCount;
    }


    /// @brief	Gets the current animation asset
    /// @return	The asset currently connected
    AnimationAsset const* Animation::GetAsset() const
    {
	    return m_Asset;
    }

    /// @brief	Sets the animation asset this Animation will use
    /// @param	newAsset    animation asset to set
    void Animation::SetAsset( AnimationAsset const* newAsset )
    {
        int index = GetFrameIndex(true);
	    m_Asset = newAsset;
        SetFrameIndex(index, true);
    }


//-----------------------------------------------------------------------------
// private: virtual override methods
//-----------------------------------------------------------------------------


    /// @brief  gets called once when entering the scene
    void Animation::OnInit()
    {
        Behaviors<Animation>()->AddBehavior(this);
        m_Sprite = GetEntity()->GetComponent<Sprite>();
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


    /// @brief  shows the Inspector for this Animation
    void Animation::Inspector()
    {
        Inspection::SelectAssetFromLibrary( "Animation Asset", &m_Asset );

        if ( m_Asset == nullptr )
        {
            return;
        }

        ImGui::NewLine();

        int relativeFrameIndex = GetFrameIndex( true );
        if ( ImGui::DragInt(
            "Frame Index", &relativeFrameIndex, 0.05f, 0, m_Asset->GetFrameCount() - 1, "%i", m_Asset->GetFrameCount() > 1 ? ImGuiSliderFlags_None : ImGuiSliderFlags_NoInput
        ) )
        {
           SetFrameIndex( relativeFrameIndex );
        }

        ImGui::DragInt( "Loop Count", &m_LoopCount, 0.05f, -1, INT_MAX );

        ImGui::DragFloat( "Frame Delay", &m_FrameDelay, 0.01f, 0.1f, INFINITY );

        ImGui::Checkbox( "Is Running", &m_IsRunning );

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
            m_FrameIndex = m_Asset->GetStart();

		    if ( m_LoopCount != 0 )
		    {
                m_LoopCount -= ( m_LoopCount != -1 );
		    }
		    else
		    {
			    m_IsRunning = false;
                
                for ( auto callback : m_OnAnimationCompleteCallbacks )
                {
                    callback.second();
                }
		    }
	    }
	
	    if ( m_IsRunning )
	    {
		    m_FrameDelay += m_Asset->GetFrameDuration();
	    }
	    else
	    {
		    m_FrameDelay = 0;
	    }


        m_Sprite->SetFrameIndex( m_FrameIndex );
    }

//-----------------------------------------------------------------------------
// private: reading/writing
//-----------------------------------------------------------------------------

    /// @brief		  Read from a JSON the frame index.
    /// @param stream The JSON to read from.
    void Animation::readFrameIndex( nlohmann::ordered_json const& data )
    {
	    m_FrameIndex = Stream::Read<unsigned>(data);
    }

    /// @brief		  Read from JSON the frame count.
    /// @param stream The JSON to read from.
    void Animation::readFrameDelay( nlohmann::ordered_json const& data )
    {
	    m_FrameDelay = Stream::Read<float>(data);
    }

    /// @brief		  Read from JSON is the animation running.
    /// @param stream The JSON to read from.
    void Animation::readIsRunning( nlohmann::ordered_json const& data )
    {
	    m_IsRunning = Stream::Read<bool>(data);
    }

    /// @brief  reads the loop count of this Animation
    /// @param  data    the json data to read from
    void Animation::readLoopCount( nlohmann::ordered_json const& data )
    {
        m_LoopCount = Stream::Read< int >( data );
    }

    /// @brief  reads the animation asset this Animation is using
    /// @param  stream  the json data to read from
    void Animation::readAnimation( nlohmann::ordered_json const& data )
    {
        m_Asset = AssetLibrary< AnimationAsset >()->GetAsset( Stream::Read<std::string>(data) );
    }

    /// @brief  Write all Animation component data to a JSON file.
    /// @return The JSON file containing the Animation component data.
    nlohmann::ordered_json Animation::Write() const
    {
        nlohmann::ordered_json data;

        data[ "FrameIndex" ] = Stream::Write( m_FrameIndex );
        data[ "FrameDelay" ] = Stream::Write( m_FrameDelay );
        data[ "IsRunning" ] = Stream::Write( m_IsRunning );
        data[ "LoopCount" ] = Stream::Write( m_LoopCount );
        
        std::string name = AssetLibrary< AnimationAsset >()->GetAssetName( m_Asset );
        if ( !name.empty() )
        {
            data[ "Animation" ] = Stream::Write( name );
        }

        return data;
    }


    /// @brief  map of read methods
    ReadMethodMap< Animation > const Animation::s_ReadMethods = {
	    { "FrameIndex", &readFrameIndex },
	    { "FrameDelay", &readFrameDelay },
	    { "IsRunning" , &readIsRunning  },
        { "LoopCount" , &readLoopCount  },
        { "Animation" , &readAnimation  }
    };


//-----------------------------------------------------------------------------
// private: copying
//-----------------------------------------------------------------------------


    /// @brief	Copy constructor
    /// @param	Other animation to copy
    Animation::Animation(const Animation& other) : 
        Behavior( other ),
        m_FrameIndex( other.m_FrameIndex ),
        m_FrameDelay( other.m_FrameDelay ),
        m_IsRunning( other.m_IsRunning ),
        m_Asset( other.m_Asset ),
        m_LoopCount( other.m_LoopCount )
    {}


//-----------------------------------------------------------------------------
