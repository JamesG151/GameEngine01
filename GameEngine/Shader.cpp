#include "Shader.h"

Shader::Shader(std::string vertex, std::string frag, Light light, glm::vec3 ambientLight)
{
	directLight.diffuse = { 0, 0.25f, 0 };
	directLight.specular = { 1.0f, 1.0f, 1.0f };
	directLight.direction = glm::normalize(glm::vec3(0, -1.0f, 0));
	ambLight = glm::vec3(0.25f, 0.25f, 0.25f);

	success = false;
	std::string path = "..\\Shaders\\";

	vertexPath << path << vertex;
	fragPath << path << frag;

	Vertex();
	Frag();
	LinkProgram();
}

unsigned int Shader::GetVertexShader()
{
	return vertexShaderID;
}

unsigned int Shader::GetFragmentShader()
{
	return fragShaderID;
}

unsigned int Shader::GetShaderProgram()
{
	return shaderProgramID;
}

void Shader::Update(FlyCamera* camera, glm::mat4 model, glm::vec3 rgb, glm::vec3 lightDir)
{
	//The color of the rendered model.
	//rgb = glm::vec4(rgb.x - 0.5f, rgb.y - 0.5f, rgb.z - 0.5f, 1.0f);
	rgb = glm::vec4(rgb.x + 1, rgb.y + 1, rgb.z + 1, 1.0f);

	directLight.direction = glm::normalize(glm::vec3(glm::cos(glfwGetTime() * 2), glm::sin(glfwGetTime() * 2), 0));

	////Use the shader program with this ID to render.
	glUseProgram(shaderProgramID);

	auto uniformLocation = glGetUniformLocation(shaderProgramID, "projectionViewModel");
	glUniformMatrix4fv(uniformLocation, 1, false, glm::value_ptr(camera->GetProjectionView() * model));

	uniformLocation = glGetUniformLocation(shaderProgramID, "normalMatrix");
	glUniformMatrix3fv(uniformLocation, 1, false, glm::value_ptr(glm::inverseTranspose(glm::mat3(model))));

	uniformLocation = glGetUniformLocation(shaderProgramID, "modelMatrix");
	glUniformMatrix4fv(uniformLocation, 1, false, glm::value_ptr(model));

	uniformLocation = glGetUniformLocation(shaderProgramID, "color");
	glUniform4fv(uniformLocation, 1, glm::value_ptr(rgb));

	uniformLocation = glGetUniformLocation(shaderProgramID, "time");
	glUniform1f(uniformLocation, glfwGetTime());

	uniformLocation = glGetUniformLocation(shaderProgramID, "iA");
	glUniform3fv(uniformLocation, 1, glm::value_ptr(ambLight));

	uniformLocation = glGetUniformLocation(shaderProgramID, "iS");
	glUniform3fv(uniformLocation, 1, glm::value_ptr(directLight.specular));

	uniformLocation = glGetUniformLocation(shaderProgramID, "iD");
	glUniform3fv(uniformLocation, 1, glm::value_ptr(directLight.diffuse));

	uniformLocation = glGetUniformLocation(shaderProgramID, "kA");
	glUniform3fv(uniformLocation, 1, glm::value_ptr(glm::vec3(1)));

	uniformLocation = glGetUniformLocation(shaderProgramID, "kS");
	glUniform3fv(uniformLocation, 1, glm::value_ptr(glm::vec3(1)));

	uniformLocation = glGetUniformLocation(shaderProgramID, "kD");
	glUniform3fv(uniformLocation, 1, glm::value_ptr(glm::vec3(1)));

	uniformLocation = glGetUniformLocation(shaderProgramID, "lightDir");
	glUniform3fv(uniformLocation, 1, glm::value_ptr(directLight.direction));

	uniformLocation = glGetUniformLocation(shaderProgramID, "fSpecularPower");
	glUniform1f(uniformLocation, 32.0f);

	uniformLocation = glGetUniformLocation(shaderProgramID, "cameraPosition");
	glUniform3fv(uniformLocation, 1, glm::value_ptr(glm::vec3(camera->GetWorldTransform()[3])));

}

void Shader::Vertex()
{
	//Read in this shader file.
	std::ifstream inFileStream("..\\Shaders\\VertShaderV2.shader", std::ifstream::in);

	std::stringstream stringStream;

	//If the file successfully opened.
	//Read in all the code from it.
	if (inFileStream.is_open())
	{
		stringStream << inFileStream.rdbuf();
		shaderData = stringStream.str();
		inFileStream.close();
	}

	//Init the vertex shader.
	vertexShaderID = glCreateShader(GL_VERTEX_SHADER);

	//convert the data to a c-string.
	const char* data = shaderData.c_str();

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
}

void Shader::Frag()
{
	//Read in the frag shader file.
	std::ifstream inFileStreamFrag("..\\Shaders\\FragShaderV2.shader", std::ifstream::in);
	std::stringstream fragStringStream;

	//If the file successfully opens.
	//Read in the file's code.
	if (inFileStreamFrag.is_open())
	{
		fragStringStream << inFileStreamFrag.rdbuf();
		shaderData = fragStringStream.str();
		inFileStreamFrag.close();
	}

	//Init the fragment shader.
	fragShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	//Convert the data to a c-string.
	data = shaderData.c_str();

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
}

void Shader::LinkProgram()
{
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
}
