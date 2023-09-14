/******************************************************************************
filename    Component.cpp
author      Tyler Birdsall
DP email    tyler.birdsall@digipen.edu
course      GAM200
section     A
Project		Dig Deeper

Brief Description:
  Function declerations for the componnt class.

******************************************************************************/

#include "Component.h"

/// @brief Default constructor for Component.
/// @param type_ type of Component being created.
Component::Component( std::type_index _type ) :
    type( _type ),
    parent( nullptr )
{}

/// @brief Copy-constructor for Component base class
/// @param other the other Component to copy
Component::Component( Component const& other ) :
    type( other.type ),
    parent( nullptr )
{}