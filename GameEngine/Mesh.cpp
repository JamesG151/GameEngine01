#include "gl_core_4_5.h"

#include "Mesh.h"

//#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"

Mesh::Mesh()
{
	triCount = 0;
	IBO = 0;
	VBO = 0;
	VAO = 0;

	//glGenTextures(1, &texture);
	//glGenTextures(1, &normal);
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &IBO);

	InitialiseQuad();
}

Mesh::~Mesh()
{
	//glDeleteTextures(1, &texture);
	//glDeleteTextures(1, &normal);
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &IBO);
}

void Mesh::InitialiseQuad()
{
	//Defining verticies to plant tris.
	Vertex verticies[24] =
	{
		//Front Face
		Vertex{ glm::vec4(-1.5f, -0.5f, 1.5f, 1), glm::vec4(0, 0, 1, 0), glm::vec2(0, (1.0f / 3.0f)) }, //BL
		Vertex{ glm::vec4(-1.5f, 0.5f, 1.5f, 1), glm::vec4(0, 0, 1, 0), glm::vec2(0, 0) },  //TL
		Vertex{ glm::vec4(1.5f, 0.5f, 1.5f, 1), glm::vec4(0, 0, 1, 0), glm::vec2((1.0f / 3.0f), 0) },   //TR
		Vertex{ glm::vec4(1.5f, -0.5f, 1.5f, 1), glm::vec4(0, 0, 1, 0), glm::vec2((1.0f / 3.0f), (1.0f / 3.0f)) },  //BR
		
		//Back Face
		Vertex{ glm::vec4(1.5f, -0.5f, -1.5f, 1), glm::vec4(0, 0, -1, 0), glm::vec2(0, (1.0f / 3.0f)) },   //BL
		Vertex{ glm::vec4(1.5f, 0.5f, -1.5f, 1), glm::vec4(0, 0, -1, 0), glm::vec2(0, 0) },    //TL
		Vertex{ glm::vec4(-1.5f, 0.5f, -1.5f, 1), glm::vec4(0, 0, -1, 0), glm::vec2((1.0f / 3.0f), 0) },   //TR
		Vertex{ glm::vec4(-1.5f, -0.5f, -1.5f, 1), glm::vec4(0, 0, -1, 0), glm::vec2((1.0f / 3.0f), (1.0f / 3.0f)) },  //BR
			    	
		//Left Face
		Vertex{ glm::vec4(-1.5f, -0.5f, -1.5f, 1), glm::vec4(-1, 0, 0, 0), glm::vec2(0, (1.0f / 3.0f)) },  //BL      
		Vertex{ glm::vec4(-1.5f, 0.5f, -1.5f, 1), glm::vec4(-1, 0, 0, 0), glm::vec2(0, 0) },   //TL
		Vertex{ glm::vec4(-1.5f, 0.5f, 1.5f, 1), glm::vec4(-1, 0, 0, 0), glm::vec2((1.0f / 3.0f), 0) },  //TR
		Vertex{ glm::vec4(-1.5f, -0.5f, 1.5f, 1), glm::vec4(-1, 0, 0, 0), glm::vec2((1.0f / 3.0f), (1.0f / 3.0f)) }, //BR
			    				
		//Right Face
		Vertex{ glm::vec4(1.5f, -0.5f, 1.5f, 1), glm::vec4(1, 0, 0, 0), glm::vec2(0, (1.0f / 3.0f)) },  //BL
		Vertex{ glm::vec4(1.5f, 0.5f, 1.5f, 1), glm::vec4(1, 0, 0, 0), glm::vec2(0, 0) },   //TL
		Vertex{ glm::vec4(1.5f, 0.5f, -1.5f, 1), glm::vec4(1, 0, 0, 0), glm::vec2((1.0f / 3.0f), 0) },    //TR
		Vertex{ glm::vec4(1.5f, -0.5f, -1.5f, 1), glm::vec4(1, 0, 0, 0), glm::vec2((1.0f / 3.0f), (1.0f / 3.0f)) },   //BR
			    				 			    
		//Top Face
		Vertex{ glm::vec4(-1.5f, 0.5f, 1.5f, 1), glm::vec4(0, 1, 0, 0), glm::vec2((1.0f / 3.0f), 1.0f) },  //BL
		Vertex{ glm::vec4(-1.5f, 0.5f, -1.5f, 1), glm::vec4(0, 1, 0, 0), glm::vec2((1.0f / 3.0f), (2.0f / 3.0f)) },   //TL
		Vertex{ glm::vec4(1.5f, 0.5f, -1.5f, 1), glm::vec4(0, 1, 0, 0), glm::vec2((2.0f / 3.0f), (2.0f / 3.0f)) },    //TR
		Vertex{ glm::vec4(1.5f, 0.5f, 1.5f, 1), glm::vec4(0, 1, 0, 0), glm::vec2((2.0f / 3.0f), 1.0f) },   //BR

		//Bottom Face
		Vertex{ glm::vec4(-1.5f, -0.5f, -1.5f, 1), glm::vec4(0, -1, 0, 0), glm::vec2(0, 1) },  //BL
		Vertex{ glm::vec4(-1.5f, -0.5f, 1.5f, 1), glm::vec4(0, -1, 0, 0), glm::vec2(0, (2.0f / 3.0f)) }, //TL
		Vertex{ glm::vec4(1.5f, -0.5f, 1.5f, 1), glm::vec4(0, -1, 0, 0), glm::vec2((1.0f / 3.0f), (2.0f / 3.0f)) },  //TR
		Vertex{ glm::vec4(1.5f, -0.5f, -1.5f, 1), glm::vec4(0, -1, 0, 0), glm::vec2((1.0f / 3.0f), 1) }    //BR
	};

	index = 
	{
		//Front Face
		2, 1, 0,
		0, 3, 2,

		//Back Face
		6, 5, 4,
		4, 7, 6,

		//Left Face
		10, 9, 8,
		8, 11, 10,

		//Right Face
		14, 13, 12,
		12, 15, 14,

		//Top Face
		18, 17, 16,
		16, 19, 18,

		//Bottom Face
		22, 21, 20,
		20, 23, 22
	};

	//unsigned char* data = stbi_load("../Images/MinecraftTextures/minecraft_grass_block.png", &x, &y, &n, 0);

	//glGenTextures(1, &texture);
	//glGenTextures(1, &normal);
	//glBindTexture(GL_TEXTURE_2D, texture);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	//stbi_image_free(data);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(Vertex), verticies, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index.size() * sizeof(unsigned int), index.data() , GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec3)));

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_TRUE, sizeof(Vertex), (void*)16);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

void Mesh::Draw()
{
	//glBindTexture(GL_TEXTURE_2D, texture);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, index.size(), GL_UNSIGNED_INT, 0);
}