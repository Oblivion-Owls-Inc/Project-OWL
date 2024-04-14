/// @file       GeneratorActivationCostUi.h
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      displays the costs of activating generators
/// @version    0.1
/// @date       April 2024
/// 
/// @copyright  Copyright (c) 2024 Digipen Institute of Technology

#pragma once

#include "Behavior.h"

#include "ComponentReference.h"
class Interactor;
class Inventory;

#include "EntityReference.h"
class ResourcesUiManager;

class Interactable;


/// @brief  displays the costs of activating generators
class GeneratorActivationCostUi : public Behavior
{
//-----------------------------------------------------------------------------
public: // constructor / Destructor
//-----------------------------------------------------------------------------


    /// @brief  default constructor
    GeneratorActivationCostUi();


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


    /// @brief  called once every simulation frame
    virtual void OnFixedUpdate() override;


//-----------------------------------------------------------------------------
private: // members
//-----------------------------------------------------------------------------


    /// @brief  the offset from the Interactable's Transform of the cost UI
    glm::vec2 m_CostUiOffset = glm::vec2( 0.0f, 1.0f );


    /// @brief  the Interactor Component attached to this Entity
    ComponentReference< Interactor > m_Interactor;

    /// @brief  the Inventory Component attached to this entity
    ComponentReference< Inventory > m_Inventory;


    /// @brief  the ResourcesUiManager to use to display the cost
    ComponentReference< ResourcesUiManager > m_ResourcesUiManager;

    /// @brief  the Entity the ResourcesUiManager is attached to
    EntityReference m_ResourcesUiEntity = EntityReference( { &m_ResourcesUiManager } );


    /// @brief  the currently targetted interactable
    Interactable const* m_TargetedInteractable = nullptr;


//-----------------------------------------------------------------------------
private: // methods
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
public: // inspection
//-----------------------------------------------------------------------------


    /// @brief  shows the inspector for GeneratorActivationCostUi
    virtual void Inspector() override;


//-----------------------------------------------------------------------------
private: // reading
//-----------------------------------------------------------------------------

    
    /// @brief  read sthe offset from the Interactable's Transform of the cost UI
    /// @param  data    the JSON data to read from
    void readCostUiOffset( nlohmann::ordered_json const& data );


    /// @brief  reads the Entity the ResourcesUiManager is attached to
    /// @param  data    the JSON data to read from
    void readResourcesUiEntity( nlohmann::ordered_json const& data );


//-----------------------------------------------------------------------------
public: // reading / writing
//-----------------------------------------------------------------------------


    /// @brief  gets the map of read methods for this GeneratorActivationCostUi
    /// @return the map of read methods for this GeneratorActivationCostUi
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override;


    /// @brief  writes this GeneratorActivationCostUi to JSON
    /// @return the JSON data of this GeneratorActivationCostUi
    virtual nlohmann::ordered_json Write() const override;

    
//-----------------------------------------------------------------------------
public: // copying
//-----------------------------------------------------------------------------


    /// @brief  clones this GeneratorActivationCostUi
    /// @return the newly created clone of this GeneratorActivationCostUi
    virtual GeneratorActivationCostUi* Clone() const override;


//-----------------------------------------------------------------------------
private: // copying
//-----------------------------------------------------------------------------


    /// @brief  copy-constructor for the GeneratorActivationCostUi
    /// @param  other   the other GeneratorActivationCostUi to copy
    GeneratorActivationCostUi( GeneratorActivationCostUi const& other );


    // disable assignment operator
    void operator =( GeneratorActivationCostUi const& ) = delete;


//-----------------------------------------------------------------------------
};
