/// @file       ResourcesUiManager.cpp
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      Component that manages the UI for displaying what resources a player currently has
/// @version    0.1
/// @date       2024-02-21
/// 
/// @copyright  Copyright (c) 2024 Digipen Institute of Technology

#pragma once

#include "pch.h" // precompiled header has to be included first
#include "ResourcesUiManager.h"

#include "ItemStack.h"

#include "ComponentReference.t.h"
#include "Inventory.h"
#include "UiElement.h"
#include "Sprite.h"
#include "Text.h"
#include "ResourceCounterUi.h"
#include "TilemapSprite.h"



//-----------------------------------------------------------------------------
// public: constructor / Destructor
//-----------------------------------------------------------------------------


    /// @brief  default constructor
    ResourcesUiManager::ResourcesUiManager() :
        Component( typeid( ResourcesUiManager ) )
    {}



//-----------------------------------------------------------------------------
// public: methods
//-----------------------------------------------------------------------------


    /// @brief  sets the opacity of the resources UI
    /// @param  opacity the opacity the resources UI should be
    void ResourcesUiManager::SetOpacity( float opacity )
    {
        m_Opacity = opacity;

        if ( m_Sprite != nullptr )
        {
            m_Sprite->SetOpacity( opacity );
        }

        for ( auto& [ itemId, resourceCounter ] : m_ResourceCounters )
        {
            resourceCounter->SetOpacity( opacity );
        }
    }


//-----------------------------------------------------------------------------
// public: accessors
//-----------------------------------------------------------------------------


    /// @brief  gets the UiElement attached to this ResourcesUiManager
    /// @return the UiElement attached to this ResourcesUiManager
    UiElement* ResourcesUiManager::GetUiElement()
    {
        return m_UiElement;
    }


//-----------------------------------------------------------------------------
// public: virtual override methods
//-----------------------------------------------------------------------------


    /// @brief  called once when entering the scene
    void ResourcesUiManager::OnInit()
    {

        getItemCountersFromChildren();

        m_Inventory.SetOnConnectCallback(
            [ this ]()
            {
                setupItemCounters();

                m_Inventory->AddOnAddItemStackCallback(
                    GetId(),
                    [ this ]( ItemStack const& itemStack )
                    {
                        onItemStackCallback( itemStack );
                    }
                );
                m_Inventory->AddOnRemoveItemStackCallback(
                    GetId(),
                    [ this ]( ItemStack const& itemStack )
                    {
                        onItemStackCallback( itemStack );
                    }
                );
            }
        );
        m_Inventory.SetOnDisconnectCallback(
            [ this ]()
            {
                m_Inventory->RemoveOnAddItemStackCallback   ( GetId() );
                m_Inventory->RemoveOnRemoveItemStackCallback( GetId() );
            }
        );

        m_ResourceCounterPrefab.Init();

        m_UiElement.Init( GetEntity() );
        m_Sprite   .Init( GetEntity() );

        m_InventoryEntity.Init();

    }

    /// @brief  called once when exiting the scene
    void ResourcesUiManager::OnExit()
    {
        m_InventoryEntity.Exit();

        m_UiElement.Exit();
        m_Sprite   .Exit();
    }


    /// @brief  called before a child is removed from this Entity
    /// @param  child   the child that will be removed
    void ResourcesUiManager::OnRemoveChild( Entity* child )
    {
        for ( auto& [ itemId, resourceCounter ] : m_ResourceCounters )
        {
            if ( resourceCounter->GetEntity() == child )
            {
                m_ResourceCounters.erase( itemId );
                updateTransforms();
                return;
            }
        }
    }


