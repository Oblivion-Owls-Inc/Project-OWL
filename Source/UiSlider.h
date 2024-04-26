/// @file       UiSlider.h
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      Ui Slider component
/// @version    0.1
/// @date       2024-03-26
/// 
/// @copyright  Copyright (c) 2024 Digipen Institute of Technology

#pragma once




class UiBarSprite;
class Sprite;
class UiElement;

#include "ActionReference.h"


/// @brief  Ui Slider component
class UiSlider : public Behavior
{
//-----------------------------------------------------------------------------
public: // constructor / Destructor
//-----------------------------------------------------------------------------


    /// @brief  default constructor
    UiSlider();


//-----------------------------------------------------------------------------
public: // methods
//-----------------------------------------------------------------------------


    /// @brief  adds a callback to call whenever the slider is moved
    /// @param  ownerId     the ID of the owner of the callback to add
    /// @param  callback    the callback to add
    void AddOnSliderValueChangedCallback( unsigned ownerId, std::function< void( float newValue ) > const& callback );

    /// @brief  removes a callback to call whenever the slider is moved
    /// @param  ownerId the ID of the owner of the callback to remove
    void RemoveOnSliderValueChangedCallback( unsigned ownerId );


//-----------------------------------------------------------------------------
public: // accessors
//-----------------------------------------------------------------------------


    /// @brief  gets the current value of the slider from 0.0f to 1.0f
    /// @return the current value of the slider from 0.0f to 1.0f
    float GetValue() const;

    /// @brief  sets the current value of the slider from 0.0f to 1.0f
    /// @param  value   the value from 0.0f to 1.0f to set the slider to
    void SetValue( float value );


//-----------------------------------------------------------------------------
public: // virtual override methods
//-----------------------------------------------------------------------------


    /// @brief  called once when entering the scene
    virtual void OnInit() override;

    /// @brief  called once when exiting the scene
    virtual void OnExit() override;


    /// @brief  called every simulation frame
    virtual void OnFixedUpdate() override;


    /// @brief  called after a child is added to this Entity
    /// @param  child   the child that was added
    virtual void OnAddChild( Entity* child ) override;

    /// @brief  called when a child is removed from this Entity
    /// @param  child   the child that is being removed
    virtual void OnRemoveChild( Entity* child ) override;


//-----------------------------------------------------------------------------
private: // members
//-----------------------------------------------------------------------------


    /// @brief  the current value of the slider from 0.0f to 1.0f
    float m_Value = 0.0f;


    /// @brief  the control action used to drag the slider
    ActionReference m_DragSliderAction;


    /// @brief  whether the slider is currently being dragged
    bool m_IsBeingDragged = false;


    /// @brief  the UiElement Transform attached to this Entity
    ComponentReference< UiElement > m_UiElement;

    /// @brief  the UiBarSprite Component attached to this Entity
    ComponentReference< UiBarSprite > m_UiBarSprite;


    /// @brief  the UiElement Transform attached to the handle of this UiSlider
    ComponentReference< UiElement, false > m_HandleUiElement;

    /// @brief  the Sprite Component attached to the handle of this UiSlider
    ComponentReference< Sprite, false > m_HandleSprite;


    /// @brief  the callbacks to call whenever the slider's value is changed.
    std::map< unsigned, std::function< void ( float newValue ) > > m_OnSliderValueChangedCallbacks = {};


//-----------------------------------------------------------------------------
private: // methods
//-----------------------------------------------------------------------------


    /// @brief  updates the UiBarSprite and handle position
    void updateVisuals();


    /// @brief  calls the OnSliderValueChanded callbacks attached to this UiSlider
    void callOnSliderValueChangedCallbacks();


//-----------------------------------------------------------------------------
public: // inspection
//-----------------------------------------------------------------------------


    /// @brief  shows the inspector for UiSlider
    virtual void Inspector() override;


//-----------------------------------------------------------------------------
private: // reading
//-----------------------------------------------------------------------------


    /// @brief  reads the current value of the slider
    /// @param  data    the JSON data to read from
    void readValue( nlohmann::ordered_json const& data );


    /// @brief  reads the control action used to drag the slider
    /// @param  data    the JSON data to read from
    void readDragSliderAction( nlohmann::ordered_json const& data );


//-----------------------------------------------------------------------------
public: // reading / writing
//-----------------------------------------------------------------------------


    /// @brief  gets the map of read methods for this UiSlider
    /// @return the map of read methods for this UiSlider
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override;


    /// @brief  writes this UiSlider to JSON
    /// @return the JSON data of this UiSlider
    virtual nlohmann::ordered_json Write() const override;

    
//-----------------------------------------------------------------------------
public: // copying
//-----------------------------------------------------------------------------


    /// @brief  clones this UiSlider
    /// @return the newly created clone of this UiSlider
    virtual UiSlider* Clone() const override;


//-----------------------------------------------------------------------------
private: // copying
//-----------------------------------------------------------------------------


    /// @brief  copy-constructor for the UiSlider
    /// @param  other   the other UiSlider to copy
    UiSlider( UiSlider const& other );


    // disable assignment operator
    void operator =( UiSlider const& ) = delete;


//-----------------------------------------------------------------------------
};
