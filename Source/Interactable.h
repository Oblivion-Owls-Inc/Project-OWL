/// @file       Interactable.h
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      Component which can be interacted with by pressing a button when nearby
/// @version    0.1
/// @date       March 2024
/// 
/// @copyright  Copyright (c) 2024 Digipen Institute of Technology

#pragma once

#include "Component.h"

#include "ActionReference.h"
#include "EntityReference.h"
#include "ComponentReference.h"
class Transform;
class Sprite;
class Interactor;


/// @brief  Component which can be interacted with by pressing a button when nearby
class Interactable : public Component
{
//-----------------------------------------------------------------------------
public: // constructor / Destructor
//-----------------------------------------------------------------------------


    /// @brief  default constructor
    Interactable();


//-----------------------------------------------------------------------------
public: // methods
//-----------------------------------------------------------------------------

    
    /// @brief  interacts with this Interactable
    /// @param  interactor  the Interactor Component that interacted with this Interactable
    void Interact( Interactor* interactor );


    /// @brief  adds a callback to be called when this Interactable Component is interacted with
    /// @param  ownerId     the ID of the owner of the callback
    /// @param  callback    the callback to add
    void AddOnInteractCallback( unsigned ownerId, std::function< void ( Interactor* interactor ) > callback );

    /// @brief  removes an OnInteractCallback from this Interactable Component
    /// @param  ownerId     the ownerId of the callback to remove
    void RemoveOnInteractCallback( unsigned ownerId );


//-----------------------------------------------------------------------------
public: // accessors
//-----------------------------------------------------------------------------


    /// @brief  gets whether the Interactable can be interacted with
    /// @return whether the Interactable can be interacted with
    bool GetEnabled() const;

    /// @brief  sets whether the Interactable can be interacted with
    /// @param  enabled whether the Interactable can be interacted with
    void SetEnabled( bool enabled );

    /// @brief  gets the radius at which this Interactable can be interacted with
    /// @return the radius at which this Interactable can be interacted with
    float GetInteractionRadius() const;

    /// @brief  sets the radius at which this Interactable can be interacted with
    /// @param  radius  the radius at which this Interactable can be interacted with
    void SetInteractionRadius( float radius );

    /// @brief  gets the Transform attached to this Interactable
    /// @return the Transform attached to this Interactable
    Transform const* GetTransform() const;

    /// @brief  gets the Sprite used to display the interact control prompt
    /// @return the Sprite used to display the interact control prompt
    Sprite* GetPromptSprite();

    /// @brief  gets the Interact Action used to interact with this Interactable
    /// @return the Interact Action used to interact with this Interactable
    Action const* GetInteractAction() const;


//-----------------------------------------------------------------------------
public: // virtual override methods
//-----------------------------------------------------------------------------


    /// @brief  called once when entering the scene
    virtual void OnInit() override;

    /// @brief  called once when exiting the scene
    virtual void OnExit() override;


    /// @brief  called after a child is added to this Entity
    /// @param  child   the child Entity that was added
    virtual void OnAddChild( Entity* child ) override;

    /// @brief  called before a child is removed from this Entity
    /// @param  child   the child that will be removed
    virtual void OnRemoveChild( Entity* child ) override;


//-----------------------------------------------------------------------------
private: // members
//-----------------------------------------------------------------------------

    
    /// @brief  whether the Interactable can be interacted with
    bool m_Enabled = true;

    /// @brief  the radius at which this Interactable can be interacted with
    float m_InteractionRadius = 1.0f;

    /// @brief  the control Action used to interact with this Interactable
    ActionReference m_InteractAction;

    /// @brief  the offset position to display the prompt at
    glm::vec2 m_PromptOffset = { 0.0f, 1.0f };


    /// @brief  the Sprite used to display the interact control prompt
    ComponentReference< Sprite > m_PromptSprite;

    /// @brief  the Transform Component used to display the control prompt
    ComponentReference< Transform > m_PromptTransform;

    // /// @brief  the Entity used to display the interact control prompt
    // EntityReference m_PromptEntity = EntityReference( { &m_PromptSprite, &m_PromptTransform } );


    /// @brief  the Transform attached to this Interactable
    ComponentReference< Transform > m_Transform;


    /// @brief  the callbacks to call whenever this Interactable is interacted with
    std::vector<
        std::pair<
            unsigned,
            std::function< void ( Interactor* interactor ) >
        >
    > m_OnInteractCallbacks = {};

    std::string m_EventCast = "";



//-----------------------------------------------------------------------------
private: // methods
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
public: // inspection
//-----------------------------------------------------------------------------


    /// @brief  shows the inspector for Interactable
    virtual void Inspector() override;


//-----------------------------------------------------------------------------
private: // reading
//-----------------------------------------------------------------------------


    /// @brief  reads whether the Interactable can be interacted with
    /// @param  data    the JSON data to read from
    void readEnabled( nlohmann::ordered_json const& data );

    /// @brief  reads the radius at which this Interactable can be interacted with
    /// @param  data    the JSON data to read from
    void readInteractionRadius( nlohmann::ordered_json const& data );

    /// @brief  reads the control Action used to interact with this Interactable
    /// @param  data    the JSON data to read from
    void readInteractAction( nlohmann::ordered_json const& data );

    /// @brief  reads the offset position to display the prompt at
    /// @param  data    the JSON data to read from
    void readPromptOffset( nlohmann::ordered_json const& data );

    /// @brief  reads the event to cast when interacted with
    /// @param  data    the JSON data to read from
    void readEventCast(nlohmann::ordered_json const& data);

    // /// @brief  reads the Entity used to display the interact control prompt
    // /// @param  data    the JSON data to read from
    // void readPromptEntity( nlohmann::ordered_json const& data );


//-----------------------------------------------------------------------------
public: // reading / writing
//-----------------------------------------------------------------------------


    /// @brief  gets the map of read methods for this Interactable
    /// @return the map of read methods for this Interactable
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override;


    /// @brief  writes this Interactable to JSON
    /// @return the JSON data of this Interactable
    virtual nlohmann::ordered_json Write() const override;

    
//-----------------------------------------------------------------------------
public: // copying
//-----------------------------------------------------------------------------


    /// @brief  clones this Interactable
    /// @return the newly created clone of this Interactable
    virtual Interactable* Clone() const override;


//-----------------------------------------------------------------------------
private: // copying
//-----------------------------------------------------------------------------


    /// @brief  copy-constructor for the Interactable
    /// @param  other   the other Interactable to copy
    Interactable( Interactable const& other );


    // disable assignment operator
    void operator =( Interactable const& ) = delete;


//-----------------------------------------------------------------------------
};