//-----------------------------------------------------------------------------
// private: members
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// private: methods
//-----------------------------------------------------------------------------


    /// @brief  gets the Item Counters from the child Entities
    void ResourcesUiManager::getItemCountersFromChildren()
    {
        for ( Entity* child : GetEntity()->GetChildren() )
        {
            ResourceCounterUi* resourceCounter = child->GetComponent< ResourceCounterUi >();
            if ( resourceCounter == nullptr )
            {
                continue;
            }

            m_ResourceCounters.emplace( resourceCounter->GetResourceId(), resourceCounter );
        }
    }


    /// @brief  sets up the item counters using the contents of the Inventory
    void ResourcesUiManager::setupItemCounters()
    {
        if ( m_ResourceCounters.empty() == false )
        {
            Debug() << "Error: m_ResourceCounters was not empty (" << GetName() << ")" << std::endl;
            return;
        }

        for ( ItemStack const& itemStack : m_Inventory->GetItems() )
        {
            updateResourceCounter( itemStack );
        }
    }


    /// @brief  callback which gets called after an ItemStack is added/removed from the Inventory
    /// @param  itemStack   the ItemStack that was added/removed
    void ResourcesUiManager::onItemStackCallback( ItemStack const& itemStack )
    {
        int count = m_Inventory->GetItemCount( itemStack.M_ItemId );

        updateResourceCounter( ItemStack( itemStack.M_ItemId, count ) );
    }


    /// @brief  updates the resource counter of the item of the itemstack with the count of the itemstack
    /// @param  itemStack   the itemStack representing the resource counter to change, and what to change it to
    void ResourcesUiManager::updateResourceCounter( ItemStack const& itemStack )
    {
        auto it = m_ResourceCounters.find( itemStack.M_ItemId );

        // if it doesn't exist yet, add it
        if ( it == m_ResourceCounters.end() )
        {
            addResourceCounter( itemStack );
            return;
        }

        // otherwise, set its resources
        ResourceCounterUi* resourceCounter = it->second;
        resourceCounter->SetResources( itemStack );
    }


    /// @brief  adds and initializes a ResourceCounter
    /// @param  itemStack   the ItemStack to initialize the ResourceCounter with
    void ResourcesUiManager::addResourceCounter( ItemStack const& itemStack )
    {
        if ( m_ResourceCounterPrefab == nullptr )
        {
            return;
        }

        // create the entity from a prefab
        Entity* resourceCounterEntity = m_ResourceCounterPrefab->Clone();
        ResourceCounterUi* resourceCounter = resourceCounterEntity->GetComponent< ResourceCounterUi >();

        // validate that the ResourceCounterUi exists
        if ( resourceCounter == nullptr )
        {
            Debug() << "WARNING: resourceCounterPrefab does not have a ResourceCounterUi Component" << std::endl;
            delete resourceCounterEntity;
            return;
        }

        // set up the ResourceCounterUi and add it to the scene
        resourceCounter->SetOpacity( m_Opacity );
        resourceCounter->SetResources( itemStack );
        resourceCounterEntity->SetParent( GetEntity() );
        resourceCounterEntity->AddToScene();

        // add it to the UI
        m_ResourceCounters.emplace( itemStack.M_ItemId, resourceCounter );

        if ( m_HideWhenEmpty && m_Sprite != nullptr )
        {
            m_Sprite->SetOpacity( 1.0f );
        }

        updateTransforms();
    }


    /// @brief  updates the Transforms off all UiElements in the resources ui
    void ResourcesUiManager::updateTransforms()
    {
        if ( m_UiElement == nullptr )
        {
            return;
        }

        glm::vec2 frameSize = m_UiElement->GetFrameSize();

        if ( m_ResourceCounters.size() == 0 )
        {
            frameSize.y = 2.0f * m_Padding;
            m_UiElement->SetFrameSize( frameSize );
            return;
        }

        frameSize.y = 2.0f * m_Padding + (m_ResourceCounters.size() - 1) * std::abs( m_Spacing );
        m_UiElement->SetFrameSize( frameSize );

        float normalizedPadding = 2.0f * m_Padding / frameSize.y;
        int sign = ((m_Spacing > 0.0f) - (m_Spacing < 0.0f));
        float pos = -sign * (1.0f - normalizedPadding);
        float offset = sign * ( 2.0f - 2.0f * normalizedPadding ) / (m_ResourceCounters.size() - 1.0f);

        // evenly space out the ResourceCounters
        for ( auto& [ itemId, resourceCounter ] : m_ResourceCounters )
        {
            if ( resourceCounter == nullptr || resourceCounter->GetUiElement() == nullptr )
            {
                continue;
            }

            glm::vec2 anchor = resourceCounter->GetUiElement()->GetAnchor();
            anchor.y = pos;
            resourceCounter->GetUiElement()->SetAnchor( anchor );

            pos += offset;
        }
    }


//-----------------------------------------------------------------------------
// public: inspection
//-----------------------------------------------------------------------------


    /// @brief  shows the inspector for ResourcesUiManager
    void ResourcesUiManager::Inspector()
    {
        
        ImGui::DragFloat( "padding", &m_Padding, 0.01f, 0.0f, 1.0f );

        ImGui::DragFloat( "spacing", &m_Spacing, 0.05f );

        ImGui::DragFloat( "opacity", &m_Opacity, 0.01f, 0.0f, 1.0f );

        if ( ImGui::Checkbox( "hide when empty", &m_HideWhenEmpty ) )
        {
            if ( m_Sprite != nullptr )
            {
                m_Sprite->SetOpacity( m_HideWhenEmpty && m_ResourceCounters.empty() ? 0.0f : 1.0f );
            }
        }

        ImGui::NewLine();

        m_InventoryEntity.Inspect( "Inventory entity" );

        m_ResourceCounterPrefab.Inspect( "Resource Counter Prefab" );

    }


