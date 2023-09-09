#pragma once
#include <vector>
#include "glm/glm.hpp"

class Mesh
{
public:
	struct Vertex
	{
		glm::vec2 pos;
		glm::vec2 UV;
	};

	// Default square (or empty)
	Mesh(bool init_square = false);

	// Custom mesh (NOTE: it draws in trianglestrip mode)
	Mesh(std::vector<Vertex> vertices);
	~Mesh();

	void load_vertices(std::vector<Vertex> vertices);
	void load_square();
	void draw();


private:
	unsigned int _vaoID, _bufferID;
	unsigned int _vertexCount;

	void initVAO();
};
