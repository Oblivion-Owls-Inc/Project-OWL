/// @file       ItemComponent.cpp
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      Item in the world that can be picked up
/// @version    0.1
/// @date       2023-10-20
/// 
/// @copyright  Copyright (c) 2023 Digipen Institute of Technology


#include "ItemComponent.h"
#include "ComponentSystem.h"

#include "Entity.h"
#include "Transform.h"
#include "Sprite.h"


//-----------------------------------------------------------------------------
// public: types
//-----------------------------------------------------------------------------

    
//-----------------------------------------------------------------------------
// private: types
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// public: accessors
//-----------------------------------------------------------------------------



    /// @brief  gets the ItemStack that this ItemComponent holds
    /// @return the ItemStack that this ItemComponent holds
    ItemStack const& ItemComponent::GetItemStack() const
    {
        return m_ItemStack;
    }

    /// @brief  sets the ItemStack that this ItemComponent holds
    /// @param  itemStack  the ID of the item this ItemComponent will hold
    void ItemComponent::SetItemStack( ItemStack const& itemStack )
    {
        m_ItemStack = itemStack;

        m_Sprite->SetFrameIndex( itemStack.M_ItemId );
    }


    /// @brief  gets the Transform attached to this ItemComponent
    /// @return the Transform attached to this ItemComponent
    Transform* ItemComponent::GetTransform() const
    {
        return m_Transform;
    }

    /// @brief  gets the Sprite attached to this ItemComponent
    /// @return the Sprite attached to this ItemComponent
    Sprite* ItemComponent::GetSprite() const
    {
        return m_Sprite;
    }


//-----------------------------------------------------------------------------
// public:  methods
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// private: virtual override methods
//-----------------------------------------------------------------------------


    /// @brief  called once when entering the scene
    void ItemComponent::OnInit()
    {
        Components< ItemComponent >()->AddComponent( this );

        m_Transform = GetEntity()->GetComponent< Transform >();
        m_Sprite = GetEntity()->GetComponent< Sprite >();
    }

    /// @brief  called once when exiting the scene
    void ItemComponent::OnExit()
    {
        Components< ItemComponent >()->RemoveComponent( this );
    }


//-----------------------------------------------------------------------------
// private: helper methods
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// private: inspection
//-----------------------------------------------------------------------------


    /// @brief  displays this ItemComponent in the Inspector
    void ItemComponent::Inspector()
    {
        m_ItemStack.Inspect();
    }


//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------


    /// @brief  reads the ItemStack that this ItemComponent holds
    /// @param  data    the json data to read from
    void ItemComponent::readItemStack( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_ItemStack, data );
    }


//-----------------------------------------------------------------------------
// public: reading / writing
//-----------------------------------------------------------------------------


    /// @brief  gets the map of read methods for this Component
    /// @return the map of read methods for this Component
    ReadMethodMap< ISerializable > const& ItemComponent::GetReadMethods() const
    {
        static ReadMethodMap< ItemComponent > const readMethods = {
            { "ItemStack", &ItemComponent::readItemStack }
        };

        return (ReadMethodMap< ISerializable > const&)readMethods;
    }


    /// @brief  Write all ItemComponent data to a JSON file.
    /// @return The JSON file containing the ItemComponent data.
    nlohmann::ordered_json ItemComponent::Write() const
    {
        nlohmann::ordered_json json;

        json[ "ItemStack" ] = Stream::Write( m_ItemStack );

        return json;
    }


//-----------------------------------------------------------------------------
// public: constructor / Destructor
//-----------------------------------------------------------------------------


    /// @brief  constructor
    ItemComponent::ItemComponent() :
        Component( typeid( ItemComponent ) )
    {}


//-----------------------------------------------------------------------------
// public: copying
//-----------------------------------------------------------------------------


    /// @brief  clones this ItemComponent
    /// @return the newly created clone of this ItemComponent
    ItemComponent* ItemComponent::Clone() const
    {
        return new ItemComponent( *this );
    }


//-----------------------------------------------------------------------------
// private: copying
//-----------------------------------------------------------------------------


    /// @brief  copy-constructor for the ItemComponent
    /// @param  other   the other ItemComponent to copy
    ItemComponent::ItemComponent( const ItemComponent& other ) :
        Component( other ),
        m_ItemStack( other.m_ItemStack )
    {}


//-----------------------------------------------------------------------------
