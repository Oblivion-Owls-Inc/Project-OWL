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

#include <functional>

class Sprite;

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
    /// @param  assetName   the AnimationAsset to play
    void Play( std::string const& assetName );

    /// @brief  starts playing an Animation
    /// @param  asset   the AnimationAsset to play
    void Play( AnimationAsset const* asset );

    /// @brief  starts playing this Animation
    void Play();

//-----------------------------------------------------------------------------
public: // accessors
//-----------------------------------------------------------------------------

	/// @brief	Gets the frame index of the animation
    /// @param  relative    if true, return the index relative to the start of the current animation
	/// @return	Frame index of the animation
    unsigned GetIndex( bool relative = true ) const;
	/// @brief	Sets a new frame index
    /// @param  relative    if true, set the index relative to the start of the current animation
	/// @param	index       frame index to set
	void SetIndex( unsigned index, bool relative = true );

	/// @brief	Gets the amount of time until the frame changes
	/// @return	the amount of time until the frame next changes
    float GetDelay() const;
	/// @brief	Sets a new frame delay
	/// @param	delay   frame delay to set
	void SetDelay( float delay );

	/// @brief	Gets the running status of the animation
	/// @return	Running status of the animation
    bool GetRunning() const;
	/// @brief	Sets a new running status
	/// @param	running running status to set
	void SetRunning( bool running );

	/// @brief  sets a callback function to run when the current animation finishes playing
	/// @param  callback 
	void SetCallback( std::function< void() > const& callback );

	/// @brief	Gets the current animation asset
	/// @return	The asset currently connected
	AnimationAsset const* GetAsset() const;
	/// @brief	Sets the animation asset this Animation Component is using
	/// @param	asset   animation asset to set
	void SetAsset( AnimationAsset const* asset );

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
    Sprite* m_Sprite;

    /// @brief  the animation asset this Animation Component is using
    AnimationAsset const* m_Asset;

    /// @brief  the current frame index
    unsigned m_FrameIndex;
    
    /// @brief  how long until the next frame
    float m_FrameDelay;

    /// @brief  whether the animation is currenty running
    bool m_IsRunning;

    /// @brief  callback which gets called when the current animation finishes playing
    std::function< void() > m_Callback;

//-----------------------------------------------------------------------------
private: // methods
//-----------------------------------------------------------------------------

    /// @brief  advances the frame of this Animation
    void AdvanceFrame();

//-----------------------------------------------------------------------------
private: // reading
//-----------------------------------------------------------------------------

	/// @brief  reads the current frame of this Animation (non-relative)
	/// @param  stream  the json data to read from
	void readFrameIndex( Stream stream );

    /// @brief  reads the frame delay of this Animation
    /// @param  stream  the json data to read from
	void readFrameDelay( Stream stream );

    /// @brief  reads the running state of this Animation
    /// @param  stream  the json data to read from
	void readIsRunning( Stream stream );

    /// @brief  reads the animation asset this Animation is using
    /// @param  stream  the json data to read from
    void readAnimation( Stream stream );

	/// @brief  map of read methods
	static ReadMethodMap< Animation > const s_ReadMethods;

    /// @brief  gets the map of read methods
    /// @return the map of read methods
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override
    {
        return (ReadMethodMap< ISerializable > const&)s_ReadMethods;
    }

//-----------------------------------------------------------------------------
private: // copying
//-----------------------------------------------------------------------------

    /// @brief	Clones an animation
    /// @return New animation copy
    virtual Component* Behavior::Clone() const override;

    /// @brief  copy constructor
    /// @param  other the Animation to copy
    Animation( Animation const& other );

//-----------------------------------------------------------------------------
};
