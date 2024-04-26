////////////////////////////////////////////////////////////////////////////////
/// @file       pch.h
/// @author     Jax Clayton (jax.clayton@digipen.edu)
/// @brief      This is a precompiled header file.
/// @version    0.1
/// @date       3/6/2024
/// 
/// @copyright  Copyright (c) 2024 Digipen Institute of Technology
////////////////////////////////////////////////////////////////////////////////
#pragma once

////////////////////////////////////////////////////////////////////////////////
/// STL
////////////////////////////////////////////////////////////////////////////////
#include <functional>  // std::function, std::bind 
#include <iostream>    // std::cout, std::endl
#include <cassert>
#include <cmath>
#include <cstdlib>
#include <atomic>
#include <typeindex>
#include <random>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <climits>
#include <chrono>
#include <iterator>
#include <filesystem>
#include <iterator>
#include <deque>
#include <thread>  
#include <atomic>  

////////////////////////////////////////////////////////////////////////////////
/// ImGui / ImPlot
////////////////////////////////////////////////////////////////////////////////
#include <imgui.h>    // ImGui Functions
#include "implot.h"
#include <imgui_stdlib.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
#include <imgui_internal.h>


////////////////////////////////////////////////////////////////////////////////
/// GLFW / GLEW / GLM
////////////////////////////////////////////////////////////////////////////////
#include <glew.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/vec2.hpp>
#include <glm/gtc/random.hpp>

////////////////////////////////////////////////////////////////////////////////
/// Nlohmann JSON
////////////////////////////////////////////////////////////////////////////////

#include <nlohmann/json.hpp>


////////////////////////////////////////////////////////////////////////////////
/// Steam API
////////////////////////////////////////////////////////////////////////////////

#include <SteamAPI/steam_api.h>

////////////////////////////////////////////////////////////////////////////////
/// Project OWL 
////////////////////////////////////////////////////////////////////////////////

#include "ISerializable.h"
#include "Stream.h"
#include "basics.h"

#include "Component.h"  // These files are included in a specific order to avoid circular dependencies
#include "ComponentReference.h"

#include "Entity.h"
#include "EntityReference.h"

#include "AssetReference.h"
#include "AssetLibrarySystem.h"


#include "Mesh.h"
#include "Texture.h"
#include "Transform.h"
#include "Sprite.h"
#include "StaticBody.h"
#include "Behavior.h"
#include "Collider.h"
#include "RigidBody.h"
#include "Tilemap.h"
#include "Health.h"
