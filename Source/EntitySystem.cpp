/// @file       EntitySystem.cpp
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      System that manages the storing and handling of Entities
/// @version    0.1
/// @date       2023-09-05
/// 
/// @copyright  Copyright (c) 2023 Digipen Institute of Technology

#include "EntitySystem.h"

#include "Entity.h"

#include "DebugSystem.h"
#include <algorithm>

//-----------------------------------------------------------------------------
// public: methods
//-----------------------------------------------------------------------------


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

    /// @brief  returns the container of all Entities in the Scene
    /// @return the container of all Entities in the Scene
    std::vector< Entity* > const& EntitySystem::GetEntities() const
    {
        return m_Entities;
    }


    /// @brief  checks if the EntitySystem contains the given Entity
    /// @param  entity  the Entity to search for
    /// @return whether or not the EntitySystem has the specified Entity
    bool EntitySystem::HasEntity( Entity* entity )
    {
        auto iterator = std::find( m_Entities.begin(), m_Entities.end(), entity );

        return iterator != m_Entities.end();
    }


//-----------------------------------------------------------------------------
// private: virtual override methods
//-----------------------------------------------------------------------------


    /// @brief  Gets called whenever a scene is exited
    void EntitySystem::OnSceneExit()
    {
        for ( Entity* entity : m_Entities )
        {
            entity->Exit();
        }

        for ( Entity* entity : m_Entities )
        {
            delete entity;
        }

        m_Entities.clear();
    }


    /// @brief Gets Called each frame
    void EntitySystem::OnUpdate(float)
    {
        removeEntities();

        addEntities();
    }

//-----------------------------------------------------------------------------
// public: engine methods
//-----------------------------------------------------------------------------


    /// @brief  queues an Entity to be added to the EntitySystem
    /// @param  entity  the entity to add the the EntitySystem
    void EntitySystem::QueueAddEntity( Entity* entity )
    {
        m_EntitiesToAdd.push_back( entity );
    }


    /// @brief  moves an Entity to the end of its parent's children
    /// @brief  FOR ENGINE USE ONLY - call this only if you're modifying core engine functionality
    /// @param  entity  - the entity to move
    void EntitySystem::MoveEntityAfterParent( Entity* entity )
    {
        auto destination = std::find( m_Entities.begin(), m_Entities.end(), entity->GetParent() ) + entity->GetParent()->GetNumDescendants() + 1;
        auto sourceBegin = std::find( m_Entities.begin(), m_Entities.end(), entity );
        auto sourceEnd = sourceBegin + entity->GetNumDescendants();

        if ( destination > sourceBegin )
        {
            std::rotate( sourceBegin, sourceEnd, destination );
        }
        else
        {
            std::rotate( destination, sourceBegin, sourceEnd );
        }
    }

    /// @brief  moves an Entity to the end the EntitySystem
    /// @brief  FOR ENGINE USE ONLY - call this only if you're modifying core engine functionality
    /// @param  entity  - the entity to move
    void EntitySystem::MoveToEnd( Entity* entity )
    {
        auto it = std::find( m_Entities.begin(), m_Entities.end(), entity );
        std::rotate( it, it + entity->GetNumDescendants() + 1, m_Entities.end() );
    }
    

//-----------------------------------------------------------------------------
// private: methods
//-----------------------------------------------------------------------------


    /// @brief  removes all queued Entities from the EntitySystem
    void EntitySystem::removeEntities()
    {
        // lambda to filter for entities to remove
        auto isDestroyed = []( Entity* entity ) -> bool
        {
            return entity->IsDestroyed();
        };

        // get all the entities that need to be removed
        std::vector< Entity* > entitiesToRemove;
        std::copy_if( m_Entities.begin(), m_Entities.end(), std::back_inserter( entitiesToRemove ), isDestroyed );

        // exit the entities
        for ( Entity* entity : entitiesToRemove )
        {
            entity->Exit();
        }

        // delete the entities
        for ( Entity* entity : entitiesToRemove )
        {
            delete entity;
        }

        // remove the entities from the System
        m_Entities.erase(
            std::remove_if(
                m_Entities.begin(),
                m_Entities.end(),
                isDestroyed
            ),
            m_Entities.end()
        );
    }


    /// @brief  adds all queued Entites to the EntitySystem
    void EntitySystem::addEntities()
    {
        // add entities to the system
        for ( Entity* entity : m_EntitiesToAdd )
        {
            if ( entity->GetParent() != nullptr )
            {
                // if the entity has a parent, insert after its parent/siblings
                // possible optimization: keep track of the position of the previously added Entity, and try there first
                m_Entities.insert(
                    std::find( m_Entities.begin(), m_Entities.end(), entity->GetParent() ) + 1 + entity->GetParent()->GetNumDescendants(),
                    entity    
                );
            }
            else
            {
                m_Entities.insert( m_Entities.end(), entity );
            }
        }

        // move the new Entities into a new vector, in case any component OnInit functions add more Entities to m_EntitiesToAdd
        std::vector< Entity* > newEntities = std::move( m_EntitiesToAdd );
        m_EntitiesToAdd.clear();

        // initialize the entities
        for ( Entity* entity : newEntities )
        {
            entity->Init();
        }
    }


