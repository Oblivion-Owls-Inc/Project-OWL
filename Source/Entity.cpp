///--------------------------------------------------------------------------//
/// @file   Entity.cpp
/// @brief  Function definitions for the Entity class
/// 
/// @author Aidan Straker (aidan.straker)
/// @date   September 2023
///
/// @copyright © 2024 DigiPen (USA) Corporation.
///--------------------------------------------------------------------------//

#include "pch.h" // precompiled header has to be included first
#include "Entity.h"

#include "EntitySystem.h"


#include "Component.h"		  // Type
#include "ComponentFactory.h" // Create.
#include "InputSystem.h"
#include "AssetLibrarySystem.h"
#include "DebugSystem.h"
#include "basics.h"

#include "EntityReference.h"
#include "ComponentReference.h"
#include "PathfindSystem.h"


///-----------------------------------------------------------------------------
/// Static Variables
///-----------------------------------------------------------------------------

///------------------------------------------------------------------------------
/// Public: constructor / destructor
///------------------------------------------------------------------------------


    /// @brief  default constrcutor
    Entity::Entity() :
        m_Id( GetUniqueId() )
    {}

    /// @brief destructor
    Entity::~Entity()
    {
        for ( auto& [ type, component ] : m_Components )
        {
            delete component;
        }

        for (Entity* child : m_Children)
        {
            delete child;
        }
    }


//------------------------------------------------------------------------------
// Public: methods
//------------------------------------------------------------------------------


    /// @brief  flags this Entity for destruction
    void Entity::Destroy()
    {
        m_IsDestroyed = true;

        for ( Entity* child : m_Children )
        {
            child->Destroy();
        }
    }


    /// @brief  attaches a component to this Entity
    /// @param  component   the component to attach to this Entity
    void Entity::AddComponent( Component* component )
    {
        // You tried to add a component that already exists on this entity.
        // Check if the component already exists. 
        if ( m_Components.find( component->GetType() ) != m_Components.end() )
        {
            Debug() << "WARNING: attempting to add a duplicate component to the Entity \"" << m_Name << "\"" << std::endl;
            return;
        }

        // Set the component's parent as this entity
        component->SetEntity( this );

        // add it to the entity.
        m_Components[ component->GetType() ] = component;
    }


    /// @brief  sets the parent of this Entity
    /// @param  parent  the Entity that should be the parent of this one
    void Entity::SetParent( Entity* parent )
    {
        // if the child isn't in the scene, just set the parent pointer and handle the rest when it gets added
        if ( parent != nullptr && parent->IsInScene() == true && IsInScene() == false )
        {
            if ( m_Parent != nullptr )
            {
                Debug() << "Error: cannot set the parent of an Enity that already has a parent and isn't in the scene to an Entity that is in the scene" << std::endl;
                return;
            }

            m_Parent = parent;
            m_SetParentOnInit = true;
            return;
        }

        // warn in the edge case that the new parent is a descendant
        if ( parent != nullptr && parent->IsDescendedFrom( this ) )
        {
            Debug() << "WARNING: cannot set the parent of Entity \"" << m_Name <<
                "\" to its descendant \"" << parent->GetName() << "\"" << std::endl;
            return;
        }

        // separate this from its previous parent
        if ( m_Parent != nullptr )
        {
            m_Parent->removeChild( this );
        }

        Entity* previousParent = m_Parent;
        m_Parent = parent;

        // add this to its new parent
        if ( parent != nullptr )
        {
            parent->addChild( this );
        }

        propagateHeirachyChangeEvent( previousParent );
    }


    /// @brief  checks if this Entity is descended from another Entity
    /// @param  ancestor    The entity to check if this Entity is descended from
    /// @return whether this Entity is descended from the other Entity
    bool Entity::IsDescendedFrom( Entity const* ancestor ) const
    {
        for ( Entity* parent = m_Parent; parent != nullptr; parent = parent->m_Parent )
        {
            if ( parent == ancestor )
            {
                return true;
            }
        }

        return false;
    }


    /// @brief  queues this Entity and its children to be added to the Scene
    void Entity::AddToScene()
    {
        Entities()->QueueAddEntity( this );

        for ( Entity* child : m_Children )
        {
            child->AddToScene();
        }
    }


