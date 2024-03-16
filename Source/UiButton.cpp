/// @file       UiButton.h
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      Ui Button that sends an event when clicked
/// @version    0.1
/// @date       2024-01-31
/// 
/// @copyright  Copyright (c) 2024 Digipen Institute of Technology

#pragma once
#include "pch.h" // precompiled header has to be included first
#include "UiButton.h"
#include "BehaviorSystem.h"

#include "Entity.h"

#include "InputSystem.h"
#include "EventSystem.h"

#include "DebugSystem.h"

#include "ComponentReference.t.h"

#include "RenderSystem.h"

#include "Engine.h"

    
//-----------------------------------------------------------------------------
// public: constructor / Destructor
//-----------------------------------------------------------------------------


    /// @brief  default constructor
    UiButton::UiButton() :
        Behavior( typeid( UiButton ) )
    {}


//-----------------------------------------------------------------------------
// public: methods
//-----------------------------------------------------------------------------


    /// @brief  adds a callback to be called whenever the button is clicked
    /// @param  ownerId     the ID of the owner of the callback to add
    /// @param  callback    the callback to add
    void UiButton::AddOnClickedCallback( unsigned ownerId, std::function< void () > callback )
    {
        m_OnClickedCallbacks.emplace( ownerId, callback );
    }

    /// @brief  removes a callback to be called whenever the button is clicked
    /// @param  ownerId     the ID of the owner of the callback to remove
    void UiButton::RemoveOnOnClickedCallback( unsigned ownerId )
    {
        auto it = m_OnClickedCallbacks.find( ownerId );
        if ( it == m_OnClickedCallbacks.end() )
        {
            Debug() << "WARNING: could not find callback belonging to " << ownerId << " to remove from UiButton OnClicked" << std::endl;
            return;
        }

        m_OnClickedCallbacks.erase( it );
    }


//-----------------------------------------------------------------------------
// public: accessors
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// public: virtual override methods
//-----------------------------------------------------------------------------


    /// @brief  called once when entering the scene
    void UiButton::OnInit()
    {
        Behaviors< UiButton >()->AddComponent( this );

        m_UiElement        .Init( GetEntity() );
        m_Sprite           .Init( GetEntity() );
        m_AudioPlayer      .Init( GetEntity() );

        m_PressSound       .SetOwnerName( GetName() );
        m_PressSound       .Init();
        m_ReleaseSound     .SetOwnerName( GetName() );
        m_ReleaseSound     .Init();

        m_UpButtonEntity   .SetOwnerName(GetName());
        m_UpButtonEntity   .Init();
        m_BelowButtonEntity.SetOwnerName(GetName());
        m_BelowButtonEntity.Init();

        m_VerticalNavigationAction .SetOwnerName(GetName());
        m_VerticalNavigationAction .Init();
        m_PressAction      .SetOwnerName(GetName());
        m_PressAction      .Init();

        s_currentlyTargetedButton = this;
    }

    /// @brief  called once when exiting the scene
    void UiButton::OnExit()
    {
        Behaviors< UiButton >()->RemoveComponent( this );

        m_UiElement        .Exit();
        m_Sprite           .Exit();
        m_AudioPlayer      .Exit();

        m_UpButtonEntity   .Exit();
        m_BelowButtonEntity.Exit();

        m_VerticalNavigationAction .Exit();
        m_PressAction      .Exit();

        if (s_currentlyTargetedButton == this && Behaviors< UiButton >()->GetComponents().empty() == false)
        {
            s_currentlyTargetedButton = Behaviors< UiButton >()->GetComponents().back();
        }
    }


    /// @brief  called every graphics frame
    /// @param  dt  the duration of the frame
    void UiButton::OnUpdate( float dt )
    {
        if ( m_UiElement == nullptr )
        {
            return;
        }

        // use if statements instead of switch statement so that the functions can cascade if an earlier function changes the state
        if ( m_State == ButtonState::Idle )
        {
            updateWhenIdle();
        }

        if ( m_State == ButtonState::Hovered )
        {
            updateWhenHovered();
        }

        if ( m_State == ButtonState::Down )
        {
            updateWhenDown();
        }

        if (m_VerticalNavigationAction == nullptr)
        {
            return;
        }

        m_NavigationTimer -= GameEngine()->GetFixedFrameDuration();

        if (isTargeted() && s_currentlyDownButton == nullptr && m_NavigationTimer <= 0.0f)
        {
            if (m_VerticalNavigationAction->GetAxis() > 0)
            {
                s_currentlyTargetedButton = m_UpButtonComponent;
                m_UpButtonComponent->m_NavigationTimer = m_NavigationDelay;
            }
            else if (m_VerticalNavigationAction->GetAxis() < 0)
            {
                s_currentlyTargetedButton = m_BelowButtonComponent;
                m_BelowButtonComponent->m_NavigationTimer = m_NavigationDelay;
            }
        }
    }

    /// @brief  called every simulation frame
    void UiButton::OnFixedUpdate()
    {
        if ( m_ActivatedSinceLastFixedUpdate )
        {
            m_ActivatedSinceLastFixedUpdate = false;
            callCallbacksAndEvents();
        }
    }


