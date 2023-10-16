/// @file Component.cpp
/// @author Tyler Birdsall (tyler.birdsall@digipen.edu)
/// @brief Virtual component header
/// @version 0.1
/// @date 2023-09-05
/// 
/// @copyright Copyright (c) 2023 Digipen Institute of Technology

#include "Component.h"
#include "basics.h"

//-----------------------------------------------------------------------------
// protected: constructors
//-----------------------------------------------------------------------------

    /// @brief Default constructor for Component.
    /// @param type_ type of Component being created.
    Component::Component( std::type_index type ) :
        m_Type( type ),
        m_Parent( nullptr ),
        m_Id( GetUniqueId() )
    {}

    /// @brief Copy-constructor for Component base class
    /// @param other the other Component to copy
    Component::Component( Component const& other ) :
        m_Type( other.m_Type ),
        m_Parent( nullptr ),
        m_Id( GetUniqueId() )
    {}

//-----------------------------------------------------------------------------
