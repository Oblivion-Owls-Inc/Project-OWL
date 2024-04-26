/// @file       ItemCollector.cpp
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      Component that attracts and collects item entities in the world
/// @version    0.1
/// @date       2023-10-20
/// 
/// @copyright  Copyright (c) 2024 Digipen Institute of Technology


#include "pch.h" // precompiled header has to be included first
#include "ItemCollector.h"
#include "BehaviorSystem.h"

#include "ItemComponent.h"

#include "AudioPlayer.h"


//-----------------------------------------------------------------------------
// public: types
//-----------------------------------------------------------------------------

    
//-----------------------------------------------------------------------------
// private: types
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// public: accessors
//-----------------------------------------------------------------------------


    /// @brief  gets the Transform attached to this ItemCollector
    /// @return the Transform attached to this ItemCollector
    Transform* ItemCollector::GetTransform() const
    {
        return m_Transform;
    }


//-----------------------------------------------------------------------------
// public:  methods
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// private: virtual override methods
//-----------------------------------------------------------------------------


    /// @brief  called once when entering the scene
    void ItemCollector::OnInit()
    {
        Behaviors< Behavior >()->AddComponent( this );

        m_Transform.Init( GetEntity() );
        m_Inventory.Init( GetEntity() );
    }

    /// @brief  called once when exiting the scene
    void ItemCollector::OnExit()
    {
        Behaviors< Behavior >()->RemoveComponent( this );

        m_Transform.Exit();
        m_Inventory.Exit();
    }


    /// @brief  gets called once per simulation frame
    void ItemCollector::OnFixedUpdate()
    {
        for ( ItemComponent* item : Components< ItemComponent >()->GetComponents() )
        {
            glm::vec2 itemPos = item->GetTransform()->GetTranslation();
            glm::vec2 collectorPos = m_Transform->GetTranslation();
            glm::vec2 offset = collectorPos - itemPos;
            float distanceSquared = glm::dot( offset, offset );

            if ( distanceSquared < m_CollectionRadius * m_CollectionRadius )
            {
                collectItem( item );
            }
            else if ( distanceSquared < m_AttractionRadius * m_AttractionRadius )
            {
                attractItem( item, offset );
            }

        }
    }



//-----------------------------------------------------------------------------
// private: helper methods
//-----------------------------------------------------------------------------


    /// @brief  collects an item
    /// @param  item    the item to collect
    void ItemCollector::collectItem( ItemComponent* item )
    {
        m_Inventory->AddItemStack( item->GetItemStack() );

        AudioPlayer* audioPlayer = item->GetAudioPlayer();

        if (audioPlayer)
        {
            audioPlayer->Play();
        }

        item->GetEntity()->Destroy();
    }

    /// @brief  attracts an item
    /// @param  item    the item to attract
    /// @param  offfset the offset from the item to the collector
    void ItemCollector::attractItem( ItemComponent* item, glm::vec2 const& offset )
    {
        glm::vec2 force = glm::normalize( offset ) * m_AttractionStrength;
        item->GetRigidBody()->ApplyForce( force );
    }


//-----------------------------------------------------------------------------
// private: inspection
//-----------------------------------------------------------------------------

    
    /// @brief  displays this ItemCollector in the Inspector
    void ItemCollector::Inspector()
    {
        ImGui::DragFloat( "collection radius", &m_CollectionRadius, 0.05f, 0.0f, INFINITY );

        ImGui::DragFloat( "attraction radius", &m_AttractionRadius, 0.05f, 0.0f, INFINITY );

        ImGui::DragFloat( "attraction strength", &m_AttractionStrength, 0.05f, 0.0f, INFINITY );
    }


//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------


    /// @brief  reads the radius at which items will be instantly collected
    /// @param  data    the json data to read from
    void ItemCollector::readCollectionRadius( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_CollectionRadius, data );
    }

    /// @brief  reads the radius at which items will be attracted
    /// @param  data    the json data to read from
    void ItemCollector::readAttractionRadius( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_AttractionRadius, data );
    }

    /// @brief  reads the strength with which items will be attracted
    /// @param  data    the json data to read from
    void ItemCollector::readAttractionStrength( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_AttractionStrength, data );
    }


//-----------------------------------------------------------------------------
// public: reading / writing
//-----------------------------------------------------------------------------


    /// @brief  gets the map of read methods for this Component
    /// @return the map of read methods for this Component
    ReadMethodMap< ISerializable > const& ItemCollector::GetReadMethods() const
    {
        static ReadMethodMap< ItemCollector > const readMethods = {
            { "CollectionRadius"  , &ItemCollector::readCollectionRadius   },
            { "AttractionRadius"  , &ItemCollector::readAttractionRadius   },
            { "AttractionStrength", &ItemCollector::readAttractionStrength }
        };

        return (ReadMethodMap< ISerializable > const&)readMethods;
    }


    /// @brief  Write all ItemCollector data to a JSON file.
    /// @return The JSON file containing the ItemCollector data.
    nlohmann::ordered_json ItemCollector::Write() const
    {
        nlohmann::ordered_json json;

        json[ "CollectionRadius"   ] = Stream::Write( m_CollectionRadius   );
        json[ "AttractionRadius"   ] = Stream::Write( m_AttractionRadius   );
        json[ "AttractionStrength" ] = Stream::Write( m_AttractionStrength );

        return json;
    }


//-----------------------------------------------------------------------------
// public: constructor / Destructor
//-----------------------------------------------------------------------------


    /// @brief  constructor
    ItemCollector::ItemCollector() :
        Behavior( typeid( ItemCollector ) )
    {}


//-----------------------------------------------------------------------------
// public: copying
//-----------------------------------------------------------------------------


    /// @brief  clones this ItemCollector
    /// @return the newly created clone of this ItemCollector
    ItemCollector* ItemCollector::Clone() const
    {
        return new ItemCollector( *this );
    }


//-----------------------------------------------------------------------------
// private: copying
//-----------------------------------------------------------------------------


    /// @brief  copy-constructor for the ItemCollector
    /// @param  other   the other ItemCollector to copy
    ItemCollector::ItemCollector( const ItemCollector& other ) :
        Behavior( other ),
        m_CollectionRadius( other.m_CollectionRadius ),
        m_AttractionRadius( other.m_AttractionRadius ),
        m_AttractionStrength( other.m_AttractionStrength )
    {}


//-----------------------------------------------------------------------------