//-----------------------------------------------------------------------------
// private: static members
//-----------------------------------------------------------------------------


    /// @brief  the button that is currently down. static to ensure only one button is interacted with at a time
    UiButton const* UiButton::s_currentlyDownButton = nullptr;

    /// @brief Keeps track of currently targeted button
    UiButton const* UiButton::s_currentlyTargetedButton = nullptr;

//-----------------------------------------------------------------------------
// private: methods
//-----------------------------------------------------------------------------


    /// @brief  update that runs when this button is idle
    void UiButton::updateWhenIdle()
    {
        // check if the button should switch to the hovered state
        if ( s_currentlyDownButton == nullptr && isTargeted() == true )
        {
            m_State = ButtonState::Hovered;
            s_currentlyTargetedButton = this;
            if ( m_Sprite != nullptr )
            {
                m_Sprite->SetFrameIndex( m_HoveredFrame );
            }
        }
    }

    /// @brief  update that runs when this button is hovered
    void UiButton::updateWhenHovered()
    {
        // check if the button should switch to the idle state
        if ( s_currentlyDownButton != nullptr || isTargeted() == false )
        {
            m_State = ButtonState::Idle;
            if ( m_Sprite != nullptr )
            {
                m_Sprite->SetFrameIndex( m_IdleFrame );
            }
            return;
        }

        // check if the button should switch to the down state
        if ( m_PressAction != nullptr && m_PressAction->GetTriggered() )
        {
            // activate now if that's how this button is configured
            if ( m_ShouldActivateOnPress == true )
            {
                m_ActivatedSinceLastFixedUpdate = true;
            }

            s_currentlyDownButton = this;

            m_State = ButtonState::Down;
            if ( m_Sprite != nullptr )
            {
                m_Sprite->SetFrameIndex( m_DownFrame );
            }

            if ( m_AudioPlayer != nullptr )
            {
                m_AudioPlayer->SetSound( m_PressSound );
                m_AudioPlayer->Play();
            }
        }
    }

    /// @brief  update that runs when this button is down
    void UiButton::updateWhenDown()
    {
        // wait for the mouse to be released
        if (m_PressAction != nullptr && m_PressAction->GetReleased())
        {
            s_currentlyDownButton = nullptr;

            if ( m_AudioPlayer != nullptr )
            {
                m_AudioPlayer->SetSound( m_ReleaseSound );
                m_AudioPlayer->Play();
            }

            // only activate if still hovered
            if ( isTargeted() == true )
            {
                // activate now if that's how this button is configured
                if ( m_ShouldActivateOnPress == false )
                {
                    m_ActivatedSinceLastFixedUpdate = true;
                }

                m_State = ButtonState::Hovered;

                if ( m_Sprite != nullptr )
                {
                    m_Sprite->SetFrameIndex( m_HoveredFrame );
                }
            }
            else
            {
                m_State = ButtonState::Idle;

                if ( m_Sprite != nullptr )
                {
                    m_Sprite->SetFrameIndex( m_IdleFrame );
                }
            }
        }
    }

    /// @brief  calls all callbacks and events attached to this Button
    void UiButton::callCallbacksAndEvents()
    {
        if ( m_ClickEventName.empty() == false )
        {
            Events()->BroadcastEvent< std::string >( m_ClickEventName );
        }

        for ( auto& [ id, callback ] : m_OnClickedCallbacks )
        {
            callback();
        }
    }

    /// @brief  Determines if a button is being hovered.
    /// @return Is the button in a hovered state.
    bool UiButton::isTargeted() const
    {
        if ( Input()->IsControllerMostRecentInput() == false )
        {
            return Renderer()->GetMouseOverSprite() == m_Sprite;
        }
        else
        {
            return s_currentlyTargetedButton == this;
        }
    }


