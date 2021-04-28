#pragma once

#include <iostream>
#include <vector>
#include "ext.hpp"
#include "glm.hpp"
#include <fstream>
#include <sstream>
#include <string>
#include "gl_core_4_5.h"
#include "glfw3.h"
#include "FlyCamera.h"
#include "Light.h"

class Shader
{

public:

	Shader(std::string vertex, std::string frag, Light light, glm::vec3 ambientLight);

	unsigned int GetVertexShader();
	unsigned int GetFragmentShader();
	unsigned int GetShaderProgram();

	void Update(FlyCamera* camera, glm::mat4 model, glm::vec3 rgb, glm::vec3 lightDir);

private:

	void Vertex();
	void Frag();
	void LinkProgram();

	GLint success;

	std::string shaderData;

	std::stringstream vertexShaderData;
	std::stringstream fragShaderData;
	std::stringstream vertexPath;
	std::stringstream fragPath;

	const char* data;

	unsigned int vertexShaderID = 0;
	unsigned int fragShaderID = 0;
	unsigned int shaderProgramID = 0;

	Light directLight;
	glm::vec3 ambLight;
};
