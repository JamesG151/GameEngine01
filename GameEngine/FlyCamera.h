#pragma once

#include "Camera.h"

//This camera inherits from the baseclass called Camera.
class FlyCamera : public Camera
{

public:

	//Camera constructor.
	FlyCamera(const glm::vec3 v3Position, const glm::vec3 v3LookAtPoint, const float fAspectRatio = 16.0f / 9.0f, const float fFOVY = 3.14159f * 0.5f, const float fNear = 0.01f, const float fFar = 100.0f);

	//Overriden update.
	void Update(float fDeltaTime) override;

	//Set the camera's move speed.
	void SetSpeed(float fSpeed);

private:

	//Speeds for the camera's movement.
	float fSpeed = 2.5f;
	float fAngularSpeed = 0.5f;

	//A vector 3 that is straight up the y axis.
	glm::vec3 v3Up = glm::vec3(0, 1, 0);
};