//-----------------------------------------------------------------------------
// public: inspection
//-----------------------------------------------------------------------------


    /// @brief  shows the inspector for UiButton
    void UiButton::Inspector()
    {

        ImGui::InputText( "OnClick event name", &m_ClickEventName );

        ImGui::Checkbox( "activate on press instead of release", &m_ShouldActivateOnPress );

        ImGui::DragInt( "idle frame index"   , (int*)&m_IdleFrame   , 0.05f, 0, INT_MAX );
        ImGui::DragInt( "hovered frame index", (int*)&m_HoveredFrame, 0.05f, 0, INT_MAX );
        ImGui::DragInt( "down frame index"   , (int*)&m_DownFrame   , 0.05f, 0, INT_MAX );
        ImGui::DragFloat( "Navigation Delay" , &m_NavigationDelay, 0.05f, 0, INFINITY   );

        m_PressSound       .Inspect( "button down sound" );
        m_ReleaseSound     .Inspect( "button up sound"   );

        m_UpButtonEntity   .Inspect("Button Above This One");
        m_BelowButtonEntity.Inspect("Button Below This One");

        m_VerticalNavigationAction .Inspect("Action for Navigating The Menu");
        m_PressAction      .Inspect("Action for Controller Button Pressing");
    }


//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------

    /// @brief  reads the name of the event this UiButton broadcasts when it's clicked.
    /// @param  data    the JSON data to read from
    void UiButton::readClickEventName( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_ClickEventName, data );
    }

    /// @brief  reads whether this UiButton should activate when it's pressed instead of when it's released
    /// @param  data    the JSON data to read from
    void UiButton::readShouldActivateOnPress( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_ShouldActivateOnPress, data );
    }

    /// @brief  reads the frame the UiButton displays when idle
    /// @param  data    the JSON data to read from
    void UiButton::readIdleFrame( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_IdleFrame, data );
    }

    /// @brief  reads the frame the UiButton displays when hovered
    /// @param  data    the JSON data to read from
    void UiButton::readHoveredFrame( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_HoveredFrame, data );
    }

    /// @brief  reads the frame the UiButton displays when down
    /// @param  data    the JSON data to read from
    void UiButton::readDownFrame( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_DownFrame, data );
    }

    /// @brief  reads the sound this UiButton plays when it is pressed
    /// @param  data    the JSON data to read from
    void UiButton::readPressSound( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_PressSound, data );
    }

    /// @brief  reads the sound this UiButton plays when it is released
    /// @param  data    the JSON data to read from
    void UiButton::readReleaseSound( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_ReleaseSound, data );
    }

    /// @brief Reads in a reference to the button above this one.
    /// @param data The JSON data to read from.
    void UiButton::readUpButtonEntity(nlohmann::ordered_json const& data)
    {
        Stream::Read(m_UpButtonEntity, data);
    }

    /// @brief Reads in a reference to the button below this one.
    /// @param data The JSON data to read from.
    void UiButton::readBelowButtonEntity(nlohmann::ordered_json const& data)
    {
        Stream::Read(m_BelowButtonEntity, data);
    }

    /// @brief Read in the action from a JSON file.
    /// @param data The JSON data to read from.
    void UiButton::readNavigationAction(nlohmann::ordered_json const& data)
    {
        Stream::Read(m_VerticalNavigationAction, data);
    }

    /// @brief Read in the action from a JSON file.
    /// @param data The JSON data to read from.
    void UiButton::readPressAction(nlohmann::ordered_json const& data)
    {
        Stream::Read(m_PressAction, data);
    }

    /// @brief Read in the action from a JSON file.
    /// @param data The JSON data to read from.
    void UiButton::readNavigationDelay(nlohmann::ordered_json const& data)
    {
        Stream::Read(m_NavigationDelay, data);
    }

