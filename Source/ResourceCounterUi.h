/// @file       ResourceCounterUi.h
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      Ui Component that displays a resource sprite and a count
/// @version    0.1
/// @date       2024-02-21
/// 
/// @copyright  Copyright (c) 2024 Digipen Institute of Technology

#pragma once

#include "Component.h"

#include "ItemStack.h"

#include "ComponentReference.h"
class Sprite;
class Text;
class UiElement;


/// @brief  Ui Component that displays a count of resources
class ResourceCounterUi : public Component
{
//-----------------------------------------------------------------------------
public: // constructor / Destructor
//-----------------------------------------------------------------------------


    /// @brief  default constructor
    ResourceCounterUi();


//-----------------------------------------------------------------------------
public: // methods
//-----------------------------------------------------------------------------


    /// @brief  sets the resources this ResourceCounter should display
    /// @param  itemStack   the ItemStack of resources to display
    void SetResources( ItemStack const& itemStack );


//-----------------------------------------------------------------------------
public: // accessors
//-----------------------------------------------------------------------------

    
    /// @brief  gets the itemId of the resource this ResourceCounter displays
    /// @return the itemId of the resource this ResourceCounter displays
    int GetResourceId() const;


    /// @brief  gets the UiElement attached to this ResourceCounterUi
    /// @return the UiElement attached to this ResourceCounterUi
    UiElement* GetUiElement();

    /// @brief  gets the Sprite attached to this ResourceCounterUi
    /// @return the Sprite attached to this ResourceCounterUi
    Sprite* GetSprite();

    /// @brief  gets the Text attached to this ResourceCounterUi
    /// @return the Text attached to this ResourceCounterUi
    Text* GetText();


//-----------------------------------------------------------------------------
public: // virtual override methods
//-----------------------------------------------------------------------------


    /// @brief  called once when entering the scene
    virtual void OnInit() override;

    /// @brief  called once when exiting the scene
    virtual void OnExit() override;


//-----------------------------------------------------------------------------
private: // members
//-----------------------------------------------------------------------------


    /// @brief  the resources this ResourceCounterUi displays
    ItemStack m_Resources;


    /// @brief  the UiElement attached to this Entity
    ComponentReference< UiElement, false > m_UiElement;

    /// @brief  the Sprite attached to this Entity
    ComponentReference< Sprite > m_Sprite;
    
    /// @brief  the Text attached to this Entity
    ComponentReference< Text > m_Text;


//-----------------------------------------------------------------------------
private: // methods
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
public: // inspection
//-----------------------------------------------------------------------------


    /// @brief  shows the inspector for ResourceCounterUi
    virtual void Inspector() override;


//-----------------------------------------------------------------------------
private: // reading
//-----------------------------------------------------------------------------


    /// @brief  reads the resources this ResourceCounterUi displays
    /// @param  data    the JSON data to read from
    void readResources( nlohmann::ordered_json const& data );


//-----------------------------------------------------------------------------
public: // reading / writing
//-----------------------------------------------------------------------------


    /// @brief  gets the map of read methods for this ResourceCounterUi
    /// @return the map of read methods for this ResourceCounterUi
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override;


    /// @brief  writes this ResourceCounterUi to JSON
    /// @return the JSON data of this ResourceCounterUi
    virtual nlohmann::ordered_json Write() const override;

    
//-----------------------------------------------------------------------------
public: // copying
//-----------------------------------------------------------------------------


    /// @brief  clones this ResourceCounterUi
    /// @return the newly created clone of this ResourceCounterUi
    virtual ResourceCounterUi* Clone() const override;


//-----------------------------------------------------------------------------
private: // copying
//-----------------------------------------------------------------------------


    /// @brief  copy-constructor for the ResourceCounterUi
    /// @param  other   the other ResourceCounterUi to copy
    ResourceCounterUi( ResourceCounterUi const& other );


    // disable assignment operator
    void operator =( ResourceCounterUi const& ) = delete;


//-----------------------------------------------------------------------------
};
