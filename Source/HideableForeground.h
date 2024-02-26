/// @file       HideableForeground.h
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      Component that hides an attached Sprite when it overlaps something
/// @version    0.1
/// @date       2024-02-25
/// 
/// @copyright  Copyright (c) 2024 Digipen Institute of Technology

#pragma once

#include "Behavior.h"

#include "ComponentReference.h"
class Sprite;
class Collider;


/// @brief  Component that hides an attached Sprite when it overlaps something
class HideableForeground : public Behavior
{
//-----------------------------------------------------------------------------
public: // constructor / Destructor
//-----------------------------------------------------------------------------


    /// @brief  default constructor
    HideableForeground();


//-----------------------------------------------------------------------------
public: // methods
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
public: // accessors
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
public: // virtual override methods
//-----------------------------------------------------------------------------


    /// @brief  called once when entering the scene
    virtual void OnInit() override;

    /// @brief  called once when exiting the scene
    virtual void OnExit() override;


    /// @brief  called every graphics frame
    /// @param  dt  the amount of time since the last frame
    virtual void OnUpdate( float dt ) override;


//-----------------------------------------------------------------------------
private: // members
//-----------------------------------------------------------------------------


    /// @brief  the opacity when overlapping something
    float m_OverlappedOpacity = 0.0f;

    /// @brief  the opacity when not overlapping something
    float m_DefaultOpacity = 1.0f;


    /// @brief  how quickly the opacity fades between the two states
    float m_FadeSpeed = 1.0f;


    /// @brief  whether this HideableForeground is currently overlapping something
    bool m_CurrentlyOverlapping = false;

    /// @brief  whether this HideableForeground is currently fading in/out
    bool m_IsFading = false;


    /// @brief  the Sprite attached to this HideableForeground
    ComponentReference< Sprite > m_Sprite;

    /// @brief  the Collider attached to this HideableForeground
    ComponentReference< Collider > m_Collider;


//-----------------------------------------------------------------------------
private: // methods
//-----------------------------------------------------------------------------


    /// @brief  callback called when entering a collision
    /// @param  collider    the collider that was collided with
    void onCollisionEnter( Collider* collider );

    /// @brief  callback called when entering a collision
    /// @param  collider    the collider that was collided with
    void onCollisionExit( Collider* collider );


//-----------------------------------------------------------------------------
public: // inspection
//-----------------------------------------------------------------------------


    /// @brief  shows the inspector for HideableForeground
    virtual void Inspector() override;


//-----------------------------------------------------------------------------
private: // reading
//-----------------------------------------------------------------------------


    /// @brief  reads the opacity when overlapping the player
    /// @param  data    the JSON data to read from
    void readOverlappedOpacity( nlohmann::ordered_json const& data );

    /// @brief  reads the opacity when not overlapping the player
    /// @param  data    the JSON data to read from
    void readDefaultOpacity( nlohmann::ordered_json const& data );

    /// @brief  reads how quickly the opacity fades between the two states
    /// @param  data    the JSON data to read from
    void readFadeSpeed( nlohmann::ordered_json const& data );


//-----------------------------------------------------------------------------
public: // reading / writing
//-----------------------------------------------------------------------------


    /// @brief  gets the map of read methods for this HideableForeground
    /// @return the map of read methods for this HideableForeground
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override;


    /// @brief  writes this HideableForeground to JSON
    /// @return the JSON data of this HideableForeground
    virtual nlohmann::ordered_json Write() const override;

    
//-----------------------------------------------------------------------------
public: // copying
//-----------------------------------------------------------------------------


    /// @brief  clones this HideableForeground
    /// @return the newly created clone of this HideableForeground
    virtual HideableForeground* Clone() const override;


//-----------------------------------------------------------------------------
private: // copying
//-----------------------------------------------------------------------------


    /// @brief  copy-constructor for the HideableForeground
    /// @param  other   the other HideableForeground to copy
    HideableForeground( HideableForeground const& other );


    // disable assignment operator
    void operator =( HideableForeground const& ) = delete;


//-----------------------------------------------------------------------------
};
