///*****************************************************************/
/// @file	 BehaviorSystem.cpp
/// @Author  Jax Clayton (jax.clayton@digipen.edu)
/// @date	 9/15/2021
/// @brief   BehaviorSystem  
/// @details This class handles all behaviors for entities
///*****************************************************************/
#define BEHAVIORSYSTEM_C

#ifndef BEHAVIORSYSTEM_H
#include "BehaviorSystem.h"
#endif

#include "Entity.h"
#include "Collider.h"
#include "Behavior.h"
#include "DebugSystem.h"

///---------------------------------------------------------------------------
/// Static 
///---------------------------------------------------------------------------

template<typename BehaviorType>
bool BehaviorSystem<BehaviorType>::s_ShowBehaviorSystemList = false;


template<typename BehaviorType>
void BehaviorSystem<BehaviorType>::OnFixedUpdate()
{
	for (auto behavior : m_BehaviorList)
	{
		behavior->OnFixedUpdate();
	}
}

template<typename BehaviorType>
void BehaviorSystem<BehaviorType>::OnUpdate(float dt)
{
	for (auto behavior : m_BehaviorList)
	{
		behavior->OnUpdate(dt);
	}
}

template<typename BehaviorType>
void BehaviorSystem<BehaviorType>::OnSceneExit()
{
    m_BehaviorList.clear();
}


template<typename BehaviorType>
void BehaviorSystem<BehaviorType>::AddBehavior(BehaviorType* behavior)
{
	m_BehaviorList.push_back(behavior);
}

template<typename BehaviorType>
void BehaviorSystem<BehaviorType>::RemoveBehavior(BehaviorType* behavior)
{
	m_BehaviorList.erase(
        std::remove(m_BehaviorList.begin(), m_BehaviorList.end(), behavior), m_BehaviorList.end()
    );
}

template<typename BehaviorType>
std::vector<BehaviorType*>& BehaviorSystem<BehaviorType>::GetBehaviors() const
{
	return &m_BehaviorList;
}

template<typename BehaviorType>
void BehaviorSystem<BehaviorType>::DebugWindow()
{
    std::string behaviorName(typeid(BehaviorType).name() + 5); // Move over the "class" part of the name

    char buttonLabel[128];
    snprintf(buttonLabel, sizeof(buttonLabel), s_ShowBehaviorSystemList ?
        "Hide%s List" : "Show%s List", behaviorName.c_str());

    if (ImGui::Button(buttonLabel))
        s_ShowBehaviorSystemList = !s_ShowBehaviorSystemList;

    if (s_ShowBehaviorSystemList)
    {
        for (auto behavior : m_BehaviorList)
        {
            std::string parentName = behavior->GetParent()->GetName();
            int nodeId = (int)behavior->GetId();
            std::string label = parentName + "'s Behavior";

            if (ImGui::TreeNode(std::string(std::to_string(nodeId)).c_str(), label.c_str()))
            {
                behavior->Inspector();
                ImGui::TreePop();
            }
        }
    }

}

//-----------------------------------------------------------------------------
// singleton stuff
//-----------------------------------------------------------------------------

    // Define the GetInstance function for the templated BehaviorSystem
    template < typename BehaviorType >
    BehaviorSystem< BehaviorType >* BehaviorSystem< BehaviorType >::GetInstance()
    {
        if (s_Instance == nullptr)
        {
            s_Instance = new BehaviorSystem< BehaviorType >();
        }
        return s_Instance;
    }

    // Define the static instance variable for the templated BehaviorSystem
    template < typename BehaviorType >
    BehaviorSystem< BehaviorType >* BehaviorSystem< BehaviorType >::s_Instance = nullptr;

    /// @brief  Constructs the BehaviorSystem
    template < typename BehaviorType >
    BehaviorSystem< BehaviorType >::BehaviorSystem() :
        System( std::string( "BehaviorSystem<" ) + (typeid( BehaviorType ).name() + 6) + ">" )
    {}


//-----------------------------------------------------------------------------
