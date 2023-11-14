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
#include "InputSystem.h"
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
	    m_Components(),
        m_Id( GetUniqueId() )
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

        if ( ImGui::BeginCombo( "Add Component", "Select Component" ) )
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
                    component->OnInit();
                }
            }
            ImGui::EndCombo();
        }

        if ( ImGui::BeginCombo( "Remove Component", "Select Component" ) )
        {
            for ( auto& [ key, component ] : m_Components )
            {
                if ( ImGui::Selectable( PrefixlessName( key ).c_str(), false ) )
                {
                    component->OnExit();
                    delete component;
                    m_Components.erase( key );
                    break;
                }
            }
            ImGui::EndCombo();
        }

        static char name[ 128 ];
        //strcpy_s( name, 128, m_Name.c_str() )
        ImGui::InputText("Entity Name", name, 128);

        if (ImGui::Button("Rename Entity") || 
            Input()->GetKeyTriggered(GLFW_KEY_ENTER))
        {
            if (strlen(name) > 0)
            {
                Debug() << "Renamed Entity " << m_Name << " to " << name << "\n";
                m_Name = name;
            }
        }

        for (const auto& componentPair : this->getComponents())
        {
            const std::string componentName = componentPair.second->GetType().name() + 5; // Skip "class "
            /// Create a unique identifier for the popup menu based on the entity's ID
            std::string popup_id = "ComponentContextMenu##" + std::to_string(GetId());


            if (ImGui::TreeNode(componentName.c_str()))
            {
                // Check for right-click on the tree node while open
                if (ImGui::BeginPopupContextItem(popup_id.c_str()))
                {
                    if (ImGui::MenuItem("Copy"))
                    {
                        Stream::CopyToClipboard(*this);
                    }
                    if (ImGui::MenuItem("Paste"))
                    {
                        Stream::PasteFromClipboard(*this);
                    }
                    ImGui::EndPopup();
                }

                componentPair.second->BaseComponentInspector();
                ImGui::TreePop();
            }

            // Check for right-click on the tree node while closed
            if (ImGui::BeginPopupContextItem(popup_id.c_str()))
            {
                if (ImGui::MenuItem("Copy"))
                {
                    Stream::CopyToClipboard(*this);
                }
                if (ImGui::MenuItem("Paste"))
                {
                    Stream::PasteFromClipboard(*this);
                }
                ImGui::EndPopup();
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

            // No dangling ptr
            component.second = nullptr;
        }
        // Clear the component list.
        m_Components.clear();
    }

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


//------------------------------------------------------------------------------
// private: reading
//------------------------------------------------------------------------------

    /// @brief  Clone this entity from an archetype.
    /// @param  data    The json value to read from.
    void Entity::readArchetype( nlohmann::ordered_json const& data )
    {
        *this = *AssetLibrary<Entity>()->GetAsset( Stream::Read<std::string>( data ) );
    }

    /// @brief  Read in the name of entity.
    /// @param  data    The json value to read from.
    void Entity::readName( nlohmann::ordered_json const& data )
    {
	    m_Name = Stream::Read<std::string>( data );
    }

    /// @brief  Read in the data for all the components of entity.
    /// @param  data    The json object to read from.
    void Entity::readComponents( nlohmann::ordered_json const& data )
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

    /// @brief  Write all Entity data to a JSON file.
    /// @return The JSON file containing the Entity data.
    nlohmann::ordered_json Entity::Write() const
    {
        nlohmann::ordered_json data;

        data["Name"] = m_Name;

        auto& components = data["Components"];
        for ( auto& [ key, value ] : m_Components )
        {
            components[ ComponentFactory::GetTypeName( key ) ] = value->Write();
        }

        return data;
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
