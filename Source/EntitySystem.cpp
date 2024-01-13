/// @file       EntitySystem.cpp
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      System that manages the storing and handling of Entities
/// @version    0.1
/// @date       2023-09-05
/// 
/// @copyright  Copyright (c) 2023 Digipen Institute of Technology

#include "EntitySystem.h"
#include "ScriptingSystem.h"
#include "DebugSystem.h"
#include <algorithm>

///-----------------------------------------------------------------------------
/// Set Static Variables
///-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// public methods
//-----------------------------------------------------------------------------

    /// @brief  Adds an Entity to the EntitySystem
    /// @param  entity  the entity to add the the EntitySystem
    void EntitySystem::AddEntity( Entity* entity )
    {
        m_Entities.push_back( entity );
        entity->InitComponents();
    }

    /// @brief  gets an Entity by name
    /// @param  entityName  the name of the Entity to get
    /// @return the found Entity (nullptr if not found)
    Entity* EntitySystem::GetEntity( std::string const& entityName )
    {
        auto iterator = std::find_if(
            m_Entities.begin(),
            m_Entities.end(),
            [ entityName ]( Entity* entity ) -> bool
            {
                return entity->GetName() == entityName;
            }
        );

        if ( iterator == m_Entities.end() )
        {
            return nullptr;
        }

        return *iterator;
    }

    /// @brief  removes an Entity from the EntitySystem
    /// @param  entity  the Entity to remove
    void EntitySystem::RemoveEntity( Entity* entity )
    {
        auto iterator = std::find( m_Entities.begin(), m_Entities.end(), entity );
        
        if ( iterator != m_Entities.end() )
        {
            m_Entities.erase( iterator );
            entity->ExitComponents();
        }
        else
        {
            std::ostringstream errorMessage;
            errorMessage <<
                "Error: Could not find entity \"" << entity->GetName() <<
                "\" to remove from the EntitySystem";
            throw std::runtime_error( errorMessage.str() );
        }

    }

    /// @brief  checks if the EntitySystem contains the given Entity (for debugging)
    /// @param  entity  the Entity to search for
    /// @return whether or not the EntitySystem has the specified Entity
    bool EntitySystem::HasEntity( Entity* entity )
    {
        auto iterator = std::find( m_Entities.begin(), m_Entities.end(), entity );

        return iterator != m_Entities.end();
    }

    /// @brief  loads all of the m_Entities in a scene
    /// @param  entityData  the json object containing the entity data
    void EntitySystem::LoadEntities( nlohmann::ordered_json const& data )
    {
        for ( auto& [ key, value ] : data.items() )
        {
            Entity * entity = new Entity();
            Stream::Read( entity, value );
            m_Entities.push_back( entity );
        }

        for ( Entity* entity : m_Entities )
        {
            entity->InitComponents();
        }
    }

    /// @brief  saves all of the entities in a scene
    /// @return the written json data
    nlohmann::ordered_json EntitySystem::SaveEntities() const
    {
        nlohmann::ordered_json json;

        for ( Entity* entity : m_Entities )
        {
            json[ entity->GetName() ] = entity->Write();
        }

        return json;
    }

    /// @brief Opens a window to create a new entity
    /// @return - true if the window is open
    bool EntitySystem::EntityCreateWindow()
    {
        /// Creates the window and auto resizes it
        ImGui::Begin("Add New Entity", NULL, ImGuiWindowFlags_AlwaysAutoResize);

        /// Sets the size of the window if it is the first time it is opened
        ImGui::SetWindowSize(ImVec2(500, 500), ImGuiCond_FirstUseEver);

        /// Input text for entity name
        static char buffer[128] = ""; /// Buffer to hold the input, you can save this
        ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.45f);
        ImGui::InputText("##Entity Name", buffer, IM_ARRAYSIZE(buffer));

        // add entity button
        ImGui::SameLine();

        if (ImGui::Button("Add Entity", ImVec2(100, 0)) || ImGui::IsKeyPressed(ImGuiKey_Enter))
        {
            
            Entity* entity = new Entity(); /// Create a new entity
            entity->SetName(std::string(buffer)); /// Set the name of the entity
            AddEntity(entity); /// Add the entity to the EntitySystem

            /// if the entity is added, close the window
            ImGui::End();
            return false; // close the window
            
        }

        /// Aligns the cancel button with the add entity button
        ImGui::SameLine();
        /// Creates the cancel button
        if (ImGui::Button("Cancel", ImVec2(100, 0)))
        {
            /// If the cancel button is pressed, close the window
            ImGui::End();
            return false; // close the window
        }

        ///The Matching End to the Begin to create the window
        ImGui::End();
        return true; // keep the window open
    }

