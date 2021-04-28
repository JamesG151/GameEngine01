#include "Camera.h"

//Camera Constructor
Camera::Camera()
{
	//Initialize all transforms to their identity.
	worldTransform = glm::mat4(1);
	viewTransform = glm::mat4(1);
	projectionTransform = glm::mat4(1);
	projectionViewTransform = glm::mat4(1);
}

//Set Camera's perspective.
void Camera::SetPerspective(float fFOV, float fAspectRatio, float fNear, float fFar)
{
	projectionTransform = glm::perspective(fFOV, fAspectRatio, fNear, fFar);
	UpdateProjectionViewTransform();
}

//Set which direction the camera is looking at.
void Camera::SetLookAt(glm::vec3 v3From, glm::vec3 v3To, glm::vec3 v3Up)
{
	viewTransform = glm::lookAt(v3From, v3To, v3Up);
	worldTransform = glm::inverse(viewTransform);
	UpdateProjectionViewTransform();
}

//Set the current position of the camera using a Vector 3.
void Camera::SetPosition(glm::vec3 v3position)
{
	worldTransform[3] = glm::vec4(v3position, 1);
	viewTransform = glm::inverse(worldTransform);
	UpdateProjectionViewTransform();
}

//Getters that return that respective transform.
glm::mat4 Camera::GetWorldTransform()
{
	return worldTransform;
}

glm::mat4 Camera::GetView()
{
	return viewTransform;
}

glm::mat4 Camera::GetProjection()
{
	return projectionTransform;
}

glm::mat4 Camera::GetProjectionView()
{
	return projectionViewTransform;
}

// P * V
void Camera::UpdateProjectionViewTransform()
{
	projectionViewTransform = projectionTransform * viewTransform;
}