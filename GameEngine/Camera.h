#pragma once

//Includes.
#include "glm.hpp"
#include "ext.hpp"
#include "glfw3.h"

class Camera
{

public:
	//Camera constructor.
	Camera();

	//Pure virtual update.
	virtual void Update(float fDeltaTime) = 0;

	//Function to set the perspective of the camera using FOV, Ratio.
	void SetPerspective(float fFOV, float fAspectRatio, float fNear, float fFar);

	//Set what the camera is currently looking at.
	void SetLookAt(glm::vec3 v3From, glm::vec3 v3To, glm::vec3 v3Up);

	//Set the position of the camera.
	void SetPosition(glm::vec3 v3position);

	//Getters that return a matrix 4.
	glm::mat4 GetWorldTransform();
	glm::mat4 GetView();
	glm::mat4 GetProjection();
	glm::mat4 GetProjectionView();

	void UpdateProjectionViewTransform();

protected:

	//Various matrix 4s.
	glm::mat4 worldTransform;
	glm::mat4 viewTransform;
	glm::mat4 projectionTransform;
	glm::mat4 projectionViewTransform;
};

