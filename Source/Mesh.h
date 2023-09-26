/// @file     Mesh.h
/// @author   Eli Tsereteli (ilya.tsereteli@digipen.edu)
/// 
/// @brief    Mesh class - holds on to buffer/VAO references, draws their contents.
#pragma once
#include <vector>       // for LoadVertices()
#include "glm/glm.hpp"  // for Vertex struct and UV size


/// @brief       Stores data needed to render a basic mesh.
class Mesh
{
public:
    /// @brief   Vertex struct - defines data that will be loaded into buffer.
    struct Vertex
    {
        glm::vec2 pos;  /// @brief  Vertex coordinate
        glm::vec2 UV;   /// @brief  Texture coordinate of this vertex
    };


    /// @brief              Initializes mesh to square, or to nothing.
    /// @param init_square  (optional) init square or no?
    /// @param rows         (optional) spritesheet rows
    /// @param columns      (optional) spritesheet columns
    Mesh(bool init_square = false, int rows = 1, int columns = 1);


    /// @brief              Constructor: inits mesh with given vertices.
    /// @param vertices     Vector of vertices to initialize this mesh with
    Mesh(std::vector<Vertex> vertices);

    /// @brief              Destructor: cleans up memory
    virtual ~Mesh();

    /// @brief              Loads provided vertices to initialize (or change) the mesh.
    ///                     Provide rows and columns if this is for a spritesheet.
    /// @param vertices     vector of vertices to load
    void LoadVertices(std::vector<Vertex> vertices);

    /// @brief              Loads a list of vertices that make a centered unit square.
    ///                     Provide rows and columns if this is for a spritesheet.
    void LoadSquare(int rows = 1, int columns = 1);

    /// @brief               Returns the UV size.
    /// @return              UV size (x = width, y = height)
    glm::vec2 GetUVsize();

    /// @brief               Returns the VAO index of this mesh. It can be used for rendering the mesh, or
    ///                      defining additional attributes.
    /// @return              UV size (x = width, y = height)
    unsigned int GetVAO();

    /// @brief               Returns the amount of vertices in this mesh.
    /// @return              vertex count
    unsigned int GetVertCount();



    //-------------------------------------------------------------------------
    //          data
    //-------------------------------------------------------------------------
private:
    unsigned int m_VAO = 0;         /// @brief      Vertex Array Object ID
    unsigned int m_Buffer = 0;      /// @brief      Array Buffer ID
    unsigned int m_VertexCount = 0; /// @brief      How many vertices to draw
    glm::vec2 m_UVsize ={1,1};      /// @brief      UV width + height



    // helper
    /// @brief   Initializes the Vertex Array Object and the buffer of this mesh.
    void initVAO();
};
