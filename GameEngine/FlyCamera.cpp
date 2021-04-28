#include "FlyCamera.h"

//Camera constructor.
FlyCamera::FlyCamera(const glm::vec3 v3Position, const glm::vec3 v3LookAtPoint, const float fAspectRatio, const float fFOVY, const float fNear, const float fFar) : Camera::Camera()
{
	//Sets the perspective and which direction the camera is looking at.
	SetPerspective(fFOVY, fAspectRatio, fNear, fFar);
	SetLookAt(v3Position, v3LookAtPoint, v3Up);
}

//Update the camera.
void FlyCamera::Update(float fDeltaTime)
{
	//Make this window the focus.
	auto glfwWindow = glfwGetCurrentContext();

	//Bool to track if keys are pressed.
	bool bInputFlag = false;

	//Displacement vector. 
	glm::vec3 displacement = glm::vec3(0, 0, 0);

	//If the W key is pressed.
	if (glfwGetKey(glfwWindow, GLFW_KEY_W))
	{
		//Move the camera forward on the z axis.
		//Input has been detected.
		displacement -= glm::vec3(GetWorldTransform()[2]);
		bInputFlag = true;
	}
	//If the S key is pressed.
	if (glfwGetKey(glfwWindow, GLFW_KEY_S))
	{
		//Move the camera backward on the z axis
		//Input has been detected.
		displacement += glm::vec3(GetWorldTransform()[2]);
		bInputFlag = true;
	}
	//If the A key is pressed.
	if (glfwGetKey(glfwWindow, GLFW_KEY_A))
	{	
		//Move the camera to the left along the x axis.
		//Input has been detected.
		displacement -= glm::vec3(GetWorldTransform()[0]);
		bInputFlag = true;
	}
	//If the D key is pressed.
	if (glfwGetKey(glfwWindow, GLFW_KEY_D))
	{
		//Move the camera to the right along the x axis.
		//Input has been detected.
		displacement += glm::vec3(GetWorldTransform()[0]);
		bInputFlag = true;
	}
	//If the left control key is pressed.
	if (glfwGetKey(glfwWindow, GLFW_KEY_LEFT_CONTROL))
	{
		//Move the camera down along the y axis.
		//Input has been detected.
		displacement -= glm::vec3(GetWorldTransform()[1]);
		bInputFlag = true;
	}
	//If the space key is pressed.
	if (glfwGetKey(glfwWindow, GLFW_KEY_SPACE))
	{
		//Move the camera up along the y axis.
		//Input has been detected.
		displacement += glm::vec3(GetWorldTransform()[1]);
		bInputFlag = true;
	}

	//If input is detected.
	if (bInputFlag)
	{
		//Update the camera's position with a certain speed and direction.
		SetPosition(GetWorldTransform()[3] + (glm::vec4(displacement, 0.0f) * fSpeed * fDeltaTime));
		UpdateProjectionViewTransform();
	}

	//Mouse cursor positions.
	double cursorPosX;
	double cursorPosY;

	//Gets the mouse cursor position of the window.
	glfwGetCursorPos(glfwWindow, &cursorPosX, &cursorPosY);

	//Sets delta's to be the center of their respective axis.
	double deltaX = cursorPosX - (1280 * 0.5);
	double deltaY = cursorPosY - (720 * 0.5);

	//Set the cursor position to the center of the window.
	glfwSetCursorPos(glfwWindow, 1280 * 0.5, 720 * 0.5);

	//This rotates the camera based on mouse movement.
	if (deltaX || deltaY)
	{	
		auto rotation = glm::mat4(1.0f);

		rotation = glm::rotate(rotation, float(fAngularSpeed * fDeltaTime * -deltaX), glm::vec3(GetView()[1]));
		rotation = glm::rotate(rotation, float(fAngularSpeed * fDeltaTime * -deltaY), glm::vec3(1, 0, 0));

		worldTransform = GetWorldTransform() * rotation;
		viewTransform = glm::inverse(worldTransform);
		UpdateProjectionViewTransform();
	}
}

void FlyCamera::SetSpeed(float fSpeed)
{

}