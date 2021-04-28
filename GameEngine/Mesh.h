#include "glm.hpp"
#include <vector>

class Mesh
{

public:

	struct Vertex
	{
		glm::vec4 position;
		glm::vec4 normal;
		glm::vec2 uv;
	};

	std::vector<glm::vec3> verticies;
	std::vector<unsigned int> index;

	Mesh();
	virtual ~Mesh();

	void InitialiseQuad();

	virtual void Draw();

protected:

	unsigned int triCount;
	unsigned int VAO;
	unsigned int VBO;
	unsigned int IBO;

	unsigned int texture;
	unsigned int normal;
	unsigned int specular;

	int x, y, n;
};