//-----------------------------------------------------------------------------
// public: reading / writing
//-----------------------------------------------------------------------------


    /// @brief  gets the map of read methods for this Component
    /// @return the map of read methods for this Component
    ReadMethodMap< ISerializable > const& UiButton::GetReadMethods() const
    {
        static ReadMethodMap< UiButton > const readMethods = {
            { "ClickEventName"       , &UiButton::readClickEventName        },
            { "ShouldActivateOnPress", &UiButton::readShouldActivateOnPress }, 
            { "IdleFrame"            , &UiButton::readIdleFrame             },
            { "HoveredFrame"         , &UiButton::readHoveredFrame          },
            { "DownFrame"            , &UiButton::readDownFrame             },
            { "PressSound"           , &UiButton::readPressSound            },
            { "ReleaseSound"         , &UiButton::readReleaseSound          },
            { "UpButton"             , &UiButton::readUpButtonEntity        },
            { "BelowButton"          , &UiButton::readBelowButtonEntity     },
            { "NavigationAction"     , &UiButton::readNavigationAction      },
            { "PressAction"          , &UiButton::readPressAction           },
            { "NavigationDelay"      , &UiButton::readNavigationDelay       },
        };

        return (ReadMethodMap< ISerializable > const&)readMethods;
    }

    /// @brief  Writes all AudioPlayr data to a JSON file.
    /// @return The JSON file containing the data.
    nlohmann::ordered_json UiButton::Write() const
    {
        nlohmann::ordered_json json;

        json [ "ClickEventName"        ] = Stream::Write( m_ClickEventName        );
        json [ "ShouldActivateOnPress" ] = Stream::Write( m_ShouldActivateOnPress );
        json [ "IdleFrame"             ] = Stream::Write( m_IdleFrame             );
        json [ "HoveredFrame"          ] = Stream::Write( m_HoveredFrame          );
        json [ "DownFrame"             ] = Stream::Write( m_DownFrame             );
        json [ "PressSound"            ] = Stream::Write( m_PressSound            );
        json [ "ReleaseSound"          ] = Stream::Write( m_ReleaseSound          );
        json [ "UpButton"              ] = m_UpButtonEntity.Write(                );
        json [ "BelowButton"           ] = m_BelowButtonEntity.Write(             );
        json [ "NavigationAction"      ] = m_VerticalNavigationAction.Write(              );
        json [ "PressAction"           ] = m_PressAction.Write(                   );
        json [ "NavigationDelay"       ] = Stream::Write( m_NavigationDelay       );

        return json;
    }

//-----------------------------------------------------------------------------
// public: copying
//-----------------------------------------------------------------------------


    /// @brief  clones this UiButton
    /// @return the newly created clone of this UiButton
    UiButton* UiButton::Clone() const
    {
        return new UiButton( *this );
    }

//-----------------------------------------------------------------------------
// private: copying
//-----------------------------------------------------------------------------


    /// @brief  copy-constructor for the UiButton
    /// @param  other   the other UiButton to copy
    UiButton::UiButton( UiButton const& other ) :
        Behavior( other ),
        m_ClickEventName       ( other.m_ClickEventName                               ),
        m_ShouldActivateOnPress( other.m_ShouldActivateOnPress                        ),
        m_IdleFrame            ( other.m_IdleFrame                                    ),
        m_HoveredFrame         ( other.m_HoveredFrame                                 ),
        m_DownFrame            ( other.m_DownFrame                                    ),
        m_PressSound           ( other.m_PressSound                                   ),
        m_ReleaseSound         ( other.m_ReleaseSound                                 ),
        m_UpButtonEntity       ( other.m_UpButtonEntity, {&m_UpButtonComponent}       ),
        m_BelowButtonEntity    ( other.m_BelowButtonEntity, {&m_BelowButtonComponent} ),
        m_VerticalNavigationAction     ( other.m_VerticalNavigationAction                             ),
        m_PressAction          ( other.m_PressAction                                  ),
        m_NavigationDelay      ( other.m_NavigationDelay                              )  
    {}


//-----------------------------------------------------------------------------
