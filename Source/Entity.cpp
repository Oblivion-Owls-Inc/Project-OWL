///--------------------------------------------------------------------------//
/// @file   Entity.cpp
/// @brief  Function definitions for the Entity class
/// 
/// @author Aidan Straker (aidan.straker)
/// @date   September 2023
///
/// @copyright © 2023 DigiPen (USA) Corporation.
///--------------------------------------------------------------------------//
#include "Entity.h"

#include <algorithm>		  // std::sort
#include "Component.h"		  // Type
#include <cassert>			  // assert
#include "ComponentFactory.h" // Create.
#include "AssetLibrarySystem.h"
#include "DebugSystem.h"
#include "basics.h"


///-----------------------------------------------------------------------------
/// Static Variables
///-----------------------------------------------------------------------------

///------------------------------------------------------------------------------
/// Public: constructor / destructor
///------------------------------------------------------------------------------

    /// @brief  default constrcutor
    Entity::Entity() :
        m_Name( "" ),
	    m_IsDestroyed( false ),
	    m_Components()
    {}

    /// @brief destructor
    Entity::~Entity()
    {
        free();
    }

//------------------------------------------------------------------------------
// Public: methods
//------------------------------------------------------------------------------

    /// @brief  Initializes all components of this Entity
    /// @note   ONLY CALL THIS IF YOU KNOW WHAT YOU'RE DOING
    void Entity::InitComponents()
    {
        for ( auto& component : m_Components )
        {
            component.second->OnInit();
        }
    }

    /// @brief  exits all components of this Entity
    /// @note   ONLY CALL THIS IF YOU KNOW WHAT YOU'RE DOING
    void Entity::ExitComponents()
    {
        for ( auto& component : m_Components )
        {
            component.second->OnExit();
        }
    }

//------------------------------------------------------------------------------
// Public: accessors
//------------------------------------------------------------------------------

    /// @brief  attaches a component to this Entity
    /// @param  component   the component to attach to this Entity
    void Entity::AddComponent(Component* component)
    {

        ///You tried to add a component that already exists on this entity.
        // Check if the component already exists. 
        assert( m_Components.find( component->GetType() ) == m_Components.end() );

        // Set the component's parent as this entity
        component->SetParent( this );

        // add it to the entity.
        m_Components[ component->GetType() ] = component;
    }

