/// @file     Mesh.cpp
/// @author   Eli Tsereteli (ilya.tsereteli@digipen.edu)
/// 
/// @brief    Implementation of Mesh class.
#include "Mesh.h"
#include "glew.h"
#include "RenderSystem.h"

/// @brief              Constructor: loads unit square vertices if needed.
/// @param init_square  true/false: should unit square be initialized
Mesh::Mesh(bool init_square, int columns, int rows)
{
    if (!init_square)
        return;

    LoadSquare(rows, columns);
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


/// @brief      Loads a list of vertices that make a centered unit square. (rows/columns for spritesheet)
void Mesh::LoadSquare(int rows, int columns)
{
    float usize = 1.0f / columns;
    float vsize = 1.0f / rows;
    LoadVertices({ {{-0.5,  0.5}, {0.0,   0.0}},
                    {{ 0.5,  0.5}, {usize, 0.0}},
                    {{-0.5, -0.5}, {0.0,   vsize}},
                    {{ 0.5, -0.5}, {usize, vsize}} });   // unit square

    m_UVsize = { usize, vsize };
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