//------------------------------------------------------------------------------
// Public: accessors
//------------------------------------------------------------------------------

    
    /// @brief  gets all components in this Entity
    /// @return the map of all components in this Entity
    std::map< std::type_index, Component* >& Entity::getComponents()
    {
        return m_Components;
    }


    /// @brief  gets whether this Entity is flagged for destruction
    /// @return whether this Entity is flagged for destruction
    bool Entity::IsDestroyed() const
    {
        return m_IsDestroyed;
    }


    /// @brief  gets this Entity's name
    /// @return this Entity's name
    std::string const& Entity::GetName() const
    {
        return m_Name;
    }

    /// @brief  sets this Entity's name
    /// @param  name    the new name for this Entity
    void Entity::SetName( std::string const& name )
    {
        m_Name = name;
    }


    /// @brief  gets the Id of this Component
    /// @return the Id of this Component
    unsigned Entity::GetId() const
    {
        return m_Id;
    }


    /// @brief  gets the parent of this Entity
    /// @return the parent of this Entity
    Entity const* Entity::GetParent() const
    {
        return m_Parent;
    }

    /// @brief  gets the parent of this Entity
    /// @return the parent of this Entity
    Entity* Entity::GetParent()
    {
        return m_Parent;
    }


    /// @brief  gets the children of this Entity
    /// @return the children of this Entity
    std::vector< Entity const* > const& Entity::GetChildren() const
    {
        return reinterpret_cast< std::vector< Entity const* > const& >( m_Children );
    }

    /// @brief  gets the children of this Entity
    /// @return the children of this Entity
    std::vector< Entity* > const& Entity::GetChildren()
    {
        return m_Children;
    }


    /// @brief  gets the number of descendants this Entity has
    /// @return the number of descendants this Entity has
    int Entity::GetNumDescendants() const
    {
        return m_NumDescendants;
    }


    /// @brief  gets whether the Entity is currently in the scene
    /// @return whether the Entity is currently in the scene
    bool Entity::IsInScene() const
    {
        return m_IsInScene;
    }


//------------------------------------------------------------------------------
// Public: engine methods
//------------------------------------------------------------------------------


    /// @brief  Initializes all components / children of this Entity
    /// @brief  FOR ENGINE USE ONLY - only call this if you're modifying core engine functionality
    void Entity::Init()
    {
        m_IsInScene = true;

        if ( m_SetParentOnInit )
        {
            Entity* parent = m_Parent;
            m_Parent = nullptr;
            SetParent( parent );
        }

        for ( auto& [ type, component ] : m_Components )
        {
            component->OnInit();
        }
    }

    /// @brief  exits all components / children of this Entity
    /// @brief  FOR ENGINE USE ONLY - only call this if you're modifying core engine functionality
    void Entity::Exit()
    {
        for ( auto& [ type, component ] : m_Components )
        {
            component->OnExit();
        }

        for ( ComponentReferenceBase* componentReference : m_ComponentReferences )
        {
            componentReference->Clear();
        }
        m_ComponentReferences.clear();

        for ( EntityReference* entityReference : m_EntityReferences )
        {
            entityReference->Clear();
        }
        m_EntityReferences.clear();

        m_IsInScene = false;

        if ( m_Parent != nullptr )
        {
            m_Parent->removeChild( this );
        }
    }


    /// @brief  adds an EntityReference to this Entity
    /// @param  entityReference the refernce to add
    void Entity::AddEntityReference( EntityReference* entityReference )
    {
        m_EntityReferences.insert( entityReference );
    }

    /// @brief  removes an EntityReference to this Entity
    /// @param  entityReference the refernce to remove
    void Entity::RemoveEntityReference( EntityReference* entityReference )
    {
        m_EntityReferences.erase( entityReference );
    }


    /// @brief  adds an ComponentReference to this Component
    /// @param  componentReference  the refernce to add
    void Entity::AddComponentReference( ComponentReferenceBase* componentReference ) noexcept
    {
        m_ComponentReferences.insert( componentReference );
    }

    /// @brief  removes an ComponentReference to this Component
    /// @param  componentReference  the refernce to remove
    void Entity::RemoveComponentReference( ComponentReferenceBase* componentReference ) noexcept
    {
        m_ComponentReferences.erase( componentReference );
    }

    