//-----------------------------------------------------------------------------
// virtual override methods
//-----------------------------------------------------------------------------

    void EntitySystem::OnSceneInit()
    {
        CreateLuaEntityBindings();
    }

    /// @brief  Gets called whenever a scene is exited
    void EntitySystem::OnSceneExit()
    {
        for ( Entity* entity : m_Entities )
        {
            entity->ExitComponents();
        }

        for ( Entity* entity : m_Entities )
        {
            if (entity->IsScript())
            {
                continue; 
            }

            if (entity)
                delete entity;
        }

        m_Entities.clear();
    }

    /// @brief Gets Called each frame
    void EntitySystem::OnUpdate(float)
    {
        for ( int i = 0; i < m_Entities.size(); ++i )
        {
            Entity * entity = m_Entities[i];

            if (entity->IsDestroyed())
            {
                RemoveEntity(entity);
                delete entity;
                --i;
            }
        }
    }

    /// @brief Called by the DebugSystem to display the debug window
    void EntitySystem::DebugWindow()
    {
        static bool createEntity = false; 

        /// Used to make the Entity List a pop out window
        if (ImGui::Button(m_PopOut ? "Pop In" : "Pop Out"))
        {
            m_PopOut = !m_PopOut;
        }
        ImGui::SameLine();
        if (ImGui::Button("Create Entity"))
        {
            createEntity = true;
        }

        if (createEntity)
        {
            Entity* entity = new Entity(); /// Create a new entity
            entity->SetName(std::string("New Entity")); /// Set the name of the entity
            AddEntity(entity); /// Add the entity to the EntitySystem
            createEntity = false;
        }

        /// Display the Entity List
        EntityListWindow();
    }

