/*****************************************************************//**
 * \file   Animation.h
 * \brief  animation component class
 * 
 * \author Tyler Birdsall (tyler.birdsall@digipen.edu)
 * \date   September 2023
 *********************************************************************/

#pragma once

#include "Behavior.h"

#include "AnimationAsset.h"

#include "ComponentReference.h"
#include "Sprite.h"

#include <functional>

#include "AssetReference.h"
#include "Sprite.h"

class Animation : public Behavior
{
//-----------------------------------------------------------------------------
public: // constructor / destructor
//-----------------------------------------------------------------------------
	

	/// @brief	Defualt constructor
	Animation();

	/// @brief	Default Destructor
	~Animation() = default;


//-----------------------------------------------------------------------------
public: // methods
//-----------------------------------------------------------------------------

    /// @brief  starts playing an Animation
    /// @param  asset   the animation to play
    void Play( AssetReference< AnimationAsset > const& asset );

    /// @brief  starts playing this Animation
    void Play();


    /// @brief  pauses the current animation
    void Pause();


    /// @brief  how much longer until the current animation is done playing
    /// @return the amount of remaining time
    float GetRemainingTime() const;


    /// @brief  adds a callback function to be called when the animation completes
    /// @param  ownerId     the ID of the owner of the callback
    /// @param  callback    the function to be called when the animation completes
    /// @note   YOU MUST REMOVE THE CALLBACK USING THE CALLBACK HANDLE WHEN YOU ARE DONE WITH IT
    void AddOnAnimationCompleteCallback( unsigned ownerId, std::function< void() > callback );

    /// @brief  removes a callback function to be called when the animation completes
    /// @param  ownerId the ID of the owner of the callback to remove
    void RemoveOnAnimationCompleteCallback( unsigned ownerId );


//-----------------------------------------------------------------------------
public: // accessors
//-----------------------------------------------------------------------------


	/// @brief	Gets the frame index of the animation
    /// @param  relative    if true, return the index relative to the start of the current animation
	/// @return	Frame index of the animation
    unsigned GetFrameIndex( bool relative = true ) const;

	/// @brief	Sets a new frame index
    /// @param  relative    if true, set the index relative to the start of the current animation
	/// @param	index       frame index to set
	void SetFrameIndex( unsigned index, bool relative = true );


	/// @brief	Gets the amount of time until the frame changes
	/// @return	the amount of time until the frame next changes
    float GetDelay() const;

	/// @brief	Sets a new frame delay
	/// @param	delay   frame delay to set
	void SetDelay( float delay );


	/// @brief	Gets the running status of the animation
	/// @return	Running status of the animation
    bool GetIsRunning() const;

	/// @brief	Sets a new running status
	/// @param	running running status to set
	void SetIsRunning( bool running );


    /// @brief  gets the remaining loop count of this Animation
    /// @return the loop count of this Animation
    int GetLoopCount() const;

    /// @brief  gets the remaining loop count of this Animation
    /// @param  loopCount   the loop count of this Animation
    void SetLoopCount( int loopCount );


	/// @brief	Gets the current animation asset
	/// @return	The asset currently connected
	AnimationAsset const* GetAsset() const;

	/// @brief	Sets the animation asset this Animation Component is using
	/// @param	asset   animation asset to set
	void SetAsset( AssetReference< AnimationAsset > const& asset );


//-----------------------------------------------------------------------------
private: // virtual override methods
//-----------------------------------------------------------------------------


    /// @brief  gets called once when entering the scene
    virtual void OnInit() override;

    /// @brief  gets called once when exiting the scene
    virtual void OnExit() override;


	/// @brief	updates animation
	/// @param	dt  the time since the last graphics frame
	virtual void OnUpdate(float dt) override;


//-----------------------------------------------------------------------------
private: // member variables
//-----------------------------------------------------------------------------


    /// @brief  the sprite that this Animation Component is animating
    ComponentReference< Sprite > m_Sprite;


    /// @brief  the animation asset this Animation Component is using
    AssetReference< AnimationAsset > m_Asset;


    /// @brief  the current frame index
    int m_FrameIndex = 0;
    
    /// @brief  how long until the next frame
    float m_FrameDelay = 0.0f;

    /// @brief  whether the animation is currenty running
    bool m_IsRunning = false;

    /// @brief  the number of times this Animation will loop before stopping
    /// @note   -1 will loop infinitely
    int m_LoopCount = 0;


    /// @brief  callbacks which gets called when the current animation finishes playing
    std::map< unsigned, std::function< void() > > m_OnAnimationCompleteCallbacks = {};


//-----------------------------------------------------------------------------
private: // methods
//-----------------------------------------------------------------------------


    /// @brief  advances the frame of this Animation
    void AdvanceFrame();


//-----------------------------------------------------------------------------
public: // inspection
//-----------------------------------------------------------------------------


    /// @brief  shows the Inspector for this Animation
    virtual void Inspector() override;


//-----------------------------------------------------------------------------
private: // reading
//-----------------------------------------------------------------------------


	/// @brief  reads the current frame of this Animation (non-relative)
	/// @param  data    the json data to read from
	void readFrameIndex( nlohmann::ordered_json const& data );

    /// @brief  reads the frame delay of this Animation
    /// @param  data    the json data to read from
	void readFrameDelay( nlohmann::ordered_json const& data );

    /// @brief  reads the running state of this Animation
    /// @param  data    the json data to read from
	void readIsRunning( nlohmann::ordered_json const& data );

    /// @brief  reads the loop count of this Animation
    /// @param  data    the json data to read from
    void readLoopCount( nlohmann::ordered_json const& data );

    /// @brief  reads the animation asset this Animation is using
    /// @param  data    the json data to read from
    void readAnimation( nlohmann::ordered_json const& data );


//-----------------------------------------------------------------------------
public: // reading / writing
//-----------------------------------------------------------------------------


    /// @brief  gets the map of read methods
    /// @return the map of read methods
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override;


     /// @brief  Write all Animation component data to a JSON file.
     /// @return The JSON file containing the Animation component data.
     virtual nlohmann::ordered_json Write() const override;


//-----------------------------------------------------------------------------
public: // copying
//-----------------------------------------------------------------------------


    /// @brief	Clones an animation
    /// @return New animation copy
    virtual Animation* Clone() const override;


//-----------------------------------------------------------------------------
private: // copying
//-----------------------------------------------------------------------------


    /// @brief  copy constructor
    /// @param  other the Animation to copy
    Animation( Animation const& other );


//-----------------------------------------------------------------------------
};