//-----------------------------------------------------------------------------
// private: methods
//-----------------------------------------------------------------------------
    
    
    /// @brief  adds a child to this Enity
    /// @param  child   the child to add to this Enitity
    /// @note   assumes that this and the child's IsInScene status is the same
    /// @note   assumes that the child currently has no parent
    void Entity::addChild( Entity* child )
    {
        m_Children.push_back( child );

        if ( IsInScene() )
        {
            Entities()->MoveEntityAfterParent( child );
        }

        propagateDescendantChange( child->GetNumDescendants() + 1 );

        if ( IsInScene() )
        {
            for ( auto& [ type, component ] : m_Components )
            {
                component->OnAddChild( child );
            }
        }
    }

    /// @brief  removes a child from this Enity
    /// @param  child   the child to remove from this Enitity
    /// @note   assumes that this and the child's IsInScene status is the same
    void Entity::removeChild( Entity* child )
    {
        if ( IsInScene() )
        {
            for ( auto& [ type, component ] : m_Components )
            {
                component->OnRemoveChild( child );
            }
        }

        // remove child from the children list
        auto it = std::find( m_Children.begin(), m_Children.end(), child );
        if ( it == m_Children.end() )
        {
            Debug() << "ERROR: cannot find child \"" << child->GetName() << "\" to remove" << std::endl;
            return;
        }
        m_Children.erase( it );

        if ( child->IsInScene() )
        {
            Entities()->MoveToEnd( child );
        }

        propagateDescendantChange( -(child->GetNumDescendants() + 1) );
    }


    /// @brief  changes descendant count and propagates that change
    /// @param  changeBy    the number of descendants added or removed
    void Entity::propagateDescendantChange( int changeBy )
    {
        m_NumDescendants += changeBy;

        if ( m_Parent != nullptr && IsInScene() == m_Parent->IsInScene() )
        {
            m_Parent->propagateDescendantChange( changeBy );
        }
    }


    /// @brief  propagates an OnHeirarchyChange event downwards
    /// @param  previousParent the previous parent of this Entity
    void Entity::propagateHeirachyChangeEvent( Entity* previousParent )
    {
        for ( auto& [ type, component ] : m_Components )
        {
            component->OnHierarchyChange( previousParent );
        }

        for ( Entity* child : m_Children )
        {
            child->propagateHeirachyChangeEvent( this );
        }
    }


//------------------------------------------------------------------------------
// private: inspection
//------------------------------------------------------------------------------


    /// @brief used by the Debug System to display information about this Entity
    void Entity::Inspect()
    {
        if ( ImGui::BeginCombo( "##Add Component", "Add Component" , ImGuiComboFlags_HeightLarge) )
        {
            for ( auto& [ name, info ] : ComponentFactory::GetComponentTypes() )
            {
                if ( m_Components.contains( info.first ) )
                {
                    continue;
                }

                if ( ImGui::Selectable( name.c_str(), false ) )
                {
                    Component* component = info.second();
                    AddComponent( component );

                    if ( IsInScene() )
                    {
                        component->OnInit();

                        // tell other components that a component was added
                        for ( ComponentReferenceBase* componentReference : m_ComponentReferences )
                        {
                            componentReference->TrySet( component );
                        }
                    }
                }
            }
            ImGui::EndCombo();
        }

        //if ( ImGui::BeginCombo( "##Remove Component", "Remove Component" , ImGuiComboFlags_HeightLarge ) )
        //{
        //    for ( auto& [ key, component ] : m_Components )
        //    {
        //        if ( ImGui::Selectable( PrefixlessName( key ).c_str(), false ) )
        //        {
        //            removeComponent( component );
        //            break;
        //        }
        //    }
        //    ImGui::EndCombo();
        //}

        static std::string name = "";
        ImGui::InputText( "Entity Name", &name );
        if ( ImGui::IsItemFocused() && Input()->GetKeyTriggered( GLFW_KEY_ENTER ) )
        {
            if ( name.empty() == false )
            {
                Debug() << "Renamed Entity " << m_Name << " to " << name << "\n";
                m_Name = name;
                name.clear();
            }
        }

        // keep track of a component to be deleted, as Components can't be deleted mid-iteration
        Component* componentToDelete = nullptr;

        // Loop through all the components of the entity
        //  To display the components in the inspector
        for ( auto const& [ key, component ] : m_Components )
        {
            std::string const componentName = PrefixlessName( key );

            // Check if the tree node is open
            bool open = ImGui::TreeNode( componentName.c_str() );

            // right click menu for the tree node
            if ( ImGui::BeginPopupContextItem() )
            {
                if ( ImGui::MenuItem( "Copy" ) )
                {
                    Stream::CopyToClipboard( component );
                }
                if ( ImGui::MenuItem( "Paste" ) )
                {
                    Stream::PasteFromClipboard( component );
                }
                if ( ImGui::MenuItem( "Delete" ) )
                {
                    componentToDelete = component;
                }
                ImGui::EndPopup();
            }


            // Display the component's inspector
            if ( open )
            {
                component->BaseComponentInspector();
                ImGui::TreePop();
            }
        }

        if ( componentToDelete != nullptr )
        {
            removeComponent( componentToDelete );
        }
    }


    /// @brief used by the Debug System to Rename this Entity
    void Entity::RenameEntity(const char* popup_id)
    {
		// Open a popup window to rename the entity
        if (ImGui::BeginPopupModal(popup_id, NULL, ImGuiWindowFlags_AlwaysAutoResize))
        {
            static char buffer[128] = "";  // Buffer to hold the input, you can save this
            ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.45f);
            ImGui::InputText("##Entity Name", buffer, IM_ARRAYSIZE(buffer));

            // add entity button
            ImGui::SameLine();
            if (ImGui::Button("Enter", ImVec2(100, 0)))
            {
                SetName(buffer);  // Set the new name
                ImGui::CloseCurrentPopup();  // Close the popup
            }

            ImGui::EndPopup();
        }
    }

    