//-----------------------------------------------------------------------------
// private: methods
//-----------------------------------------------------------------------------

    /// @brief Displays the Entity List Window
    void EntitySystem::EntityListWindow()
    {
        /// used make this a pop out window
        if(m_PopOut)
        {
            ImGui::Begin("Entity List");
        }
        /// Used to Create a Frame around the Entity List
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
        /// Creates a table with 2 columns that can be resized
        if (ImGui::BeginTable("##split", 2, ImGuiTableFlags_BordersOuter | ImGuiTableFlags_Resizable | ImGuiTableFlags_ScrollY))
        {
            /// Freeze the first row so it doesn't scroll
            ImGui::TableSetupScrollFreeze(0, 1);
            /// Set the width of the first column
            ImGui::TableSetupColumn("Entities");
            /// Set the width of the second column
            ImGui::TableSetupColumn("Contents");
            ImGui::TableHeadersRow();
            
            /// Lists all the entities in the EntitySystem
            for (const auto& entity : m_Entities)
            {
                /// Shows the Right C
                EntityPropertiesWindow(entity);
            }

            /// End the table
            ImGui::EndTable();
        }
        /// Pop the style var
        ImGui::PopStyleVar();

        ///if the pop out window is open, end it
        if (m_PopOut)
        {
            ImGui::End();
        }
    }

    /// @brief Shows the properties of an Entity in the Contents column of the Entity List
    /// @param entity - the entity to display the properties of 
    void EntitySystem::EntityPropertiesWindow(Entity* entity)
    {
        ImGui::TableNextRow();  // Move to the next row

        ImGui::TableSetColumnIndex(0);  // Set focus to the first column

        /// Create a unique identifier for the entity based on its name and ID
        std::string label = entity->GetName() + "##" + std::to_string(entity->GetId());

        bool node_open = ImGui::TreeNodeEx(label.c_str());  // Create a tree node for the entity
        static bool open_popup = false ;  // Flag to check if the popup menu should be open

        /// Create a unique identifier for the popup menu based on the entity's ID
        std::string popup_id = "EntityContextMenu" + std::to_string(entity->GetId());
        /// Create a unique identifier for the delete popup menu based on the entity's ID
        std::string delete_id = "Confirm Deletion##" + std::to_string(entity->GetId());

        // Check for right-click on the tree node
        if (ImGui::BeginPopupContextItem(popup_id.c_str()))
        {
            /// Creates the context to copy, paste , and delete entities
            if (ImGui::MenuItem("Copy")) 
            {
               Stream::CopyToClipboard(entity);
            }
            if (ImGui::MenuItem("Paste"))
            {
                Stream::PasteFromClipboard(entity);
            }
            if (ImGui::MenuItem("Delete"))
            {
                open_popup = true;
            }
 
            ImGui::EndPopup();
        }

        if (open_popup)
        {
            ImGui::OpenPopup(delete_id.c_str(), ImGuiPopupFlags_NoOpenOverExistingPopup);
            /// This section needs to be fixed so the Modal popup works
            /// Always center this window when appearing
            ImVec2 center = ImGui::GetMainViewport()->GetCenter();
            ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f)); 
            /// Create a modal popup to confirm deletion
            if (ImGui::BeginPopupModal(delete_id.c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize))
            {

                ImGui::Text("Are you sure you want to delete this entity?");
                ImGui::Separator();
                /// Creates the buttons to confirm or cancel the deletion
                if (ImGui::Button("OK", ImVec2(120, 0)))
                {
                    entity->Destroy();
                    open_popup = false;
                }
                /// Set the focus on the cancel button
                ImGui::SetItemDefaultFocus();
                /// Aligns the cancel button with the OK button
                ImGui::SameLine();
                /// Creates the cancel button
                if (ImGui::Button("Cancel", ImVec2(120, 0)))
                {
                    /// Closes the popup
                    ImGui::CloseCurrentPopup();
                    open_popup = false;
                }

                /// Ends the popup to match with the BeginPopupModal
                ImGui::EndPopup();
            }
        }

        /// if the tree node is open, display the entity's properties
        if (node_open)
        {
            ImGui::TableSetColumnIndex(1);  // Set focus to the second column
            entity->Inspect();  // Display entity properties

            ImGui::TreePop();  // Pop the tree node
        }

    }

    void EntitySystem::CreateLuaEntityBindings()
    {

        Entity::LuaInit();

        Lua()->GetLuaInstance()->new_usertype<Entity>(
            "Entity",  // Name of the Class to be used in Lua
            sol::call_constructor, sol::constructors<Entity()>(),
            "IsDestroyed", &Entity::IsDestroyed,
            "Destroy", &Entity::Destroy,
            "SetName", &Entity::SetName,
            "Clone", &Entity::Clone,
            "GetName", &Entity::GetName,
            "AddComponent", sol::overload(
				static_cast<void (Entity::*)(std::string)>(&Entity::AddComponent)
			),
            "GetId", &Entity::GetId
        );

        Lua()->GetLuaInstance()->set_function(
            "GetEntityByName",
            [this](std::string const& entityName) -> Entity*
            {
                return GetEntity( entityName );
            }
        );

        Lua()->GetLuaInstance()->set_function(
            "AddToEntitySystem",
            [this](Entity* entity) {
                AddEntity(entity);
            }
        );


        Lua()->GetLuaInstance()->set_function(
            "GetEntity", [this](std::string const& entityName) -> Entity*
            {
				return GetEntity(entityName);
			}
        );


    }

//-----------------------------------------------------------------------------
// singleton stuff
//-----------------------------------------------------------------------------


    /// @brief  Constructs the EntitySystem
    EntitySystem::EntitySystem() :
        System( "EntitySystem" ),
        m_Entities()
    {}

    /// @brief  The singleton instance of EntitySystem
    EntitySystem * EntitySystem::s_Instance = nullptr;

    /// @brief  gets the instance of EntitySystem
    /// @return the instance of the EntitySystem
    EntitySystem * EntitySystem::GetInstance()
    {
        if ( s_Instance == nullptr )
        {
            s_Instance = new EntitySystem();
        }
        return s_Instance;
    }

//-----------------------------------------------------------------------------
