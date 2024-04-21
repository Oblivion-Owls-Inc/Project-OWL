/// \file     Mesh.cpp
/// \author   Eli Tsereteli (ilya.tsereteli@digipen.edu)
/// 
/// \brief    Implementation of Mesh class.
/// 
/// \copyright Copyright (c) 2024 Digipen Institute of Technology


#include "pch.h" // precompiled header has to be included first
#include "Mesh.h"
#include "RenderSystem.h"

/// @brief  Creates mesh as a quad.
/// @param  scale           (optional) the size of the mesh
/// @param  sheetDimensions (optional) the dimensions of the spriteSheet
/// @param  pivot           (optional) the pivot point of the mesh
Mesh::Mesh( glm::vec2 scale, glm::ivec2 sheetDimensions, glm::vec2 pivot )
{
    LoadQuad( scale, sheetDimensions, pivot );
}

/// @brief              Constructor: inits data to 0, loads provided vertices to make a mesh.
/// @param vertices     Vector of vertices to initialize this mesh with
Mesh::Mesh(std::vector<Vertex> vertices) {  LoadVertices(vertices); }


/// @brief              Cleans up memory
Mesh::~Mesh()
{
    glDeleteBuffers(1, &m_Buffer);
    glDeleteVertexArrays(1, &m_VAO);
}


/// @brief              Loads vertices into the buffer.
/// @param vertices     Vector of vertices to load.
void Mesh::LoadVertices(std::vector<Vertex> vertices)
{
    if (!m_VAO)
        initVAO();

    // Load the vertices into buffer, store their amount
    glBindBuffer(GL_ARRAY_BUFFER, m_Buffer);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
    m_VertexCount = (unsigned int)vertices.size();
}


/// @brief  Initializes mesh as a quad.
/// @param  scale           (optional) the size of the mesh
/// @param  sheetDimensions (optional) the dimensions of the spriteSheet
/// @param  pivot           (optional) the pivot point of the mesh
void Mesh::LoadQuad( glm::vec2 scale, glm::ivec2 sheetDimensions, glm::vec2 pivot )
{
    m_UVsize = glm::vec2( 1, 1 ) / (glm::vec2)sheetDimensions;
    pivot *= scale;

    m_Bounds[ 0 ] = glm::vec2( 0      , 0       ) - pivot;
    m_Bounds[ 1 ] = glm::vec2( scale.x, scale.y ) - pivot;

    LoadVertices( {
        { glm::vec2( m_Bounds[ 0 ].x, m_Bounds[ 1 ].y ), glm::vec2( 0         , 0          ) },
        { glm::vec2( m_Bounds[ 1 ].x, m_Bounds[ 1 ].y ), glm::vec2( m_UVsize.x, 0          ) },
        { glm::vec2( m_Bounds[ 0 ].x, m_Bounds[ 0 ].y ), glm::vec2( 0         , m_UVsize.y ) },
        { glm::vec2( m_Bounds[ 1 ].x, m_Bounds[ 0 ].y ), glm::vec2( m_UVsize.x, m_UVsize.y ) }
    } );
}


/// @brief  gets the scale of this mesh
/// @return the scale of this mesh
glm::vec2 Mesh::GetScale() const
{
    return m_Bounds[ 1 ] - m_Bounds[ 0 ];
}


// helper
/// @brief      Initializes the Vertex Array Object and the buffer tied to this mesh.
void Mesh::initVAO()
{
    // Vertex array object (keeps track of attributes)
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    // Vertex buffer
    glGenBuffers(1, &m_Buffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_Buffer);

    // Define the layout. Just 2 attributes.
    // index 0: position (2 floats)
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
    // index 1: UV (2 floats)
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, UV));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    // No index buffer, I'll just use trianglestrip.

    glBindVertexArray(0);
}
