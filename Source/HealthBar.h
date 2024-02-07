/// @file       HealthBar.h
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      health bar UI that displays offset from an Entity with a Health component
/// @version    0.1
/// @date       2023-10-20
/// 
/// @copyright  Copyright (c) 2023 Digipen Institute of Technology

#pragma once

#include "Behavior.h"

class Transform;
class UiBarSprite;
class Health;


/// @brief  health bar UI that displays offset from an Entity with a Health component
class HealthBar : public Behavior
{
//-----------------------------------------------------------------------------
public: // types
//-----------------------------------------------------------------------------

    
//-----------------------------------------------------------------------------
private: // types
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
public: // accessors
//-----------------------------------------------------------------------------


    


//-----------------------------------------------------------------------------
public: //  methods
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
private: // virtual override methods
//-----------------------------------------------------------------------------


    /// @brief  called once when entering the scene
    virtual void OnInit() override;

    /// @brief  called once when exiting the scene
    virtual void OnExit() override;


    /// @brief  called once per graphics frame
    /// @param  dt  the length of time the frame lasts
    virtual void OnUpdate( float dt ) override;


    /// @brief  called every time after the Entity this Component is attached to's heirarchy changes
    virtual void OnHeirarchyChange() override;


//-----------------------------------------------------------------------------
private: // members
//-----------------------------------------------------------------------------


    /// @brief  the offset from the target Entity to display the health bar
    glm::vec2 m_Offset = { 0, 0 };


    /// @brief  whether the bar should be hidden when the health is full
    bool m_HideWhenFull = true;

    /// @brief  how long the opacity takes to animate in and out when the health is full
    float m_OpacityAnimationTime = 1.0f;

    /// @brief  the maximum opacity of the health bar
    float m_MaxOpacity = 1.0f;


    /// @brief  the acceleration of how quickly the recent health display depletes
    float m_RecentHealthAcceleration = 1.0f;


    /// @brief  the current velocity of recent health depletion
    float m_RecentHealthVelocity = 0.0f;

    /// @brief  the portion of current health
    float m_CurrentHealthPortion = 1.0f;

    /// @brief  the proportion of recent health
    float m_RecentHealthPortion = 1.0f;


    /// @brief  the Transform attached to this HealthBar
    Transform* m_Transform = nullptr;

    /// @brief  the UiBarSprite attached to this HealthBar
    UiBarSprite* m_UiBarSprite = nullptr;


    /// @brief  the name of the entity to track the health of
    std::string m_TargetEntityName = "";

    /// @brief  the Health component of the Entity to display the health of
    Health* m_TargetHealth = nullptr;


    /// @brief  the Transform component of the Entity to display the health of
    Transform* m_ParentTransform = nullptr;


//-----------------------------------------------------------------------------
private: // helper methods
//-----------------------------------------------------------------------------


    /// @brief  callback called whenever the health component changes
    void onHealthChangedCallback();


    /// @brief  updates the recent health portion of the health bar
    /// @param  dt  the length of time the frame lasts
    void updateRecentHealth( float dt );


    /// @brief  updates the Sprite and Transform attached to the HealthBar
    /// @param  dt  the length of time the frame lasts
    void updateVisuals( float dt );


//-----------------------------------------------------------------------------
private: // inspection
//-----------------------------------------------------------------------------

    
    /// @brief  displays this HealthBar in the Inspector
    virtual void Inspector() override;


//-----------------------------------------------------------------------------
private: // reading
//-----------------------------------------------------------------------------


    /// @brief  reads the offset from the target Entity to display the health bar
    /// @param  data    the json data to read from
    void readOffset( nlohmann::ordered_json const& data );


    /// @brief  reads whether the bar should be hidden when the health is full
    /// @param  data    the json data to read from
    void readHideWhenFull( nlohmann::ordered_json const& data );

    /// @brief  how long the opacity takes to animate in and out when the health is full
    /// @param  data    the json data to read from
    void readOpacityAnimationTime( nlohmann::ordered_json const& data );

    /// @brief  the maximum opacity of the health bar
    /// @param  data    the json data to read from
    void readMaxOpacity( nlohmann::ordered_json const& data );


    /// @brief  reads the acceleration of how quickly the recent health display depletes
    /// @param  data    the json data to read from
    void readRecentHealthAcceleration( nlohmann::ordered_json const& data );


    /// @brief  reads the name of the entity to track the health of
    /// @param  data    the json data to read from
    void readTargetEntityName( nlohmann::ordered_json const& data );


//-----------------------------------------------------------------------------
public: // reading / writing
//-----------------------------------------------------------------------------


    /// @brief  gets the map of read methods for this Component
    /// @return the map of read methods for this Component
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override;


    /// @brief  Write all HealthBar data to a JSON file.
    /// @return The JSON file containing the HealthBar data.
    virtual nlohmann::ordered_json Write() const override;


//-----------------------------------------------------------------------------
public: // constructor / Destructor
//-----------------------------------------------------------------------------


    /// @brief  constructor
    HealthBar();


//-----------------------------------------------------------------------------
public: // copying
//-----------------------------------------------------------------------------


    /// @brief  clones this HealthBar
    /// @return the newly created clone of this HealthBar
    virtual HealthBar* Clone() const override;


//-----------------------------------------------------------------------------
private: // copying
//-----------------------------------------------------------------------------


    /// @brief  copy-constructor for the HealthBar
    /// @param  other   the other HealthBar to copy
    HealthBar( const HealthBar& other );

    // diable = operator
    void operator =( HealthBar const& ) = delete;


//-----------------------------------------------------------------------------
};
