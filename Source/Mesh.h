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


    /// @brief  Creates mesh as a quad.
    /// @param  scale           (optional) the size of the mesh
    /// @param  sheetDimensions (optional) the dimensions of the spriteSheet
    /// @param  pivot           (optional) the pivot point of the mesh
    Mesh( glm::vec2 scale = { 1.0f, 1.0f }, glm::ivec2 sheetDimensions = { 1, 1 }, glm::vec2 pivot = { 0.5f, 0.5f } );


    /// @brief              Constructor: inits mesh with given vertices.
    /// @param vertices     Vector of vertices to initialize this mesh with
    Mesh( std::vector<Vertex> vertices );

    /// @brief              Destructor: cleans up memory
    virtual ~Mesh();

    /// @brief              Loads provided vertices to initialize (or change) the mesh.
    ///                     Provide rows and columns if this is for a spritesheet.
    /// @param vertices     vector of vertices to load
    void LoadVertices(std::vector<Vertex> vertices);

    /// @brief  Initializes mesh as a quad.
    /// @param  scale           (optional) the size of the mesh
    /// @param  sheetDimensions (optional) the dimensions of the spriteSheet
    /// @param  pivot           (optional) the pivot point of the mesh
    void LoadQuad( glm::vec2 scale = { 1.0f, 1.0f }, glm::ivec2 sheetDimensions = { 1, 1 }, glm::vec2 pivot = { 0.5f, 0.5f } );



    /// @brief               Returns the UV size.
    /// @return              UV size (x = width, y = height)
    __inline glm::vec2 GetUVsize() const { return m_UVsize; }

    /// @brief               gets this mesh's AABB
    /// @return              this mesh's AABB (an array of 2 glm::vec2s)
    __inline glm::vec2 const* GetBounds() const { return m_Bounds; }

    /// @brief               Returns the VAO index of this mesh. It can be used for rendering the mesh, or
    ///                      defining additional attributes.
    /// @return              UV size (x = width, y = height)
    __inline unsigned int GetVAO() const { return m_VAO; }

    /// @brief               Returns the amount of vertices in this mesh.
    /// @return              vertex count
    __inline unsigned int GetVertexCount() const { return m_VertexCount; }

    /// @return              Id of the vertex buffer
    __inline unsigned int GetBuffer() const { return m_Buffer; }




    //-------------------------------------------------------------------------
    //          data
    //-------------------------------------------------------------------------
private:
    unsigned int m_VAO = 0;         /// @brief      Vertex Array Object ID
    unsigned int m_Buffer = 0;      /// @brief      Array Buffer ID
    unsigned int m_VertexCount = 0; /// @brief      How many vertices to draw
    glm::vec2 m_UVsize ={1,1};      /// @brief      UV width + height

    /// @brief  this meshes object-space AABB
    glm::vec2 m_Bounds[ 2 ] = { { 0.0f, 0.0f }, { 0.0f, 0.0f } };

    // helper
    /// @brief   Initializes the Vertex Array Object and the buffer of this mesh.
    void initVAO();
};
