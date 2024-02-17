/// @file       EntitySystem.cpp
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      System that manages the storing and handling of Entities
/// @version    0.1
/// @date       2023-09-05
/// 
/// @copyright  Copyright (c) 2023 Digipen Institute of Technology

#include "EntitySystem.h"

#include "Entity.h"
#include "Transform.h"

#include "DebugSystem.h"
#include "imgui_internal.h"
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

    /// @brief Clears the Entity being inspected
    void EntitySystem::ClearSelectedEntity()
    {
        SelectedEntity = nullptr;
    }

    /// @brief  returns the entity being viewed in the inspector
    /// @return the Entity being viewed in the inspector
    Entity* EntitySystem::GetSelectedEntity() const
    {
        return SelectedEntity;
    }


//-----------------------------------------------------------------------------
// private: virtual override methods
//-----------------------------------------------------------------------------


    /// @brief  Gets called whenever a scene is exited
    void EntitySystem::OnSceneExit()
    {
        ClearSelectedEntity();

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
            if (entity == SelectedEntity)
                SelectedEntity = nullptr;

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
                    std::find( m_Entities.begin(), m_Entities.end(), entity->GetParent() ) + 1,
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


    /// @brief  adds the children of a loaded entity to the entities array
    /// @param  entity  the Entity to add the children of
    void EntitySystem::addLoadedChildren( Entity* entity )
    {
        for ( Entity* child : entity->GetChildren() )
        {
            m_Entities.push_back( child );
            addLoadedChildren( child );
        }
    }


//-----------------------------------------------------------------------------
// public: inspection
//-----------------------------------------------------------------------------


    /// @brief Called by the DebugSystem to display the debug window
    void EntitySystem::DebugWindow()
    {
        static bool createEntity = false; 

        if ( createEntity )
        {
            Entity* entity = new Entity(); /// Create a new entity
            entity->SetName( "New Entity" ); /// Set the name of the entity
            entity->AddToScene(); /// Add the entity to the EntitySystem
            createEntity = false;
        }

        /// Display the Entity List
	    DisplayEntityHierarchy();
        entityPropertiesWindow( SelectedEntity );
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
            Transform* transform = new Transform(); /// Create a new transform
            entity->SetName( name ); /// Set the name of the entity
            entity->AddComponent( transform ); /// Add the transform to the entity)
            entity->AddToScene(); /// Add the entity to the EntitySystem

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

    /// @brief Displays the Entity Hierarchy in the Entity List window
    void EntitySystem::DisplayEntityHierarchy()
    {
        static bool showDeletePopup = false; // Flag to check if the delete popup should be shown
        static Entity* selectedEntityToDelete = nullptr; // Entity selected for deletion

        static ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize;
        ImGui::Begin("Entity List", nullptr, window_flags);
        ImGui::SetWindowSize(ImVec2(500, 1000), ImGuiCond_FirstUseEver);


        // Function to display entities recursively
        std::function<void(Entity*,bool)> displayEntityRecursive = [&](Entity* entity, bool child)
        {
            std::vector< Entity* > entities;
            Entity* parent = entity ? entity->GetParent() : nullptr; // Get the parent of the current entity
            // Get the children of the parent if it exists, otherwise get the root entities
            if (!child)
            {
                entities = parent ? parent->GetChildren() : m_Entities; // Get the children of the parent if it exists, otherwise get the root entities
            }
            else
            {
				entities = entity->GetChildren(); // Get the children of the current entity
			}


            for (int i = 0; i < entities.size(); ++i)
            {
                Entity* currentEntity = entities[i];

                if (!child)
                {
                    if (!entity && currentEntity->GetParent() != nullptr)
                    {
                        continue; // Skip entities that are not root when entity is null
                    }
                }
				else
				{
					if (entity && currentEntity->GetParent() != entity)
					{
						continue; // Skip entities that are not children of the current entity
					}
				}

                    
                std::string popup_id = "EntityContextMenu##" + std::to_string(currentEntity->GetId());
                std::string delete_id = "Confirm Deletion##" + std::to_string(currentEntity->GetId());

                // Determine if the current entity is a leaf node (no children)
                bool isLeaf = currentEntity->GetChildren().empty();

                // Adjust flags based on whether the entity has children
                ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

                if (isLeaf)
                {
                    nodeFlags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen; // Leaf nodes do not expand
                }

                // Highlight selected entity
                if (SelectedEntity == currentEntity) 
                {
                    // Push style for highlight text color
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 0.0f, 1.0f)); // Yellow text for the selected entity

                    nodeFlags |= ImGuiTreeNodeFlags_Selected; // Additionally, mark the node as selected if desired
                }

                /// Get a Node Name for the entity
                std::string id = currentEntity->GetName();

                bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)i, nodeFlags, id.c_str());

                // If this entity was the selected one, pop the highlight style
                if (SelectedEntity == currentEntity) 
                {
                    ImGui::PopStyleColor(1); // Ensure to only pop once to revert the text color change
                }

                // Context menu for the entity
                if (ImGui::BeginPopupContextItem(popup_id.c_str()))
                {
                    if (ImGui::MenuItem("Copy"))
                    {
                        Stream::CopyToClipboard(currentEntity);
                    }

                    if (ImGui::MenuItem("Paste"))
                    {
                        currentEntity->Exit();
                        Stream::PasteFromClipboard(currentEntity);
                        currentEntity->Init();
                    }

                    if (ImGui::MenuItem("Delete"))
                    {
                        showDeletePopup = true;
                        selectedEntityToDelete = currentEntity;
                    }

                    ImGui::EndPopup();
                }

                // Display the delete popup if the flag is set and the current entity is the selected entity
                if (showDeletePopup && selectedEntityToDelete == currentEntity)
                {
                    ImGui::OpenPopup(delete_id.c_str(), ImGuiPopupFlags_NoOpenOverExistingPopup);

                    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
                    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

                    if (ImGui::BeginPopupModal(delete_id.c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize)) // Create a modal window
                    {
                        std::string sentence = "Are you sure you want to delete " + currentEntity->GetName() + "?";
                        ImGui::Text(sentence.c_str());

                        ImGui::Separator(); // Add a separator

                        if (ImGui::Button("OK", ImVec2(120, 0)))
                        {
                            currentEntity->Destroy();

                            if (SelectedEntity == currentEntity)
                            {
                                SelectedEntity = nullptr;
                            }
                            showDeletePopup = false;
                            selectedEntityToDelete = nullptr;
                        }

                        ImGui::SetItemDefaultFocus(); // Set the focus on the OK button
                        ImGui::SameLine(); // Align the cancel button to the right

                        if (ImGui::Button("Cancel", ImVec2(120, 0)))
                        {
                            ImGui::CloseCurrentPopup();
                            showDeletePopup = false;
                        }
                        ImGui::EndPopup();
                    }
                }

                // Drag Source
                if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) 
                {
                    // Set payload to be the entity's ID or reference
                    ImGui::SetDragDropPayload("ENTITY_PAYLOAD", &currentEntity, sizeof(Entity*));
                    ImGui::Text("Moving %s", currentEntity->GetName().c_str());
                    ImGui::EndDragDropSource();
                }

                // Drop Target
                if (ImGui::BeginDragDropTarget()) 
                {
                    const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ENTITY_PAYLOAD");

                    if (payload != NULL) 
                    {
                         Entity* droppedEntity = *(Entity**)payload->Data;

                         // Set the parent of the dropped entity to the current entity
                        droppedEntity->SetParent(currentEntity);
                       
                    }

                    ImGui::EndDragDropTarget();
                }

                if (ImGui::IsItemClicked())
                {
                    SelectedEntity = currentEntity;
                }

                // Only attempt to display children if the entity is not a leaf
                if (!isLeaf && node_open)
                {
                    displayEntityRecursive(currentEntity, true);
                    ImGui::TreePop(); // Only pop if we actually pushed the tree node (non-leaf)
                }
            }

            
        };

        if (ImGui::BeginDragDropTargetCustom(ImGui::GetCurrentWindow()->Rect(), ImGui::GetID("ENTITY_PAYLOAD")))
        {
			const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ENTITY_PAYLOAD");

            if (payload != NULL)
            {
				Entity* droppedEntity = *(Entity**)payload->Data;

				droppedEntity->SetParent(nullptr);

			}

			ImGui::EndDragDropTarget();
		}
        
        displayEntityRecursive(nullptr,false); // Start with null to display root entities

        if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(0))
        {
            SelectedEntity = nullptr;
        }

        ImGui::End(); // End the window
    }




    /// @brief Shows the properties of an Entity in the Contents column of the Entity List
    /// @param entity - the entity to display the properties of 
    void EntitySystem::entityPropertiesWindow(Entity* entity)
    {
        ImGui::Begin("Inspector", NULL, ImGuiWindowFlags_AlwaysAutoResize); ///< Start The Window
        if (entity == nullptr) /// If there is no entity selected
		{
			ImGui::Text("No Entity Selected"); /// Display that no entity is selected
			ImGui::End(); /// End the window
			return;
		}
        entity->Inspect(); /// Display the properties of the entity

        ImGui::End(); /// End the window
    }

    
//-----------------------------------------------------------------------------
// public: reading / writing
//-----------------------------------------------------------------------------


    /// @brief  loads all of the m_Entities in a scene
    /// @param  entityData  the json object containing the entity data
    void EntitySystem::LoadEntities( nlohmann::ordered_json const& data )
    {
        for ( auto& [ name, entityData ] : data.items() )
        {
            Stream::PushDebugLocation( name + "." );

            Entity* entity = new Entity();
            Stream::Read( entity, entityData );
            m_Entities.push_back( entity );
            addLoadedChildren( entity );

            Stream::PopDebugLocation();
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
            // skip entities that have a parent - they'll be saved separately by their parent
            if ( entity->GetParent() != nullptr )
            {
                continue;
            }

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
