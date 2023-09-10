#include "Mesh.h"
#include "glew.h"
#include "Renderer.h"

// Creates centered unit square
Mesh::Mesh(bool init_square) : _vaoID(0), _bufferID(0), _vertexCount(0)
{
    if (!init_square)
        return;

    load_square();
}

// Creates custom mesh out of passed vertices
Mesh::Mesh(std::vector<Vertex> vertices) {  load_vertices(vertices);  }


// Delete stuff
Mesh::~Mesh()
{
    glDeleteBuffers(1, &_bufferID);
    glDeleteVertexArrays(1, &_vaoID);
}


void Mesh::load_vertices(std::vector<Vertex> vertices)
{
    if (!_vaoID)
        initVAO();

    // Load the vertices into buffer, store their amount
    glBindBuffer(GL_ARRAY_BUFFER, _bufferID);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
    _vertexCount = (unsigned int)vertices.size();
}


void Mesh::load_square()
{
    load_vertices({ {{-0.5,  0.5}, {0.0, 0.0}},
                    {{ 0.5,  0.5}, {1.0, 0.0}},
                    {{-0.5, -0.5}, {0.0, 1.0}},
                    {{ 0.5, -0.5}, {1.0, 1.0}} });   // unit square
}


// Initialize VAO and buffer
void Mesh::initVAO()
{
    // Vertex array object (contains the rest)
    glGenVertexArrays(1, &_vaoID);
    glBindVertexArray(_vaoID);

    // Main buffer
    glGenBuffers(1, &_bufferID);
    glBindBuffer(GL_ARRAY_BUFFER, _bufferID);

    // Define the layout (attributes): vertex position + UV.   (do we need individual colors? nah...)
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);                              // index 0: position (2 floats)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, UV));    // index 2: UV (2 floats)
    glEnableVertexAttribArray(1);

    // No index buffer, I'll just use trianglestrip.
}


// Draws contents of the buffer/VAO using triangle strip mode.
// Make sure proper shader is already chosen when calling this function.
void Mesh::draw()
{
    glBindVertexArray(_vaoID);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, _vertexCount);
}
