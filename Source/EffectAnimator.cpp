/// @file       EffectAnimator.cpp
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      Component that animates a Transform's matrix
/// @version    0.1
/// @date       2023-10-06
/// 
/// @copyright  Copyright (c) 2023 Digipen Institute of Technology

#pragma once

#include "EffectAnimator.h"

#include "BehaviorSystem.h"

//-----------------------------------------------------------------------------
// public: constructor / Destructor
//-----------------------------------------------------------------------------


    /// @brief  constructor
    EffectAnimator::EffectAnimator() :
        Behavior( typeid( EffectAnimator) )
    {}


//-----------------------------------------------------------------------------
// public: methods
//-----------------------------------------------------------------------------

    /// @brief  starts playing an effect
    /// @param  effect          the effect to play
    /// @param  playbackSpeed   the speed multiplier for how fast to play the effect
    /// @param  loopCount       how many times to play the effect ( negative to loop infinitely )
    void EffectAnimator::Play( AssetReference< TransformAnimation > const& effect, float playbackSpeed, int loopCount )
    {
        m_Effect = effect;
        Play( playbackSpeed, loopCount );
    }

    /// @brief  starts playing the current effect
    /// @param  playbackSpeed   the speed multiplier for how fast to play the effect
    /// @param  loopCount       how many times to play the effect ( negative to loop infinitely )
    void EffectAnimator::Play( float playbackSpeed, int loopCount)
    {
        m_Speed = playbackSpeed;
        m_LoopCount = loopCount;
        m_Time = 0.0f;
        m_IsPlaying = true;
    }


    /// @brief  pauses the current effect
    void EffectAnimator::Pause()
    {
        m_IsPlaying = false;
    }


    /// @brief  how much longer until the current effect is done playing
    /// @return the amount of remaining time
    float EffectAnimator::GetRemainingTime() const
    {
        return m_Effect->GetTotalTime() - m_Time;
    }
    

    /// @brief  adds a callback function to be called when the animation completes
    /// @param  ownerId     the ID of the owner of the callback
    /// @param  callback    the function to be called when the animation completes
    /// @note   YOU MUST REMOVE THE CALLBACK USING THE CALLBACK HANDLE WHEN YOU ARE DONE WITH IT
    void EffectAnimator::AddOnAnimationCompleteCallback( unsigned ownerId, std::function< void() > callback )
    {
        m_OnAnimationCompleteCallbacks.emplace( ownerId, std::move( callback ) );
    }

    /// @brief  removes a callback function to be called when the animation completes
    /// @param  ownerId the ID of the owner of the callback to remove
    void EffectAnimator::RemoveOnAnimationCompleteCallback( unsigned ownerId )
    {
        m_OnAnimationCompleteCallbacks.erase( ownerId );
    }

    
//-----------------------------------------------------------------------------
// public: accessors
//-----------------------------------------------------------------------------


    /// @brief  gets the effect currently in this EffectAnimator
    /// @return the effect currently in this EffectAnimator
    AssetReference< TransformAnimation > const& EffectAnimator::GetCurrentEffect() const
    {
        return m_Effect;
    }

    /// @brief  sets the effect in this EffectAnimator
    /// @param  effect  the effect in this EffectAnimator
    void EffectAnimator::SetCurrentEffect( AssetReference< TransformAnimation > const& effect )
    {
        m_Effect = effect;
    }


    /// @brief  gets how far into the current effect we are
    /// @return how far into the current effect we are
    float EffectAnimator::GetTime() const
    {
        return m_Time;
    }

    /// @brief  sets how far into the current effect we are
    /// @param  time    how far into the current effect we are
    void EffectAnimator::SetTime( float time )
    {
        m_Time = time;
    }


    /// @brief  gets how many loops are remaining
    /// @return how many loops are remaining ( -1 for infinit looping )
    int EffectAnimator::GetLoopCount() const
    {
        return m_LoopCount;
    }

    /// @brief  sets how many loops are remaining
    /// @param  loopCount   how many loops are remaining ( -1 for infinit looping )
    void EffectAnimator::SetLoopCount( int loopCount )
    {
        m_LoopCount = loopCount;
    }


    /// @brief  gets whether or not this EffectAnimator is currently playing
    /// @return whether or not this EffectAnimator is currently playing ( -1 for infinit looping )
    float EffectAnimator::GetIsPlaying() const
    {
        return m_IsPlaying;
    }

    /// @brief  sets whether or not this EffectAnimator is currently playing
    /// @param  isPlaying   whether or not this EffectAnimator is currently playing ( -1 for infinit looping )
    void EffectAnimator::SetIsPlaying( float isPlaying )
    {
        m_IsPlaying = isPlaying;
    }


