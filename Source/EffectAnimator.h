/// @file       EffectAnimator.h
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      Component that animates a Transform's matrix
/// @version    0.1
/// @date       2023-10-06
/// 
/// @copyright  Copyright (c) 2023 Digipen Institute of Technology

#pragma once

#include "basics.h"
#include "Behavior.h"


#include "AssetReference.h"
#include "TransformAnimation.h"

#include "ComponentReference.h"
#include "Transform.h"

/// @brief  Component that can play audio
class EffectAnimator : public Behavior
{
//-----------------------------------------------------------------------------
public: // constructor / Destructor
//-----------------------------------------------------------------------------

    /// @brief  constructor
    EffectAnimator();

    /// @brief  destructor
    ~EffectAnimator() = default;

//-----------------------------------------------------------------------------
public: // methods
//-----------------------------------------------------------------------------

    /// @brief  starts playing an effect
    /// @param  effect          the effect to play
    /// @param  playbackSpeed   the speed multiplier for how fast to play the effect
    /// @param  loopCount       how many times to play the effect ( -1 to loop infinitely )
    void Play( AssetReference< TransformAnimation > const& effect, float playbackSpeed = 1.0f, int loopCount = 1 );

    /// @brief  starts playing the current effect
    /// @param  playbackSpeed   the speed multiplier for how fast to play the effect
    /// @param  loopCount       how many times to play the effect ( -1 to loop infinitely )
    void Play( float playbackSpeed = 1.0f, int loopCount = 1 );


    /// @brief  pauses the current effect
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


    /// @brief  gets the effect currently in this EffectAnimator
    /// @return the effect currently in this EffectAnimator
    AssetReference< TransformAnimation > const& GetCurrentEffect() const;

    /// @brief  sets the effect in this EffectAnimator
    /// @param  effect  the effect in this EffectAnimator
    void SetCurrentEffect( AssetReference< TransformAnimation > const& effect );


    /// @brief  gets how far into the current effect we are
    /// @return how far into the current effect we are
    float GetTime() const;

    /// @brief  sets how far into the current effect we are
    /// @param  time    how far into the current effect we are
    void SetTime( float time );


    /// @brief  gets how many loops are remaining
    /// @return how many loops are remaining ( -1 for infinit looping )
    int GetLoopCount() const;

    /// @brief  sets how many loops are remaining
    /// @param  loopCount   how many loops are remaining ( -1 for infinit looping )
    void SetLoopCount( int loopCount );


    /// @brief  gets whether or not this EffectAnimator is currently playing
    /// @return whether or not this EffectAnimator is currently playing ( -1 for infinit looping )
    float GetIsPlaying() const;

    /// @brief  sets whether or not this EffectAnimator is currently playing
    /// @param  isPlaying   whether or not this EffectAnimator is currently playing ( -1 for infinit looping )
    void SetIsPlaying( float isPlaying );


//-----------------------------------------------------------------------------
private: // virtual override methods
//-----------------------------------------------------------------------------


    /// @brief  called once when entering the scene
    virtual void OnInit() override;

    /// @brief  called once when exiting the scene
    virtual void OnExit() override;

    /// @brief  called every frame
    /// @param  dt  the amount of time since the previous frame
    virtual void OnUpdate( float dt ) override;

    virtual void OnFixedUpdate() {};

    /// @brief  displays this EffectAnimator in the Inspector
    virtual void Inspector() override;


//-----------------------------------------------------------------------------
private: // members
//-----------------------------------------------------------------------------
    

    /// @brief  the transform associated with this Entity
    ComponentReference< Transform > m_Transform;

    /// @brief  the effect currently being used by this EffectAnimator
    AssetReference< TransformAnimation > m_Effect;

    /// @brief  how far into the current effect we are
    float m_Time = 0.0f;

    /// @brief  how many times left to loop (-1 to loop infinitely)
    int m_LoopCount = 0;

    /// @brief  whether the Effect is currently playing
    bool m_IsPlaying = false;

    /// @brief  speed multiplier for how quickly the effect is played
    float m_Speed = 1.0f;

    /// @brief  callback functions to call when the animation completes
    std::map< unsigned, std::function< void() > > m_OnAnimationCompleteCallbacks = {};


//-----------------------------------------------------------------------------
private: // reading
//-----------------------------------------------------------------------------


    /// @brief  reads the current effect
    /// @param  stream  the json data to read from
    void readEffect( nlohmann::ordered_json const& data );

    /// @brief  reads the current time
    /// @param  stream  the json data to read from
    void readTime( nlohmann::ordered_json const& data );

    /// @brief  reads the playback speed
    /// @param  stream  the json data to read from
    void readSpeed( nlohmann::ordered_json const& data );

    /// @brief  reads the loop count
    /// @param  stream  the json data to read from
    void readLoopCount( nlohmann::ordered_json const& data );

    /// @brief  reads whether the effect is currently playing
    /// @param  stream  the json data to read from
    void readIsPlaying( nlohmann::ordered_json const& data );


//-----------------------------------------------------------------------------
public: // reading / writing
//-----------------------------------------------------------------------------


    /// @brief  gets the map of read methods for this Component
    /// @return the map of read methods for this Component
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override;

    /// @brief  Write all EffectAnimator data to a JSON file.
    /// @return The JSON file containing the EffectAnimator data.
    virtual nlohmann::ordered_json Write() const override;


//-----------------------------------------------------------------------------
public: // copying
//-----------------------------------------------------------------------------


    /// @brief  clones this EffectAnimator
    /// @return the newly created clone of this EffectAnimator
    virtual EffectAnimator* Clone() const override;


//-----------------------------------------------------------------------------
private: // copying
//-----------------------------------------------------------------------------


    /// @brief  copy-constructor for the EffectAnimator
    /// @param  other   the other EffectAnimator to copy
    EffectAnimator( EffectAnimator const& other );


    // diable = operator
    void operator =( EffectAnimator const& ) = delete;


//-----------------------------------------------------------------------------
};
