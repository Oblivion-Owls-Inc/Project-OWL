#include "Component.h"


Component::Component( std::type_index type_ ) :
    type( type_ ),
    parent( nullptr )
{}