/// @file       ComponentSystem.h
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      System that keeps track of a specific kind of Component
/// @version    0.1
/// @date       2024-01-18
/// 
/// @copyright  Copyright (c) 2024 Digipen Institute of Technology

#pragma once


#include "pch.h" 
#include "System.h"
#include "Component.h"
#include "Entity.h"

#include "Inspection.h"


/// @brief  System that keeps track of a specific kind of Component
/// @tparam ComponentType   the type of Component this ComponentSystem keeps track of
template < class ComponentType >
class ComponentSystem : public System
{
//-----------------------------------------------------------------------------
public: // accessors
//-----------------------------------------------------------------------------


    /// @brief  gets the array of components in this ComponentSystem
    /// @return the array of Components in this ComponentSystem
    std::vector< ComponentType* > const& GetComponents() const
    {
        return m_Components;
    }


    /// @brief  adds a component to the ComponentSystem
    /// @param  component   the component to add
    void AddComponent( ComponentType* component )
    {
        m_Components.push_back( component );
    }

    /// @brief  removes a component to the ComponentSystem
    /// @param  component   the component to remove
    void RemoveComponent( ComponentType* component )
    {
        m_Components.erase( std::find( m_Components.begin(), m_Components.end(), component ) );
    }

    
//-----------------------------------------------------------------------------
private: // inspection
//-----------------------------------------------------------------------------


    /// @brief  shows this System's debug window
    virtual void DebugWindow() override
    {
        bool windowOpen = GetDebugEnabled();
        std::string componentName = PrefixlessName( typeid( ComponentType ) ); // Move over the "class" part of the name

        if ( ImGui::Begin( componentName.c_str(), &windowOpen ) )
        {
            for ( ComponentType* component : m_Components )
            {
                ImGui::PushID( component->GetId() );
                std::string label = component->GetEntity()->GetName() + "'s " + componentName;

                if ( ImGui::TreeNode( label.c_str() ) )
                {
                    static_cast< Component* >( component )->Inspector();
                    ImGui::TreePop();
                }

                ImGui::PopID();
            }
        }
        ImGui::End();

        SetDebugEnable( windowOpen );
    }


//-----------------------------------------------------------------------------
private: // members
//-----------------------------------------------------------------------------

    
    /// @brief  the components this System is keeping track of
    std::vector< ComponentType* > m_Components = {};


//-----------------------------------------------------------------------------
private: // reading
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
protected: // constructor
//-----------------------------------------------------------------------------
    

    /// @brief  Constructs the ComponentSystem
    ComponentSystem( std::string const& name ) :
        System( name )
    {}


//-----------------------------------------------------------------------------
private: // singleton stuff
//-----------------------------------------------------------------------------


    /// @brief  Constructs the ComponentSystem
    ComponentSystem() :
        System( "ComponentSystem<" + PrefixlessName( typeid( ComponentType ) ) + ">" )
    {}


//-----------------------------------------------------------------------------
public: // singleton stuff
//-----------------------------------------------------------------------------


    /// @brief  gets the instance of ComponentSystem
    /// @return the instance of the ComponentSystem
    static ComponentSystem< ComponentType >* GetInstance()
    {
        static std::unique_ptr< ComponentSystem< ComponentType > > s_Instance = nullptr;

        if (s_Instance == nullptr )
        {
            s_Instance.reset( new ComponentSystem< ComponentType >() );
        }

        return s_Instance.get();
    }

    // Prevent copying
    ComponentSystem( ComponentSystem const& ) = delete;
    void operator =( ComponentSystem const& ) = delete;


//-----------------------------------------------------------------------------
};


/// @brief  shorthand method for getting a ComponentSystem instance
/// @tparam ComponentType   the ComponentType of the ComponentSystem to get
/// @return the ComponentSystem instance
template< class ComponentType >
__inline ComponentSystem< ComponentType >* Components()
{
    return ComponentSystem< ComponentType >::GetInstance();
}
