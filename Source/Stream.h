/**
 * @file   Stream.h
 * @author Aidan Straker (aidan.straker@digipen.edu)
 * @brief  JSON File reading/writing
 * @version 0.1
 * @date 2023-09-10
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "document.h"  // rapidjson::Document, Parse, HasParseError, GetParseError
#include <string>      // std::string
#include <fstream>     // std::ifstream
#include "glm/glm.hpp" // glm::vec3

//------------------------------------------------------------------------------

/// @brief Open a json file and parse a rapidjson document.
/// @param filePath - name of the json file.
/// @return rapidjson document
rapidjson::Document StreamOpen( const std::string& name );

/// @brief Read the data for a vector from a document.
/// @param doc - the document to read from.
void StreamReadVector( const rapidjson::Document& doc, const std::string& property, glm::vec3* vector );