//------------------------------------------------------------------------------
// private: methods
//------------------------------------------------------------------------------

    void Entity::Inspect()
    {

        if (ImGui::Button("Add Component", ImVec2(-1, 0)))
        {
            m_AddComponent = !m_AddComponent;
        }

        if (ImGui::Button("Remove Component", ImVec2(-1, 0)))
        {
			m_RemoveComponent = !m_RemoveComponent;
		}

        if(ImGui::Button("Rename Entity", ImVec2(-1,0)))
        {
			m_RenameEntity = !m_RenameEntity;
        }

        if (m_AddComponent)
        {
            AddComponent();
        }

        if (m_RemoveComponent)
        {
			RemoveComponent();
		}

        if (m_RenameEntity)
        {
            RenameEntity();
        }

        for (const auto& componentPair : this->getComponents())
        {
            const std::string componentName = componentPair.second->GetType().name() + 5; // Skip "class "
            if (ImGui::TreeNode(componentName.c_str()))
            {
                componentPair.second->Inspector();
                ImGui::TreePop();
            }
        }

    }

    /// @brief Deallocates all memory associated with an entity.
    void Entity::free()
    {
        // Traverse the component list
        for (auto& component : m_Components)
        {
            // Make sure the component is valid.
            assert(component.second);

            // Delete the component.
            delete component.second;
        }
        // Clear the component list.
        m_Components.clear();
    }

    void Entity::AddComponent()
    {
        std::string windowName = "Add Component to " + this->GetName();
        ImGui::Begin(windowName.c_str(), &m_AddComponent);

        for (auto& component : ComponentFactory::GetComponentTypes())
        {
            // Skip the component if it's already attached to this entity
            if (m_Components.find(component.second.first) != m_Components.end())
            {
                continue; // Skip to the next iteration
            }

            if (ImGui::Button(component.first.c_str(), ImVec2(-1, 0)))
            {
                ///Create a new component and add it to the entity.
                Component* newComponent = ComponentFactory::Create(component.first);

                ///Initialize the component.
                if (newComponent) 
                {
                    newComponent->SetParent(this);
                    newComponent->OnInit();
                }
                ///Add the component to the entity.
                AddComponent(newComponent);
                ///Close the window
                m_AddComponent = false;
            }
        }

        ImGui::End();
    }


    void Entity::RemoveComponent()
    {
        std::string windowName = "Remove Component from " + this->GetName();
        ImGui::Begin(windowName.c_str(), &m_RemoveComponent);

        for (auto& component : ComponentFactory::GetComponentTypes())
        {
            /// Skip the component if it's not already attached to this entity
            if (!(m_Components.find(component.second.first) != m_Components.end()))
            {
                continue; /// Skip to the next iteration
            }


            if (ImGui::Button(component.first.c_str(), ImVec2(-1, 0)))
            {
                m_Components.find(component.second.first)->second->OnExit();
                m_Components.erase(component.second.first);
                m_RemoveComponent = false; ///Close the window
            }
        }

            ImGui::End();
    }

    void Entity::RenameEntity()
    {
		std::string windowName = "Rename " + this->GetName();

		ImGui::Begin(windowName.c_str(),&m_RenameEntity);

        static char buffer[256] = "";

        ImGui::InputText("Name", buffer, sizeof(buffer));

        if(ImGui::Button("Rename"))
		{

            SetName(std::string(buffer));

            buffer[0] = '\0';

            m_RenameEntity = false;
		}
		ImGui::End();
    }

//------------------------------------------------------------------------------
// private: reading
//------------------------------------------------------------------------------

    /// @brief  Clone this entity from an archetype.
    /// @param  data    The json value to read from.
    void Entity::readArchetype( nlohmann::json const& data )
    {
        *this = *AssetLibrary<Entity>()->GetAsset( Stream::Read<std::string>( data ) );
    }

    /// @brief  Read in the name of entity.
    /// @param  data    The json value to read from.
    void Entity::readName( nlohmann::json const& data )
    {
	    m_Name = Stream::Read<std::string>( data );
    }

    /// @brief  Read in the data for all the components of entity.
    /// @param  data    The json object to read from.
    void Entity::readComponents( nlohmann::json const& data )
    {
        for ( auto& [ key, value ] : data.items() )
	    {

            // [] operator finds the key in the map, or creates it if it doesn't exist yet.
            Component*& component = m_Components[ ComponentFactory::GetTypeId( key ) ];

            // if the component doesn't exist yet, create and add it.
            if ( component == nullptr )
            {
                component = ComponentFactory::Create( key );
                component->SetParent( this );
            }

            // read the component data
		    try
		    {
			    // Read in all the data for the component from the json.
			    Stream::Read( component, value );
		    }
		    catch (std::runtime_error error)
		    {
			    std::cerr << error.what() << std::endl;
		    }
	    }
    }

    /// @brief A map of the all read methods used by the Entity class.
    ReadMethodMap< Entity > const Entity::s_ReadMethods = {
	    { "Archetype"  , &readArchetype  },
	    { "Components" , &readComponents },
	    { "Name"       , &readName	     }
    };

//------------------------------------------------------------------------------
// copying
//------------------------------------------------------------------------------

    /// @brief  copies all of another Entity's data and Components into this Entity
    /// @param  other   the entity to copy from
    void Entity::operator =( Entity const& other )
    {
        assert( m_Components.empty() );

        m_Name = other.m_Name;
        m_IsDestroyed = false;

        for ( auto component : other.m_Components )
        {
            AddComponent( component.second->Clone() );
        }
    }

//------------------------------------------------------------------------------