//-----------------------------------------------------------------------------
// public: inspection
//-----------------------------------------------------------------------------


    /// @brief Called by the DebugSystem to display the debug window
    void EntitySystem::DebugWindow()
    {
        static bool createEntity = false; 

        /// Used to make the Entity List a pop out window
        if ( ImGui::Button( m_PopOut ? "Pop In" : "Pop Out" ) )
        {
            m_PopOut = !m_PopOut;
        }
        ImGui::SameLine();
        if ( ImGui::Button( "Create Entity" ) )
        {
            createEntity = true;
        }

        if ( createEntity )
        {
            Entity* entity = new Entity(); /// Create a new entity
            entity->SetName( "New Entity" ); /// Set the name of the entity
            QueueAddEntity( entity ); /// Add the entity to the EntitySystem
            createEntity = false;
        }

        /// Display the Entity List
	    entityListWindow();
    }


    /// @brief Opens a window to create a new entity
    /// @return - true if the window is open
    bool EntitySystem::EntityCreateWindow()
    {
        /// Creates the window and auto resizes it
        ImGui::Begin( "Add New Entity", NULL, ImGuiWindowFlags_AlwaysAutoResize );

        /// Sets the size of the window if it is the first time it is opened
        ImGui::SetWindowSize( ImVec2( 500, 500 ), ImGuiCond_FirstUseEver );

        /// Input text for entity name
        static std::string name = "";
        ImGui::PushItemWidth( ImGui::GetWindowWidth() * 0.45f );
        ImGui::InputText( "##Entity Name", &name );

        // add entity button
        ImGui::SameLine();

        if ( ImGui::Button( "Add Entity", ImVec2( 100, 0 ) ) || ImGui::IsKeyPressed( ImGuiKey_Enter ) )
        {

            Entity* entity = new Entity(); /// Create a new entity
            entity->SetName( name ); /// Set the name of the entity
            QueueAddEntity( entity ); /// Add the entity to the EntitySystem

            /// if the entity is added, close the window
            ImGui::End();
            return false; // close the window

        }

        /// Aligns the cancel button with the add entity button
        ImGui::SameLine();
        /// Creates the cancel button
        if ( ImGui::Button( "Cancel", ImVec2( 100, 0 ) ) )
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
// private: inspection
//-----------------------------------------------------------------------------


    /// @brief Displays the Entity List Window
    void EntitySystem::entityListWindow()
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
            for ( Entity* entity : m_Entities )
            {
                /// Shows the Right C
                entityPropertiesWindow( entity );
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
    void EntitySystem::entityPropertiesWindow(Entity* entity)
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

    
//-----------------------------------------------------------------------------
// public: reading / writing
//-----------------------------------------------------------------------------


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
            entity->Init();
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


//-----------------------------------------------------------------------------
// public: singleton stuff
//-----------------------------------------------------------------------------


    /// @brief  gets the instance of EntitySystem
    /// @return the instance of the EntitySystem
    EntitySystem* EntitySystem::GetInstance()
    {
        static EntitySystem* instance = nullptr;
        if ( instance == nullptr )
        {
            instance = new EntitySystem();
        }

        return instance;
    }

    
//-----------------------------------------------------------------------------
// private: singleton stuff
//-----------------------------------------------------------------------------


    /// @brief  Constructs the EntitySystem
    EntitySystem::EntitySystem() :
        System( "EntitySystem" )
    {}


//-----------------------------------------------------------------------------
