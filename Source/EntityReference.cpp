/// @file       EntityReference.cpp
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      a reference to an Entity in the scene
/// @version    0.1
/// @date       2024-02-11
/// 
/// @copyright  Copyright (c) 2024 Digipen Institute of Technology

#include "EntityReference.h"

#include "Entity.h"
#include "Component.h"

#include "ComponentReference.h"

#include "EntitySystem.h"

#include <imgui.h>


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

        if ( m_Entity == nullptr )
        {
            return;
        }

        m_Entity->AddEntityReference( this );

        initComponentReferences();
    }


    /// @brief  exits this EntityReference, stopping keeping track of any Entities
    void EntityReference::Exit()
    {
        if ( m_Entity == nullptr )
        {
            return;
        }

        exitComponentReferences();

        m_Entity->RemoveEntityReference( this );
        m_Entity = nullptr;
    }


//-----------------------------------------------------------------------------
// public: accessors
//-----------------------------------------------------------------------------


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


    /// @brief  assignment operator
    /// @param  entity  the Entity to assign to this EntityReference
    void EntityReference::operator =( Entity* entity )
    {
        exitComponentReferences();

        m_Entity = entity;
        m_EntityName = entity->GetName();

        initComponentReferences();
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
    void EntityReference::initComponentReferences()
    {
        for ( ComponentReferenceBase* componentReference : m_ComponentReferences )
        {
            componentReference->Init( m_Entity );
        }
    }


    /// @brief  exits all attached ComponentReferences
    void EntityReference::exitComponentReferences()
    {
        for ( ComponentReferenceBase* componentReference : m_ComponentReferences )
        {
            componentReference->Exit( m_Entity );
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
            for ( Entity* entity : Entities()->GetEntities() )
            {
                if ( ImGui::Selectable( entity->GetName().c_str(), entity == m_Entity ) )
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
