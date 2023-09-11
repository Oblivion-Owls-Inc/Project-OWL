/// @file     Mesh.h
/// @author   Eli Tsereteli (ilya.tsereteli@digipen.edu)
/// 
/// @brief    Mesh class - holds on to buffer/VAO references, draws their contents.
#pragma once
#include <vector>
#include "glm/glm.hpp"  // Vertex struct


class Mesh
{
public:
    /// @brief                 Vertex struct - defines data that will be loaded into buffer.
    struct Vertex
    {
        glm::vec2 pos;
        glm::vec2 UV;
    };

    /// @brief                 Initializes mesh to square, or to nothing (can be used for static variables).
    ///                        Provide rows and columns if this is for a spritesheet.
    Mesh(bool init_square = false, int rows = 1, int columns = 1);

    /// @brief                 Constructor: inits mesh with given vertices. Note: draw mode is trienglestrip. 
    ///                                                                      (TODO: allow choosing drawmode?)
    /// @param vertices        Vector of vertices to initialize this mesh with
    Mesh(std::vector<Vertex> vertices);

    /// @brief                 Destructor: cleans up memory
    ~Mesh();

    /// @brief                 Loads provided vertices to initialize (or change) the mesh.
    ///                        Provide rows and columns if this is for a spritesheet.
    /// @param vertices        vector of vertices to load
    void load_vertices(std::vector<Vertex> vertices);

    /// @brief                 Loads a list of vertices that make a centered unit square.
    ///                        Provide rows and columns if this is for a spritesheet.
    void load_square(int rows = 1, int columns = 1);

    /// @brief                 Draws the mesh.  Make sure proper shader is selected prior to calling this.
    void draw();

    /// @brief                 Returns the UV size.
    /// @return                UV size (x = width, y = height)
    glm::vec2 get_uvSize();

private:
    unsigned int _vaoID, _bufferID;  /// @brief        Vertex Array Object and Array Buffer references
    unsigned int _vertexCount;       /// @brief        How many vertices to draw
    glm::vec2 _uvSize;               /// @brief        UV width + height

    /// @brief      Initializes the Vertex Array Object and the buffer tied to this mesh.
    void initVAO();
};
