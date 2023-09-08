/**
* @file ExampleSystem.cpp
* @author Steve Bukowinski (steve.bukowinski@digipen.edu)
* @brief Example System meant to be copy-pasted when creating new Systems
* @version 0.1
* @date 2023-09-05
* 
* @copyright Copyright (c) 2023
* 
*/

#include "ExampleSystem.h"


/**
* @brief Constructs the ExampleSystem
*/
ExampleSystem::ExampleSystem() {}

// v v v YOUR CODE HERE v v v




// ^ ^ ^ YOUR CODE HERE ^ ^ ^

/// @brief The singleton instance of ExampleSystem
ExampleSystem * ExampleSystem::instance = nullptr;

/**
* @brief gets the instance of ExampleSystem
* @return the instance of the ExampleSystem
*/
ExampleSystem * ExampleSystem::getInstance()
{
    if ( instance == nullptr )
    {
        instance = new ExampleSystem();
    }
    return instance;
}

// MAY INCLUDE A DUPLICATE OF THE ABOVE FUNCTION WITH CONSTRUCTOR ARGUMENTS HERE
