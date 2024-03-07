/// ////////////////////////////////////////////////////////////////////
/// 
/// @file AssetLibarySystem.cpp
/// @author Jax Clayton (jax.clayton/@digipen.edu)
/// @brief Implementation file for Specialized AssetLibrarySystem Functions
/// @version 0.1
/// @date 2023-02-26
/// @copyright Copyright (c) 2023
/// ///////////////////////////////////////////////////////////////////


#include "pch.h" // precompiled header has to be included first

#include "AssetLibrarySystem.h"
#include "EntitySystem.h"
#include "Entity.h"


/// @brief Specialized DebugWindow for AssetLibrarySystem of type Entity
template<>
void AssetLibrarySystem<Entity>::DebugWindow()
{
    s_ShowAssetLibraryList = GetDebugEnabled(); 

    std::vector<Entity*> entities;

    for (auto& key : m_Assets)
    {
        entities.push_back(key.second);
    }

    Entities()->DisplayEntityHierarchy(entities, "Prefab Library",false);

    /// Update the AssetLibarySystem Map
    
    m_Assets.clear();

    for (auto& entity : entities)
    {
        if (entity->IsInScene())
            entity->Exit();

        m_Assets[entity->GetName()] = entity;
    }

    SetDebugEnable(s_ShowAssetLibraryList);
}


/// @brief Specialized DebugCreateAssetWindow for AssetLibrarySystem of type Entity
template<>
bool AssetLibrarySystem<Entity>::DebugCreateAssetWindow()
{
    bool show = true;

    std::string assetName("Entity"); 

    std::string windowName = "Create New " + assetName;

    ImGui::Begin(windowName.c_str(), &show, ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::SetNextWindowSize(ImVec2(500, 100), ImGuiCond_FirstUseEver);
    // Input text for asset name
    static char buffer[128] = ""; // Buffer to hold the input, you can save this
    ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.45f);
    ImGui::InputText("##Asset Name", buffer, IM_ARRAYSIZE(buffer));

    // add asset button
    ImGui::SameLine();
    if (ImGui::Button("Add Asset", ImVec2(100, 0)))
    {
        if (buffer[0] == '\0')
        {
            Debug() << "Warning: Asset must have a name" << std::endl;
        }
        else
        {
            Debug() << "Log: Creating new " << assetName << " with name: " << buffer << std::endl;

            Entity* entity = new Entity();
            entity->SetName(buffer);

            AddAsset(buffer, entity);
            ImGui::End();
            return false; //close window
        }
    }
    ImGui::SameLine();

    if (ImGui::Button("Cancel", ImVec2(100, 0)))
    {
        ImGui::End();
        return false; //close window
    }

    if (!show)
    {
        ImGui::End();
        return false; //close window
    }

    ImGui::End();
    return true; // keep window open
}
