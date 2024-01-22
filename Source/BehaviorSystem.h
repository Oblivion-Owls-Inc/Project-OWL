/// @file       BehaviorSystem.h
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      System that keeps track of and updates a specific kind of Behavior
/// @version    0.1
/// @date       2024-01-18
/// 
/// @copyright  Copyright (c) 2023 Digipen Institute of Technology

#pragma once

#include "ComponentSystem.h"
#include "Behavior.h"

/// @brief  System that keeps track of and updates a specific kind of Behavior
/// @tparam BehaviorType    the type of Behavior this BehaviorSystem keeps track of
template < class BehaviorType >
class BehaviorSystem : public ComponentSystem< BehaviorType >
{

//-----------------------------------------------------------------------------
private: // virtual override methods
//-----------------------------------------------------------------------------
    

    /// @brief  updates each Behavior every graphics frame
    /// @param  dt  the length in seconds the frame lasts
    virtual void OnUpdate( float dt ) override
    {
        for ( size_t i = 0, count = ComponentSystem< BehaviorType >::GetComponents().size(); i < count; ++i )
        {
            static_cast< Behavior* >( ComponentSystem< BehaviorType >::GetComponents()[ i ] )->OnUpdate( dt );
        }
    }

    /// @brief  updates each Behavior every simulation frame
    virtual void OnFixedUpdate() override
    {
        for ( size_t i = 0, count = ComponentSystem< BehaviorType >::GetComponents().size(); i < count; ++i )
        {
            static_cast< Behavior* >( ComponentSystem< BehaviorType >::GetComponents()[ i ] )->OnFixedUpdate();
        }
    }


//-----------------------------------------------------------------------------
private: // singleton stuff
//-----------------------------------------------------------------------------


    /// @brief  Constructs the BehaviorSystem
    BehaviorSystem() :
        ComponentSystem< BehaviorType >( "BehaviorSystem<" + PrefixlessName( typeid( BehaviorType ) ) + ">" )
    {}


//-----------------------------------------------------------------------------
public: // singleton stuff
//-----------------------------------------------------------------------------


    /// @brief  gets the instance of BehaviorSystem
    /// @return the instance of the BehaviorSystem
    static BehaviorSystem< BehaviorType >* GetInstance()
    {
        static BehaviorSystem< BehaviorType >* instance = nullptr;

        if ( instance == nullptr )
        {
            instance = new BehaviorSystem();
        }

        return instance;
    }

    // Prevent copying
    BehaviorSystem( BehaviorSystem const& ) = delete;
    void operator =( BehaviorSystem const& ) = delete;


//-----------------------------------------------------------------------------
};


/// @brief  shorthand method for getting a BehaviorSystem instance
/// @tparam BehaviorType    the BehaviorType of the BehaviorSystem to get
/// @return the BehaviorSystem instance
template< class BehaviorType >
__inline BehaviorSystem< BehaviorType >* Behaviors()
{
    return BehaviorSystem< BehaviorType >::GetInstance();
}
