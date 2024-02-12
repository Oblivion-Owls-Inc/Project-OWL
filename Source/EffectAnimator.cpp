/// @file       EffectAnimator.cpp
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      Component that animates a Transform's matrix
/// @version    0.1
/// @date       2023-10-06
/// 
/// @copyright  Copyright (c) 2023 Digipen Institute of Technology

#pragma once

#include "EffectAnimator.h"

#include "Transform.h"
#include "TransformAnimation.h"
#include "Entity.h"

#include "AssetLibrarySystem.h"
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
    /// @param  effectName      the name of the effect to get from the AssetLibrary
    /// @param  playbackSpeed   the speed multiplier for how fast to play the effect
    /// @param  loopCount       how many times to play the effect ( negative to loop infinitely )
    void EffectAnimator::Play( std::string const& effectName, float playbackSpeed, int loopCount )
    {
        Play( AssetLibrary<TransformAnimation>()->GetAsset( effectName ), playbackSpeed, loopCount );
    }

    /// @brief  starts playing an effect
    /// @param  effect          the effect to play
    /// @param  playbackSpeed   the speed multiplier for how fast to play the effect
    /// @param  loopCount       how many times to play the effect ( negative to loop infinitely )
    void EffectAnimator::Play( TransformAnimation const* effect, float playbackSpeed, int loopCount )
    {
        m_CurrentEffect = effect;
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


    /// @brief  how much longer until the current effect is done playing
    /// @return the amount of remaining time
    float EffectAnimator::GetRemainingTime() const
    {
        return m_CurrentEffect->GetTotalTime() - m_Time;
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
// private: virtual override methods
//-----------------------------------------------------------------------------


    /// @brief  called once when entering the scene
    void EffectAnimator::OnInit()
    {
        Behaviors< EffectAnimator >()->AddComponent( this );

        m_Transform.Init( GetEntity() );
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

        m_Time += dt * m_Speed;
        float duration = m_CurrentEffect->GetTotalTime();

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
            m_Transform->SetMatrix( m_Transform->GetMatrix() * m_CurrentEffect->SampleAtTime( m_Time ) );
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


        ImGui::DragFloat( "Time"     , &m_Time      );
        ImGui::DragFloat( "Speed"    , &m_Speed     );
        ImGui::DragInt(   "LoopCount", &m_LoopCount );
        ImGui::Checkbox(  "IsPlaying", &m_IsPlaying );

        // TODO: utilize Inspection::SelectAssetFromLibrary
        
        std::map< std::string, TransformAnimation* > const& effects = AssetLibrary<TransformAnimation>()->GetAssets();
        
        std::vector< std::string > effectNames = { "Select Effect" };
        int selectedEffect = 0;
        int i = 1;
        for ( auto effect : effects )
        {
            if ( effect.second == m_CurrentEffect )
            {
                selectedEffect = i;
            }
            effectNames.push_back( effect.first );
            ++i;
        }
        
        if ( ImGui::BeginCombo( "Effect", effectNames[ selectedEffect ].c_str() ) ) // Display the selected effect name
        {
            for ( i = 0; i < effectNames.size(); ++i )
            {
                bool isSelected = (selectedEffect == i);
                if ( ImGui::Selectable( effectNames[ i ].c_str(), isSelected ) )
                {
                    selectedEffect = i; // Change the selected scene
                }
        
                if ( isSelected )
                {
                    ImGui::SetItemDefaultFocus(); // Automatically focus on the selected item
                }
            }
            ImGui::EndCombo();
        }
        
        if ( selectedEffect != 0 )
        {
            m_CurrentEffect = AssetLibrary<TransformAnimation>()->GetAsset( effectNames[ selectedEffect ] );
        }

    }

//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------

    /// @brief  reads the current effect
    /// @param  stream  the json data to read from
    void EffectAnimator::readCurrentEffect( nlohmann::ordered_json const& data )
    {
        m_CurrentEffect = AssetLibrary<TransformAnimation>()->GetAsset( Stream::Read<std::string>(data) );
    }

    /// @brief  reads the current time
    /// @param  stream  the json data to read from
    void EffectAnimator::readTime( nlohmann::ordered_json const& data )
    {
        m_Time = Stream::Read<float>(data);
    }

    /// @brief  reads the playback speed
    /// @param  stream  the json data to read from
    void EffectAnimator::readSpeed( nlohmann::ordered_json const& data )
    {
        m_Speed = Stream::Read<float>(data);
    }

    /// @brief  reads the loop count
    /// @param  stream  the json data to read from
    void EffectAnimator::readLoopCount( nlohmann::ordered_json const& data )
    {
        m_LoopCount = Stream::Read<int>(data);
    }

    /// @brief  reads whether the effect is currently playing
    /// @param  stream  the json data to read from
    void EffectAnimator::readIsPlaying( nlohmann::ordered_json const& data )
    {
        m_IsPlaying = Stream::Read<bool>(data);
    }

    /// @brief  Write all EffectAnimator data to a JSON file.
    /// @return The JSON file containing the EffectAnimator data.
    nlohmann::ordered_json EffectAnimator::Write() const
    {
        nlohmann::ordered_json data;

        data["Time"] = m_Time;
        data["Speed"] = m_Speed;
        data["LoopCount"] = m_LoopCount;
        data["IsPlaying"] = m_IsPlaying;

        std::string const& name = AssetLibrary<TransformAnimation>()->GetAssetName(m_CurrentEffect);
        if (!name.empty())
        {
            data["CurrentEffect"] = name;
        }

        return data;
    }

    /// @brief  map of the read methods for this Component
    ReadMethodMap< EffectAnimator > EffectAnimator::s_ReadMethods = {
        { "CurrentEffect", &readCurrentEffect },
        { "Time"         , &readTime          },
        { "Speed"        , &readSpeed         },
        { "LoopCount"    , &readLoopCount     },
        { "IsPlaying"    , &readIsPlaying     }
    };


//-----------------------------------------------------------------------------
// private: copying
//-----------------------------------------------------------------------------

    /// @brief  copy-constructor for the EffectAnimator
    /// @param  other   the other EffectAnimator to copy
    EffectAnimator::EffectAnimator( const EffectAnimator& other ) :
        Behavior( other ),
        m_CurrentEffect( other.m_CurrentEffect ),
        m_IsPlaying( other.m_IsPlaying ),
        m_LoopCount( other.m_LoopCount ),
        m_Speed( other.m_Speed ),
        m_Time( other.m_Time )
    {}

//-----------------------------------------------------------------------------