//-----------------------------------------------------------------------------
// private: virtual override methods
//-----------------------------------------------------------------------------


    /// @brief  called once when entering the scene
    void EffectAnimator::OnInit()
    {
        Behaviors< EffectAnimator >()->AddComponent( this );

        m_Transform.Init( GetEntity() );

        m_Effect.SetOwnerName( GetName() );
        m_Effect.Init();
    }

    /// @brief  called once when exiting the scene
    void EffectAnimator::OnExit()
    {
        Behaviors< EffectAnimator >()->RemoveComponent( this );

        m_Transform.Exit( GetEntity() );
    }

    /// @brief  called every frame
    /// @param  dt  the amount of time since the previous frame
    void EffectAnimator::OnUpdate( float dt )
    {
        if ( m_IsPlaying == false )
        {
            return;
        }

        if ( m_Effect == nullptr )
        {
            return;
        }

        m_Time += dt * m_Speed;
        float duration = m_Effect->GetTotalTime();

        if ( m_Time >= duration )
        {
            m_LoopCount -= ( m_LoopCount > 0 );
            if ( m_LoopCount == 0 )
            {
                m_Time = duration;
                m_IsPlaying = false;
            }
            else
            {
                m_Time -= duration;
            }
        }

        if ( m_Transform != nullptr )
        {
            m_Transform->SetIsDirty( true );
            m_Transform->SetMatrix( m_Transform->GetMatrix() * m_Effect->SampleAtTime( m_Time ) );
        }

        for ( auto& [ id, callback ] : m_OnAnimationCompleteCallbacks )
        {
            callback();
        }
    }

    /// @brief  displays this EffectAnimator in the Inspector
    void EffectAnimator::Inspector()
    {
        if ( m_Transform == nullptr )
        {
            ImGui::Text( "WARNING: no Transform attached to this Entity" );
        }

        m_Effect.Inspect( "Effect" );

        ImGui::Checkbox( "Is Playing", &m_IsPlaying );

        ImGui::DragFloat( "Time", &m_Time, 0.05f, 0.0f, INFINITY );

        ImGui::DragFloat( "Speed", &m_Speed, 0.05f );

        ImGui::DragInt( "Loop Count", &m_LoopCount, 0.05f, -1, INT_MAX );


    }

//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------

    /// @brief  reads the current effect
    /// @param  stream  the json data to read from
    void EffectAnimator::readEffect( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_Effect, data );
    }

    /// @brief  reads the current time
    /// @param  stream  the json data to read from
    void EffectAnimator::readTime( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_Time, data );
    }

    /// @brief  reads the playback speed
    /// @param  stream  the json data to read from
    void EffectAnimator::readSpeed( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_Speed, data );
    }

    /// @brief  reads the loop count
    /// @param  stream  the json data to read from
    void EffectAnimator::readLoopCount( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_LoopCount, data );
    }

    /// @brief  reads whether the effect is currently playing
    /// @param  stream  the json data to read from
    void EffectAnimator::readIsPlaying( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_IsPlaying, data );
    }

    
//-----------------------------------------------------------------------------
// pbulic: reading / writing
//-----------------------------------------------------------------------------


    /// @brief  gets the map of read methods for this Component
    /// @return the map of read methods for this Component
    ReadMethodMap< ISerializable > const& EffectAnimator::GetReadMethods() const
    {
        static ReadMethodMap< EffectAnimator > const readMethods = {
            { "Effect"   , &EffectAnimator::readEffect    },
            { "Time"     , &EffectAnimator::readTime      },
            { "Speed"    , &EffectAnimator::readSpeed     },
            { "LoopCount", &EffectAnimator::readLoopCount },
            { "IsPlaying", &EffectAnimator::readIsPlaying }
        };

        return (ReadMethodMap< ISerializable > const&)readMethods;
    }


    /// @brief  Write all EffectAnimator data to a JSON file.
    /// @return The JSON file containing the EffectAnimator data.
    nlohmann::ordered_json EffectAnimator::Write() const
    {
        nlohmann::ordered_json data;

        data[ "Effect"    ] = Stream::Write( m_Effect    );
        data[ "Time"      ] = Stream::Write( m_Time      );
        data[ "Speed"     ] = Stream::Write( m_Speed     );
        data[ "LoopCount" ] = Stream::Write( m_LoopCount );
        data[ "IsPlaying" ] = Stream::Write( m_IsPlaying );

        return data;
    }

    
//-----------------------------------------------------------------------------
// public: copying
//-----------------------------------------------------------------------------


    /// @brief  clones this EffectAnimator
    /// @return the newly created clone of this EffectAnimator
    EffectAnimator* EffectAnimator::Clone() const
    {
        return new EffectAnimator( *this );
    }


//-----------------------------------------------------------------------------
// private: copying
//-----------------------------------------------------------------------------


    /// @brief  copy-constructor for the EffectAnimator
    /// @param  other   the other EffectAnimator to copy
    EffectAnimator::EffectAnimator( const EffectAnimator& other ) :
        Behavior( other ),
        m_Effect   ( other.m_Effect    ),
        m_IsPlaying( other.m_IsPlaying ),
        m_LoopCount( other.m_LoopCount ),
        m_Speed    ( other.m_Speed     ),
        m_Time     ( other.m_Time      )
    {}


//-----------------------------------------------------------------------------