//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------


    /// @brief  reads whether to hide the box sprite when empty
    /// @param  data    the JSON data to read from
    void ResourcesUiManager::readHideWhenEmpty( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_HideWhenEmpty, data );
    }

    /// @brief  reads the amount of space between the top and bottom of the box and the resource counters
    /// @param  data    the JSON data to read from
    void ResourcesUiManager::readPadding( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_Padding, data );
    }

    /// @brief  reads the amount of space between the top and bottom of the box and the resource counters
    /// @param  data    the JSON data to read from
    void ResourcesUiManager::readSpacing( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_Spacing, data );
    }

    /// @brief  reads the opacity of the resources Ui
    /// @param  data    the JSON data to read from
    void ResourcesUiManager::readOpacity( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_Opacity, data );
    }
    
    /// @brief  reads the reference to the Entity that has the Inventory to display
    /// @param  data    the JSON data to read from
    void ResourcesUiManager::readInventoryEntity( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_InventoryEntity, data );
    }

    /// @brief  reads the prefab to spawn of a counter of a single resource type
    /// @param  data    the JSON data to read from
    void ResourcesUiManager::readResourceCounterPrefab( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_ResourceCounterPrefab, data );
    }


//-----------------------------------------------------------------------------
// public: reading / writing
//-----------------------------------------------------------------------------


    /// @brief  gets the map of read methods for this Component
    /// @return the map of read methods for this Component
    ReadMethodMap< ISerializable > const& ResourcesUiManager::GetReadMethods() const
    {
        static ReadMethodMap< ResourcesUiManager > const readMethods = {
            { "HideWhenEmpty"        , &ResourcesUiManager::readHideWhenEmpty         },
            { "Padding"              , &ResourcesUiManager::readPadding               },
            { "Spacing"              , &ResourcesUiManager::readSpacing               },
            { "Opacity"              , &ResourcesUiManager::readOpacity               },
            { "InventoryEntity"      , &ResourcesUiManager::readInventoryEntity       },
            { "ResourceCounterPrefab", &ResourcesUiManager::readResourceCounterPrefab } 
        };

        return (ReadMethodMap< ISerializable > const&)readMethods;
    }


    /// @brief  Writes all HomeBase data to a JSON file.
    /// @return The JSON file containing the data.
    nlohmann::ordered_json ResourcesUiManager::Write() const
    {
        nlohmann::ordered_json json;

        json[ "HideWhenEmpty"         ] = Stream::Write( m_HideWhenEmpty         );
        json[ "Padding"               ] = Stream::Write( m_Padding               );
        json[ "Spacing"               ] = Stream::Write( m_Spacing               );
        json[ "Opacity"               ] = Stream::Write( m_Opacity               );
        json[ "InventoryEntity"       ] = Stream::Write( m_InventoryEntity       );            
        json[ "ResourceCounterPrefab" ] = Stream::Write( m_ResourceCounterPrefab );

        return json;
    }

    
//-----------------------------------------------------------------------------
// public: copying
//-----------------------------------------------------------------------------


    /// @brief  clones this ResourcesUiManager
    /// @return the newly created clone of this ResourcesUiManager
    ResourcesUiManager* ResourcesUiManager::Clone() const
    {
        return new ResourcesUiManager( *this );
    }


//-----------------------------------------------------------------------------
// private: copying
//-----------------------------------------------------------------------------


    /// @brief  copy-constructor for the ResourcesUiManager
    /// @param  other   the other ResourcesUiManager to copy
    ResourcesUiManager::ResourcesUiManager( ResourcesUiManager const& other ) :
        Component( other ),
        m_HideWhenEmpty        ( other.m_HideWhenEmpty         ),
        m_Padding              ( other.m_Padding               ),
        m_Spacing              ( other.m_Spacing               ),
        m_Opacity              ( other.m_Opacity               ),
        m_ResourceCounterPrefab( other.m_ResourceCounterPrefab ),

        m_InventoryEntity( other.m_InventoryEntity, { &m_Inventory } )
    {}


//-----------------------------------------------------------------------------
