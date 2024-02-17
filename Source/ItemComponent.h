/// @file       ItemComponent.h
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      Item in the world that can be picked up
/// @version    0.1
/// @date       2023-10-20
/// 
/// @copyright  Copyright (c) 2023 Digipen Institute of Technology

#pragma once

#include "Component.h"

#include "ItemStack.h"

#include "ComponentReference.h"
#include "Transform.h"
#include "Sprite.h"
#include "RigidBody.h"


/// @brief      Item in the world that can be picked up
class ItemComponent : public Component
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



    /// @brief  gets the ItemStack that this ItemComponent holds
    /// @return the ItemStack that this ItemComponent holds
    ItemStack const& GetItemStack() const;

    /// @brief  sets the ItemStack that this ItemComponent holds
    /// @param  itemStack   the ID of the item this ItemComponent will hold
    void SetItemStack( ItemStack const& itemStack );


    /// @brief  gets the Transform attached to this ItemComponent
    /// @return the Transform attached to this ItemComponent
    Transform* GetTransform() const;

    /// @brief  gets the RigidBody attached to this ItemComponent
    /// @return the RigidBody attached to this ItemComponent
    RigidBody* GetRigidBody() const;

    /// @brief  gets the Sprite attached to this ItemComponent
    /// @return the Sprite attached to this ItemComponent
    Sprite* GetSprite() const;


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


//-----------------------------------------------------------------------------
private: // members
//-----------------------------------------------------------------------------


    /// @brief  the ItemStack that this ItemComponent holds
    ItemStack m_ItemStack = 0;


    /// @brief  the Transform attached to this ItemComponent
    ComponentReference< Transform > m_Transform;

    /// @brief  the Sprite attached to this ItemComponent
    ComponentReference< Sprite > m_Sprite;

    /// @brief  the RigidBody attached to this ItemComponent
    ComponentReference< RigidBody > m_RigidBody;


//-----------------------------------------------------------------------------
private: // helper methods
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
private: // inspection
//-----------------------------------------------------------------------------

    
    /// @brief  displays this ItemComponent in the Inspector
    virtual void Inspector() override;


//-----------------------------------------------------------------------------
private: // reading
//-----------------------------------------------------------------------------


    /// @brief  reads the ItemStack that this ItemComponent holds
    /// @param  data    the json data to read from
    void readItemStack( nlohmann::ordered_json const& data );


//-----------------------------------------------------------------------------
public: // reading / writing
//-----------------------------------------------------------------------------


    /// @brief  gets the map of read methods for this Component
    /// @return the map of read methods for this Component
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override;


    /// @brief  Write all ItemComponent data to a JSON file.
    /// @return The JSON file containing the ItemComponent data.
    virtual nlohmann::ordered_json Write() const override;


//-----------------------------------------------------------------------------
public: // constructor / Destructor
//-----------------------------------------------------------------------------


    /// @brief  constructor
    ItemComponent();


//-----------------------------------------------------------------------------
public: // copying
//-----------------------------------------------------------------------------


    /// @brief  clones this ItemComponent
    /// @return the newly created clone of this ItemComponent
    virtual ItemComponent* Clone() const override;


//-----------------------------------------------------------------------------
private: // copying
//-----------------------------------------------------------------------------


    /// @brief  copy-constructor for the ItemComponent
    /// @param  other   the other ItemComponent to copy
    ItemComponent( const ItemComponent& other );

    // disable = operator
    void operator =( ItemComponent const& ) = delete;


//-----------------------------------------------------------------------------
};
