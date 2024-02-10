/// @file Component.h
/// @author Tyler Birdsall (tyler.birdsall@digipen.edu)
/// @brief Virtual component header
/// @version 0.1
/// @date 2023-09-05
/// 
/// @copyright Copyright (c) 2023 Digipen Institute of Technology

#pragma once

#include <typeindex>
#include <map>
#include <string>

#include "ISerializable.h"
#include "Stream.h"

class Entity;

class Component : public ISerializable
{
//-----------------------------------------------------------------------------
public: // destructor
//-----------------------------------------------------------------------------


	/// @brief virtual destructor
	virtual ~Component() {};


//-----------------------------------------------------------------------------
protected: // constructors
//-----------------------------------------------------------------------------


    /// @brief default component constructor
    /// @param type what type of component this is
    Component( std::type_index m_Type );

    /// @brief copy constructor
    /// @param the component to clone
    Component( Component const& other );


//-----------------------------------------------------------------------------
public: // virtual methods
//-----------------------------------------------------------------------------


    /// @brief called when this Component's Entity is added to the Scene
    virtual void OnInit() {};

    /// @brief  called when this Component's Entity is removed from the Scene
    virtual void OnExit() {};


    /// @brief  called every time after the Entity this Component is attached to's heirarchy changes
    virtual void OnHeirarchyChange() {};
    

    /// @brief Used by the Debug System to display information about this Component
    virtual void Inspector() {};


    /// @brief  called whenever another component is added to this component's Entity in the inspector
    /// @param  component   the component that was added
    virtual void OnInspectorAddComponent( Component* component ) {};

    /// @brief  called whenever another component is removed from this component's Entity in the inspector
    /// @param  component   the component that will be removed
    virtual void OnInspectorRemoveComponent( Component* component ) {};


    /// @brief virtual component clone function
    /// @return new clone of component
    virtual Component* Clone() const = 0;


//-----------------------------------------------------------------------------
public: // Accessors
//-----------------------------------------------------------------------------


    /// @brief  gets the components type
    /// @return component type
    std::type_index GetType() const { return m_Type; }

    /// @brief  sets the parent entity of the component
    /// @param  entity  the parent entity of the component
    void SetEntity( Entity* entity ) { m_Entity = entity; }

    /// @brief returns the components parent entity
    /// @return the parent entity of the component
    Entity* GetEntity() const { return m_Entity; }

    /// @brief  gets the Id of this Component
    /// @return the Id of this Component
    unsigned GetId() const { return m_Id; }


//-----------------------------------------------------------------------------
private: // member variables
//-----------------------------------------------------------------------------


	/// @brief  the type of this Component
	std::type_index m_Type;

	/// @brief  the parent Entity of this Component
	Entity* m_Entity;

    /// @brief  the ID of this Component
    unsigned m_Id;


//-----------------------------------------------------------------------------
public: // methods
//-----------------------------------------------------------------------------


    /// @brief  base Inspector for Components
    void BaseComponentInspector();
    

//-----------------------------------------------------------------------------
};