//-----------------------------------------------------------------------------
// private: inspection
//-----------------------------------------------------------------------------


    /// @brief  removes a Component using the Inspector
    /// @param  component   the Component to remove
    void Entity::removeComponent( Component* component )
    {
        if ( IsInScene() )
        {
            // tell other components a Component is about to be removed
            for ( ComponentReferenceBase* componentReference : m_ComponentReferences )
            {
                componentReference->TryRemove( component );
            }

            component->OnExit();
        }

        m_Components.erase( component->GetType() );
        delete component;
    }


//------------------------------------------------------------------------------
// private: reading
//------------------------------------------------------------------------------


    /// @brief  Clone this entity from an archetype.
    /// @param  data    The json value to read from.
    void Entity::readArchetype( nlohmann::ordered_json const& data )
    {
        *this = *AssetLibrary< Entity >()->GetAsset( Stream::Read< std::string >( data ) );
    }

    /// @brief  Read in the name of entity.
    /// @param  data    The json value to read from.
    void Entity::readName( nlohmann::ordered_json const& data )
    {
	    Stream::Read( m_Name, data );
    }

    /// @brief  Read in the data for all the components of entity.
    /// @param  data    The json object to read from.
    void Entity::readComponents( nlohmann::ordered_json const& data )
    {
        for ( auto& [ key, value ] : data.items() )
	    {

            // verify that the Component type is valid
            std::type_index const* type = ComponentFactory::GetTypeId( key );
            if ( type == nullptr )
            {
                Debug() << "JSON WARNING: unrecognized token \"" << key << "\" encountered at " << Stream::GetDebugLocation() << std::endl;
                continue;
            }

            // [] operator finds the key in the map, or creates it if it doesn't exist yet.
            Component*& component = m_Components[ *type ];

            // if the component doesn't exist yet, create and add it.
            if ( component == nullptr )
            {
                component = ComponentFactory::Create( key );
                component->SetEntity( this );
            }

            Stream::PushDebugLocation( key + "." );

			// Read in all the data for the component from the json.
			Stream::Read( component, value );

            Stream::PopDebugLocation();
	    }
    }


    /// @brief  reads the Children of this Entity
    /// @param  data    the json data to read from
    void Entity::readChildren( nlohmann::ordered_json const& data )
    {
        if ( IsInScene() )
        {
            Debug() << "WARNING: cannot paste children into an Entity in the scene" << std::endl;
            return;
        }

        for ( auto& childData : data )
        {
            Entity* child = new Entity();
            Stream::Read( child, childData );

            child->SetParent( this );
        }
    }

    
//-----------------------------------------------------------------------------
// public: reading / writing
//-----------------------------------------------------------------------------


    /// @brief  gets the map of read methods for Entities
    /// @return the map of read methods for Entities
    ReadMethodMap< ISerializable > const& Entity::GetReadMethods() const
    {
        static ReadMethodMap< Entity > const readMethods = {
            { "Archetype"  , &Entity::readArchetype  },
            { "Name"       , &Entity::readName	     },
            { "Components" , &Entity::readComponents },
            { "Children"   , &Entity::readChildren   }
        };
        
        return (ReadMethodMap< ISerializable > const&)readMethods;
    }


    /// @brief  Write all Entity data to a JSON file.
    /// @return The JSON file containing the Entity data.
    nlohmann::ordered_json Entity::Write() const
    {
        nlohmann::ordered_json json;

        json[ "Name" ] = m_Name;

        nlohmann::ordered_json& components = json[ "Components" ];
        for ( auto& [ key, value ] : m_Components )
        {
            components[ ComponentFactory::GetTypeName( key ) ] = value->Write();
        }

        nlohmann::ordered_json& children = json[ "Children" ];
        for ( Entity* child : m_Children )
        {
            children.push_back( child->Write() );
        }

        return json;
    }



//------------------------------------------------------------------------------
// copying
//------------------------------------------------------------------------------


    /// @brief  copies all of another Entity's data and Components into this Entity
    /// @param  other   the entity to copy from
    void Entity::operator =( Entity const& other )
    {
        assert( m_Components.empty() && m_Children.empty() && m_IsInScene == false );

        m_Name = other.m_Name;
        m_IsDestroyed = false;

        for ( auto& [ type, component ] : other.m_Components )
        {
            AddComponent( component->Clone() );
        }

        for ( Entity const* child : other.m_Children )
        {
            child->Clone()->SetParent( this );
        }
    }


//------------------------------------------------------------------------------
