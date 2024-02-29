/// ////////////////////////////////////////////////////////////////////
/// 
/// @file AssetLibarySystem.cpp
/// @author Jax Clayton (jax.clayton/@digipen.edu)
/// @brief Implementation file for Specialized AssetLibrarySystem Functions
/// @version 0.1
/// @date 2023-02-26
/// @copyright Copyright (c) 2023
/// ///////////////////////////////////////////////////////////////////


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
    
    for (auto& entity : entities)
    {
        m_Assets[entity->GetName()] = entity;
    }

    SetDebugEnable(s_ShowAssetLibraryList);
}