/// @file       EntityReference.cpp
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      a reference to an Entity in the scene
/// @version    0.1
/// @date       2024-02-11
/// 
/// @copyright  Copyright (c) 2024 Digipen Institute of Technology

#include "pch.h" 







#include "DebugSystem.h"

#include "EntitySystem.h"



//-----------------------------------------------------------------------------
// public: constructor
//-----------------------------------------------------------------------------


    /// @brief  constructor
    /// @param  componentReferences references to the Components attached to the Entity to keep track of
    EntityReference::EntityReference( std::initializer_list< ComponentReferenceBase* > componentReferences ) :
        m_ComponentReferences( componentReferences )
    {}


    /// @brief  copy constructor
    /// @param  other               the EntityReference to copy
    /// @param  componentReferences references to the Components attached to the Entity to keep track of
    EntityReference::EntityReference( EntityReference const& other, std::initializer_list< ComponentReferenceBase* > componentReferences ) :
        m_EntityName( other.m_EntityName ),
        m_ComponentReferences( componentReferences )
    {}


//-----------------------------------------------------------------------------
// public: methods
//-----------------------------------------------------------------------------


    /// @brief  initializes this EntityReference to point to an Entity in the scene
    void EntityReference::Init()
    {
        m_Entity = Entities()->GetEntity( m_EntityName );

        // log an error if no entity was found, and a reference is required
        if ( m_Entity == nullptr )
        {
            for ( ComponentReferenceBase* reference : m_ComponentReferences )
            {
                if ( reference->GetIsRequired() )
                {
                    Debug() << "WARNING: Could not find Entity \"" << m_EntityName
                        << "\" (EntityReference owned by Entity \"" << m_OwnerName << "\")\n" << std::endl;
                    return;
                }
            }
        }

        m_Entity->AddEntityReference( this );

        initComponentReferencesAndCallbacks();
    }


    /// @brief  exits this EntityReference, stopping keeping track of any Entities
    void EntityReference::Exit()
    {
        if ( m_Entity == nullptr )
        {
            return;
        }

        exitComponentReferencesAndCallbacks();

        m_Entity->RemoveEntityReference( this );
        m_Entity = nullptr;
    }


    /// @brief  sets the callback to call when this EntityReference connects to an Entity
    /// @param  callback    the callback to call
    void EntityReference::SetOnConnectCallback( std::function< void () > callback )
    {
        m_OnConnectCallback = callback;
    }

    /// @brief  sets the callback to call when this EntityReference disconnects from an Entity
    /// @param  callback    the callback to call
    void EntityReference::SetOnDisconnectCallback( std::function< void () > callback )
    {
        m_OnDisconnectCallback = callback;
    }


//-----------------------------------------------------------------------------
// public: accessors
//-----------------------------------------------------------------------------


    /// @brief  gets the name of the Entity
    /// @return the name of the Entity
    std::string const& EntityReference::GetName() const
    {
        return m_EntityName;
    }


    /// @brief  sets this EntityReference's Owner Name
    /// @param  ownerName   the name of the owner of this EntityReference
    void EntityReference::SetOwnerName( std::string const& ownerName )
    {
        m_OwnerName = ownerName;
    }


    /// @brief  dereference operator
    /// @return the Component this EntityReference refers to
    Entity& EntityReference::operator *() const
    {
        return *m_Entity;
    }

    /// @brief  member dereference operator
    /// @return the Component this EntityReference refers to
    Entity* EntityReference::operator ->() const
    {
        return m_Entity;
    }

    /// @brief  implicit cast operator
    /// @return this EntityReference's internal pointer
    EntityReference::operator Entity*() const
    {
        return m_Entity;
    }


    /// @brief  assignment operator (also initializes)
    /// @param  entity  the Entity to assign to this EntityReference
    void EntityReference::operator =( Entity* entity )
    {
        Exit();

        m_Entity = entity;

        if ( entity == nullptr )
        {
            m_EntityName.clear();
            return;
        }

        m_EntityName = entity->GetName();

        m_Entity->AddEntityReference( this );

        initComponentReferencesAndCallbacks();
    }


//-----------------------------------------------------------------------------
// public: engine methods
//-----------------------------------------------------------------------------


    /// @brief  sets this EntityReference to nullptr
    /// @brief  SHOULD ONLY BE CALLED BY Entity::Exit()
    void EntityReference::Clear()
    {
        m_Entity = nullptr;
    }


//-----------------------------------------------------------------------------
// private: methods
//-----------------------------------------------------------------------------


    /// @brief  inits all attached ComponentReferences
    void EntityReference::initComponentReferencesAndCallbacks()
    {
        for ( ComponentReferenceBase* componentReference : m_ComponentReferences )
        {
            componentReference->Init( m_Entity );
        }

        if ( m_OnConnectCallback )
        {
            m_OnConnectCallback();
        }
    }


    /// @brief  exits all attached ComponentReferences
    void EntityReference::exitComponentReferencesAndCallbacks()
    {
        if ( m_OnDisconnectCallback )
        {
            m_OnDisconnectCallback();
        }

        for ( ComponentReferenceBase* componentReference : m_ComponentReferences )
        {
            componentReference->Exit();
        }
    }


//-----------------------------------------------------------------------------
// public: inspection
//-----------------------------------------------------------------------------


    /// @brief  inspects this EntityReference
    /// @param  label   what to label this EntityReference inspector as
    /// @return whether this EntityReference was modified
    bool EntityReference::Inspect( char const* label )
    {
        if ( ImGui::BeginCombo( label, m_EntityName.c_str() ) )
        {
            if ( ImGui::Selectable( "[ none ]", m_EntityName.empty() ) )
            {
                *this = nullptr;

                ImGui::EndCombo();
                return true;
            }

            for ( Entity* entity : Entities()->GetEntities() )
            {
                if (
                    ImGui::Selectable(
                        entity->GetName().c_str(),
                        entity == m_Entity || ( m_Entity == nullptr && entity->GetName() == m_EntityName )
                    )
                )
                {
                    *this = entity;

                    ImGui::EndCombo();
                    return true;
                }
            }
            ImGui::EndCombo();
        }

        return false;
    }

    
//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------


    /// @brief  reads the name of the Entity this EntityReference points to
    /// @param  data    the JSON data to read from
    void EntityReference::readEntityName( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_EntityName, data );
    }


//-----------------------------------------------------------------------------
// public: reading / writing
//-----------------------------------------------------------------------------


    /// @brief  gets the map of read methods for Entities
    /// @return the map of read methods for Entities
    ReadMethodMap< ISerializable > const& EntityReference::GetReadMethods() const
    {
        static ReadMethodMap< EntityReference > const readMethods = {
            { "EntityName", &EntityReference::readEntityName }
        };

        return (ReadMethodMap< ISerializable > const&)readMethods;
    }


    /// @brief  Write all Entity data to a JSON file.
    /// @return The JSON file containing the Entity data.
    nlohmann::ordered_json EntityReference::Write() const
    {
        nlohmann::ordered_json json;

        json[ "EntityName" ] = Stream::Write( m_EntityName );

        return json;
    }


//-----------------------------------------------------------------------------
