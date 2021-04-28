//Includes for the glm, glfw libraries, file handling and camera.
#include "glm.hpp"
#include "ext.hpp"
#include "gl_core_4_5.h"
#include "glfw3.h"
#include "FlyCamera.h"
#include "Mesh.h"
#include "Shader.h"
#include "OBJMesh.h"
#include "Light.h"

#include <fstream>
#include <sstream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	//Initialize hooks to windows.
	if (glfwInit() == false)
		return -1;

	//Window dimensions
	int windowWidth = 1280;
	int windowHeight = 720;

	//Setting window properties.
	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "GameEngine", nullptr, nullptr);

	//Terminate the window if not found.
	if (window == nullptr)
	{
		glfwTerminate();
		
		return -2;
	}

	//Make the openGL window the main window.
	glfwMakeContextCurrent(window);

	//Testing the OpenGL functions
	//Destroy the window if it fails.
	if (ogl_LoadFunctions() == ogl_LOAD_FAILED)
	{
		glfwDestroyWindow(window);
		glfwTerminate();

		return -3;
	}

	//Major and Minor versions.
	auto major = ogl_GetMajorVersion();
	auto minor = ogl_GetMinorVersion();

	//Prints the version of openGL we are using to the console.
	printf("GL: %i.%i\n", major, minor);

	const int lightCount = 2;
	Light phongLight[lightCount];
	glm::vec3 ambientLight;

	phongLight[0].diffuse = { 0.0f, 0.75f, 0.75f };
	phongLight[0].specular = { 1, 1, 1 };

	phongLight[1].diffuse = { 1.0f, 0.0f, 0.0f };
	phongLight[1].specular = { 1, 1, 1 };

	ambientLight = { 0, 0, 0 };

	glm::vec3 lightDirection(0.75f, 0.75f, 0.75f);
	
   // int x, y, n;
	//unsigned char* texturePath;

	unsigned int shieldTextureID = 0;
	unsigned int shieldNormalID = 0;
	unsigned int swordTextureID = 0;
	unsigned int swordNormalID = 0;

	//unsigned int vertexShaderID = 0;
	//unsigned int fragShaderID = 0;
	//unsigned int shaderProgramID = 0;

	//Create the camera for the program with a direction and FOV and an aspect ratio.
	FlyCamera* camera = new FlyCamera(glm::vec3(0, 0, 1), glm::vec3(0), 16.0f / 9.0f, 3.14159f * 0.5f, 0.1f, 50.f);
	//Mesh* mesh = new Mesh();
	//Shader* shader = new Shader("VertShaderV2.shader", "FragShaderV2.shader", phongLight, ambientLight);
	
	OBJMesh* obj = new OBJMesh();
	obj->load("../Models/meshSwordShield.obj", false);

	int x, y, n;
	unsigned char* texturePath;

	//unsigned int shieldTextureID = 0;
	//unsigned int shieldNormalID = 0;
	//unsigned int swordTextureID = 0;
	//unsigned int swordNormalID = 0;

	glGenTextures(1, &shieldTextureID);
	glBindTexture(GL_TEXTURE_2D, shieldTextureID);
	stbi_set_flip_vertically_on_load(true);

	texturePath = stbi_load("../Images/OtherTextures/UVAlbedoMap_Shield.png", &x, &y, &n, 0);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, texturePath);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	stbi_image_free(texturePath);

	glGenTextures(1, &shieldNormalID);
	glBindTexture(GL_TEXTURE_2D, shieldNormalID);
	stbi_set_flip_vertically_on_load(true);

	texturePath = stbi_load("../Images/OtherTextures/UVNormalMap_Shield.png", &x, &y, &n, 0);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, texturePath);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	stbi_image_free(texturePath);

	glGenTextures(1, &swordTextureID);
	glBindTexture(GL_TEXTURE_2D, swordTextureID);
	stbi_set_flip_vertically_on_load(true);

	texturePath = stbi_load("../Images/OtherTextures/UVAlbedoMap_Sword.png", &x, &y, &n, 0);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, texturePath);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	stbi_image_free(texturePath);

	glGenTextures(1, &swordNormalID);
	glBindTexture(GL_TEXTURE_2D, swordNormalID);
	stbi_set_flip_vertically_on_load(true);

	texturePath = stbi_load("../Images/OtherTextures/UVNormalMap_Sword.png", &x, &y, &n, 0);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, texturePath);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	stbi_image_free(texturePath);

	unsigned int vertexShaderID = 0;
	unsigned int fragShaderID = 0;
	unsigned int shaderProgramID = 0;

	std::string sShaderData;
	std::ifstream inFileStream("..\\Shaders\\VertShaderV2.shader", std::ifstream::in);

	std::stringstream stringStream;

	//If the file successfully opened.
	//Read in all the code from it.
	if (inFileStream.is_open())
	{
		stringStream << inFileStream.rdbuf();
		sShaderData = stringStream.str();
		inFileStream.close();
	}

	//Init the vertex shader.
	vertexShaderID = glCreateShader(GL_VERTEX_SHADER);

	//convert the data to a c-string.
	const char* data = sShaderData.c_str();

	//Shader compilers.
	glShaderSource(vertexShaderID, 1, (const GLchar**)&data, 0);
	glCompileShader(vertexShaderID);

	//Used to check if something was loaded properly.
	GLint success = GL_FALSE;
	glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &success);

	//If the vertex shader failed.
	if (success == GL_FALSE)
	{
		//printf("Vertex Shader Failed");

		//------------------------------
		//DEBUG
		//------------------------------
		GLint logLength = 0;
		glGetShaderiv(shaderProgramID, GL_INFO_LOG_LENGTH, &logLength);
		char* log = new char[logLength];
		glGetShaderInfoLog(shaderProgramID, logLength, 0, log);

		std::string errorMessage(log);
		errorMessage += "SHADER_FAILED_TO_COMPILE";
		printf(errorMessage.c_str());
		delete[] log;
		//------------------------------
		//------------------------------
		//------------------------------
	}

	//Read in the frag shader file.
	std::ifstream inFileStreamFrag("..\\Shaders\\FragShaderV2.shader", std::ifstream::in);
	std::stringstream fragStringStream;

	//If the file successfully opens.
	//Read in the file's code.
	if (inFileStreamFrag.is_open())
	{
		fragStringStream << inFileStreamFrag.rdbuf();
		sShaderData = fragStringStream.str();
		inFileStreamFrag.close();
	}

	//Init the fragment shader.
	fragShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	//Convert the data to a c-string.
	data = sShaderData.c_str();

	glShaderSource(fragShaderID, 1, (const GLchar**)&data, 0);
	glCompileShader(fragShaderID);

	success = GL_FALSE;
	glGetShaderiv(fragShaderID, GL_COMPILE_STATUS, &success);

	//If the fragment shader failed.
	if (success == GL_FALSE)
	{
		//printf("Fragment Shader Failed");

		//------------------------------
		//DEBUG
		//------------------------------
		GLint logLength = 0;
		glGetShaderiv(shaderProgramID, GL_INFO_LOG_LENGTH, &logLength);
		char* log = new char[logLength];
		glGetShaderInfoLog(shaderProgramID, logLength, 0, log);

		std::string errorMessage(log);
		errorMessage += "SHADER_FAILED_TO_COMPILE";
		printf(errorMessage.c_str());
		delete[] log;
		//------------------------------
		//------------------------------
		//------------------------------
	}

	//Create the shader linking program.
	shaderProgramID = glCreateProgram();

	//Attach the vert and frag shaders to the program.
	glAttachShader(shaderProgramID, vertexShaderID);
	glAttachShader(shaderProgramID, fragShaderID);

	//Begin linking the program.
	glLinkProgram(shaderProgramID);

	success = GL_FALSE;
	glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &success);

	//If the shader program link fails.
	if (!success)
	{
		//printf("Shader linking failed");

		//------------------------------
		//DEBUG
		//------------------------------
		GLint logLength = 0;
		glGetProgramiv(shaderProgramID, GL_INFO_LOG_LENGTH, &logLength);
		char* log = new char[logLength];
		glGetProgramInfoLog(shaderProgramID, logLength, 0, log);

		std::string errorMessage(log);
		errorMessage += "SHADER_FAILED_TO_COMPILE";
		printf(errorMessage.c_str());
		delete[] log;
		//------------------------------
		//------------------------------
		//------------------------------
	}

	glm::mat4 model = glm::mat4(1.0f);

	/**  Change This To Make It Render The Full Shape, Wireframe Or Points **/
	//glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);

	//Frame tracking and deltaTime.
	float currentFrame = 0;
	float lastFrame = 0;
	float deltaTime = 0;

	//Set the mouse to the center of the window before rendering and disable it's visibility.
	glfwSetCursorPos(window, windowWidth * 0.5, windowHeight * 0.5);
	ShowCursor(false);

	glm::vec3 rgb = glm::vec3(0.0f, 0.0f, 0.0f);

	//Size of the wireframe.
	float pointSize = 0.005f;

	//Bool used to check if the line width is finished resizing.
	bool resizeComplete = false;

	//Keeps the window open until the escape key is pressed.
	while (glfwWindowShouldClose(window) == false && glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS)
	{
		//Track the frame count and increment it every frame.
		static int framecount = 0;
		framecount++;

		currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		//This makes the rendered shape pulse during wireframe mode.
		if (pointSize >= 10.0f && resizeComplete == false)
			resizeComplete = true;
		else if (pointSize <= 0.5f && resizeComplete == true)
			resizeComplete = false;

		if (resizeComplete == true)
			pointSize -= 1.0f;
		else
			pointSize += 1.0f;

		//glLineWidth(pointSize);
		glPointSize(pointSize);
		
		phongLight[0].direction = glm::vec3(-1, 0, 0);/*glm::normalize(glm::vec3(glm::cos(glfwGetTime() * 2), glm::sin(glfwGetTime() * 2), 0));*/
		phongLight[1].direction = glm::vec3(0, -1, 0);

		//Use the shader program with this ID to render.
		glUseProgram(shaderProgramID);

		auto uniformLocation = glGetUniformLocation(shaderProgramID, "projectionViewModel");
		glUniformMatrix4fv(uniformLocation, 1, false, glm::value_ptr(camera->GetProjectionView()* model));

		uniformLocation = glGetUniformLocation(shaderProgramID, "normalMatrix");
		glUniformMatrix3fv(uniformLocation, 1, false, glm::value_ptr(glm::inverseTranspose(glm::mat3(model))));

		uniformLocation = glGetUniformLocation(shaderProgramID, "modelMatrix");
		glUniformMatrix4fv(uniformLocation, 1, false, glm::value_ptr(model));

		uniformLocation = glGetUniformLocation(shaderProgramID, "color");
		glUniform4fv(uniformLocation, 1, glm::value_ptr(rgb));

		uniformLocation = glGetUniformLocation(shaderProgramID, "time");
		glUniform1f(uniformLocation, glfwGetTime());

		uniformLocation = glGetUniformLocation(shaderProgramID, "light[0].iA");
		glUniform3fv(uniformLocation, 1, glm::value_ptr(ambientLight));

		uniformLocation = glGetUniformLocation(shaderProgramID, "light[0].iS");
		glUniform3fv(uniformLocation, 1, glm::value_ptr(phongLight[0].specular));

		uniformLocation = glGetUniformLocation(shaderProgramID, "light[0].iD");
		glUniform3fv(uniformLocation, 1, glm::value_ptr(phongLight[0].diffuse));

		uniformLocation = glGetUniformLocation(shaderProgramID, "light[1].iA");
		glUniform3fv(uniformLocation, 1, glm::value_ptr(ambientLight));

		uniformLocation = glGetUniformLocation(shaderProgramID, "light[1].iS");
		glUniform3fv(uniformLocation, 1, glm::value_ptr(phongLight[1].specular));

		uniformLocation = glGetUniformLocation(shaderProgramID, "light[1].iD");
		glUniform3fv(uniformLocation, 1, glm::value_ptr(phongLight[1].diffuse));

		uniformLocation = glGetUniformLocation(shaderProgramID, "kA");
		glUniform3fv(uniformLocation, 1, glm::value_ptr(glm::vec3(1)));

		uniformLocation = glGetUniformLocation(shaderProgramID, "kS");
		glUniform3fv(uniformLocation, 1, glm::value_ptr(glm::vec3(1)));

		uniformLocation = glGetUniformLocation(shaderProgramID, "kD");
		glUniform3fv(uniformLocation, 1, glm::value_ptr(glm::vec3(1)));

		uniformLocation = glGetUniformLocation(shaderProgramID, "light[0].lightDir");
		glUniform3fv(uniformLocation, 1, glm::value_ptr(phongLight[0].direction));

		uniformLocation = glGetUniformLocation(shaderProgramID, "light[1].lightDir");
		glUniform3fv(uniformLocation, 1, glm::value_ptr(phongLight[1].direction));

		uniformLocation = glGetUniformLocation(shaderProgramID, "fSpecularPower");
		glUniform1f(uniformLocation, 32.0f);

		uniformLocation = glGetUniformLocation(shaderProgramID, "cameraPosition");
		glUniform3fv(uniformLocation, 1, glm::value_ptr(glm::vec3(camera->GetWorldTransform()[3])));

		uniformLocation = glGetUniformLocation(shaderProgramID, "lightCount");
		glUniform1i(uniformLocation, lightCount);


		//Color fading of background.
		//if (rgb.z >= 1 && rgb.y <= 0 && rgb.x <= 1)
		//	rgb.x += 0.01f;
		//else if (rgb.z >= 1 && rgb.x <= 0)
		//	rgb.y -= 0.01f;
		//else if (rgb.y >= 1 && rgb.x <= 0)
		//	rgb.z += 0.01f;
		//else if (rgb.y >= 1 && rgb.z <= 0)
		//	rgb.x -= 0.01f;
		//else if (rgb.x >= 1 && rgb.z <= 0)
		//	rgb.y += 0.01f;
		//else if (rgb.x >= 1 && rgb.y <= 0)
		//	rgb.z -= 0.01f;

		//This rotates the model.
		model = glm::rotate(model, 0.005f, glm::vec3(0, 0.005f, 0));

		//Changes the background color.
		glClearColor(0, 0, 0, 1);

		//Update the camera.
		camera->Update(deltaTime);

		glEnable(GL_DEPTH_TEST); //Enables depth buffer.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/*	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);*/

		//mesh->Draw();
		//shader->Update(camera, model, rgb, lightDirection);

		// Diffuse
		uniformLocation = glGetUniformLocation(shaderProgramID, "diffuseTexture");
		glUniform1i(uniformLocation, 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, shieldTextureID);
		// Normal
		uniformLocation = glGetUniformLocation(shaderProgramID, "normalTexture");
		glUniform1i(uniformLocation, 1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, shieldNormalID);

		glBindVertexArray(obj->m_meshChunks[0].vao);
		glDrawElements(GL_TRIANGLES, obj->m_meshChunks[0].indexCount, GL_UNSIGNED_INT, 0);

		//Diffuse
		uniformLocation = glGetUniformLocation(shaderProgramID, "diffuseTexture");
		glUniform1i(uniformLocation, 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, swordTextureID);
		//Normal
		uniformLocation = glGetUniformLocation(shaderProgramID, "normalTexture");
		glUniform1i(uniformLocation, 1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, swordNormalID);

		glBindVertexArray(obj->m_meshChunks[1].vao);
		glDrawElements(GL_TRIANGLES, obj->m_meshChunks[1].indexCount, GL_UNSIGNED_INT, 0);
		
		//obj->draw();

		//Swap buffers and poll for events.
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	delete camera;
	//delete mesh;
	//delete shader;
	delete obj;

	camera = nullptr;
	//mesh = nullptr;
	//shader = nullptr;
	obj = nullptr;

	//Close the program.
	return 0;
}