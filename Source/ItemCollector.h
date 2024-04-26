/// @file       ItemCollector.h
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      Component that attracts and collects item entities in the world
/// @version    0.1
/// @date       2023-10-20
/// 
/// @copyright  Copyright (c) 2024 Digipen Institute of Technology

#pragma once





#include "Inventory.h"

class ItemComponent;


/// @brief      Component that attracts and collects item entities in the world
class ItemCollector : public Behavior
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


    /// @brief  gets the Transform attached to this ItemCollector
    /// @return the Transform attached to this ItemCollector
    Transform* GetTransform() const;


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


    /// @brief  gets called once per simulation frame
    virtual void OnFixedUpdate() override;


//-----------------------------------------------------------------------------
private: // members
//-----------------------------------------------------------------------------


    /// @brief  the radius at which items will be instantly collected
    float m_CollectionRadius = 1.0f;

    /// @brief  the radius at which items will be attracted
    float m_AttractionRadius = 2.0f;

    /// @brief  the strength with which items will be attracted
    float m_AttractionStrength = 10.0f;


    /// @brief  the Transform attached to this ItemCollector
    ComponentReference< Transform > m_Transform;

    /// @brief  the Inventory attached to this ItemCollector
    ComponentReference< Inventory > m_Inventory;


//-----------------------------------------------------------------------------
private: // helper methods
//-----------------------------------------------------------------------------


    /// @brief  collects an item
    /// @param  item    the item to collect
    void collectItem( ItemComponent* item );

    /// @brief  attracts an item
    /// @param  item    the item to attract
    /// @param  offfset the offset from the item to the collector
    void attractItem( ItemComponent* item, glm::vec2 const& offset );


//-----------------------------------------------------------------------------
private: // inspection
//-----------------------------------------------------------------------------

    
    /// @brief  displays this ItemCollector in the Inspector
    virtual void Inspector() override;


//-----------------------------------------------------------------------------
private: // reading
//-----------------------------------------------------------------------------


    /// @brief  reads the radius at which items will be instantly collected
    /// @param  data    the json data to read from
    void readCollectionRadius( nlohmann::ordered_json const& data );

    /// @brief  reads the radius at which items will be attracted
    /// @param  data    the json data to read from
    void readAttractionRadius( nlohmann::ordered_json const& data );

    /// @brief  reads the strength with which items will be attracted
    /// @param  data    the json data to read from
    void readAttractionStrength( nlohmann::ordered_json const& data );


//-----------------------------------------------------------------------------
public: // reading / writing
//-----------------------------------------------------------------------------


    /// @brief  gets the map of read methods for this Component
    /// @return the map of read methods for this Component
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override;


    /// @brief  Write all ItemCollector data to a JSON file.
    /// @return The JSON file containing the ItemCollector data.
    virtual nlohmann::ordered_json Write() const override;


//-----------------------------------------------------------------------------
public: // constructor / Destructor
//-----------------------------------------------------------------------------


    /// @brief  constructor
    ItemCollector();


//-----------------------------------------------------------------------------
public: // copying
//-----------------------------------------------------------------------------


    /// @brief  clones this ItemCollector
    /// @return the newly created clone of this ItemCollector
    virtual ItemCollector* Clone() const override;


//-----------------------------------------------------------------------------
private: // copying
//-----------------------------------------------------------------------------


    /// @brief  copy-constructor for the ItemCollector
    /// @param  other   the other ItemCollector to copy
    ItemCollector( const ItemCollector& other );

    // diable = operator
    void operator =( ItemCollector const& ) = delete;

//-----------------------------------------------------------------------------
};
